// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998, 1999 Alan Borning and Greg Badros
// See ../LICENSE for legal details regarding this software
//
// ClSimplexSolver.h

#ifndef ClSimplexSolver_H
#define ClSimplexSolver_H

#include <stack>
#include "Cassowary.h"
#include "ClTableau.h"
#include "ClLinearInequality.h"
#include "ClStrength.h"
#include "ClStayConstraint.h"
#include "ClEditConstraint.h"
#include "ClSlackVariable.h"
#include "ClObjectiveVariable.h"
#include "ClErrors.h"
#include "ClTypedefs.h"

class ClVariable;
class ClPoint;
class ExCLRequiredFailureWithExplanation;


// ClSimplexSolver encapsulates the solving behaviour
// of the cassowary algorithm
class ClSimplexSolver : public ClTableau {
 protected: typedef ClTableau super;
  class ClEditInfo;
  typedef ClMap<ClVariable, ClEditInfo *> ClVarToEditInfoMap;

 public:

  // Constructor
  ClSimplexSolver() :
    _objective(ClVariable(new ClObjectiveVariable("Z"))),
    _slackCounter(0),
    _artificialCounter(0),
#ifdef CL_FIND_LEAK
    _cArtificialVarsDeleted(0),
#endif
    _dummyCounter(0),
    _epsilon(1e-8),
    _fOptimizeAutomatically(true),
    _fNeedsSolving(false),
    _fExplainFailure(false),
    _pfnChangeClvCallback(NULL),
    _pfnResolveCallback(NULL),
    _pfnCnSatCallback(NULL)
    { 
    _rows[_objective] = new ClLinearExpression(); 
    // start out with no edit variables
    _stkCedcns.push(0);
#ifdef CL_TRACE
    cerr << "objective row new@ " << _rows[_objective] << endl;
#endif
    }

  virtual ~ClSimplexSolver();
  
  // Add constraints so that lower<=var<=upper.  (nil means no  bound.)
  ClSimplexSolver &addLowerBound(ClVariable v, Number lower)
    { 
    ClLinearInequality *pcn = new ClLinearInequality(ClLinearExpression(v - lower));
    return addConstraint(pcn);
    }
  ClSimplexSolver &addUpperBound(ClVariable v, Number upper)
    {
    ClLinearInequality *pcn = new ClLinearInequality(ClLinearExpression(upper - v));
    return addConstraint(pcn);
    }
  ClSimplexSolver &addBounds(ClVariable v, Number lower, Number upper)
    { addLowerBound(v,lower); addUpperBound(v,upper); return *this; }

  // Add the constraint cn to the tableau
  ClSimplexSolver &addConstraint(const ClConstraint *const pcn);

  // Deprecated! --02/19/99 gjb
  ClSimplexSolver &addConstraint(const ClConstraint &cn) 
    { return addConstraint(&cn); }

  // Same as above, but returns false if the constraint cannot be solved
  // (i.e., the resulting system would be unsatisfiable)
  // The above function "addConstraint" throws an exception in that case
  // which may be inconvenient
  bool addConstraintNoException(const ClConstraint *const pcn);

  // Add an edit constraint for "v" with given strength
  ClSimplexSolver &addEditVar(const ClVariable &v, const ClStrength &strength = clsStrong(),
                              double weight = 1.0 )
    { 
      ClEditConstraint *pedit = new ClEditConstraint(v, strength, weight);
      return addConstraint(pedit);
    }

  ClSimplexSolver &removeEditVar(ClVariable v)
    {
      const ClEditInfo *pcei = _editVarMap[v];
      const ClConstraint *pcnEdit = pcei->_pconstraint;
      removeConstraint(pcnEdit);
      delete pcnEdit;
      return *this;
    }

  // beginEdit() should be called before sending
  // resolve() messages, after adding the appropriate edit variables
  ClSimplexSolver &beginEdit()
    {
      assert(_editVarMap.size() != 0);
      // may later want to do more in here
      _infeasibleRows.clear();
      resetStayConstants();
      _stkCedcns.push(_editVarMap.size());
      return *this;
    }

  // endEdit should be called after editing has finished
  // for now, it just removes all edit variables
  ClSimplexSolver &endEdit()
    {
      assert(_editVarMap.size() != 0);
      resolve();
      _stkCedcns.pop();
      removeEditVarsTo(_stkCedcns.top());
      // may later want to do more in here
      return *this;
    }

  // removeAllEditVars() just eliminates all the edit constraints
  // that were added
  ClSimplexSolver &removeAllEditVars() { removeEditVarsTo(0); return *this; }

  // remove the last added edit vars to leave only n edit vars left
  ClSimplexSolver &removeEditVarsTo(int n);

