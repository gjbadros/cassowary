// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998 Alan Borning and Greg Badros
// See ../COPYRIGHT for legal details regarding this software
//
// ClLinearExpression.h

#ifndef ClLinearExpression_H
#define ClLinearExpression_H

#include <map>
#include "Cassowary.h"
#include "debug.h"
#include "ClAbstractVariable.h"
#include "ClLinearExpression_fwd.h"

class ClSimplexSolver;
class ClTableau;
class ClSymbolicWeight;

ClLinearExpression &cleNil();


template <class T>
class ClGenericLinearExpression  {
 public:
  typedef map<const ClAbstractVariable *,T> ClVarToCoeffMap;

  // convert Number-s into ClLinearExpression-s
  ClGenericLinearExpression(T num = 0.0);

  // Convert from ClVariable to a ClLinearExpression
  // this replaces ClVariable::asLinearExpression
  ClGenericLinearExpression(const ClAbstractVariable &clv, T value = 1.0, T constant = 0.0);

  // copy ctr
  ClGenericLinearExpression(const ClGenericLinearExpression<T> &expr) :
    _constant(expr._constant),
    _terms(expr._terms)
    { }

  virtual ~ClGenericLinearExpression();

  // Return a new linear expression formed by multiplying self by x.
  // (Note that this result must be linear.)
  ClGenericLinearExpression<T> times(Number x) const;

  // Return a new linear expression formed by multiplying self by x.
  // (Note that this result must be linear.)
  ClGenericLinearExpression<T> times(const ClGenericLinearExpression<T> &expr) const;

  // Return a new linear expression formed by adding x to self.
  ClGenericLinearExpression<T> plus(const ClGenericLinearExpression<T> &expr) const;

  // Return a new linear expression formed by subtracting x from self.
  ClGenericLinearExpression<T> minus(const ClGenericLinearExpression<T> &expr) const;

  // Return a new linear expression formed by dividing self by x.
  // (Note that this result must be linear.)
  ClGenericLinearExpression<T> divide(Number x) const;



  // Return a new linear expression formed by multiplying self by x.
  // (Note that this result must be linear.)
  ClGenericLinearExpression<T> *p_times(Number x) const
    { return new ClGenericLinearExpression<T>(times(x)); }

  // Return a new linear expression formed by adding x to self.
  ClGenericLinearExpression<T> *p_plus(const ClGenericLinearExpression<T> &expr) const
    { return new ClGenericLinearExpression<T>(plus(expr)); }

  // Return a new linear expression formed by subtracting x from self.
  ClGenericLinearExpression<T> *p_minus(const ClGenericLinearExpression<T> &expr) const
    { return new ClGenericLinearExpression<T>(minus(expr)); }

  // Return a new linear expression formed by dividing self by x.
  // (Note that this result must be linear.)
  ClGenericLinearExpression<T> *p_divide(Number x) const
    { return new ClGenericLinearExpression<T>(divide(x)); }

  // Return a new linear expression formed by dividing self by x.
  // (Note that this result must be linear.)
  ClGenericLinearExpression<T> divide(const ClGenericLinearExpression<T> &expr) const;

  // Return a new linear expression (aNumber/this).  Since the result
  // must be linear, this is permissible only if 'this' is a constant.
  ClGenericLinearExpression<T> divFrom(const ClGenericLinearExpression<T> &expr) const;

  // Return a new linear expression (aNumber-this).
  ClGenericLinearExpression<T> subtractFrom(const ClGenericLinearExpression<T> &expr) const
  { return expr.minus(*this); }

  // Add n*expr to this expression for another expression expr.
  ClGenericLinearExpression<T> &addExpression(const ClGenericLinearExpression<T> &expr, 
				    Number n = 1.0);

  // Add n*expr to this expression for another expression expr.
  // Notify the solver if a variable is added or deleted from this
  // expression.
  ClGenericLinearExpression<T> &addExpression(const ClGenericLinearExpression<T> &expr, Number n,
				    const ClAbstractVariable &subject,
				    ClTableau &solver);

  // Add a term c*v to this expression.  If the expression already
  // contains a term involving v, add c to the existing coefficient.
  // If the new coefficient is approximately 0, delete v.
  ClGenericLinearExpression<T> &addVariable(const ClAbstractVariable &v, T c = 1.0);

  // Add a term c*v to this expression.  If the expression already
  // contains a term involving v, add c to the existing coefficient.
  // If the new coefficient is approximately 0, delete v.
  ClGenericLinearExpression<T> &setVariable(const ClAbstractVariable &v, T c)
    {assert(c != 0.0);  _terms[&v] = c; return *this; }

  // Add a term c*v to this expression.  If the expression already
  // contains a term involving v, add c to the existing coefficient.
  // If the new coefficient is approximately 0, delete v.  Notify the
  // solver if v appears or disappears from this expression.
  ClGenericLinearExpression<T> &addVariable(const ClAbstractVariable &v, T c,
				  const ClAbstractVariable &subject,
				  ClTableau &solver);

