// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998 Alan Borning and Greg Badros
// See ../COPYRIGHT for legal details regarding this software
//
// ClSimplexSolver.h

#ifndef ClSimplexSolver_H
#define ClSimplexSolver_H

#include "Cassowary.h"
#include "ClTableau.h"
#include "ClLinearInequality.h"
#include "ClStrength.h"
#include "ClStayConstraint.h"
#include "ClEditConstraint.h"
#include "ClObjectiveVariable.h"

class ClVariable;
class ClPoint;

class ClConstraintAndIndex {
  friend ClSimplexSolver;
public:
  ClConstraintAndIndex()
      :pconstraint(NULL), index(-1)
    { }
  ClConstraintAndIndex(const ClConstraint *pconstraint_, int i)
      :pconstraint(pconstraint_), index(i)
    { }
private:
  const ClConstraint *pconstraint;
  int index;
};


typedef map<const ClConstraint *, ClTableauVarSet > ClConstraintToVarSetMap;
typedef map<const ClConstraint *, const ClAbstractVariable *> ClConstraintToVarMap;
typedef map<const ClVariable *, const ClConstraintAndIndex *> ClVarToConstraintAndIndexMap;
typedef vector<const ClAbstractVariable *> ClVarVector;

class ClSimplexSolver : public ClTableau {
 protected: typedef ClTableau super;

 public:

  // Constructor
  ClSimplexSolver() :
    my_objective(*(new ClObjectiveVariable("Z"))),
    my_slackCounter(0),
    my_artificialCounter(0),
    my_dummyCounter(0),
    my_epsilon(1e-8)
    { 
    my_rows[&my_objective] = new ClLinearExpression(); 
#ifndef CL_NO_TRACE
    cerr << "objective row new@ " << my_rows[&my_objective] << endl;
#endif
    }

  ~ClSimplexSolver();
  
  // Add constraints so that lower<=var<=upper.  (nil means no  bound.)
  ClSimplexSolver &addLowerBound(const ClAbstractVariable &v, Number lower)
    { 
    ClLinearInequality *pcn = new ClLinearInequality(ClLinearExpression(v - lower));
    return addConstraint(*pcn);
    }
  ClSimplexSolver &addUpperBound(const ClAbstractVariable &v, Number upper)
    {
    ClLinearInequality *pcn = new ClLinearInequality(ClLinearExpression(upper - v));
    return addConstraint(*pcn);
    }
  ClSimplexSolver &addBounds(const ClAbstractVariable &v, Number lower, Number upper)
    { addLowerBound(v,lower); addUpperBound(v,upper); return *this; }

  // Add the constraint cn to the tableau
  ClSimplexSolver &addConstraint(const ClConstraint &cn);


  // Same as above, but returns false if the constraint cannot be solved
  // (i.e., the resulting system would be unsatisfiable)
  // The above function "addConstraint" throws an exception in that case
  // which may be inconvenient
  bool addConstraintNoException(const ClConstraint &cn);

  ClSimplexSolver &addEditVar(const ClVariable &v, ClStrength &strength = clsStrong())
    { 
      ClEditConstraint *pedit = new ClEditConstraint(v, strength);
      return addConstraint(*pedit);
    }

  ClSimplexSolver &removeEditVar(const ClVariable &v)
    {
      const ClConstraintAndIndex *pcai = my_editVarMap[&v];
      const ClConstraint *pcnEdit = pcai->pconstraint;
      removeConstraint(*pcnEdit);
      delete pcnEdit;
      return *this;
    }

  // beginEdit() should be called before sending
  // resolve() messages, after adding the appropriate edit variables
  // 
  // it is basically a no-op for now,
  // but it is specified in the API just in case
  // we need it later
  ClSimplexSolver &beginEdit()
    {
      assert(my_editVarMap.size() != 0);
      // may later want to do more in here
      my_infeasibleRows.clear();
      resetStayConstants();
      return *this;
    }

  // endEdit should be called after editing has finished
  // for now, it just removes all edit variables
  ClSimplexSolver &endEdit()
    {
      assert(my_editVarMap.size() != 0);
      removeAllEditVars();
      // may later want to do more in here
      return *this;
    }

  // removeAllEditVars() just eliminates all the edit constraints
  // that were added
  ClSimplexSolver &removeAllEditVars();

  int numEditVars() const
  { return my_editVarMap.size(); }

  // Add weak stays to the x and y parts of each point. These have
  // increasing weights so that the solver will try to satisfy the x
  // and y stays on the same point, rather than the x stay on one and
  // the y stay on another.
  ClSimplexSolver &addPointStays(const vector<const ClPoint *> &listOfPoints);

  ClSimplexSolver &addPointStay(const ClVariable &vx, const ClVariable &vy, double weight)
    { addStay(vx,clsWeak(),weight); addStay(vy,clsWeak(),weight); return *this; }

