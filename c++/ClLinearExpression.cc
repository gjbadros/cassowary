// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998 Alan Borning and Greg Badros
// See ../COPYRIGHT for legal details regarding this software
//
// ClLinearExpression.cc

#include "ClLinearExpression.h"
#include "ClSymbolicWeight.h"  /// needed only to instantiate with T=ClSymbolicWeight
#include "ClVariable.h"
#include "ClTableau.h"
#include "ClErrors.h"

template <class T>
ClGenericLinearExpression<T>::ClGenericLinearExpression(T num) :
    _constant(num)
{ }

// Convert from ClVariable to a ClLinearExpression
// this replaces ClVariable::asLinearExpression
template <class T>
ClGenericLinearExpression<T>::ClGenericLinearExpression(const ClAbstractVariable &clv, T value,
                                                        T constant) :
  _constant(constant)
{
  _terms[&clv] = value;
}

template <class T>
ClGenericLinearExpression<T>::~ClGenericLinearExpression()
{ }

template <class T>
ostream &
ClGenericLinearExpression<T>::printOn(ostream &xo) const
{
  ClVarToCoeffMap::const_iterator i = _terms.begin();

  if (!clApprox(_constant,0.0) || i == _terms.end())
    {
    xo << _constant;
    }
  else
    {
    if (i == _terms.end())
      return xo;
    xo << (*i).second << "*" << *((*i).first);
    ++i;
    }
  for ( ; i != _terms.end(); ++i)
    {
    xo << " + " << (*i).second << "*" << *((*i).first);
    }
  return xo;
}



// Destructively multiply self by x.
// (private memfn)
template <class T>
ClGenericLinearExpression<T> &
ClGenericLinearExpression<T>::multiplyMe(T x)
{
  _constant *= x;

  ClVarToCoeffMap::const_iterator i = _terms.begin();
  for ( ; i != _terms.end(); ++i)
    {
    _terms[(*i).first] = (*i).second * x;
    }
  return *this;
}

// Return a new linear expression formed by multiplying self by x.
// (Note that this result must be linear.)
template <class T>
ClGenericLinearExpression<T> 
ClGenericLinearExpression<T>::times(Number x) const
{
  ClGenericLinearExpression<T> result = *this;
  return result.multiplyMe(x);
}

// Return a new linear expression formed by multiplying self by x.
// (Note that this result must be linear.)
// The above function optimizes the specific case of multiplying
// by a constant, here is the more general case
template <class T>
ClGenericLinearExpression<T> 
ClGenericLinearExpression<T>::times(const ClGenericLinearExpression<T> &expr) const
{
  if (isConstant())
    {
    return expr.times(_constant);
    }
  else if (!expr.isConstant())
    {
    // neither are constants, so we'd introduce non-linearity
    throw ExCLNonlinearExpression();
    }	
  return times(expr._constant);
}


// Return a new linear expression formed by adding x to self.
template <class T>
ClGenericLinearExpression<T> 
ClGenericLinearExpression<T>::plus(const ClGenericLinearExpression<T> &expr) const
{
  ClGenericLinearExpression<T> result = *this;
  result.addExpression(expr,1.0);
  return result;
}

// Return a new linear expression formed by subtracting x from self.
template <class T>
ClGenericLinearExpression<T> 
ClGenericLinearExpression<T>::minus(const ClGenericLinearExpression<T> &expr) const
{
  ClGenericLinearExpression<T> result = *this;
  result.addExpression(expr,-1.0);
  return result;
}

// Return a new linear expression formed by dividing self by x.
// (Note that this result must be linear.)
template <class T>
ClGenericLinearExpression<T> 
ClGenericLinearExpression<T>::divide(Number x) const
{
  if (clApprox(x,0.0))
    {
    throw ExCLNonlinearExpression();
    }
  return times(1.0/x);
}

