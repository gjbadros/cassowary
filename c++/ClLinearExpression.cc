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
#include "ClVariable.h"
#include "ClTableau.h"
#include "ClErrors.h"

ClLinearExpression::ClLinearExpression(Number num) :
    my_constant(num)
{ }

// Convert from ClVariable to a ClLinearExpression
// this replaces ClVariable::asLinearExpression
ClLinearExpression::ClLinearExpression(const ClAbstractVariable &clv, Number value) :
  my_constant(0.0)
{
  my_terms[&clv] = value;
}

ClLinearExpression::~ClLinearExpression()
{ }

ostream &
ClLinearExpression::printOn(ostream &xo) const
{
  ClVarToNumberMap::const_iterator i = my_terms.begin();

  if (!clApprox(my_constant,0.0) || i == my_terms.end())
    {
    xo << my_constant;
    }
  else
    {
    if (i == my_terms.end())
      return xo;
    xo << (*i).second << "*" << *((*i).first);
    ++i;
    }
  for ( ; i != my_terms.end(); ++i)
    {
    xo << " + " << (*i).second << "*" << *((*i).first);
    }
  return xo;
}



// Destructively multiply self by x.
// (private memfn)
ClLinearExpression &
ClLinearExpression::multiplyMe(Number x)
{
  my_constant *= x;

  ClVarToNumberMap::const_iterator i = my_terms.begin();
  for ( ; i != my_terms.end(); ++i)
    {
    my_terms[(*i).first] = (*i).second * x;
    }
  return *this;
}

// Return a new linear expression formed by multiplying self by x.
// (Note that this result must be linear.)
ClLinearExpression 
ClLinearExpression::times(Number x) const
{
  ClLinearExpression result = *this;
  return result.multiplyMe(x);
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
  else if (!expr.isConstant())
    {
    // neither are constants, so we'd introduce non-linearity
    throw ExCLNonlinearExpression();
    }	
  return times(expr.my_constant);
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
    throw ExCLNonlinearExpression();
    }
  return times(1.0/x);
}

// Return a new linear expression formed by dividing self by x.
// (Note that this result must be linear.)
ClLinearExpression 
ClLinearExpression::divide(const ClLinearExpression &expr) const
{
  if (!expr.isConstant())
    {
    throw ExCLNonlinearExpression();
    }
  return divide(expr.my_constant);
}


// Return a new linear expression (expr/this).  Since the result
// must be linear, this is permissible only if 'this' is a constant.
ClLinearExpression 
ClLinearExpression::divFrom(const ClLinearExpression &expr) const
{
  if (!isConstant() || clApprox(my_constant,0.0))
    {
    throw ExCLNonlinearExpression();
    }
  return expr.divide(my_constant);
}

// Add n*expr to this expression for another expression expr.
ClLinearExpression &
ClLinearExpression::addExpression(const ClLinearExpression &expr, Number n)
{
  incrementConstant(n*expr.constant());

  ClVarToNumberMap::const_iterator i = expr.my_terms.begin();
  for ( ; i != expr.my_terms.end(); ++i)
    {
    addVariable(*((*i).first), n * (*i).second);
    }
  return *this;
}

// Add n*expr to this expression for another expression expr.
// Notify the solver if a variable is added or deleted from this
// expression.
ClLinearExpression &
ClLinearExpression::addExpression(const ClLinearExpression &expr, Number n,
				  const ClAbstractVariable &subject,
				  ClTableau &solver)
{
  incrementConstant(n*expr.constant());

  ClVarToNumberMap::const_iterator i = expr.my_terms.begin();
  for ( ; i != expr.my_terms.end(); ++i)
    {
    addVariable(*((*i).first), n * (*i).second, subject, solver);
    }
  return *this;
}

// Add a term c*v to this expression.  If the expression already
// contains a term involving v, add c to the existing coefficient.
// If the new coefficient is approximately 0, delete v.
ClLinearExpression &
ClLinearExpression::addVariable(const ClAbstractVariable &v, Number c)
{ // body largely duplicated below
#ifndef CL_NO_TRACE
  Tracer TRACER(__FUNCTION__);
  cerr << "(" << v << ", " << c << ")" << endl;
#endif
  ClVarToNumberMap::iterator i = my_terms.find(&v);
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
      my_terms[&v] = c;
      }
    }
  return *this;
}