  ClSimplexSolver &addPointStay(const ClPoint &clp, double weight);


  // Add a stay of the given strength (default to weak) of v to the tableau
  ClSimplexSolver &addStay(const ClVariable &v,
			   const ClStrength &strength = clsWeak(), double weight = 1.0 )
    { 
    ClStayConstraint *pcn = new ClStayConstraint(v,strength,weight); 
    return addConstraint(*pcn); 
    }

  // Remove the constraint cn from the tableau
  // Also remove any error variable associated with cn
  ClSimplexSolver &removeConstraint(const ClConstraint &pcn);

  // Re-initialize this solver from the original constraints, thus
  // getting rid of any accumulated numerical problems.  (Actually,
  // Alan hasn't observed any such problems yet, but here's the method
  // anyway.)
  void reset();

  // Re-solve the current collection of constraints for new values for
  // the constants of the edit variables.
  void resolve(const vector<Number> &newEditConstants);

  // Re-solve the cuurent collection of constraints, given the new
  // values for the edit variables that have already been
  // suggested (see suggestValue() method)
  void resolve();

  // Convenience function for resolve-s of two variables
  void resolve(Number x, Number y)
    {
    vector<Number> vals;
    vals.push_back(x);
    vals.push_back(y);
    resolve(vals);
    }

  // Suggest a new value for an edit variable
  // the variable needs to be added as an edit variable
  // and beginEdit() needs to be called before this is called.
  // The tableau will not be solved completely until
  // after resolve() has been called
  ClSimplexSolver &suggestValue(ClVariable &v, Number x);


  friend ostream &operator<<(ostream &xo, const ClSimplexSolver &tableau);
  ostream &printOn(ostream &xo) const;
  
  ostream &printInternalInfo(ostream &xo) const;

  ostream &printDebugInfo(ostream &xo) const 
    { printOn(xo); printInternalInfo(xo); xo << endl; return xo; }

 protected:
  // Add the constraint expr=0 to the inequality tableau using an
  // artificial variable.  To do this, create an artificial variable
  // av and add av=expr to the inequality tableau, then make av be 0.
  // (Raise an exception if we can't attain av=0.)
  bool addWithArtificialVariable(ClLinearExpression &pexpr);

  // We are trying to add the constraint expr=0 to the appropriate
  // tableau.  Try to add expr directly to the tableax without
  // creating an artificial variable.  Return true if successful and
  // false if not.
  bool tryAddingDirectly(ClLinearExpression &pexpr);

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
  const ClAbstractVariable *chooseSubject(ClLinearExpression &pexpr);
  
  void deltaEditConstant(Number delta, const ClAbstractVariable &pv1, const ClAbstractVariable &pv2);
  
  // We have set new values for the constants in the edit constraints.
  // Re-optimize using the dual simplex algorithm.
  void dualOptimize();

  // Make a new linear expression representing the constraint cn,
  // replacing any basic variables with their defining expressions.
  // Normalize if necessary so that the constant is non-negative.  If
  // the constraint is non-required give its error variables an
  // appropriate weight in the objective function.
  ClLinearExpression *newExpression(const ClConstraint &cn);

  // Minimize the value of the objective.  (The tableau should already
  // be feasible.)
  void optimize(const ClObjectiveVariable &pzVar);

  // Do a pivot.  Move entryVar into the basis (i.e. make it a basic variable),
  // and move exitVar out of the basis (i.e., make it a parametric variable)
  void pivot(const ClAbstractVariable &pentryVar, const ClAbstractVariable &pexitVar);

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
  void resetEditConstants(const vector<Number> &newEditConstants);

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
  void resetStayConstants();

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
  void setExternalVariables();

  /// instance variables

  // the arrays of positive and negative error vars for the edit constraints
  // (need both positive and negative since they have only non-negative values)
  ClVarVector my_editMinusErrorVars;
  ClVarVector my_editPlusErrorVars;

  // the arrays of positive and negative error vars for the stay constraints
  // (need both positive and negative since they have only non-negative values)
  ClVarVector my_stayMinusErrorVars;
  ClVarVector my_stayPlusErrorVars;

  // The array of constants for the edit constraints on the previous
  // iteration.  These must be in the same order as editPlusErrorVars
  // and editMinusErrorVars
  vector<Number> my_prevEditConstants;

  // give error variables for a non required constraint,
  // maps to ClSlackVariable-s
  ClConstraintToVarSetMap my_errorVars;

  // Return a lookup table giving the marker variable for each
  // constraint (used when deleting a constraint).
  ClConstraintToVarMap my_markerVars;

  ClObjectiveVariable &my_objective;

  // Map edit variables to their constraints and the index into
  // the parallel ClVarVector arrays, above
  ClVarToConstraintAndIndexMap my_editVarMap;

  int my_slackCounter;
  int my_artificialCounter;
  int my_dummyCounter;
  const double my_epsilon;

};

#endif
