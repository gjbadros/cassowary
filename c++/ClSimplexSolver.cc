// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998, All Rights Reserved.
//
// ClSimplexSolver.cc

#include "ClSimplexSolver.h"
#include "debug.h"
#include "ClErrors.h"

// Add the constraint cn to the tableau
void 
ClSimplexSolver::addConstraint(ClConstraint &cn)
{
#ifndef NO_TRACE
  Tracer TRACER(__FUNCTION__);
  cerr << "(" << cn << ")" << endl;
#endif
    
  ClLinearExpression expr = makeExpression(cn);

  // If possible add expr directly to the appropriate tableau by
  // choosing a subject for expr (a variable to become basic) from
  // among the current variables in expr.  If this doesn't work use an
  // artificial variable.  After adding expr re-optimize.
  if (!tryAddingDirectly(expr))
    { // could not add directly
    addWithArtificialVariable(expr);
    }
  optimize(my_objective);
  setExternalVariables();
}

// Add weak stays to the x and y parts of each point. These have
// increasing weights so that the solver will try to satisfy the x
// and y stays on the same point, rather than the x stay on one and
// the y stay on another.
void 
ClSimplexSolver::addPointStays(const vector<ClPoint> &listOfPoints)
{
#ifndef NO_TRACE
  Tracer TRACER(__FUNCTION__);
  cerr << "(" << "FIXGJB" << ")" << endl;
#endif

  vector<ClPoint>::const_iterator it = listOfPoints.begin();
  double weight = 1.0;
  static const double multiplier = 2.0;
  for ( ; it != listOfPoints.end(); it++ )
    {
    addPointStay((*it).first,(*it).second,weight);
    weight *= multiplier;
    }
}