// Add a term c*v to this expression.  If the expression already
// contains a term involving v, add c to the existing coefficient.
// If the new coefficient is approximately 0, delete v.  Notify the
// solver if v appears or disappears from this expression.
ClLinearExpression &
ClLinearExpression::addVariable(const ClAbstractVariable &v, Number c,
				const ClAbstractVariable &subject,
				ClTableau &solver)
{ // body largely duplicated above
#ifndef CL_NO_TRACE
  Tracer TRACER(__FUNCTION__);
  cerr << "(" << v << ", " << c << ", " << subject << ", ...)" << endl;
#endif
  ClVarToNumberMap::iterator i = my_terms.find(&v);
  if (i != my_terms.end())
    {
    // expression already contains that variable, so add to it
    Number new_coefficient = 0;
    new_coefficient = (*i).second + c;
    if (clApprox(new_coefficient,0.0))
      {
      // new coefficient is zero, so erase it
      solver.noteRemovedVariable(*((*i).first),subject);
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
      my_terms[&v] = c;
      solver.noteAddedVariable(v,subject);
      }
    }
#ifndef CL_NO_TRACE
  cerr << "Now *this == " << *this << endl;
#endif
  return *this;
}

// Return a variable in this expression.  (It is an error if this
// expression is constant -- signal ExCLInternalError in that case).
const ClAbstractVariable *
ClLinearExpression::anyVariable() const
{
  if (isConstant())
    {
    throw ExCLInternalError();
    }
  return (*my_terms.begin()).first;
}

// Replace var with a symbolic expression expr that is equal to it.
// If a variable has been added to this expression that wasn't there
// before, or if a variable has been dropped from this expression
// because it now has a coefficient of 0, inform the solver.
// PRECONDITIONS:
//   var occurs with a non-zero coefficient in this expression.
void 
ClLinearExpression::substituteOut(const ClAbstractVariable &var, 
				  const ClLinearExpression &expr,
				  const ClAbstractVariable &subject,
				  ClTableau &solver)
{
#ifndef CL_NO_TRACE
  cerr << "* ClLinearExpression::";
  Tracer TRACER(__FUNCTION__);
  cerr << "(" << var << ", " << expr << ", " << subject << ", " 
       << solver << ")" << endl;
  cerr << "*this == " << *this << endl;
#endif
  ClVarToNumberMap::iterator pv = my_terms.find(&var);
  assert(pv != my_terms.end());
  // FIXGJB: this got thrown! assert(!clApprox((*pv).second,0.0));

  Number multiplier = (*pv).second;
  my_terms.erase(pv);
  incrementConstant(multiplier * expr.my_constant);
  ClVarToNumberMap::const_iterator i = expr.my_terms.begin();
  for ( ; i != expr.my_terms.end(); ++i)
    {
    const ClAbstractVariable *pv = (*i).first;
    Number c = (*i).second;
    ClVarToNumberMap::iterator poc = my_terms.find(pv);
    if (poc != my_terms.end())
      { // if oldCoeff is not nil
#ifndef CL_NO_TRACE
      cerr << "Considering (*poc) == " << (*poc).second << "*" << *(*poc).first << endl;
#endif
      // found it, so new coefficient is old one plus what is in *i
      Number newCoeff = (*poc).second + (multiplier*c);
      if (clApprox(newCoeff,0.0))
	{
	solver.noteRemovedVariable(*((*poc).first),subject);
	my_terms.erase(poc);
	}
      else
	{
	(*poc).second = newCoeff;
	}
      }
    else
      { // did not have that variable already (oldCoeff == nil)
#ifndef CL_NO_TRACE
      cerr << "Adding (*i) == " << (*i).second << "*" << *(*i).first << endl;
#endif
      my_terms[pv] = multiplier * c;
      solver.noteAddedVariable(*pv,subject);
      }
    }
#ifndef CL_NO_TRACE
  cerr << "Now (*this) is " << *this << endl;
#endif
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
// 
// Basically, we consider the expression to be an equation with oldSubject
// equal to the expression, then resolve the equation for newSubject,
// and destructively make the expression what newSubject is then equal to
void 
ClLinearExpression::changeSubject(const ClAbstractVariable &old_subject,
				  const ClAbstractVariable &new_subject)
{
  my_terms[&old_subject] = newSubject(new_subject);
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
//    subject = -c/a - (a1/a)*v1 - ... - (an/a)*vn = 0
//
// Note that the term involving subject has been dropped.
//
// Returns the reciprocal, so that newSubject can be used by changeSubject
Number
ClLinearExpression::newSubject(const ClAbstractVariable &subject)
{
#ifndef CL_NO_TRACE
  Tracer TRACER(__FUNCTION__);
  cerr << "(" << subject << ")" << endl;
#endif
  ClVarToNumberMap::iterator pnewSubject = my_terms.find(&subject);
  assert(pnewSubject != my_terms.end());
  //  assert(!clApprox((*pnewSubject).second,0.0));
  Number reciprocal = 1.0 / (*pnewSubject).second;
  my_terms.erase(pnewSubject);
  multiplyMe(-reciprocal);
  return reciprocal;
}