  int numEditVars() const
  { return _editVarMap.size(); }

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
    return addConstraint(pcn); 
    }

  // Remove the constraint cn from the tableau
  // Also remove any error variable associated with cn
  ClSimplexSolver &removeConstraint(const ClConstraint *const pcn);

  // Deprecated! --02/19/99 gjb
  ClSimplexSolver &removeConstraint(const ClConstraint &cn) 
    { return removeConstraint(&cn); }


  // Same as above, but returns false if the constraint dne
  // The above function "removeConstraint" throws an exception in that case
  // which may be inconvenient
  bool removeConstraintNoException(const ClConstraint *const pcn);


  // Re-initialize this solver from the original constraints, thus
  // getting rid of any accumulated numerical problems.  (Actually, we
  // haven't definitely observed any such problems yet)
  void reset();

  // Re-solve the cuurent collection of constraints, given the new
  // values for the edit variables that have already been
  // suggested (see suggestValue() method)
  // This is not guaranteed to work if you remove an edit constraint
  // from the middle of the edit constraints you added
  // (e.g., edit A, edit B, edit C, remove B -> this will fail!)
  // DEPRECATED
  void resolve();

  // Re-solve the current collection of constraints for new values for
  // the constants of the edit variables.
  // This is implemented in terms of suggestValue-s, and is
  // less efficient than that more natural interface
  void resolve(const vector<Number> &newEditConstants);

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

  // Control whether optimization and setting of external variables
  // is done automatically or not.  By default it is done
  // automatically and solve() never needs to be explicitly
  // called by client code; if setAutosolve is put to false,
  // then solve() needs to be invoked explicitly before using
  // variables' values
  // (Turning off autosolve while adding lots and lots of
  // constraints [ala the addDel test in ClTests] saved
  // about 20% in runtime, from 68sec to 54sec for 900 constraints,
  // with 126 failed adds)
  ClSimplexSolver &setAutosolve(bool f)
    { _fOptimizeAutomatically = f; if (f) solve(); return *this; }

  // Tell whether we are autosolving
  bool FIsAutosolving() const
    { return _fOptimizeAutomatically; }

  // Set and check whether or not the solver will attempt to compile
  // an explanation of failure when a required constraint conflicts
  // with another required constraint
  ClSimplexSolver &setExplaining(bool f)
    { _fExplainFailure = f; return *this; }

  bool FIsExplaining() const
    { return _fExplainFailure; }

  // If autosolving has been turned off, client code needs
  // to explicitly call solve() before accessing variables
  // values
  ClSimplexSolver &solve()
    {
#ifdef CL_SOLVER_CHECK_INTEGRITY
    AssertValid();
#endif
    if (_fNeedsSolving) 
      {
      optimize(_objective);
      setExternalVariables();
#ifdef CL_TRACE_VERBOSE
      cerr << "Manual solve actually solving." << endl;
#endif
      }
    return *this;
    }

  ClSimplexSolver &setEditedValue(ClVariable &v, double n)
    {
    if (!FContainsVariable(v))
      {
      ChangeClv(v,n);
      return *this;
      }

    if (!clApprox(n, v.value())) 
      {
      addEditVar(v);
      beginEdit();
      suggestValue(v,n);
      endEdit();
      }
    return *this;
    }

  // Solver contains the variable if it's in either the columns
  // list or the rows list
  bool FContainsVariable(const ClVariable &v)
    { return columnsHasKey(v) || rowExpression(v); }

  ClSimplexSolver &addVar(const ClVariable &v)
    { if (!FContainsVariable(v)) 
        {
        addStay(v); 
#ifdef CL_TRACE
        cerr << "added initial stay on " << v << endl;
#endif
        }
      return *this; }

  typedef void (*PfnChangeClvCallback)(ClVariable clv, ClSimplexSolver *psolver);

  void SetChangeClvCallback(PfnChangeClvCallback pfn)
    { _pfnChangeClvCallback = pfn; }

  typedef void (*PfnResolveCallback)(ClSimplexSolver *psolver);

  void SetResolveCallback(PfnResolveCallback pfn)
    { _pfnResolveCallback = pfn; }

  typedef void (*PfnCnSatCallback)(ClSimplexSolver *psolver, 
                                   ClConstraint *pcn, bool fSatisfied);

  void SetCnSatCallback(PfnCnSatCallback pfn)
    { _pfnCnSatCallback = pfn; }

#ifndef CL_NO_IO
  friend ostream &operator<<(ostream &xo, const ClSimplexSolver &tableau);

  ostream &printOn(ostream &xo) const;
  
  ostream &printInternalInfo(ostream &xo) const;

  ostream &printOnVerbose(ostream &xo) const 
    { printOn(xo); printInternalInfo(xo); xo << endl; return xo; }