// Remove the constraint cn from the tableau
// Also remove any error variable associated with cn
void 
ClSimplexSolver::removeConstraint(const ClConstraint &cnconst)
{
#ifndef NO_TRACE
  Tracer TRACER(__FUNCTION__);
  cerr << "(" << cnconst << ")" << endl;
#endif

  // We are about to remove a constraint.  There may be some stay
  // constraints that were unsatisfied previously -- if we just
  // removed the constraint these could come into play.  Instead,
  // reset all of the stays so that things should stay where they are
  // at the moment.

  ClConstraint &cn = const_cast<ClConstraint &>(cnconst);

  resetStayConstants();

  // remove any error variables from the objective function
  ClLinearExpression &zRow = rowExpression(my_objective);
  map<ClConstraint *, set<ClVariable> >::iterator 
    it_eVars = my_errorVars.find(&cn);
  if (it_eVars != my_errorVars.end())
    {
    set<ClVariable> &eVars = (*it_eVars).second;
    set<ClVariable>::iterator it = eVars.begin();
    for ( ; it != eVars.end(); ++it )
      {
      const ClLinearExpression &expr = rowExpression((*it));
      if (expr == cleNil() )
	{
	zRow.addVariable((*it),-1.0,my_objective,*this);
	}
      else
	{ // the error variable was in the basis
	zRow.addExpression(expr,-1.0,my_objective,*this);
	}
      }
    my_errorVars.erase(it_eVars);
    }

  map<ClConstraint *, ClVariable>::iterator 
    it_marker = my_markerVars.find(&cn);
  // try to make the marker variable basic if it isn't already
  assert( it_marker != my_markerVars.end() );
  const ClVariable &marker = (*it_marker).second;
  if (rowExpression(marker) == cleNil() )
    { // not in the basis, so need to do some work
    // first choose which variable to move out of the basis
    // only consider restricted basic variables
    set<ClVariable> &col = my_columns[marker];
    set<ClVariable>::iterator it_col = col.begin();

    ClVariable &exitVar = clvNil();
    double minRatio = 0.0;
    for ( ; it_col != col.end(); ++it_col) 
      {
      const ClVariable &v = *it_col;
      if (v.isRestricted() )
	{
	const ClLinearExpression &expr = rowExpression(v);
	assert(expr != cleNil() );
	Number coeff = expr.coefficientFor(marker);
	// only consider negative coefficients
	if (coeff < 0.0) 
	  {
	  Number r = - expr.constant() / coeff;
	  if (exitVar == clvNil() || r < minRatio)
	    {
	    minRatio = r;
	    exitVar = v;
	    }
	  }
	}
      }
    // if exitVar is still nil at this point, then either the marker
    // variable has a positive coefficient in all equations, or it
    // only occurs in equations for unrestricted variables.  If it
    // does occur in an equation for a restricted variable, pick the
    // equation that gives the smallest ratio.  (The row with the
    // marker variable will become infeasible, but all the other rows
    // will still be feasible; and we will be dropping the row with
    // the marker variable.  In effect we are removing the
    // non-negativity restriction on the marker variable.)
    if (exitVar == clvNil() ) 
      {
      it_col = col.begin();
      for ( ; it_col != col.end(); ++it_col) 
	{
	const ClVariable &v = *it_col;
	if (v.isRestricted() )
	  {
	  const ClLinearExpression &expr = rowExpression(v);
	  assert(expr != cleNil() );
	  Number coeff = expr.coefficientFor(marker);
	  Number r = - expr.constant() / coeff;
	  if (exitVar == clvNil() || r < minRatio)
	    {
	    minRatio = r;
	    exitVar = v;
	    }
	  }
	}
      }

    if (exitVar == clvNil() )
      { // exitVar is still nil
      // If col is empty, then exitVar doesn't occur in any equations,
      // so just remove it.  Otherwise pick an exit var from among the
      // unrestricted variables whose equation involves the marker var
      if (col.size() == 0)
	{
	removeColumn(marker);
	}
      else
	{
	exitVar = *(col.begin());
	}
      }
    
    if (exitVar != clvNil() )
      {
      pivot(marker,exitVar);
      }
    }
  
  if (rowExpression(marker) != cleNil() )
    {
    removeRow(marker);
    }

  // Delete any error variables.  If cn is an inequality, it also
  // contains a slack variable; but we use that as the marker variable
  // and so it has been deleted when we removed its row.
  if (it_eVars != my_errorVars.end())
    {
    set<ClVariable> &eVars = (*it_eVars).second;
    set<ClVariable>::iterator it = eVars.begin();
    for ( ; it != eVars.end(); ++it )
      {
      const ClVariable &v = (*it);
      if (v != marker)
	{
	removeColumn(v);
	}
      }
    }

  if (cn.isStayConstraint())
    {
    // iterate over the stay{Plus,Minus}ErrorVars and remove those
    // variables v in those vectors that are also in set eVars
    if (it_eVars != my_errorVars.end())
      {
      set<ClVariable> &eVars = (*it_eVars).second;
      vector<ClVariable>::iterator itStayPlusErrorVars = my_stayPlusErrorVars.begin();
      vector<ClVariable>::iterator itStayMinusErrorVars = my_stayMinusErrorVars.begin();
      for (; itStayMinusErrorVars != my_stayMinusErrorVars.end();
	   ++itStayPlusErrorVars, ++itStayMinusErrorVars)
	{
	if (eVars.find(*itStayPlusErrorVars) != eVars.end())
	  {
	  my_stayMinusErrorVars.erase(itStayPlusErrorVars);
	  }
	if (eVars.find(*itStayMinusErrorVars) != eVars.end())
	  {
	  my_stayMinusErrorVars.erase(itStayMinusErrorVars);
	  }
	}
      }
    }
  else if (cn.isEditConstraint())
    {
    // remove the error variable for this constraint
    // need to update my_editPlusErrorVars,
    //                my_editMinusErrorVars, and
    //                my_prevEditConstants
    assert(it_eVars != my_errorVars.end());
    set<ClVariable> &eVars = (*it_eVars).second;
    set<ClVariable>::iterator it_v = eVars.begin();
    for ( ; it_v != eVars.end(); ++it_v)
      {
      const ClVariable &var = *it_v;
      // find var in my_editPlusErrorVars
      vector<ClVariable>::iterator 
	itEditPlusErrorVars = my_editPlusErrorVars.begin();
      for ( ; itEditPlusErrorVars != my_editPlusErrorVars.end(); ++itEditPlusErrorVars )
	{
	if (*itEditPlusErrorVars == var) break;
	}
      if (itEditPlusErrorVars != my_editPlusErrorVars.end())
	{ // found it
	int index = itEditPlusErrorVars - my_editPlusErrorVars.begin();
	vector<ClVariable>::iterator 
	  itEditMinusErrorVars = my_editMinusErrorVars.begin() + index;
	vector<Number>::iterator 
	  itPrevEditConstants = my_prevEditConstants.begin() + index;
	my_editPlusErrorVars.erase(itEditPlusErrorVars);
	my_editMinusErrorVars.erase(itEditMinusErrorVars);
	my_prevEditConstants.erase(itPrevEditConstants);
	// done -- can exit the iteration
	break;
	}
      }
    }

  optimize(my_objective);
  setExternalVariables();
}

