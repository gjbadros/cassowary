// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998, 1999 Alan Borning and Greg Badros
// See ../LICENSE for legal details regarding this software
//
// ClSimplexSolver.cc

#include "debug.h"
#include "ClSimplexSolver.h"
#include "ClErrors.h"
#include "ClVariable.h"
#include "ClPoint.h"
#include "ClSlackVariable.h"
#include "ClObjectiveVariable.h"
#include "ClDummyVariable.h"
#include "auto_ptr.h"
#include <algorithm>
#include <float.h>
#include <strstream>

// Need to delete all expressions
// and all slack and dummy variables
// See newExpression -- all allocation is done in there
ClSimplexSolver::~ClSimplexSolver()
{
#ifdef CL_SOLVER_STATS
  cerr << "_slackCounter == " << _slackCounter
       << "\n_artificialCounter == " << _artificialCounter
       << "\n_dummyCounter == " << _dummyCounter << endl;
  cerr << "stayMinusErrorVars " << _stayMinusErrorVars.size() << ", "
       << "stayPlusErrorVars " << _stayPlusErrorVars.size() << ", "
       << "errorVars " << _errorVars.size() << ", "
       << "markerVars " << _markerVars.size() << endl;
#endif
  // Cannot print *this here, since local ClVariable-s may have been
  // destructed already
}

// Add the constraint cn to the tableau
ClSimplexSolver &
ClSimplexSolver::addConstraint(const ClConstraint &cn)
{
#ifndef CL_NO_TRACE
  Tracer TRACER(__FUNCTION__);
  cerr << "(" << cn << ")" << endl;
#endif
  
  if (cn.isEditConstraint())
    {
    const ClEditConstraint *pcnEdit = dynamic_cast<const ClEditConstraint *>(&cn);
    const ClVariable &v = pcnEdit->variable();
    if (!v.isExternal() ||
        (!FIsBasicVar(v) && !columnsHasKey(v)))
      {
      // we could try to make this case work,
      // but it'd be unnecessarily inefficient --
      // and probably easier for the client application
      // to deal with
      throw ExCLEditMisuse("(ExCLEditMisuse) Edit constaint on variable not in tableau.");
      }
    }

  ClSlackVariable *pclvEplus, *pclvEminus;
  Number prevEConstant;
  ClLinearExpression *pexpr = newExpression(cn, /* output to: */
                                            pclvEplus,pclvEminus,
                                            prevEConstant);
  bool fAddedOkDirectly = false;

  try 
    {
    // If possible add expr directly to the appropriate tableau by
    // choosing a subject for expr (a variable to become basic) from
    // among the current variables in expr.  If this doesn't work use an
    // artificial variable.  After adding expr re-optimize.
    fAddedOkDirectly = tryAddingDirectly(*pexpr);
    }
  catch (ExCLRequiredFailure &error)
    {
#ifndef CL_NO_TRACE
    cerr << "could not add directly -- caught ExCLRequiredFailure error" << endl;
#endif
    removeConstraint(cn);
    throw;
    }

  if (!fAddedOkDirectly)
    { // could not add directly
    ExCLRequiredFailureWithExplanation e;
    if (!addWithArtificialVariable(*pexpr, e))
      {
#ifdef CL_DEBUG_FAILURES
      cerr << "Failed solve! Could not add constraint.\n"
           << *this << endl;
#endif
      removeConstraint(cn);
      if (FIsExplaining())
        throw e;
      else
        throw ExCLRequiredFailure();
      }
    }

  _fNeedsSolving = true;

  if (cn.isEditConstraint())
    {
    const ClEditConstraint *pcnEdit = dynamic_cast<const ClEditConstraint *>(&cn);
    int ccnEdit = _editVarMap.size();
    _editVarMap[&pcnEdit->variable()] = new ClEditInfo(pcnEdit,
                                                       pclvEplus,
                                                       pclvEminus,
                                                       prevEConstant,
                                                       ccnEdit);
    }

  if (_fOptimizeAutomatically)
    {
    optimize(_objective);
    setExternalVariables();
    }

  return *this;
}

// Same as above, but returns false if the constraint cannot be solved
// (i.e., the resulting system would be unsatisfiable)
// The above function "addConstraint" throws an exception in that case
// which may be inconvenient
bool
ClSimplexSolver::addConstraintNoException(const ClConstraint &cn)
{
#ifndef CL_NO_TRACE
  Tracer TRACER(__FUNCTION__);
  cerr << "(" << cn << ")" << endl;
#endif
  try 
    {
    addConstraint(cn);
    return true;
    }
  catch (const ExCLRequiredFailure &e)
    {
    return false;
    }
}

// Add weak stays to the x and y parts of each point. These have
// increasing weights so that the solver will try to satisfy the x
// and y stays on the same point, rather than the x stay on one and
// the y stay on another.
ClSimplexSolver &
ClSimplexSolver::addPointStays(const vector<const ClPoint *> &listOfPoints)
{
#ifndef CL_NO_TRACE
  Tracer TRACER(__FUNCTION__);
#endif

  vector<const ClPoint *>::const_iterator it = listOfPoints.begin();
  double weight = 1.0;
  static const double multiplier = 2.0;
  for ( ; it != listOfPoints.end(); ++it )
    {
    addPointStay((*it)->X(),(*it)->Y(),weight);
    weight *= multiplier;
    }
  return *this;
}

ClSimplexSolver &
ClSimplexSolver::addPointStay(const ClPoint &clp, double weight)
{ 
  addStay(clp.X(),clsWeak(),weight);
  addStay(clp.Y(),clsWeak(),weight);
  return *this;
}


