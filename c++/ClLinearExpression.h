// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998, All Rights Reserved.
//
// ClLinearExpression.h

#ifndef ClLinearExpression_H
#define ClLinearExpression_H

#include <map.h>
#include "ClAbstractVariable.h"
//#include "ClLinearEquation.h"

class ClSimplexSolver;

class ClLinearExpression  {
 public:
  // convert Number-s into ClLinearExpression-s
  ClLinearExpression(Number num = 0.0);

  // Convert from ClAbstractVariable to a ClLinearExpression
  // this replaces ClAbstractVariable::asLinearExpression
  ClLinearExpression(const ClAbstractVariable &clv);

  virtual ~ClLinearExpression();

  // Return a new linear expression formed by multiplying self by x.
  // (Note that this result must be linear.)
  ClLinearExpression times(Number x) const;

  // Return a new linear expression formed by multiplying self by x.
  // (Note that this result must be linear.)
  ClLinearExpression times(const ClLinearExpression &expr) const;

  // Return a new linear expression formed by adding x to self.
  ClLinearExpression plus(const ClLinearExpression &expr) const;

  // Return a new linear expression formed by subtracting x from self.
  ClLinearExpression minus(const ClLinearExpression &expr) const;

  // Return a new linear expression formed by dividing self by x.
  // (Note that this result must be linear.)
  ClLinearExpression divide(Number x) const;

  // Return a new linear expression formed by dividing self by x.
  // (Note that this result must be linear.)
  ClLinearExpression divide(const ClLinearExpression &expr) const;

  // Return a new linear expression (aNumber/this).  Since the result
  // must be linear, this is permissible only if 'this' is a constant.
  ClLinearExpression divFrom(const ClLinearExpression &expr) const;

  // Return a new linear expression (aNumber-this).
  ClLinearExpression subtractFrom(const ClLinearExpression &expr) const
  { return expr.minus(*this); }

  // Add n*expr to this expression for another expression expr.
  ClLinearExpression &addExpression(const ClLinearExpression &expr, Number n);

  // Add n*expr to this expression for another expression expr.
  // Notify the solver if a variable is added or deleted from this
  // expression.
  ClLinearExpression &addExpression(const ClLinearExpression &expr, Number n,
				    const ClAbstractVariable &subject,
				    const ClSimplexSolver &solver);

  // Add a term c*v to this expression.  If the expression already
  // contains a term involving v, add c to the existing coefficient.
  // If the new coefficient is approximately 0, delete v.
  ClLinearExpression &addVariable(const ClAbstractVariable &v, Number c);

  // Add a term c*v to this expression.  If the expression already
  // contains a term involving v, add c to the existing coefficient.
  // If the new coefficient is approximately 0, delete v.  Notify the
  // solver if v appears or disappears from this expression.
  ClLinearExpression &addVariable(const ClAbstractVariable &v, Number c,
				  const ClAbstractVariable &subject,
				  const ClSimplexSolver &solver);

  // Return a variable in this expression.  (It is an error if this
  // expression is constant -- signal ExCLInternalError in that case).
  const ClAbstractVariable &anyVariable() const;

  // Replace var with a symbolic expression expr that is equal to it.
  // If a variable has been added to this expression that wasn't there
  // before, or if a variable has been dropped from this expression
  // because it now has a coefficient of 0, inform the solver.
  // PRECONDITIONS:
  //   var occurs with a non-zero coefficient in this expression.
  void substituteOut(const ClAbstractVariable &v, 
		     const ClLinearExpression &expr,
		     const ClAbstractVariable &subject,
		     const ClSimplexSolver &solver);

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
  void changeSubject(const ClAbstractVariable &old_subject,
		     const ClAbstractVariable &new_subject);

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
  // Returns the reciprocal, so changeSubject can use it, too
  Number newSubject(const ClAbstractVariable &subject);

  // Return the coefficient corresponding to variable var, i.e.,
  // the 'ci' corresponding to the 'vi' that var is:
  //     v1*c1 + v2*c2 + .. + vn*cn + c
  Number coefficientFor(const ClAbstractVariable &var)
    { return my_terms[var]; }

  Number constant() const
    { return my_constant; }

  void set_constant(Number c)
    { my_constant = c; }

  void incrementConstant(Number c)
    { my_constant += c; }

  bool isConstant() const
    { return my_terms.size() == 0; }

  virtual ostream &printOn(ostream &xo) const;

  friend ostream &operator<<(ostream &xo,const ClLinearExpression &cle)
    { cle.printOn(xo); return xo; }

  friend ClLinearExpression operator+(const ClLinearExpression &e1,
				      const ClLinearExpression &e2)
    { return e1.plus(e2); }

  friend ClLinearExpression operator-(const ClLinearExpression &e1,
				      const ClLinearExpression &e2)
    { return e1.minus(e2); }

  friend ClLinearExpression operator*(const ClLinearExpression &e1,
				      const ClLinearExpression &e2)
    { return e1.times(e2); }


  friend ClLinearExpression operator/(const ClLinearExpression &e1,
				      const ClLinearExpression &e2)
    { return e1.divide(e2); }


#ifdef FIXGJB_OLD_SMALLTALK_WAY
  /// Below cnFoo functions are virtually duplicated in ClAbstractVariable, also

  // Return a linear constraint self=expr with given strength and weight
  ClLinearEquation cnEqual(const ClLinearExpression &expr, 
			   const ClStrength &strength,
			   double weight = 1.0);
  ClLinearEquation cnEqual(const ClAbstractVariable &expr,
			   const ClStrength &strength,
			   double weight = 1.0);
  ClLinearEquation cnEqual(Number expr,
			   const ClStrength &strength,
			   double weight = 1.0);

  // Return a linear constraint self>=expr with given strength and weight
  ClLinearInequality cnGEQ(const ClLinearExpression &expr, 
			   const ClStrength &strength,
			   double weight = 1.0);
  ClLinearInequality cnGEQ(const ClAbstractVariable &expr,
			   const ClStrength &strength,
			   double weight = 1.0);
  ClLinearInequality cnGEQ(Number expr,
			   const ClStrength &strength,
			   double weight = 1.0);


  // Return a linear constraint self<=expr with given strength and weight
  ClLinearInequality cnLEQ(const ClLinearExpression &expr, 
			   const ClStrength &strength,
			   double weight = 1.0);
  ClLinearInequality cnLEQ(const ClAbstractVariable &expr,
			   const ClStrength &strength,
			   double weight = 1.0);
  ClLinearInequality cnLEQ(Number expr,
			   const ClStrength &strength,
			   double weight = 1.0);
#endif

 private:

  ClLinearExpression &ClLinearExpression::multiplyMe(Number x);

  // Initialize this linear expression to 0.
  // I think ctr obviates need for this --01/09/98 gjb
  // void initialize();

  Number my_constant;
  map<ClAbstractVariable,Number> my_terms;

};

#endif