// Re-initialize this solver from the original constraints, thus
// getting rid of any accumulated numerical problems.  (Actually,
// Alan hasn't observed any such problems yet, but here's the method
// anyway.)
void 
ClSimplexSolver::reset()
{
#ifndef NO_TRACE
  Tracer TRACER(__FUNCTION__);
  cerr << "()" << endl;
#endif
  // FIXGJB  -- can postpone writing this for a while
  assert(false);
}

// Re-solve the current collection of constraints for new values for
// the constants of the edit variables.
void 
ClSimplexSolver::resolve(const vector<double> &newEditConstants)
{
#ifndef NO_TRACE
  Tracer TRACER(__FUNCTION__);
  cerr << "(" << "newEditConstants" /* FIXGJB */ << ")" << endl;
#endif
  while (!my_infeasibleRows.empty())
    {
    my_infeasibleRows.erase(my_infeasibleRows.begin());
    }
  resetStayConstants();
  resetEditConstants(newEditConstants);
  dualOptimize();
  setExternalVariables();
}

//// protected

// Add the constraint expr=0 to the inequality tableau using an
// artificial variable.  To do this, create an artificial variable
// av and add av=expr to the inequality tableau, then make av be 0.
// (Raise an exception if we can't attain av=0.)
void 
ClSimplexSolver::addWithArtificialVariable(ClLinearExpression expr)
{
#ifndef NO_TRACE
  Tracer TRACER(__FUNCTION__);
  cerr << "(" << expr << ")" << endl;
#endif
  
  ClVariable av(++my_artificialCounter,"a",CLSlackVar);
  ClVariable az("az",CLObjectiveVar);
  ClLinearExpression azRow(expr);
  // FIXGJB: Why is azRow a duplicate of expr?

#ifndef NO_TRACE
  cerr << __FUNCTION__ << " before addRow-s:" << endl;
  cerr << (*this) << endl;
#endif

  // the artificial objective is av, which we know is equal to expr
  // (which contains only parametric variables)
  addRow(az,azRow);
  addRow(av,expr);

#ifndef NO_TRACE
  cerr << __FUNCTION__ << " after addRow-s:" << endl;
  cerr << (*this) << endl;
#endif

  // try to optimize av to 0
  optimize(az);

  // Check that we were able to make the objective value 0
  // If not, the original constraint was not satisfiable
  if (!clApprox(azRow.constant(),0.0))
    {
    EXCEPTION_ABORT;
    throw ExCLRequiredFailure();
    }

  // see if av is a basic variable
  const ClLinearExpression &e = rowExpression(av);
  if (e != cleNil() )
    {
    // Find another variable in this row and pivot, so that av becomes parametric
    // If there isn't another variabel in the row then 
    // the tableau contains the equation av = 0  -- just delete av's row
    if (e.isConstant())
      {
      removeRow(av);
      return;
      }
    const ClVariable &entryVar = e.anyVariable();
    pivot(entryVar, av);
    }
  // now av should be parametric
  assert(rowExpression(av) == cleNil() );
  removeColumn(av);
  // remove the temporary objective function
  removeRow(az);
}

// We are trying to add the constraint expr=0 to the appropriate
// tableau.  Try to add expr directly to the tableax without
// creating an artificial variable.  Return true if successful and
// false if not.
bool 
ClSimplexSolver::tryAddingDirectly(ClLinearExpression expr)
{
#ifndef NO_TRACE
  Tracer TRACER(__FUNCTION__);
  cerr << "(" << expr << ")" << endl;
#endif
  const ClVariable &subject = chooseSubject(expr);
  if (subject == clvNil() )
    {
#ifndef NO_TRACE
    cerr << "- returning false" << endl;
#endif
    return false;
    }
  expr.newSubject(subject);
  if (columnsHasKey(subject))
    {
    substituteOut(subject,expr);
    }
  addRow(subject,expr);
#ifndef NO_TRACE
  cerr << "- returning true" << endl;
#endif
  return true; // successfully added directly
}