// Return a new linear expression formed by dividing self by x.
// (Note that this result must be linear.)
template <class T>
ClGenericLinearExpression<T> 
ClGenericLinearExpression<T>::divide(const ClGenericLinearExpression<T> &expr) const
{
  if (!expr.isConstant())
    {
    throw ExCLNonlinearExpression();
    }
  return divide(expr._constant);
}


// Return a new linear expression (expr/this).  Since the result
// must be linear, this is permissible only if 'this' is a constant.
template <class T>
ClGenericLinearExpression<T> 
ClGenericLinearExpression<T>::divFrom(const ClGenericLinearExpression<T> &expr) const
{
  if (!isConstant() || clApprox(_constant,0.0))
    {
    throw ExCLNonlinearExpression();
    }
  return expr.divide(_constant);
}

// Add n*expr to this expression for another expression expr.
template <class T>
ClGenericLinearExpression<T> &
ClGenericLinearExpression<T>::addExpression(const ClGenericLinearExpression<T> &expr, Number n)
{
  incrementConstant(expr.constant()*n);

  ClVarToCoeffMap::const_iterator i = expr._terms.begin();
  for ( ; i != expr._terms.end(); ++i)
    {
    addVariable(*((*i).first), (*i).second * n);
    }
  return *this;
}

// Add n*expr to this expression for another expression expr.
// Notify the solver if a variable is added or deleted from this
// expression.
template <class T>
ClGenericLinearExpression<T> &
ClGenericLinearExpression<T>::addExpression(const ClGenericLinearExpression<T> &expr, Number n,
				  const ClAbstractVariable &subject,
				  ClTableau &solver)
{
  incrementConstant(expr.constant() * n);

  ClVarToCoeffMap::const_iterator i = expr._terms.begin();
  for ( ; i != expr._terms.end(); ++i)
    {
    addVariable(*((*i).first), (*i).second * n, subject, solver);
    }
  return *this;
}

// Add a term c*v to this expression.  If the expression already
// contains a term involving v, add c to the existing coefficient.
// If the new coefficient is approximately 0, delete v.
template <class T>
ClGenericLinearExpression<T> &
ClGenericLinearExpression<T>::addVariable(const ClAbstractVariable &v, T c)
{ // body largely duplicated below
#ifndef CL_NO_TRACE
  Tracer TRACER(__FUNCTION__);
  cerr << "(" << v << ", " << c << ")" << endl;
#endif
  ClVarToCoeffMap::iterator i = _terms.find(&v);
  if (i != _terms.end())
    {
    // expression already contains that variable, so add to it
    T new_coefficient = 0;
    new_coefficient = (*i).second + c;
    if (clApprox(new_coefficient,0.0))
      {
      // new coefficient is zero, so erase it
      _terms.erase(i);
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
      _terms[&v] = c;
      }
    }
  return *this;
}