  // Return a pivotable variable in this expression.  (It is an error
  // if this expression is constant -- signal ExCLInternalError in
  // that case).  Return NULL if no pivotable variables
  const ClAbstractVariable *anyPivotableVariable() const;

  // Replace var with a symbolic expression expr that is equal to it.
  // If a variable has been added to this expression that wasn't there
  // before, or if a variable has been dropped from this expression
  // because it now has a coefficient of 0, inform the solver.
  // PRECONDITIONS:
  //   var occurs with a non-zero coefficient in this expression.
  void substituteOut(const ClAbstractVariable &v, 
		     const ClGenericLinearExpression<T> &expr,
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
  T newSubject(const ClAbstractVariable &subject);

  // Return the coefficient corresponding to variable var, i.e.,
  // the 'ci' corresponding to the 'vi' that var is:
  //     v1*c1 + v2*c2 + .. + vn*cn + c
  T coefficientFor(const ClAbstractVariable &var) const
    { 
    map<const ClAbstractVariable *, T>::const_iterator it = _terms.find(&var);
    if (it != _terms.end())
      return (*it).second;
    return 0.0;
    }

  T constant() const
    { return _constant; }

  void set_constant(T c)
    { _constant = c; }

  const ClVarToCoeffMap &terms() const
    { return _terms; }

  ClVarToCoeffMap &terms() 
    { return _terms; }

  void incrementConstant(T c)
    { _constant += c; }

  bool isConstant() const
    { return _terms.size() == 0; }

  virtual ostream &printOn(ostream &xo) const;

  friend ostream &operator<<(ostream &xo,const ClGenericLinearExpression<T> &cle)
    { return cle.printOn(xo); }

  friend ClGenericLinearExpression<T> operator+(const ClGenericLinearExpression<T> &e1,
				      const ClGenericLinearExpression<T> &e2)
    { return e1.plus(e2); }

  friend ClGenericLinearExpression<T> operator-(const ClGenericLinearExpression<T> &e1,
				      const ClGenericLinearExpression<T> &e2)
    { return e1.minus(e2); }

  friend ClGenericLinearExpression<T> operator*(const ClGenericLinearExpression<T> &e1,
				      const ClGenericLinearExpression<T> &e2)
    { return e1.times(e2); }


  friend ClGenericLinearExpression<T> operator/(const ClGenericLinearExpression<T> &e1,
				      const ClGenericLinearExpression<T> &e2)
    { return e1.divide(e2); }

  // FIXGJB -- this may be wrong -- should test underlying expression for equality
  friend bool operator==(const ClGenericLinearExpression<T> &e1,
			 const ClGenericLinearExpression<T> &e2)
    { return &e1 == &e2; }

  /// Named versions of the operator functions for ease of
  /// wrapping, or expressing using prefix notation

  friend ClGenericLinearExpression<T> Plus(const ClGenericLinearExpression<T> &e1,
				 const ClGenericLinearExpression<T> &e2)
    { return e1.plus(e2); }

  friend ClGenericLinearExpression<T> Minus(const ClGenericLinearExpression<T> &e1,
				  const ClGenericLinearExpression<T> &e2)
    { return e1.minus(e2); }

  friend ClGenericLinearExpression<T> Times(const ClGenericLinearExpression<T> &e1,
				  const ClGenericLinearExpression<T> &e2)
    { return e1.times(e2); }


  friend ClGenericLinearExpression<T> *Divide(const ClGenericLinearExpression<T> &e1,
				   const ClGenericLinearExpression<T> &e2)
    { return new ClGenericLinearExpression<T>(e1.divide(e2)); }

  friend ClGenericLinearExpression<T> *p_Plus(const ClGenericLinearExpression<T> &e1,
				   const ClGenericLinearExpression<T> &e2)
    { return new ClGenericLinearExpression<T>(e1.plus(e2)); }

  friend ClGenericLinearExpression<T> *p_Minus(const ClGenericLinearExpression<T> &e1,
				    const ClGenericLinearExpression<T> &e2)
    { return new ClGenericLinearExpression<T>(e1.minus(e2)); }

  friend ClGenericLinearExpression<T> *p_Times(const ClGenericLinearExpression<T> &e1,
				    const ClGenericLinearExpression<T> &e2)
    { return new ClGenericLinearExpression<T>(e1.times(e2)); }

  friend ClGenericLinearExpression<T> *p_Divide(const ClGenericLinearExpression<T> &e1,
				     const ClGenericLinearExpression<T> &e2)
    { return new ClGenericLinearExpression<T>(e1.divide(e2)); }


  // FIXGJB -- this may be wrong -- should test underlying expression for equality
  friend bool FEquals(const ClGenericLinearExpression<T> &e1,
		      const ClGenericLinearExpression<T> &e2)
    { return &e1 == &e2; }

  ClGenericLinearExpression<T> &multiplyMe(T x);

 private:

  T _constant;
  ClVarToCoeffMap _terms;

};

typedef ClGenericLinearExpression<Number>::ClVarToCoeffMap ClVarToNumberMap;

#endif