// We are trying to add the constraint expr=0 to the tableaux.  Try
// to choose a subject (a variable to become basic) from among the
// current variables in expr.  If expr contains any unrestricted
// variables, then we must choose an unrestricted variable as the
// subject.  Also, if the subject is new to the solver we won't have
// to do any substitutions, so we prefer new variables to ones that
// are currently noted as parametric.  If expr contains only
// restricted variables, if there is a restricted variable with a
// negative coefficient that is new to the solver we can make that
// the subject.  Otherwise we can't find a subject, so return nil.
// (In this last case we have to add an artificial variable and use
// that variable as the subject -- this is done outside this method
// though.)
// 
// Note: in checking for variables that are new to the solver, we
// ignore whether a variable occurs in the objective function, since
// new slack variables are added to the objective function by
// 'makeExpression:', which is called before this method.
const ClVariable &
ClSimplexSolver::chooseSubject(ClLinearExpression &expr)
{
#ifndef NO_TRACE
  Tracer TRACER(__FUNCTION__);
  cerr << "(" << expr << ")" << endl;
#endif
  ClVariable &subject = clvNil(); // the current best subject, if any

  // true iff we have found a subject that is an unrestricted variable
  bool foundUnrestricted = false; 

  // true iff we have found a restricted variable that is new to the
  // solver (except for being in the obj. function) and that has a
  // negative coefficient
  bool foundNewRestricted = false;

  const map<ClVariable,Number> &terms = expr.terms();
  map<ClVariable,Number>::const_iterator it = terms.begin();
  for ( ; it != terms.end(); ++it )
    {
    const ClVariable &v = (*it).first;
    Number c = (*it).second;

    if (foundUnrestricted)
      {
      // We have already found an unrestricted variable.  The only
      // time we will want to use v instead of the current choice
      // 'subject' is if v is unrestricted and new to the solver and
      // 'subject' isn't new.  If this is the case just pick v
      // immediately and return.
      if (!v.isRestricted())
	{
	if (!columnsHasKey(v))
	  return v;
	}
      }
    else
      { // we haven't found an restricted variable yet
      if (v.isRestricted())
	{
	// v is restricted.  If we have already found a suitable
	// restricted variable just stick with that.  Otherwise, if v
	// is new to the solver and has a negative coefficient pick
	// it.  Regarding being new to the solver -- if the variable
	// occurs only in the objective function we regard it as being
	// new to the solver, since error variables are added to the
	// objective function when we make the expression.  We also
	// never pick a dummy variable here.
	if (!foundNewRestricted && !v.isDummy() && c < 0.0)
	  {
	  const map<ClVariable, set<ClVariable> > &col = columns();
	  map<ClVariable, set<ClVariable> >::const_iterator it_col = col.find(v);
	  if (it_col == col.end() || 
	      ( col.size() == 1 && columnsHasKey(my_objective) ) )
	    {
	    subject = v;
	    foundNewRestricted = true;
	    }
	  }
	}
      else
	{
	// v is unrestricted.  
	// If v is also new to the solver just pick it now
	subject = v;
	foundUnrestricted = true;
	}
      }
    }
  if (subject != clvNil())
    return subject;

  // subject is nil. 
  // Make one last check -- if all of the variables in expr are dummy
  // variables, then we can pick a dummy variable as the subject
  Number coeff = 0;
  it = terms.begin();
  for ( ; it != terms.end(); ++it )
    {
    const ClVariable &v = (*it).first;
    Number c = (*it).second;
    if (!v.isDummy())
      return clvNil(); // nope, no luck
    // if v is new to the solver, tentatively make it the subject
    if (!columnsHasKey(v))
      {
      subject = v;
      coeff = c;
      }
    }

  // If we get this far, all of the variables in the expression should
  // be dummy variables.  If the constant is nonzero we are trying to
  // add an unsatisfiable required constraint.  (Remember that dummy
  // variables must take on a value of 0.)  Otherwise, if the constant
  // is zero, multiply by -1 if necessary to make the coefficient for
  // the subject negative."
  if (!clApprox(expr.constant(),0.0))
    {
    EXCEPTION_ABORT;
    throw ExCLRequiredFailure();
    }
  if (coeff > 0.0)
    {
    expr.multiplyMe(-1);
    }
  return subject;
}
  