#endif

  const ClConstraintToVarMap &ConstraintMap() const
    { return _markerVars; }

  const ClVarToConstraintMap &MarkerMap() const
    { return _constraintsMarked; }

  bool FIsConstraintSatisfied(const ClConstraint *const pcn) const;

  // DEPRECATED
  bool FIsConstraintSatisfied(const ClConstraint &pcn) const
    { return FIsConstraintSatisfied(&pcn); }

  void setPv(void *pv)
    { _pv = pv; }

  void *Pv() const
    { return _pv; }

 protected:
  
  // ClEditInfo is a privately-used class
  // that just wraps a constraint, its positive and negative
  // error variables, and its prior edit constant.
  // It is used as values in _editVarMap, and replaces
  // the parallel vectors of error variables and previous edit
  // constants from the smalltalk version of the code.
  class ClEditInfo {
    friend ClSimplexSolver;
  public:
    
    // These instances own none of the pointers;
    // the tableau row (the expression) owns the peplus, peminus,
    // and addEditVar/removeEditVar pair or the client code owns
    // the constraint object
    ClEditInfo(const ClEditConstraint *pconstraint, 
               ClVariable eplus, ClVariable eminus,
               Number prevEditConstant,
               int index)
        :_pconstraint(pconstraint),
         _clvEditPlus(eplus), _clvEditMinus(eminus),
         _prevEditConstant(prevEditConstant),
         _index(index)
      { }
    
    ~ClEditInfo() 
      { }
  private:
    const ClConstraint *_pconstraint;
    ClVariable _clvEditPlus;
    ClVariable _clvEditMinus;
    Number _prevEditConstant;
    int _index;
  };
  


  // Add the constraint expr=0 to the inequality tableau using an
  // artificial variable.  To do this, create an artificial variable
  // av and add av=expr to the inequality tableau, then make av be 0.
  // (Raise an exception if we can't attain av=0.)
  // (Raise an exception if we can't attain av=0.) If the add fails,
  // prepare an explanation in e that describes why it failed (note
  // that an empty explanation is considered to mean the explanation
  // encompasses all active constraints.
  bool addWithArtificialVariable(ClLinearExpression &pexpr, 
                                 ExCLRequiredFailureWithExplanation &e);
  
  // Using the given equation (av = cle) build an explanation which
  // implicates all constraints used to construct the equation. That
  // is, everything for which the variables in the equation are markers.
  // Thanks to Steve Wolfman for the implementation of the explanation feature
  void buildExplanation(ExCLRequiredFailureWithExplanation & e, 
                        ClVariable av,
                        const ClLinearExpression * pcle);

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
  ClVariable chooseSubject(ClLinearExpression &pexpr);
  
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
  void deltaEditConstant(Number delta, ClVariable pv1, ClVariable pv2);
  
  // We have set new values for the constants in the edit constraints.
  // Re-optimize using the dual simplex algorithm.
  void dualOptimize();

  // Make a new linear expression representing the constraint cn,
  // replacing any basic variables with their defining expressions.
  // Normalize if necessary so that the constant is non-negative.  If
  // the constraint is non-required give its error variables an
  // appropriate weight in the objective function.
  ClLinearExpression *newExpression(const ClConstraint &cn,
                                    ClVariable &clvEplus,
                                    ClVariable &clvEminus,
                                    Number &prevEConstant);

  // Minimize the value of the objective.  (The tableau should already
  // be feasible.)
  void optimize(ClVariable zVar);

  // Do a pivot.  Move entryVar into the basis (i.e. make it a basic variable),
  // and move exitVar out of the basis (i.e., make it a parametric variable)
  void pivot(ClVariable entryVar, ClVariable exitVar);

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
  // them.
  void setExternalVariables();

  void ChangeClv(ClVariable clv, Number n) {
    clv.change_value(n); 
    if (_pfnChangeClvCallback) 
      _pfnChangeClvCallback(clv,this);
  }

  /// instance variables

  // the arrays of positive and negative error vars for the stay constraints
  // (need both positive and negative since they have only non-negative values)
  ClVarVector _stayMinusErrorVars;
  ClVarVector _stayPlusErrorVars;

  // give error variables for a non required constraint,
  // maps to ClSlackVariable-s
  ClConstraintToVarSetMap _errorVars;

  // Return a lookup table giving the marker variable for each
  // constraint (used when deleting a constraint).
  ClConstraintToVarMap _markerVars;

  // Reverse of the above-- a lookup table giving the constraint
  // for each marker variable (used when building failure explanations)
  ClVarToConstraintMap _constraintsMarked;

  ClVariable _objective;

  // Map edit variables to their constraints, errors, and prior
  // values
  ClVarToEditInfoMap _editVarMap;

  int _slackCounter;
  int _artificialCounter;
#ifdef CL_FIND_LEAK
  int _cArtificialVarsDeleted;
#endif
  int _dummyCounter;
  const double _epsilon;

  bool _fOptimizeAutomatically;
  bool _fNeedsSolving;
  bool _fExplainFailure;

  PfnChangeClvCallback _pfnChangeClvCallback;
  PfnResolveCallback _pfnResolveCallback;
  PfnCnSatCallback _pfnCnSatCallback;

  // C-style extension mechanism so I
  // don't have to wrap ScwmClSolver separately
  void *_pv;

  // a stack of the number of edit constraints
  // that existed at the prior beginEdit.
  // an endEdit needs to pop off the top value,
  // then remove constraints to get down
  // to the # of constraints as in _stkCedcns.top()
  stack<int> _stkCedcns;

};

#ifndef CL_NO_IO
ostream &printTo(ostream &xo, const ClVarVector &varlist);
ostream &operator<<(ostream &xo, const ClVarVector &varlist);

ostream &printTo(ostream &xo, const ClConstraintToVarSetMap &mapCnToVarSet);
ostream &operator<<(ostream &xo, const ClConstraintToVarSetMap &mapCnToVarSet);
#endif


#endif

