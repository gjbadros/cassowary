// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998, All Rights Reserved.
//
// ClLinearExpression.cc

#include "ClLinearExpression.h"

ClLinearExpression::ClLinearExpression()
{
  
}

// Convert from ClVariable to a ClLinearExpression
// this replaces ClVariable::asLinearExpression
ClLinearExpression::ClLinearExpression(const ClVariable &clv)
  : my_constant(0.0)
{
  my_terms[clv] = 1.0;
}

#ifdef FIXGJB_INCOMPLETE
  // Return a new linear expression formed by multiplying self by x.
  // (Note that this result must be linear.)
  ClLinearExpression times(Number x) const;

  // Return a new linear expression formed by adding x to self.
  ClLinearExpression plus(Number x) const;

  // Return a new linear expression formed by subtracting x from self.
  ClLinearExpression minus(Number x) const;

  // Return a new linear expression formed by dividing self by x.
  // (Note that this result must be linear.)
  ClLinearExpression divide(Number x) const;

  // Return a new linear expression (aNumber/this).  Since the result
  // must be linear, this is permissible only if 'this' is a constant.
  ClLinearExpression divFrom(const ClLinearExpression &aNumber) const;

  // Return a new linear expression (aNumber-this).
  ClLinearExpression subtractFrom(const ClLinearExpression &aNumber) const;

  // Add n*expr to this expression for another expression expr.
  void addExpression(const ClLinearExpression &expr, Number n);

  // Add n*expr to this expression for another expression expr.
  // Notify the solver if a variable is added or deleted from this
  // expression.
  void addExpression(const ClLinearExpression &expr, Number n,
		     const ClAbstractVariable &subject,
		     const ClSimplexSolver &solver);

  // Add a term c*v to this expression.  If the expression already
  // contains a term involving v, add c to the existing coefficient.
  // If the new coefficient is approximately 0, delete v.
  void addVariable(const ClVariable &v, Number c);

  // Add a term c*v to this expression.  If the expression already
  // contains a term involving v, add c to the existing coefficient.
  // If the new coefficient is approximately 0, delete v.  Notify the
  // solver if v appears or disappears from this expression.
  void addVariable(const ClVariable &v, Number c,
		   const ClAbstractVariable &subject,
		   const ClSimplexSolver &solver);

  // Replace var with a symbolic expression expr that is equal to it.
  // If a variable has been added to this expression that wasn't there
  // before, or if a variable has been dropped from this expression
  // because it now has a coefficient of 0, inform the solver.
  // PRECONDITIONS:
  //   var occurs with a non-zero coefficient in this expression.
  void substituteOut(const ClVariable &v, 
		     const ClLinearExpression &expr,
		     const ClAbstractVariable &subject,
		     const ClSimplexSolver &solver);

  // Return a variable in this expression.  (It is an error if this
  // expression is constant -- signal ExCLInternalError in that case).
  ClAbstractVariable *anyVariable() const;

#ifdef FIXGJB_OLD_SMALLTALK_WAY
  ClLinearExpression asLinearExpression() const 
    { return this; }
#endif


  // This linear expression currently represents the equation
  // oldSubject=self.  Destructively modify it so that it represents
  // the equation newSubject=self.
  //
  // Precondition: newSubject currently has a nonzero coefficient in
  // this expression.
  //
  // NOTES
  //   Suppose this expression is c + a*newSubject + a1*v1 + ... + an*vn.
  //
  //   Then the current equation is 
  //       oldSubject = c + a*newSubject + a1*v1 + ... + an*vn.
  //   The new equation will be
  //        newSubject = -c/a + oldSubject/a - (a1/a)*v1 - ... - (an/a)*vn.
  //   Note that the term involving newSubject has been dropped.
  void changeSubject(const ClAbstractVariable &oldSubject,
		     const ClAbstractVariable &newSubject);

  // This linear expression currently represents the equation self=0.  Destructively modify it so 
  // that subject=self represents an equivalent equation.  
  //
  // Precondition: subject must be one of the variables in this expression.
  // NOTES
  //   Suppose this expression is
  //     c + a*subject + a1*v1 + ... + an*vn
  //   representing 
  //     c + a*subject + a1*v1 + ... + an*vn = 0
  // The modified expression will be
  //    subject = -c/a - (a1/a)*v1 - ... - (an/a)*vn
  //   representing
  //    subject = -c/a - (a1/a)*v1 - ... - (an/a)*vn
  //
  // Note that the term involving subject has been dropped.
  void newSubject(const ClAbstractVariable &subject);

  /// Below cnFoo functions are virtually duplicated in ClVariable, also

  // Return a linear constraint self=expr with given strength and weight
  ClLinearEquation cnEqual(const ClLinearExpression &expr, 
			   const ClStrength &strength,
			   double weight = 1.0);
  ClLinearEquation cnEqual(const ClVariable &expr,
			   const ClStrength &strength,
			   double weight = 1.0);
  ClLinearEquation cnEqual(Number expr,
			   const ClStrength &strength,
			   double weight = 1.0);

  // Return a linear constraint self>=expr with given strength and weight
  ClLinearInequality cnGEQ(const ClLinearExpression &expr, 
			   const ClStrength &strength,
			   double weight = 1.0);
  ClLinearInequality cnGEQ(const ClVariable &expr,
			   const ClStrength &strength,
			   double weight = 1.0);
  ClLinearInequality cnGEQ(Number expr,
			   const ClStrength &strength,
			   double weight = 1.0);


  // Return a linear constraint self<=expr with given strength and weight
  ClLinearInequality cnLEQ(const ClLinearExpression &expr, 
			   const ClStrength &strength,
			   double weight = 1.0);
  ClLinearInequality cnLEQ(const ClVariable &expr,
			   const ClStrength &strength,
			   double weight = 1.0);
  ClLinearInequality cnLEQ(Number expr,
			   const ClStrength &strength,
			   double weight = 1.0);

  // Return the coefficient corresponding to variable var, i.e.,
  // the 'ci' corresponding to the 'vi' that var is:
  //     v1*c1 + v2*c2 + .. + vn*cn + c
  Number coefficientFor(const ClVariable &var);

  Number constant()
    { return my_constant; }

  void set_constant(Number c)
    { my_constant = c; }


  void incrementConstant(Number c)
    { my_constant += c; }

  bool isConstant();

  virtual ostream &printOn(ostream &xo);

  // Initialize this linear expression to 0.
  void initialize();

#endif