void 
ClSimplexSolver::deltaEditConstant(Number delta,
				   const ClVariable &plusErrorVar,
				   const ClVariable &minusErrorVar)
{
#ifndef NO_TRACE
  Tracer TRACER(__FUNCTION__);
  cerr << "(" << delta << ", " << plusErrorVar << ", " << minusErrorVar << ")" << endl;
#endif
  // first check if the plusErrorVar is basic
  ClLinearExpression &exprPlus = rowExpression(plusErrorVar);
  if (exprPlus != cleNil() )
    {
    exprPlus.incrementConstant(delta);
    // error variables are always restricted
    // so the row is infeasible if the constant is negative
    if (exprPlus.constant() < 0.0)
      {
      my_infeasibleRows.insert(plusErrorVar);
      }
    return;
    }
  // check if minusErrorVar is basic
  ClLinearExpression &exprMinus = rowExpression(minusErrorVar);
  if (exprMinus != cleNil() )
    {
    exprMinus.incrementConstant(-delta);
    if (exprMinus.constant() < 0.0)
      {
      my_infeasibleRows.insert(minusErrorVar);
      }
    return;
    }
  // Neither is basic.  So they must both be nonbasic, and will both
  // occur in exactly the same expressions.  Find all the expressions
  // in which they occur by finding the column for the minusErrorVar
  // (it doesn't matter whether we look for that one or for
  // plusErrorVar).  Fix the constants in these expressions.
  set<ClVariable> &columnVars = my_columns[minusErrorVar];
  set<ClVariable>::iterator it = columnVars.begin();
  for (; it != columnVars.end(); ++it)
    {
    const ClVariable &basicVar = *it;
    ClLinearExpression &expr = rowExpression(basicVar);
    assert(expr != cleNil() );
    double c = expr.coefficientFor(minusErrorVar);
    expr.incrementConstant(c*delta);
    if (basicVar.isRestricted() && expr.constant() < 0.0)
      {
      my_infeasibleRows.insert(basicVar);
      }
    }
}
  
// We have set new values for the constants in the edit constraints.
// Re-optimize using the dual simplex algorithm.
void 
ClSimplexSolver::dualOptimize()
{
#ifndef NO_TRACE
  Tracer TRACER(__FUNCTION__);
  cerr << "()" << endl;
#endif
  const ClLinearExpression &zRow = rowExpression(my_objective);
  // need to handle infeasible rows
  while (!my_infeasibleRows.empty())
    {
    // need to erase it_exitVar at end
    set<ClVariable>::iterator it_exitVar = my_infeasibleRows.begin();
    const ClVariable &exitVar = *it_exitVar;
    ClVariable *pentryVar = NULL;
    // exitVar might have become basic after some other pivoting
    // so allow for the case of its not being there any longer
    ClLinearExpression &expr = rowExpression(exitVar);
    if (expr != cleNil() )
      {
      // make sure the row is still not feasible
      if (expr.constant() < 0.0)
	{
	double ratio = MAXDOUBLE;
	double r;
	map<ClVariable,Number> &terms = expr.terms();
	map<ClVariable,Number>::iterator it = terms.begin();
	for ( ; it != terms.end(); ++it )
	  {
	  ClVariable &v = (*it).first;
	  Number c = (*it).second;
	  if (c > 0.0 && v.isPivotable())
	    {
	    Number zc = zRow.coefficientFor(v);
	    if (zc != 0)
	      {
	      r = zc/c;
	      }
	    else
	      {
	      r = 0; // FIXGJB r := [ClSymbolicWeight zero]
	      }
	    if (ratio == MAXDOUBLE || r < ratio)
	      {
	      pentryVar = &v;
	      ratio = r;
	      }
	    }
	  }
	if (ratio == MAXDOUBLE)
	  {
	  EXCEPTION_ABORT;
	  throw ExCLInternalError();
	  }
	pivot(*pentryVar,exitVar);
	}
      }
    my_infeasibleRows.erase(it_exitVar);
    }
}

