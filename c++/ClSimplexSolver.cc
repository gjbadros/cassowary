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

ClSimplexSolver::ClSimplexSolver() :
  slackCounter(0),
  artificialCounter(0),
  dummyCounter(0)
{ }


// Add the constraint cn to the tableau
void 
ClSimplexSolver::addConstraint(const ClLinearConstraint &cn)
{
  ClLinearExpression expr = cn.expression();

  // If possible add expr directly to the appropriate tableau by
  // choosing a subject for expr (a variable to become basic) from
  // among the current variables in expr.  If this doesn't work use an
  // artificial variable.  After adding expr re-optimize.
  if (!tryAddingDirectly(expr))
    { // could not add directly
    addWithArtificialVariable(expr);
    }
  optimize(objective);
  setExternalVariables();
}

// Add weak stays to the x and y parts of each point. These have
// increasing weights so that the solver will try to satisfy the x
// and y stays on the same point, rather than the x stay on one and
// the y stay on another.
void 
ClSimplexSolver::addPointStays(const vector<ClPoint> &listOfPoints)
{
  
  
}

void 
ClSimplexSolver::addPointStay(const ClVariable &vx, const ClVariable &vy, double weight)
{
  
}

// Add a stay of the given strength (default to weak) of v to the tableau
void 
ClSimplexSolver::addStay(const ClVariable &v, const ClStrength &strength ), double weight )
{

}

// Remove the constraint cn from the tableau
// Also remove any error variable associated with cn
void 
ClSimplexSolver::removeConstraint(const ClConstraint &cn)
{
  
}

// Re-initialize this solver from the original constraints, thus
// getting rid of any accumulated numerical problems.  (Actually,
// Alan hasn't observed any such problems yet, but here's the method
// anyway.)
void 
ClSimplexSolver::reset()
{
  
}

// Re-solve the current collection of constraints for new values for
// the constants of the edit variables.
void 
ClSimplexSolver::resolve(const vector<double> &newEditConstants)
{
  
}

//// protected

// Add the constraint expr=0 to the inequality tableau using an
// artificial variable.  To do this, create an artificial variable
// av and add av=expr to the inequality tableau, then make av be 0.
// (Raise an exception if we can't attain av=0.)
void 
ClSimplexSolver::addWithArtificialVariable(const ClLinearConstraint &expr)
{
  ClVariable &av = *(new ClVariable(++artificialCounter,"a",CLSlackVar));
  ClVariable &az = *(new ClVariable("az",CLObjectiveVar));
  ClLinearExpression &azRow = *(new ClLinearExpression(expr));
  // FIXGJBMEM

  // the artificial objective is av, which we know is equal to expr
  // (which contains only parametric variables)
  addRow(az,azRow);
  addRow(av,expr);

  // try to optimize av to 0
  optimize(az);

  // Check that we were able to make the objective value 0
  // If not, the original constraint was not satisfiable
  if (!clApprox(azRow.constant(),0.0))
    {
    throw ExCLRequiredFailure();
    }

  // see if av is a basic variable
  const ClLinearExpression &e = rowExpression(av);
  if (e != cleNil)
    {
    // Find another variable in this row and pivot, so that av becomes parametric
    // If there isn't another variabel in the row then 
    // the tableau contains the equation av = 0  -- just delete av's row
    if (e.isConstant())
      {
      removeRow(av);
      return;
      }
    ClVariable &entryVar = e.anyVariable();
    pivot(entryVar, av);
    }
  // now av should be parametric
  assert(rowExpression(av) == cleNil);
  removeParametricVar(av);
  // remove teh temporary objective function
  removeRow(az);

  delete &av;
  delete &az;
  // FIXGJB: what about azRow?
}

// We are trying to add the constraint expr=0 to the appropriate
// tableau.  Try to add expr directly to the tableax without
// creating an artificial variable.  Return true if successful and
// false if not.
bool 
ClSimplexSolver::tryAddingDirectly(ClLinearExpression &expr)
{
  ClVariable &subject = chooseSubject(expr);
  if (subject == clvNil)
    {
    return false;
    }
  expr.newSubject(subject);
  if (columnsHasKey(subject))
    {
    substituteOut(subject,expr);
    }
  addRow(subject,expr);
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
ClVariable &
ClSimplexSolver::chooseSubject(const ClLinearConstraint &expr)
{
  assert(false);
}
  
void 
ClSimplexSolver::deltaEditConstant(Number delta, const ClVariable &v1, const ClVariable &v2)
{
  
}
  
// We have set new values for the constants in the edit constraints.
// Re-optimize using the dual simplex algorithm.
void 
ClSimplexSolver::dualOptimize()
{
  
}

// find the index in editPlusErrorVars of one of the variables in eVars
int 
ClSimplexSolver::findEditErrorIndex(const vector<ClVariable> &rgvar)
{
  
}

// Make a new linear expression representing the constraint cn,
// replacing any basic variables with their defining expressions.
// Normalize if necessary so that the constant is non-negative.  If
// the constraint is non-required give its error variables an
// appropriate weight in the objective function.
ClLinearExpression 
ClSimplexSolver::makeExpression(const ClLinearConstraint &cn)
{
  
}

// Minimize the value of the objective.  (The tableau should already
// be feasible.)
void 
ClSimplexSolver::optimize(const ClVariable &zVar)
{
  
}

// Do a pivot.  Move entryVar into the basis (i.e. make it a basic variable),
// and move exitVar out of the basis (i.e., make it a parametric variable)
void 
ClSimplexSolver::pivot(const ClVariable &entryVar, const ClVariable &exitVar)
{
  
}

// Remove the parametric variable var, updating the appropriate
// column and row entries.
void 
ClSimplexSolver::removeParametricVar(const ClVariable &v)
{
  
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
  
}
