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
#include "debug.h"
#include "ClAbstractVariable.h"

class ClSimplexSolver;
class ClTableau;
class ClLinearExpression;

ClLinearExpression &cleNil();


class ClLinearExpression  {
 public:
  // convert Number-s into ClLinearExpression-s
  ClLinearExpression(Number num = 0.0);

  // Convert from ClVariable to a ClLinearExpression
  // this replaces ClVariable::asLinearExpression
  ClLinearExpression(const ClAbstractVariable &clv, Number value = 1.0);

  // copy ctr
  ClLinearExpression(const ClLinearExpression &expr) :
    my_constant(expr.my_constant),
    my_terms(expr.my_terms)
    { }

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
				    ClTableau &solver);

  // Add a term c*v to this expression.  If the expression already
  // contains a term involving v, add c to the existing coefficient.
  // If the new coefficient is approximately 0, delete v.
  ClLinearExpression &addVariable(const ClAbstractVariable &v, Number c);

  // Add a term c*v to this expression.  If the expression already
  // contains a term involving v, add c to the existing coefficient.
  // If the new coefficient is approximately 0, delete v.
  ClLinearExpression &setVariable(const ClAbstractVariable &v, Number c)
    {assert(!clApprox(c,0.0));  my_terms[&v] = c; return *this; }

  // Add a term c*v to this expression.  If the expression already
  // contains a term involving v, add c to the existing coefficient.
  // If the new coefficient is approximately 0, delete v.  Notify the
  // solver if v appears or disappears from this expression.
  ClLinearExpression &addVariable(const ClAbstractVariable &v, Number c,
				  const ClAbstractVariable &subject,
				  ClTableau &solver);

  // Return a variable in this expression.  (It is an error if this
  // expression is constant -- signal ExCLInternalError in that case).
  const ClAbstractVariable *anyVariable() const;

  // Replace var with a symbolic expression expr that is equal to it.
  // If a variable has been added to this expression that wasn't there
  // before, or if a variable has been dropped from this expression
  // because it now has a coefficient of 0, inform the solver.
  // PRECONDITIONS:
  //   var occurs with a non-zero coefficient in this expression.
  void substituteOut(const ClAbstractVariable &v, 
		     const ClLinearExpression &expr,
		     const ClAbstractVariable &subject,
		     ClTableau &solver);

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
  Number coefficientFor(const ClAbstractVariable &var) const
    { 
    map<const ClAbstractVariable *, Number>::const_iterator it = my_terms.find(&var);
    if (it != my_terms.end())
      return (*it).second;
    return 0.0;
    }

  Number constant() const
    { return my_constant; }

  void set_constant(Number c)
    { my_constant = c; }

  const map<const ClAbstractVariable *,Number> &terms() const
    { return my_terms; }

  map<const ClAbstractVariable *,Number> &terms() 
    { return my_terms; }

  void incrementConstant(Number c)
    { my_constant += c; }

  bool isConstant() const
    { return my_terms.size() == 0; }

  virtual ostream &printOn(ostream &xo) const;

  friend ostream &operator<<(ostream &xo,const ClLinearExpression &cle)
    { return cle.printOn(xo); }

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

  friend bool operator==(const ClLinearExpression &e1,
			 const ClLinearExpression &e2)
    { return &e1 == &e2; }


  ClLinearExpression &ClLinearExpression::multiplyMe(Number x);

 private:

  Number my_constant;
  map<const ClAbstractVariable *,Number> my_terms;

};

#endif