// Make a new linear expression representing the constraint cn,
// replacing any basic variables with their defining expressions.
// Normalize if necessary so that the constant is non-negative.  If
// the constraint is non-required give its error variables an
// appropriate weight in the objective function.
ClLinearExpression 
ClSimplexSolver::makeExpression(ClConstraint &cn)
{
#ifndef NO_TRACE
  Tracer TRACER(__FUNCTION__);
  cerr << "(" << cn << ")" << endl;
#endif
  const ClLinearExpression &cnExpr = cn.expression();
  ClLinearExpression expr(cnExpr.constant());
  const map<ClVariable, Number> &cnTerms = cnExpr.terms();
  map<ClVariable,Number>::const_iterator it = cnTerms.begin();
  for ( ; it != cnTerms.end(); ++it)
    {
    const ClVariable &v = (*it).first;
    Number c = (*it).second;
    const ClLinearExpression &e = rowExpression(v);
    if (e == cleNil())
      {
      expr.addVariable(v,c);
      }
    else
      {
      expr.addExpression(e,c);
      }
    }

  // add slack and error variables as needed
  if (cn.isInequality())
    {
    // cn is an inequality, so add a slack variable.  The original
    // constraint is expr>=0, so that the resulting equality is
    // expr-slackVar=0.  If cn is also non-required add a negative
    // error variable, giving
    //    expr-slackVar = -errorVar, in other words
    //    expr-slackVar+errorVar=0.
    // Since both of these variables are newly created we can just add
    // them to the expression (they can't be basic).
    ++my_slackCounter;
    ClVariable &slackVar = *(new ClVariable("s",CLSlackVar));
    expr.addVariable(slackVar,-1);
    // index the constraint under its slack variable
    my_markerVars[&cn] = slackVar;
    if (!cn.isRequired())
      {
      ++my_slackCounter;
      ClVariable &eminus = *(new ClVariable("em",CLSlackVar));
      expr.addVariable(eminus,1.0);
      // add emnius to the objective function with the appropriate weight
      ClLinearExpression &zRow = rowExpression(my_objective);
      // FIXGJB: zRow.addVariable(eminus,cn.strength().symbolicWeight() * cn.weight());
      ClSymbolicWeight sw = cn.strength().symbolicWeight().times(cn.weight());
      zRow.addVariable(eminus,sw.asDouble());
      my_errorVars[&cn].insert(eminus);
      noteAddedVariable(eminus,my_objective);
      }
    }
  else
    { // cn is an equality
    if (cn.isRequired())
      {
      // Add a dummy variable to the expression to serve as a marker
      // for this constraint.  The dummy variable is never allowed to
      // enter the basis when pivoting.
      ++my_dummyCounter;
      ClVariable &dummyVar = *(new ClVariable("d",CLDummyVar));
      expr.addVariable(dummyVar,1.0);
      my_markerVars[&cn] = dummyVar;
      }
    else
      {
      // cn is a non-required equality.  Add a positive and a negative
      // error variable, making the resulting constraint 
      //       expr = eplus - eminus, 
      // in other words:  expr-eplus+eminus=0
      ++my_slackCounter;
      ClVariable &eplus = *(new ClVariable("ep",CLSlackVar));
      ClVariable &eminus = *(new ClVariable("em",CLSlackVar));
      expr.addVariable(eplus,-1.0);
      expr.addVariable(eminus,1.0);
      // index the constraint under one of the error variables
      my_markerVars[&cn] = eplus;
      ClLinearExpression &zRow = rowExpression(my_objective);
      // FIXGJB: zRow.addVariable(eplus,cn.strength().symbolicWeight() * cn.weight());
      ClSymbolicWeight sw = cn.strength().symbolicWeight().times(cn.weight());
      double swCoeff = sw.asDouble();
      zRow.addVariable(eplus,swCoeff);
      noteAddedVariable(eplus,my_objective);
      // FIXGJB: zRow.addVariable(eminus,cn.strength().symbolicWeight() * cn.weight());
      zRow.addVariable(eminus,swCoeff);
      noteAddedVariable(eminus,my_objective);
      if (cn.isStayConstraint()) 
	{
	my_stayPlusErrorVars.push_back(eplus);
	my_stayMinusErrorVars.push_back(eminus);
	}
      if (cn.isEditConstraint())
	{
	my_stayPlusErrorVars.push_back(eplus);
	my_stayMinusErrorVars.push_back(eminus);
	my_prevEditConstants.push_back(cnExpr.constant());
	}
      }
    }

  // the constant in the expression should be non-negative.
  // If necessary normalize the expression by multiplying by -1
  if (expr.constant() < 0)
    {
    expr.set_constant(-expr.constant());
    expr.multiplyMe(-1);
    }
#ifndef NO_TRACE
  cerr << "- returning " << expr << endl;
#endif
  return expr;
}