ClSimplexSolver &
ClSimplexSolver::removeEditVarsTo(int n)
{
  while (true)
    {
    ClVarToEditInfoMap::const_iterator it = _editVarMap.begin();
    if (it == _editVarMap.end() || _editVarMap.size() == static_cast<unsigned int>(n))
      break;
    const ClEditInfo *pcei = (*it).second;
    assert(pcei);
    if (pcei->_index >= n)
      {
      const ClEditConstraint *pcnEdit = dynamic_cast<const ClEditConstraint *>(pcei->_pconstraint);
#ifndef CL_NO_TRACE
      cerr << __FUNCTION__ << ": Removing " << pcnEdit->variable() << endl;
#endif
      removeEditVar(pcnEdit->variable());
      }
    }
  return *this;
}


/* A predicate used for remove_if */
class VarInVarSet {
public:
  VarInVarSet(const ClTableauVarSet &clvset) : 
      _set(clvset),
      _setEnd(clvset.end()) 
    { }

  bool operator ()(const ClAbstractVariable *&pclv) const {
    return (_set.find(pclv) != _setEnd);
  }
  
private:
  const ClTableauVarSet &_set;
  const ClTableauVarSet::iterator _setEnd;
};



// Remove the constraint cn from the tableau
// Also remove any error variable associated with cn
ClSimplexSolver &
ClSimplexSolver::removeConstraint(const ClConstraint &cnconst)
{
#ifndef CL_NO_TRACE
  Tracer TRACER(__FUNCTION__);
  cerr << "(" << cnconst << ")" << endl;
#endif

  // We are about to remove a constraint.  There may be some stay
  // constraints that were unsatisfied previously -- if we just
  // removed the constraint these could come into play.  Instead,
  // reset all of the stays so that things should stay where they are
  // at the moment.

  ClConstraint &cn = const_cast<ClConstraint &>(cnconst);

  _fNeedsSolving = true;

  resetStayConstants();

  // remove any error variables from the objective function
  ClLinearExpression *pzRow = rowExpression(_objective);
  ClConstraintToVarSetMap::iterator 
    it_eVars = _errorVars.find(&cn);
  if (it_eVars != _errorVars.end())
    {
    ClTableauVarSet &eVars = (*it_eVars).second;
    ClTableauVarSet::iterator it = eVars.begin();
    for ( ; it != eVars.end(); ++it )
      {
      const ClLinearExpression *pexpr = rowExpression(*(*it));
      if (pexpr == NULL )
	{
	pzRow->addVariable(*(*it),-1.0 * cnconst.strength().symbolicWeight().asDouble(),
			   _objective,*this);
	}
      else
	{ // the error variable was in the basis
	pzRow->addExpression(*pexpr,-1.0 * cnconst.strength().symbolicWeight().asDouble(),
			     _objective,*this);
	}
      }
    }

  ClConstraintToVarMap::iterator 
    it_marker = _markerVars.find(&cn);
  if (it_marker == _markerVars.end())
    { // could not find the constraint
    throw ExCLConstraintNotFound();
    }
  // try to make the marker variable basic if it isn't already
  const ClAbstractVariable &marker = *((*it_marker).second);
  _markerVars.erase(it_marker);
  _constraintsMarked.erase(&marker);
  // delete &marker happens below
#ifndef CL_NO_TRACE
  cerr << "Looking to remove var " << marker << endl;
#endif
  if (!FIsBasicVar(marker))
    { // not in the basis, so need to do some work
    // first choose which variable to move out of the basis
    // only consider restricted basic variables
    ClTableauVarSet &col = _columns[&marker];
    ClTableauVarSet::iterator it_col = col.begin();
#ifndef CL_NO_TRACE
    cerr << "Must pivot -- columns are " << col << endl;
#endif

    const ClAbstractVariable *pexitVar = NULL;
    double minRatio = 0.0;
    for ( ; it_col != col.end(); ++it_col) 
      {
      const ClAbstractVariable *pv = *it_col;
      if (pv->isRestricted() )
	{
	const ClLinearExpression *pexpr = rowExpression(*pv);
	assert(pexpr != NULL );
	Number coeff = pexpr->coefficientFor(marker);
#ifndef CL_NO_TRACE
	cerr << "Marker " << marker << "'s coefficient in " << *pexpr << " is "
	     << coeff << endl;
#endif
	// only consider negative coefficients
	if (coeff < 0.0) 
	  {
	  Number r = - pexpr->constant() / coeff;
	  if (pexitVar == NULL || r < minRatio)
	    {
	    minRatio = r;
	    pexitVar = pv;
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
    if (pexitVar == NULL ) 
      {
#ifndef CL_NO_TRACE
      cerr << "pexitVar is still NULL" << endl;
#endif
      it_col = col.begin();
      for ( ; it_col != col.end(); ++it_col) 
	{
	const ClAbstractVariable *pv = *it_col;
	if (pv->isRestricted() )
	  {
	  const ClLinearExpression *pexpr = rowExpression(*pv);
	  assert(pexpr != NULL);
	  Number coeff = pexpr->coefficientFor(marker);
	  Number r = pexpr->constant() / coeff;
	  if (pexitVar == NULL || r < minRatio)
	    {
	    minRatio = r;
	    pexitVar = pv;
	    }
	  }
	}
      }

    if (pexitVar == NULL)
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
	pexitVar = *(col.begin());
	}
      }
    
    if (pexitVar != NULL)
      {
      pivot(marker,*pexitVar);
      }
    }
  
  if (FIsBasicVar(marker))
    {
    ClLinearExpression *pexpr = removeRow(marker);
#ifndef CL_NO_TRACE
    cerr << "delete@ " << pexpr << endl;
#endif
    delete pexpr;
    }

  // Delete any error variables.  If cn is an inequality, it also
  // contains a slack variable; but we use that as the marker variable
  // and so it has been deleted when we removed its row.
  if (it_eVars != _errorVars.end())
    {
    ClTableauVarSet &eVars = (*it_eVars).second;
    ClTableauVarSet::iterator it = eVars.begin();
    for ( ; it != eVars.end(); ++it )
      {
      const ClAbstractVariable *pv = (*it);
      if (*pv != marker)
	{
	removeColumn(*pv);
	delete pv;
	}
      }
    }

  if (cn.isStayConstraint())
    {
    // iterate over the stay{Plus,Minus}ErrorVars and remove those
    // variables v in those vectors that are also in set eVars
    if (it_eVars != _errorVars.end())
      {
      ClTableauVarSet &eVars = (*it_eVars).second;
      _stayPlusErrorVars
        .erase(remove_if(_stayPlusErrorVars.begin(),_stayPlusErrorVars.end(),
                         VarInVarSet(eVars)),
               _stayPlusErrorVars.end());
      _stayMinusErrorVars
        .erase(remove_if(_stayMinusErrorVars.begin(),_stayMinusErrorVars.end(),
                         VarInVarSet(eVars)),
               _stayMinusErrorVars.end());
      }
    }
  else if (cn.isEditConstraint())
    {
    const ClEditConstraint *pcnEdit = dynamic_cast<const ClEditConstraint *>(&cn);
    const ClVariable *pclv = &pcnEdit->variable();
    delete _editVarMap[pclv];
    _editVarMap.erase(pclv);
    }

  if (it_eVars != _errorVars.end())
    {
    // FIXGJB
    // This code is not needed since the variables are deleted
    // when they are removed from the row --
    // leaving it in results in double deletions
    // delete the constraint's error variables
    //    ClTableauVarSet &evars_set = (*it_eVars).second;
    //    ClTableauVarSet::const_iterator it_set = evars_set.begin();
    //    for ( ; it_set != evars_set.end(); ++it_set)
    //      {
    //      delete *it_set;
    //      }
    _errorVars.erase(it_eVars);
    }
  delete &marker;

  if (_fOptimizeAutomatically)
    {
    optimize(_objective);
    setExternalVariables();
    }

  return *this;
}


  // Same as above, but returns false if the constraint dne
  // The above function "removeConstraint" throws an exception in that case
  // which may be inconvenient
bool
ClSimplexSolver::removeConstraintNoException(const ClConstraint &cn)
{
#ifndef CL_NO_TRACE
  Tracer TRACER(__FUNCTION__);
  cerr << "(" << cn << ")" << endl;
#endif
  try 
    {
    removeConstraint(cn);
    return true;
    }
  catch (const ExCLConstraintNotFound &e)
    {
    return false;
    }
}


// Re-initialize this solver from the original constraints, thus
// getting rid of any accumulated numerical problems.  (Actually,
// Alan hasn't observed any such problems yet, but here's the method
// anyway.)
void 
ClSimplexSolver::reset()
{
#ifndef CL_NO_TRACE
  Tracer TRACER(__FUNCTION__);
  cerr << "()" << endl;
#endif
  // FIXGJB  -- can postpone writing this for a while
  // gotta be careful, though, as it's a likely place for
  // a memory leak to sneak in
  assert(false);
}


// Re-solve the cuurent collection of constraints, given the new
// values for the edit variables that have already been
// suggested (see suggestValue() method)
void 
ClSimplexSolver::resolve()
{ // CODE DUPLICATED ABOVE
#ifndef CL_NO_TRACE
  Tracer TRACER(__FUNCTION__);
#endif
  dualOptimize();
  setExternalVariables();
  _infeasibleRows.clear();
  resetStayConstants();
}

ClSimplexSolver &
ClSimplexSolver::suggestValue(ClVariable &v, Number x)
{
#ifndef CL_NO_TRACE
  Tracer TRACER(__FUNCTION__);
#endif
  ClVarToEditInfoMap::const_iterator itEditVarMap = _editVarMap.find(&v);
  if (itEditVarMap == _editVarMap.end())
    {
#ifndef CL_NO_IO
    strstream ss;
    ss << "suggestValue for variable " << v << ", but var is not an edit variable" << ends;
    throw ExCLEditMisuse(ss.str());
#else
    throw ExCLEditMisuse(v.name().c_str());
#endif
    }
  ClEditInfo *pcei = (*itEditVarMap).second;
  const ClAbstractVariable *pvarErrorPlus = pcei->_pclvEditPlus;
  const ClAbstractVariable *pvarErrorMinus = pcei->_pclvEditMinus;
  Number delta = x - pcei->_prevEditConstant;
  pcei->_prevEditConstant = x;
  deltaEditConstant(delta,*pvarErrorPlus,*pvarErrorMinus);
  return *this;
}

// Re-solve the cuurent collection of constraints, given the new
// values for the edit variables that have already been
// suggested (see suggestValue() method)
// This is not guaranteed to work if you remove an edit constraint
// from the middle of the edit constraints you added
// (e.g., edit A, edit B, edit C, remove B -> this will fail!)
// DEPRECATED
void 
ClSimplexSolver::resolve(const vector<Number> &newEditConstants)
{
  ClVarToEditInfoMap::iterator it = _editVarMap.begin();
  for (; it != _editVarMap.end(); ++it)
    {
    ClVariable *pclv = const_cast<ClVariable *>((*it).first);
    ClEditInfo *pcei = (*it).second;
    unsigned i = pcei->_index;
    if (i < newEditConstants.size())
      {
      suggestValue(*pclv,newEditConstants[i]);
      }
    }
}


//// protected

// Add the constraint expr=0 to the inequality tableau using an
// artificial variable.  To do this, create an artificial variable
// av and add av=expr to the inequality tableau, then make av be 0.
// (Raise an exception if we can't attain av=0 -- and prepare explanation)
bool
ClSimplexSolver::addWithArtificialVariable(ClLinearExpression &expr,
                                           ExCLRequiredFailureWithExplanation &e)
{
#ifndef CL_NO_TRACE
  Tracer TRACER(__FUNCTION__);
  cerr << "(" << expr << ")" << endl;
#endif
  
  // Allocate the objects on the heap because the objects
  // will remain in the tableau if we throw an exception,
  // and that will result in the destructor cleaning up
  // after us
  ClSlackVariable *pav = new ClSlackVariable(++_artificialCounter,"a");
  ClObjectiveVariable *paz = new ClObjectiveVariable("az");
  ClLinearExpression *pazRow = new ClLinearExpression(expr);
  // the artificial objective is av, which we know is equal to expr
  // (which contains only parametric variables)

#ifdef CL_FIND_LEAK
  cerr << "aC = " << _artificialCounter
       << "\nDeletes = " << _cArtificialVarsDeleted << endl;
#endif
#ifndef CL_NO_TRACE
  cerr << __FUNCTION__ << " before addRow-s:\n"
       << (*this) << endl;
#endif

  // the artificial objective is av, which we know is equal to expr
  // (which contains only parametric variables)
  
  // objective is treated as a row in the tableau,
  // so do the substitution for its value (we are minimizing
  // the artificial variable)
  // this row will be removed from the tableau after optimizing
  addRow(*paz,*pazRow);
  
  // now add the normal row to the tableau -- when artifical
  // variable is minimized to 0 (if possible)
  // this row remains in the tableau to maintain the constraint
  // we are trying to add
  addRow(*pav,expr);

#ifndef CL_NO_TRACE
  cerr << __FUNCTION__ << " after addRow-s:\n"
       << (*this) << endl;
#endif

  // try to optimize az to 0
  // note we are *not* optimizing the real objective, but optimizing
  // the artificial objective to see if the error in the constraint
  // we are adding can be set to 0
  optimize(*paz);

  // Careful, we want to get the expression that is in
  // the tableau, not the one we initialized it with!
  ClLinearExpression *pazTableauRow = rowExpression(*paz);
#ifndef CL_NO_TRACE
  cerr << "pazTableauRow->constant() == " << pazTableauRow->constant() << endl;
#endif

  // Check that we were able to make the objective value 0
  // If not, the original constraint was not satisfiable
  if (!clApprox(pazTableauRow->constant(),0.0))
    {
    buildExplanation(e, paz, pazTableauRow);
    // remove the artificial objective row that we just
    // added temporarily
    delete removeRow(*paz);
    // and delete the artificial objective variable that we also added above
    delete paz;
    return false;
    }

  // see if av is a basic variable
  const ClLinearExpression *pe = rowExpression(*pav);
  if (pe != NULL)
    {
    // FIXGJB: do we ever even get here?
    // Find another variable in this row and pivot, so that av becomes parametric
    // If there isn't another variable in the row then 
    // the tableau contains the equation av = 0  -- just delete av's row
    if (pe->isConstant())
      {
      // FIXGJB: do we ever get here?
      assert(clApprox(pe->constant(),0.0));
      delete removeRow(*pav);
      // remove the temporary objective function
      // FIXGJB may need this too: delete removeRow(*paz);
      delete pav;
#ifdef CL_FIND_LEAK
      ++_cArtificialVarsDeleted;
#endif
      return true;
      }
    const ClAbstractVariable *pentryVar = pe->anyPivotableVariable();
    assert(pentryVar);  // this assertion may be bogus --12/03/98 gjb
    if (!pentryVar)
      {
      buildExplanation(e, pav, pe);
      return false; /* required failure */
      }
    pivot(*pentryVar, *pav);
    }
  // now av should be parametric
  assert(rowExpression(*pav) == NULL);
  removeColumn(*pav);
  delete pav;
#ifdef CL_FIND_LEAK
  ++_cArtificialVarsDeleted;
#endif
  // remove the temporary objective function
  delete removeRow(*paz);
  delete paz;
  return true;
}


// Using the given equation (av = cle) build an explanation which
// implicates all constraints used to construct the equation. That
// is, everything for which the variables in the equation are markers.
void ClSimplexSolver::buildExplanation(ExCLRequiredFailureWithExplanation & e,
                                       const ClAbstractVariable *pav, 
                                       const ClLinearExpression *pcle)
{
  ClVarToConstraintMap::iterator it_cn;
  if (pav != NULL) 
    {
    it_cn = _constraintsMarked.find(pav);
    if (it_cn != _constraintsMarked.end()) 
      {
      e.addConstraint((*it_cn).second);
      }
    }
  
  assert(pcle != NULL);
  
  const ClVarToNumberMap & terms = pcle->terms();
  ClVarToNumberMap::const_iterator it_term;
  for (it_term = terms.begin(); it_term != terms.end(); it_term++)
    {
    it_cn = _constraintsMarked.find((*it_term).first);
    if (it_cn != _constraintsMarked.end()) 
      {
      e.addConstraint((*it_cn).second);
      }
    }
}



// We are trying to add the constraint expr=0 to the appropriate
// tableau.  Try to add expr directly to the tableaus without
// creating an artificial variable.  Return true if successful and
// false if not.
bool 
ClSimplexSolver::tryAddingDirectly(ClLinearExpression &expr) 
{
#ifndef CL_NO_TRACE
  Tracer TRACER(__FUNCTION__);
  cerr << "(" << expr << ")" << endl;
#endif
  const ClAbstractVariable *psubject = chooseSubject(expr);
  if (psubject == NULL )
    {
#ifndef CL_NO_TRACE
    cerr << "- returning false" << endl;
#endif
    return false;
    }
  expr.newSubject(*psubject);
  if (columnsHasKey(*psubject))
    {
    substituteOut(*psubject,expr);
    }
  addRow(*psubject,expr);
#ifndef CL_NO_TRACE
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
// 'newExpression:', which is called before this method.
const ClAbstractVariable *
ClSimplexSolver::chooseSubject(ClLinearExpression &expr)
{
#ifndef CL_NO_TRACE
  Tracer TRACER(__FUNCTION__);
  cerr << "(" << expr << ")" << endl;
#endif
  const ClAbstractVariable *psubject = NULL; // the current best subject, if any

  // true iff we have found a subject that is an unrestricted variable
  bool foundUnrestricted = false; 

  // true iff we have found a restricted variable that is new to the
  // solver (except for being in the obj. function) and that has a
  // negative coefficient
  bool foundNewRestricted = false;

  const ClVarToNumberMap &terms = expr.terms();
  ClVarToNumberMap::const_iterator it = terms.begin();
  for ( ; it != terms.end(); ++it )
    {
    const ClAbstractVariable *pv = (*it).first;
    Number c = (*it).second;

    if (foundUnrestricted)
      {
      // We have already found an unrestricted variable.  The only
      // time we will want to use v instead of the current choice
      // 'subject' is if v is unrestricted and new to the solver and
      // 'subject' isn't new.  If this is the case just pick v
      // immediately and return.
      if (!pv->isRestricted())
	{
	if (!columnsHasKey(*pv))
	  return pv;
	}
      }
    else
      { // we haven't found an restricted variable yet
      if (pv->isRestricted())
	{
	// v is restricted.  If we have already found a suitable
	// restricted variable just stick with that.  Otherwise, if v
	// is new to the solver and has a negative coefficient pick
	// it.  Regarding being new to the solver -- if the variable
	// occurs only in the objective function we regard it as being
	// new to the solver, since error variables are added to the
	// objective function when we make the expression.  We also
	// never pick a dummy variable here.
	if (!foundNewRestricted && !pv->isDummy() && c < 0.0)
	  {
	  const ClTableauColumnsMap &col = columns();
	  ClTableauColumnsMap::const_iterator it_col = col.find(pv);
	  if (it_col == col.end() || 
	      ( col.size() == 1 && columnsHasKey(_objective) ) )
	    {
	    psubject = pv;
	    foundNewRestricted = true;
	    }
	  }
	}
      else
	{
	// v is unrestricted.  
	// If v is also new to the solver just pick it now
	psubject = pv;
	foundUnrestricted = true;
	}
      }
    }
  if (psubject)
    return psubject;

  // subject is nil. 
  // Make one last check -- if all of the variables in expr are dummy
  // variables, then we can pick a dummy variable as the subject
  Number coeff = 0;
  it = terms.begin();
  for ( ; it != terms.end(); ++it )
    {
    const ClAbstractVariable *pv = (*it).first;
    Number c = (*it).second;
    if (!pv->isDummy())
      return NULL; // nope, no luck
    // if v is new to the solver, tentatively make it the subject
    if (!columnsHasKey(*pv))
      {
      psubject = pv;
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
#ifdef CL_DEBUG_FAILURES
    cerr << "required failure in choose subject:\n"
         << *this << endl;
#endif
    if (FIsExplaining()) 
      {
      ExCLRequiredFailureWithExplanation e;
      buildExplanation(e, NULL, &expr);
      throw e;
      }
    else
      throw ExCLRequiredFailure();
    }
  if (coeff > 0.0)
    {
    expr.multiplyMe(-1);
    }
  return psubject;
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
// those.  See the UIST paper for details.  
// (This comment was for resetEditConstants(), but that is now
// gone since it was part of the screwey vector-based interface
// to resolveing. --02/15/99 gjb)
void 
ClSimplexSolver::deltaEditConstant(Number delta,
				   const ClAbstractVariable &plusErrorVar,
				   const ClAbstractVariable &minusErrorVar)
{
#ifndef CL_NO_TRACE
  Tracer TRACER(__FUNCTION__);
  cerr << "(" << delta << ", " << plusErrorVar << ", " << minusErrorVar << ")" << endl;
#endif
  // first check if the plusErrorVar is basic
  ClLinearExpression *pexprPlus = rowExpression(plusErrorVar);
  if (pexprPlus != NULL )
    {
    pexprPlus->incrementConstant(delta);
    // error variables are always restricted
    // so the row is infeasible if the constant is negative
    if (pexprPlus->constant() < 0.0)
      {
      _infeasibleRows.insert(&plusErrorVar);
      }
    return;
    }
  // check if minusErrorVar is basic
  ClLinearExpression *pexprMinus = rowExpression(minusErrorVar);
  if (pexprMinus != NULL)
    {
    pexprMinus->incrementConstant(-delta);
    if (pexprMinus->constant() < 0.0)
      {
      _infeasibleRows.insert(&minusErrorVar);
      }
    return;
    }
  // Neither is basic.  So they must both be nonbasic, and will both
  // occur in exactly the same expressions.  Find all the expressions
  // in which they occur by finding the column for the minusErrorVar
  // (it doesn't matter whether we look for that one or for
  // plusErrorVar).  Fix the constants in these expressions.
  ClTableauVarSet &columnVars = _columns[&minusErrorVar];
  ClTableauVarSet::iterator it = columnVars.begin();
  for (; it != columnVars.end(); ++it)
    {
    const ClAbstractVariable *pbasicVar = *it;
    ClLinearExpression *pexpr = rowExpression(*pbasicVar);
    assert(pexpr != NULL );
    double c = pexpr->coefficientFor(minusErrorVar);
    pexpr->incrementConstant(c*delta);
    if (pbasicVar->isRestricted() && pexpr->constant() < 0.0)
      {
      _infeasibleRows.insert(pbasicVar);
      }
    }
}
  
// We have set new values for the constants in the edit constraints.
// Re-optimize using the dual simplex algorithm.
void 
ClSimplexSolver::dualOptimize()
{
#ifndef CL_NO_TRACE
  Tracer TRACER(__FUNCTION__);
  cerr << "()" << endl;
#endif
  const ClLinearExpression *pzRow = rowExpression(_objective);
  // need to handle infeasible rows
  while (!_infeasibleRows.empty())
    {
    ClTableauVarSet::iterator it_exitVar = _infeasibleRows.begin();
    const ClAbstractVariable *pexitVar = *it_exitVar;
    _infeasibleRows.erase(it_exitVar);
    const ClAbstractVariable *pentryVar = NULL;
    // exitVar might have become basic after some other pivoting
    // so allow for the case of its not being there any longer
    ClLinearExpression *pexpr = rowExpression(*pexitVar);
    if (pexpr != NULL )
      {
      // make sure the row is still not feasible
      if (pexpr->constant() < 0.0)
	{
	double ratio = DBL_MAX;
	double r;
	ClVarToNumberMap &terms = pexpr->terms();
	ClVarToNumberMap::iterator it = terms.begin();
	for ( ; it != terms.end(); ++it )
	  {
	  const ClAbstractVariable *pv = (*it).first;
	  Number c = (*it).second;
	  if (c > 0.0 && pv->isPivotable())
	    {
	    Number zc = pzRow->coefficientFor(*pv);
	    r = zc/c; // FIXGJB r:= zc/c or zero, as ClSymbolicWeight-s
	    if (r < ratio)
	      {
	      pentryVar = pv;
	      ratio = r;
	      }
	    }
	  }
	if (ratio == DBL_MAX)
	  {
          strstream ss;
	  ss << "ratio == nil (DBL_MAX)" << ends;
	  throw ExCLInternalError(ss.str());
	  }
	pivot(*pentryVar,*pexitVar);
	}
      }
    }
}

// Make a new linear expression representing the constraint cn,
// replacing any basic variables with their defining expressions.
// Normalize if necessary so that the constant is non-negative.  If
// the constraint is non-required give its error variables an
// appropriate weight in the objective function.
ClLinearExpression *
ClSimplexSolver::newExpression(const ClConstraint &cn,
                               ClSlackVariable *&pclvEplus,
                               ClSlackVariable *&pclvEminus,
                               Number &prevEConstant)
{
#ifndef CL_NO_TRACE
  Tracer TRACER(__FUNCTION__);
  cerr << "(" << cn << ")" << endl;
  cerr << "cn.isInequality() == " << cn.isInequality() << endl;
  cerr << "cn.isRequired() == " << cn.isRequired() << endl;
#endif
  const ClLinearExpression &cnExpr = cn.expression();
  auto_ptr<ClLinearExpression> pexpr ( new ClLinearExpression(cnExpr.constant()) );
  auto_ptr<ClSlackVariable> pslackVar;
  auto_ptr<ClDummyVariable> pdummyVar;
  auto_ptr<ClSlackVariable> peminus(0);
  auto_ptr<ClSlackVariable> peplus(0);
  const ClVarToNumberMap &cnTerms = cnExpr.terms();
  ClVarToNumberMap::const_iterator it = cnTerms.begin();
  for ( ; it != cnTerms.end(); ++it)
    {
    const ClAbstractVariable *pv = (*it).first;
    Number c = (*it).second;
    const ClLinearExpression *pe = rowExpression(*pv);
    if (pe == NULL)
      {
      pexpr->addVariable(*pv,c);
      }
    else
      {
      pexpr->addExpression(*pe,c);
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
    ++_slackCounter;
    ReinitializeAutoPtr(pslackVar,new ClSlackVariable (_slackCounter, "s"));
    pexpr->setVariable(*pslackVar,-1);
    // index the constraint under its slack variable and vice-versa
    _markerVars[&cn] = pslackVar.get();
    _constraintsMarked[pslackVar.get()] = &cn;
    
    if (!cn.isRequired())
      {
      ++_slackCounter;
      ReinitializeAutoPtr(peminus,new ClSlackVariable (_slackCounter, "em"));
      pexpr->setVariable(*peminus,1.0);
      // add emnius to the objective function with the appropriate weight
      ClLinearExpression *pzRow = rowExpression(_objective);
      // FIXGJB: pzRow->addVariable(eminus,cn.strength().symbolicWeight() * cn.weight());
      ClSymbolicWeight sw = cn.strength().symbolicWeight().times(cn.weight());
      pzRow->setVariable(*peminus,sw.asDouble());
      _errorVars[&cn].insert(peminus.get());
      noteAddedVariable(*peminus,_objective);
      }
    }
  else
    { // cn is an equality
    if (cn.isRequired())
      {
      // Add a dummy variable to the expression to serve as a marker
      // for this constraint.  The dummy variable is never allowed to
      // enter the basis when pivoting.
      ++_dummyCounter;
      ReinitializeAutoPtr(pdummyVar,new ClDummyVariable (_dummyCounter, "d"));
      pexpr->setVariable(*pdummyVar,1.0);
      _markerVars[&cn] = pdummyVar.get();
      _constraintsMarked[pdummyVar.get()] = &cn;
#ifndef CL_NO_TRACE
      cerr << "Adding dummyVar == d" << _dummyCounter << endl;
#endif
      }
    else
      {
      // cn is a non-required equality.  Add a positive and a negative
      // error variable, making the resulting constraint 
      //       expr = eplus - eminus, 
      // in other words:  expr-eplus+eminus=0
      ++_slackCounter;
      ReinitializeAutoPtr(peplus,new ClSlackVariable (_slackCounter, "ep"));
      ReinitializeAutoPtr(peminus,new ClSlackVariable (_slackCounter, "em"));

      pexpr->setVariable(*peplus,-1.0);
      pexpr->setVariable(*peminus,1.0);
      // index the constraint under one of the error variables
      _markerVars[&cn] = peplus.get();
      _constraintsMarked[peplus.get()] = &cn;

      ClLinearExpression *pzRow = rowExpression(_objective);
      // FIXGJB: pzRow->addVariable(eplus,cn.strength().symbolicWeight() * cn.weight());
      ClSymbolicWeight sw = cn.strength().symbolicWeight().times(cn.weight());
      double swCoeff = sw.asDouble();
#ifndef CL_NO_TRACE
      if (swCoeff == 0) 
	{
	cerr << "sw == " << sw << endl
	     << "cn == " << cn << endl;
	cerr << "adding " << *peplus << " and " << *peminus 
	     << " with swCoeff == " << swCoeff << endl;
	}
#endif      
      pzRow->setVariable(*peplus,swCoeff);
      noteAddedVariable(*peplus,_objective);
      // FIXGJB: pzRow->addVariable(eminus,cn.strength().symbolicWeight() * cn.weight());
      pzRow->setVariable(*peminus,swCoeff);
      noteAddedVariable(*peminus,_objective);
      _errorVars[&cn].insert(peminus.get());
      _errorVars[&cn].insert(peplus.get());
      if (cn.isStayConstraint()) 
	{
	_stayPlusErrorVars.push_back(peplus.get());
	_stayMinusErrorVars.push_back(peminus.get());
	}
      else if (cn.isEditConstraint())
	{
        pclvEplus = peplus.get();
        pclvEminus = peminus.get();
        prevEConstant = cnExpr.constant();
	}
      }
    }

  // the constant in the expression should be non-negative.
  // If necessary normalize the expression by multiplying by -1
  if (pexpr->constant() < 0)
    {
#ifndef CL_NO_TRACE
    cerr << "newExpression's constant is " << pexpr->constant() << ", < 0, so flipping" << endl;
#endif
    pexpr->multiplyMe(-1);
    }
#ifndef CL_NO_TRACE
  cerr << "- returning " << *pexpr << endl;
#endif
  // Terrible name -- release() does *not* delete the object,
  // only makes sure that the destructor won't delete the object
  // (it releases the auto_ptr from the responsibility of deleting the object)
  pslackVar.release();
  pdummyVar.release();
  peminus.release();
  peplus.release();
  return pexpr.release();
}

// Minimize the value of the objective.  (The tableau should already
// be feasible.)
void 
ClSimplexSolver::optimize(const ClObjectiveVariable &zVar)
{
#ifndef CL_NO_TRACE
  Tracer TRACER(__FUNCTION__);
  cerr << "(" << zVar << ")\n"
       << *this << endl;
#endif
  ClLinearExpression *pzRow = rowExpression(zVar);
  assert(pzRow != NULL);
  const ClAbstractVariable *pentryVar = NULL;
  const ClAbstractVariable *pexitVar = NULL;
  while (true)
    {
    Number objectiveCoeff = 0;
    // Find the most negative coefficient in the objective function
    // (ignoring the non-pivotable dummy variables).  If all
    // coefficients are positive we're done
    ClVarToNumberMap &terms = pzRow->terms();
    ClVarToNumberMap::iterator it = terms.begin();
    for (; it != terms.end(); ++it)
      {
      const ClAbstractVariable *pv = (*it).first;
      Number c = (*it).second;
      if (pv->isPivotable() && c < objectiveCoeff)
	{
	objectiveCoeff = c;
	pentryVar = pv;
	}
      }
    // if all coefficients were positive (or if the objective
    // function has no pivotable variables)
    // we are at an optimum
    if (objectiveCoeff >= -_epsilon)
      return;
#ifndef CL_NO_TRACE
    cerr << "*pentryVar == " << *pentryVar << ", "
	 << "objectiveCoeff == " << objectiveCoeff
	 << endl;
#endif

    // choose which variable to move out of the basis
    // Only consider pivotable basic variables
    // (i.e. restricted, non-dummy variables)
    double minRatio = DBL_MAX;
    ClTableauVarSet &columnVars = _columns[pentryVar];
    ClTableauVarSet::iterator it_rowvars = columnVars.begin();
    Number r = 0.0;
    for (; it_rowvars != columnVars.end(); ++it_rowvars)
      {
      const ClAbstractVariable *pv = *it_rowvars;
#ifndef CL_NO_TRACE
      cerr << "Checking " << *pv << endl;
#endif
      if (pv->isPivotable()) 
	{
	const ClLinearExpression *pexpr = rowExpression(*pv);
	Number coeff = pexpr->coefficientFor(*pentryVar);
	// only consider negative coefficients
	if (coeff < 0.0)
	  {
	  r = - pexpr->constant() / coeff;
	  if (r < minRatio)
	    {
#ifndef CL_NO_TRACE
	    cerr << "New minRatio == " << r << endl;
#endif
	    minRatio = r;
	    pexitVar = pv;
	    }
	  }
	}
      }
    // If minRatio is still nil at this point, it means that the
    // objective function is unbounded, i.e. it can become
    // arbitrarily negative.  This should never happen in this
    // application.
    if (minRatio == DBL_MAX)
      {
      strstream ss;
      ss << "objective function is unbounded!" << ends;
      throw ExCLInternalError(ss.str());
      }
    pivot(*pentryVar, *pexitVar);
#ifndef CL_NO_TRACE
    cerr << "After optimize:\n"
         << *this << endl;
#endif
    }
}

// Do a pivot.  Move entryVar into the basis (i.e. make it a basic variable),
// and move exitVar out of the basis (i.e., make it a parametric variable)
void 
ClSimplexSolver::pivot(const ClAbstractVariable &entryVar, const ClAbstractVariable &exitVar)
{
#ifndef CL_NO_TRACE
  Tracer TRACER(__FUNCTION__);
  cerr << "(" << entryVar << ", " << exitVar << ")" << endl;
#endif

  // the entryVar might be non-pivotable if we're doing a removeConstraint --
  // otherwise it should be a pivotable variable -- enforced at call sites,
  // hopefully
  
  // expr is the expression for the exit variable (about to leave the basis) -- 
  // so that the old tableau includes the equation:
  //   exitVar = expr
  ClLinearExpression *pexpr = removeRow(exitVar);

  // Compute an expression for the entry variable.  Since expr has
  // been deleted from the tableau we can destructively modify it to
  // build this expression.
  pexpr->changeSubject(exitVar,entryVar);
  substituteOut(entryVar,*pexpr);

  if (entryVar.isExternal())
    {
    // entry var is no longer a parametric variable since we're moving
    // it into the basis
    _externalParametricVars.erase(static_cast<const ClVariable *>(&entryVar));
    }
  addRow(entryVar,*pexpr);
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
#ifndef CL_NO_TRACE
  Tracer TRACER(__FUNCTION__);
  cerr << "()" << endl;
#endif
  ClVarVector::const_iterator 
    itStayPlusErrorVars = _stayPlusErrorVars.begin();
  ClVarVector::const_iterator 
    itStayMinusErrorVars = _stayMinusErrorVars.begin();

  for ( ; itStayPlusErrorVars != _stayPlusErrorVars.end();
	++itStayPlusErrorVars, ++itStayMinusErrorVars )
    {
    ClLinearExpression *pexpr = rowExpression(*(*itStayPlusErrorVars));
    if (pexpr == NULL )
      {
      pexpr = rowExpression(*(*itStayMinusErrorVars));
      }
    if (pexpr != NULL)
      {
      pexpr->set_constant(0.0);
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
#ifndef CL_NO_TRACE
  Tracer TRACER(__FUNCTION__);
  cerr << "()\n"
       << *this << endl;
#endif

  // FIXGJB -- oughta check some invariants here

  // Set external parametric variables first
  // in case I've screwed up
  ClExternalVarSet::iterator itParVars = _externalParametricVars.begin();
  for ( ; itParVars != _externalParametricVars.end(); ++itParVars )
    {
    ClVariable *pv = const_cast<ClVariable *>(*itParVars);
    // skip it if it is basic -- change_value is virtual
    // so don't want to call it twice
    if (FIsBasicVar(*pv))
      {
#ifndef CL_NO_IO
      // WARNING
      cerr << __FUNCTION__ << "Error: variable " << *pv 
           << " in _externalParametricVars is basic" << endl;
      cerr << "Row is: " << *rowExpression(*pv) << endl;
#endif
      continue;
      }
    ChangeClv(pv,0.0);
    }

  // Only iterate over the rows w/ external variables
  ClExternalVarSet::iterator itRowVars = _externalRows.begin();
  for ( ; itRowVars != _externalRows.end() ; ++itRowVars )
    {
    ClVariable *pv = const_cast<ClVariable *>(*itRowVars);
    ClLinearExpression *pexpr = rowExpression(*pv);
    ChangeClv(pv,pexpr->constant());
    }

  _fNeedsSolving = false;
  if (_pfnResolveCallback)
    _pfnResolveCallback(this);
}

#ifndef CL_NO_IO
ostream &
printTo(ostream &xo, const ClVarVector &varlist)
{
  ClVarVector::const_iterator it = varlist.begin();
  xo << varlist.size() << ":" << "[ ";
  if (it != varlist.end())
    {
    xo << *(*it);
    ++it;
    }
  for (; it != varlist.end(); ++it) 
    {
    xo << ", " << *(*it);
    }
  xo << " ]";
  return xo;
}


ostream &operator<<(ostream &xo, const ClVarVector &varlist)
{
  return printTo(xo,varlist);
}


ostream &
ClSimplexSolver::printOn(ostream &xo) const
{
  super::printOn(xo);

  xo << "_stayPlusErrorVars: "
     << _stayPlusErrorVars << endl;
  xo << "_stayMinusErrorVars: "
     << _stayMinusErrorVars << endl;
  return xo;
}

ostream &
ClSimplexSolver::printInternalInfo(ostream &xo) const
{
  super::printInternalInfo(xo);
  xo << "; edvars: " << _editVarMap.size();
  return xo;
}


ostream &operator<<(ostream &xo, const ClSimplexSolver &clss)
{
  return clss.printOn(xo);
}

#endif

bool 
ClSimplexSolver::FIsConstraintSatisfied(const ClConstraint &cn) const
{
  ClConstraintToVarMap::const_iterator it_marker = _markerVars.find(&cn);
  if (it_marker == _markerVars.end())
    { // could not find the constraint
    throw ExCLConstraintNotFound();
    }

#ifndef CL_NO_IO
  bool fCnsays = cn.FIsSatisfied();
#endif

  ClConstraintToVarSetMap::const_iterator it_eVars = _errorVars.find(&cn);

  if (it_eVars != _errorVars.end())
    {
    const ClTableauVarSet &eVars = (*it_eVars).second;
    ClTableauVarSet::const_iterator it = eVars.begin();
    for ( ; it != eVars.end(); ++it )
      {
      const ClLinearExpression *pexpr = rowExpression(*(*it));
      if (pexpr != NULL && !clApprox(pexpr->constant(),0.0))
        {
#ifndef CL_NO_IO
        if (fCnsays)
          cerr << __FUNCTION__ << ": constraint says satisfiable, but solver does not" << endl;
#endif
        return false;
        }
      }
    }

#ifndef CL_NO_IO
  if (!fCnsays)
    cerr << __FUNCTION__ << ": solver says satisfiable, but constraint does not" << endl;
#endif
  return true;
}
