// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998, All Rights Reserved.
//
// ClLinearExpression.cc

#include <assert.h>
#include "ClLinearExpression.h"
#include "ClErrors.h"

ClLinearExpression::ClLinearExpression(void) :
    my_constant(0.0)
{ }

  // Convert from ClVariable to a ClLinearExpression
  // this replaces ClVariable::asLinearExpression
ClLinearExpression::ClLinearExpression(const ClVariable &clv) :
  my_constant(0.0)
{
  my_terms[clv] = 1.0;
}

ClLinearExpression::~ClLinearExpression()
{ }

ostream &
ClLinearExpression::printOn(ostream &xo) const
{
  xo << my_constant;
  map<ClVariable,Number>::const_iterator i = my_terms.begin();
  for ( ; i != my_terms.end(); ++i)
    {
    xo << " + " << (*i).first << "*" << (*i).second;
    }
  return xo;
}


// Return a new linear expression formed by multiplying self by x.
// (Note that this result must be linear.)
ClLinearExpression 
ClLinearExpression::times(Number x) const
{
  ClLinearExpression result;
  result.my_constant = my_constant * x;

  map<ClVariable,Number>::const_iterator i = my_terms.begin();
  for ( ; i != my_terms.end(); ++i)
    {
    result.my_terms[(*i).first] = (*i).second * x;
    }
  return result;
}

// Return a new linear expression formed by multiplying self by x.
// (Note that this result must be linear.)
// The above function optimizes the specific case of multiplying
// by a constant, here is the more general case
ClLinearExpression 
ClLinearExpression::times(const ClLinearExpression &expr) const
{
  if (isConstant())
    {
    return expr.times(my_constant);
    }
  else if (expr.isConstant())
    {
    return times(expr.my_constant);
    }
  else
    {
    // neither are constants, so we'd introduce non-linearity
    throw new ExCLNonlinearExpression;
    }
}


// Return a new linear expression formed by adding x to self.
ClLinearExpression 
ClLinearExpression::plus(const ClLinearExpression &expr) const
{
  ClLinearExpression result = *this;
  result.addExpression(expr,1.0);
  return result;
}

// Return a new linear expression formed by subtracting x from self.
ClLinearExpression 
ClLinearExpression::minus(const ClLinearExpression &expr) const
{
  ClLinearExpression result = *this;
  result.addExpression(expr,-1.0);
  return result;
}

// Return a new linear expression formed by dividing self by x.
// (Note that this result must be linear.)
ClLinearExpression 
ClLinearExpression::divide(Number x) const
{
  if (clApprox(x,0.0))
    {
    throw new ExCLNonlinearExpression;
    }
  else
    {
    return times(1.0/x);
    }
}

// Return a new linear expression formed by dividing self by x.
// (Note that this result must be linear.)
ClLinearExpression 
ClLinearExpression::divide(const ClLinearExpression &expr) const
{
  if (!expr.isConstant())
    {
    throw new ExCLNonlinearExpression;
    }
  else
    {
    return divide(expr.my_constant);
    }
}


// Return a new linear expression (expr/this).  Since the result
// must be linear, this is permissible only if 'this' is a constant.
ClLinearExpression 
ClLinearExpression::divFrom(const ClLinearExpression &expr) const
{
  if (!isConstant() || clApprox(my_constant,0.0))
    {
    throw new ExCLNonlinearExpression;
    }
  else
    {
    return expr.divide(my_constant);
    }
}

// Add n*expr to this expression for another expression expr.
ClLinearExpression &
ClLinearExpression::addExpression(const ClLinearExpression &expr, Number n)
{
  incrementConstant(n*expr.constant());

  map<ClVariable,Number>::const_iterator i = expr.my_terms.begin();
  for ( ; i != expr.my_terms.end(); ++i)
    {
    addVariable((*i).first, n * (*i).second);
    }
  return *this;
}

// Add n*expr to this expression for another expression expr.
// Notify the solver if a variable is added or deleted from this
// expression.
ClLinearExpression &
ClLinearExpression::addExpression(const ClLinearExpression &expr, Number n,
				  const ClAbstractVariable &subject,
				  const ClSimplexSolver &solver)
{
  incrementConstant(n*expr.constant());

  map<ClVariable,Number>::const_iterator i = expr.my_terms.begin();
  for ( ; i != expr.my_terms.end(); ++i)
    {
    addVariable((*i).first, n * (*i).second, subject, solver);
    }
  return *this;
}

// Add a term c*v to this expression.  If the expression already
// contains a term involving v, add c to the existing coefficient.
// If the new coefficient is approximately 0, delete v.
ClLinearExpression &
ClLinearExpression::addVariable(const ClVariable &v, Number c)
{
  map<ClVariable,Number>::iterator i = my_terms.find(v);
  if (i != my_terms.end())
    {
    // expression already contains that variable, so add to it
    Number new_coefficient = 0;
    new_coefficient = (*i).second + c;
    if (clApprox(new_coefficient,0.0))
      {
      // new coefficient is zero, so erase it
      my_terms.erase(i);
      }
    else
      {
      (*i).second = new_coefficient;
      }
    }
  else // expression did not contain that variable
    {
    if (!clApprox(c,0.0))
      {
      my_terms[v] = c;
      }
    }
  return *this;
}

// Add a term c*v to this expression.  If the expression already
// contains a term involving v, add c to the existing coefficient.
// If the new coefficient is approximately 0, delete v.  Notify the
// solver if v appears or disappears from this expression.
ClLinearExpression &
ClLinearExpression::addVariable(const ClVariable &v, Number c,
				const ClAbstractVariable &subject,
				const ClSimplexSolver &solver)
{
  addVariable(v,c);
  // FIXGJB: still need to handle subject, solver params
  assert(false);
  return *this;
}

// Replace var with a symbolic expression expr that is equal to it.
// If a variable has been added to this expression that wasn't there
// before, or if a variable has been dropped from this expression
// because it now has a coefficient of 0, inform the solver.
// PRECONDITIONS:
//   var occurs with a non-zero coefficient in this expression.
void 
ClLinearExpression::substituteOut(const ClVariable &v, 
				  const ClLinearExpression &expr,
				  const ClAbstractVariable &subject,
				  const ClSimplexSolver &solver)
{
  assert(false);
}

// Return a variable in this expression.  (It is an error if this
// expression is constant -- signal ExCLInternalError in that case).
const ClAbstractVariable &
ClLinearExpression::anyVariable() const
{
  if (isConstant())
    {
    throw new ExCLInternalError();
    }
  return (*my_terms.begin()).first;
}

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
void 
ClLinearExpression::changeSubject(const ClAbstractVariable &oldSubject,
				  const ClAbstractVariable &newSubject)
{
  assert(false);
}

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
void 
ClLinearExpression::newSubject(const ClAbstractVariable &subject)
{
  assert(false);
}