// Minimize the value of the objective.  (The tableau should already
// be feasible.)
void 
ClSimplexSolver::optimize(const ClVariable &zVar)
{
#ifndef NO_TRACE
  Tracer TRACER(__FUNCTION__);
  cerr << "(" << zVar << ")" << endl;
#endif
  assert(zVar.isCLObjective());
  ClLinearExpression &zRow = rowExpression(zVar);
  assert(zRow != cleNil());
  Number objectiveCoeff = -MAXDOUBLE;
  ClVariable entryVar = clvNil();
  ClVariable exitVar = clvNil();
  while (true)
    {
    // Find the most negative coefficient in the objective function
    // (ignoring dummy variables).  If all coefficients are positive
    // we're done
    map<ClVariable,Number> &terms = zRow.terms();
    map<ClVariable,Number>::iterator it = terms.begin();
    for (; it != terms.end(); ++it)
      {
      const ClVariable &v = (*it).first;
      Number c = (*it).second;
      if (v.isPivotable() && objectiveCoeff == -MAXDOUBLE ||
	  ( c < objectiveCoeff))
	{
	objectiveCoeff = c;
	entryVar = v;
	}
      }
    // if all coefficients were positive (or if the objective
    // function has no pivotable variables)
    // we are at an optimum
    if (objectiveCoeff == -MAXDOUBLE)
      return;
    if (!(objectiveCoeff < 0.0))
      return;
    // choose which variable to move out of the basis
    // Only consider pivotable basic variables
    // (i.e. restricted, non-dummy variables)
    double minRatio = MAXDOUBLE;
    set<ClVariable> &columnVars = my_columns[entryVar];
    set<ClVariable>::iterator it_rowvars = columnVars.begin();
    Number r = 0.0;
    for (; it_rowvars != columnVars.end(); ++it_rowvars)
      {
      const ClVariable &v = (*it_rowvars);
      const ClLinearExpression &expr = rowExpression(v);
      Number coeff = expr.coefficientFor(entryVar);
      // only consider negative coefficients
      if (coeff < 0.0)
	{
	r = - expr.constant() / coeff;
	if (r < minRatio)
	  {
	  minRatio = r;
	  exitVar = v;
	  }
	}
      }
    // If minRatio is still nil at this point, it means that the
    // objective function is unbounded, i.e. it can become
    // arbitrarily negative.  This should never happen in this
    // application.
    if (minRatio == MAXDOUBLE)
      {
      EXCEPTION_ABORT;
      throw ExCLInternalError();
      }
    pivot(entryVar, exitVar);
    }
}

// Do a pivot.  Move entryVar into the basis (i.e. make it a basic variable),
// and move exitVar out of the basis (i.e., make it a parametric variable)
void 
ClSimplexSolver::pivot(const ClVariable &entryVar, const ClVariable &exitVar)
{
#ifndef NO_TRACE
  Tracer TRACER(__FUNCTION__);
  cerr << "(" << entryVar << ", " << exitVar << ")" << endl;
#endif
  
  // expr is the expression for the exit variable (about to leave the basis) -- 
  // so that the old tableau includes the equation:
  //   exitVar = expr
  ClLinearExpression expr = removeRow(exitVar);

  // Compute an expression for the entry variable.  Since expr has
  // been deleted from the tableau we can destructively modify it to
  // build this expression.
  expr.changeSubject(exitVar,entryVar);
  substituteOut(entryVar,expr);
  addRow(entryVar,expr);
}


// Each of the non-required edits will be represented by an equation
// of the form
//    v = c + eplus - eminus 
// where v is the variable with the edit, c is the previous edit
// value, and eplus and eminus are slack variables that hold the
// error in satisfying the edit constraint.  We are about to change
// something, and we want to fix the constants in the equations
// representing the edit constraints.  If one of eplus and eminus is
// basic, the other must occur only in the expression for that basic
// error variable.  (They can't both be basic.)  Fix the constant in
// this expression.  Otherwise they are both nonbasic.  Find all of
// the expressions in which they occur, and fix the constants in
// those.  See the UIST paper for details.  void
void 
ClSimplexSolver::resetEditConstants(const vector<Number> &newEditConstants)
{
#ifndef NO_TRACE
  Tracer TRACER(__FUNCTION__);
  cerr << "(" << "newEditConstants" /* FIXGJB */ << ")" << endl;
#endif
  if (newEditConstants.size() != my_editPlusErrorVars.size())
    { // number of edit constants doesn't match the number of edit error variables
    cerr << "newEditConstants == " << newEditConstants << endl;
    cerr << "my_editPlusErrorVars == " << my_editPlusErrorVars << endl;
    cerr << "Sizes don't match!" << endl;
    EXCEPTION_ABORT;
    throw ExCLInternalError();
    }
  vector<Number>::const_iterator itNew = newEditConstants.begin();
  vector<Number>::iterator itPrev = my_prevEditConstants.begin();
  vector<ClVariable>::const_iterator 
    itEditPlusErrorVars = my_editPlusErrorVars.begin();
  vector<ClVariable>::const_iterator
    itEditMinusErrorVars = my_editMinusErrorVars.begin();

  for ( ; itNew != newEditConstants.end(); 
	++itNew, ++itPrev, ++itEditPlusErrorVars, ++itEditMinusErrorVars )
    {
    Number delta = (*itNew) - (*itPrev);
    (*itPrev) = (*itNew);
    deltaEditConstant(delta,(*itEditPlusErrorVars),(*itEditMinusErrorVars));
    }
}