// Add a term c*v to this expression.  If the expression already
// contains a term involving v, add c to the existing coefficient.
// If the new coefficient is approximately 0, delete v.  Notify the
// solver if v appears or disappears from this expression.
template <class T>
ClGenericLinearExpression<T> &
ClGenericLinearExpression<T>::addVariable(const ClAbstractVariable &v, T c,
                                          const ClAbstractVariable &subject,
                                          ClTableau &solver)
{ // body largely duplicated above
#ifndef CL_NO_TRACE
  Tracer TRACER(__FUNCTION__);
  cerr << "(" << v << ", " << c << ", " << subject << ", ...)" << endl;
#endif
  ClVarToCoeffMap::iterator i = _terms.find(&v);
  if (i != _terms.end())
    {
    // expression already contains that variable, so add to it
    T new_coefficient = (*i).second + c;
    if (clApprox(new_coefficient,0.0))
      {
      // new coefficient is zero, so erase it
      solver.noteRemovedVariable(*((*i).first),subject);
      _terms.erase(i);
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
      _terms[&v] = c;
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
template <class T>
const ClAbstractVariable *
ClGenericLinearExpression<T>::anyPivotableVariable() const
{
  if (isConstant())
    {
    throw ExCLInternalError("(ExCLInternalError) No pivotable variables in constant expression");
    }
  ClVarToCoeffMap::const_iterator i = _terms.begin();
  for ( ; i != _terms.end(); ++i)
    {
    if (((*i).first)->isPivotable())
      return (*i).first;
    }
  return NULL;
}

// Replace var with a symbolic expression expr that is equal to it.
// If a variable has been added to this expression that wasn't there
// before, or if a variable has been dropped from this expression
// because it now has a coefficient of 0, inform the solver.
// PRECONDITIONS:
//   var occurs with a non-zero coefficient in this expression.
template <class T>
void 
ClGenericLinearExpression<T>::substituteOut(const ClAbstractVariable &var, 
                                            const ClGenericLinearExpression<T> &expr,
                                            const ClAbstractVariable &subject,
                                            ClTableau &solver)
{
#ifndef CL_NO_TRACE
  cerr << "* ClGenericLinearExpression::";
  Tracer TRACER(__FUNCTION__);
  cerr << "(" << var << ", " << expr << ", " << subject << ", " 
       << solver << ")" << endl;
  cerr << "*this == " << *this << endl;
#endif

  ClVarToCoeffMap::iterator pv = _terms.find(&var);

#ifndef NDEBUG
  if (pv == _terms.end())
    {
    cerr << "substituteOut: pv != _terms.end()" << endl;
    cerr << "(" << var << ", " << expr << ", " << subject << ", " 
         << ")" << endl;
    cerr << "*this == " << *this << endl;
    }
#endif
  assert(pv != _terms.end());
  // FIXGJB: this got thrown! assert(!clApprox((*pv).second,0.0));

  T multiplier = (*pv).second;
  _terms.erase(pv);
  incrementConstant(multiplier * expr._constant);
  ClVarToCoeffMap::const_iterator i = expr._terms.begin();
  for ( ; i != expr._terms.end(); ++i)
    {
    const ClAbstractVariable *pv = (*i).first;
    T c = (*i).second;
    ClVarToCoeffMap::iterator poc = _terms.find(pv);
    if (poc != _terms.end())
      { // if oldCoeff is not nil
#ifndef CL_NO_TRACE
      cerr << "Considering (*poc) == " << (*poc).second << "*" << *(*poc).first << endl;
#endif
      // found it, so new coefficient is old one plus what is in *i
      T newCoeff = (*poc).second + (multiplier*c);
      if (clApprox(newCoeff,0.0))
	{
	solver.noteRemovedVariable(*((*poc).first),subject);
	_terms.erase(poc);
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
      _terms[pv] = multiplier * c;
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
template <class T>
void 
ClGenericLinearExpression<T>::changeSubject(const ClAbstractVariable &old_subject,
                                            const ClAbstractVariable &new_subject)
{
  _terms[&old_subject] = newSubject(new_subject);
}

inline double ReciprocalOf(double n)
{ return 1.0/n; }

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
template <class T>
T
ClGenericLinearExpression<T>::newSubject(const ClAbstractVariable &subject)
{
#ifndef CL_NO_TRACE
  Tracer TRACER(__FUNCTION__);
  cerr << "(" << subject << ")" << endl;
#endif
  ClVarToCoeffMap::iterator pnewSubject = _terms.find(&subject);
  assert(pnewSubject != _terms.end());
  //  assert(!clApprox((*pnewSubject).second,0.0));
  T reciprocal = ReciprocalOf((*pnewSubject).second);
  _terms.erase(pnewSubject);
  multiplyMe(-reciprocal);
  return reciprocal;
}

template <class T>
T
ClGenericLinearExpression<T>::evaluate() const
{
  T answer = _constant;
  ClVarToCoeffMap::const_iterator i = _terms.begin();

  for ( ; i != _terms.end(); ++i)
    {
    answer += (*i).second * ((*i).first)->value();
    }
  return answer;
}


template class ClGenericLinearExpression<Number>;
// template class ClGenericLinearExpression<ClSymbolicWeight>;