// Each of the non-required stays will be represented by an equation
// of the form
//     v = c + eplus - eminus
// where v is the variable with the stay, c is the previous value of
// v, and eplus and eminus are slack variables that hold the error
// in satisfying the stay constraint.  We are about to change
// something, and we want to fix the constants in the equations
// representing the stays.  If both eplus and eminus are nonbasic
// they have value 0 in the current solution, meaning the previous
// stay was exactly satisfied.  In this case nothing needs to be
// changed.  Otherwise one of them is basic, and the other must
// occur only in the expression for that basic error variable.
// Reset the constant in this expression to 0.
void 
ClSimplexSolver::resetStayConstants()
{
#ifndef NO_TRACE
  Tracer TRACER(__FUNCTION__);
  cerr << "()" << endl;
#endif
  vector<ClVariable>::const_iterator 
    itStayPlusErrorVars = my_stayPlusErrorVars.begin();
  vector<ClVariable>::const_iterator 
    itStayMinusErrorVars = my_stayMinusErrorVars.begin();

  for ( ; itStayPlusErrorVars != my_stayPlusErrorVars.end();
	++itStayPlusErrorVars, ++itStayMinusErrorVars )
    {
    ClLinearExpression &expr = rowExpression(*itStayPlusErrorVars);
    if (expr == cleNil() )
      {
      expr = rowExpression(*itStayMinusErrorVars);
      }
    if (expr != cleNil() )
      {
      expr.set_constant(0.0);
      }
    }
}

// Set the external variables known to this solver to their appropriate values.
// Set each external basic variable to its value, and set each
// external parametric variable to 0.  (It isn't clear that we will
// ever have external parametric variables -- every external
// variable should either have a stay on it, or have an equation
// that defines it in terms of other external variables that do have
// stays.  For the moment I'll put this in though.)  Variables that
// are internal to the solver don't actually store values -- their
// values are just implicit in the tableu -- so we don't need to set
// them."
void 
ClSimplexSolver::setExternalVariables()
{
#ifndef NO_TRACE
  Tracer TRACER(__FUNCTION__);
  cerr << "()" << endl;
#endif
  map<ClVariable, ClLinearExpression>::iterator itRowVars;
  map<ClVariable, set<ClVariable> >::iterator itColumnVars;

  itRowVars = my_rows.begin();
  for ( ; itRowVars != my_rows.end() ; ++itRowVars )
    {
    ClVariable &var = (*itRowVars).first;
    ClLinearExpression &expr = (*itRowVars).second;
    if (var.isExternal())
      {
      var.set_value(expr.constant());
      }
    }

  itColumnVars = my_columns.begin();
  for ( ; itColumnVars != my_columns.end(); ++itColumnVars )
    {
    ClVariable &var = (*itColumnVars).first;
    if (var.isExternal())
      {
      var.set_value(0.0);
      }
    }
}


ostream &operator<<(ostream &xo, const vector<ClVariable> &varlist)
{
  vector<ClVariable>::const_iterator it = varlist.begin();
  xo << varlist.size() << ":" << "[ ";
  if (it != varlist.end())
    {
    xo << (*it);
    ++it;
    }
  for (; it != varlist.end(); ++it) 
    {
    xo << ", " << (*it);
    }
  xo << " ]" << endl;
  return xo;
}


ostream &operator<<(ostream &xo, const ClSimplexSolver &tableau)
{
  operator<<(xo,static_cast<ClTableau>(tableau));
  xo << "my_editPlusErrorVars: " << tableau.my_editPlusErrorVars <<endl;
  return xo;
}
