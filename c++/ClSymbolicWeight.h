// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998, 1999 Alan Borning and Greg Badros
// See ../LICENSE for legal details regarding this software
//
// ClSymbolicWeight.h

#ifndef ClSymbolicWeight_H
#define ClSymbolicWeight_H

#include "Cassowary.h"
#include "ClErrors.h"
#include <vector>

#ifdef USE_GC_WEIGHT
class ClSymbolicWeight : public gc {
#else
class ClSymbolicWeight {
#endif
 public:
  ClSymbolicWeight() 
    { }

  ClSymbolicWeight(int cLevels, Number value = 0.0);

  ClSymbolicWeight(Number w1, Number w2 = 0.0, Number w3 = 0.0);

  ClSymbolicWeight(const vector<Number> &weights);

  static ClSymbolicWeight &zero();

  ClSymbolicWeight &negated();

  ClSymbolicWeight &multiplyMe(Number n);

  ClSymbolicWeight times(Number n) const
    { ClSymbolicWeight cl = *this; cl.multiplyMe(n); return cl; }

  ClSymbolicWeight divideBy(Number n) const;

  ClSymbolicWeight &addtoMe(const ClSymbolicWeight &cl);

  ClSymbolicWeight add(const ClSymbolicWeight &cl) const
    { ClSymbolicWeight clRet = *this; clRet.addtoMe(cl); return clRet; }

  ClSymbolicWeight subtract(const ClSymbolicWeight &cl) const;

  ClSymbolicWeight operator*(const Number &n) const
    { return times(n); }

  ClSymbolicWeight operator/(const Number &n) const
    { return divideBy(n); }

  // FIXGJB: can we express this statically?
  ClSymbolicWeight operator*(ClSymbolicWeight &w) const
    { throw ExCLInternalError("Multiplication of symbolic weights encountered"); 
      return w; }
  ClSymbolicWeight &operator*=(ClSymbolicWeight &w)
    { throw ExCLInternalError("Multiplicative assignment of symbolic weights encountered"); 
      return w; }

  // FIXGJB: can we express this statically?
  ClSymbolicWeight operator-() const
    { throw ExCLInternalError("Can not negate a symbolic weight");
      return ClSymbolicWeight::zero(); }

  friend ClSymbolicWeight ReciprocalOf(const ClSymbolicWeight &);

  ClSymbolicWeight &operator*=(const Number &n)
    { return multiplyMe(n); }

  ClSymbolicWeight operator+(const ClSymbolicWeight &cl) const
    { return add(cl); }

  ClSymbolicWeight operator+=(const ClSymbolicWeight &cl)
    { return addtoMe(cl); }

  bool lessThan(const ClSymbolicWeight &cl) const;
  bool lessThanOrEqual(const ClSymbolicWeight &cl) const;
  bool equal(const ClSymbolicWeight &cl) const;
  bool greaterThan(const ClSymbolicWeight &cl) const;
  bool greaterThanOrEqual(const ClSymbolicWeight &cl) const;
  bool isNegative() const;

  friend bool operator==(const ClSymbolicWeight &cl1, const ClSymbolicWeight &cl2)
    { return cl1.equal(cl2); }

  friend bool operator!=(const ClSymbolicWeight &cl1, const ClSymbolicWeight &cl2)
    { return !(cl1 == cl2); }

  friend bool operator<(const ClSymbolicWeight &cl1, const ClSymbolicWeight &cl2)
    { return cl1.lessThan(cl2); }

  friend bool operator>(const ClSymbolicWeight &cl1, const ClSymbolicWeight &cl2)
  { return (cl2 < cl1); }

  // function.h provides operator>, >=, <= from operator<

  double asDouble() const
    {
    vector<Number>::const_reverse_iterator i = _values.rbegin();
    Number sum  = 0;
    Number factor = 1;
    Number multiplier = 1000;
    for ( ; i != _values.rend(); ++i) 
      {
      sum += *i * factor;
      factor *= multiplier;
      }
    return sum;
    }

#ifndef CL_NO_IO
  ostream &printOn(ostream &xo) const
    { 
    vector<Number>::const_iterator i = _values.begin();
    if (i == _values.end())
      return xo;

    xo << *i;
    for (++i; i != _values.end(); ++i) 
      {
      xo << "," << *i;
      }
    return xo;
    }

  // FIXGJB: use a template function to generate these automatically
  friend ostream& operator<<(ostream &xos, const ClSymbolicWeight &clsw)
    { clsw.printOn(xos); return xos; }
#endif

  int cLevels() const
    { return _values.size(); }

  friend bool clApprox(const ClSymbolicWeight &cl, Number n);
  friend bool clApprox(const ClSymbolicWeight &cl1, const ClSymbolicWeight &cl2);

 private:
  vector<Number> _values;

  void push_back(Number d) 
    { _values.push_back(d); }

};

inline bool clApprox(const ClSymbolicWeight &cl, Number n)
{
  vector<Number>::const_iterator it = cl._values.begin();
  if (!clApprox(*it,n))
    return false;

  ++it;
  for (; it != cl._values.end(); ++it)
    {
    if (!clApprox(*it,0))
      return false;
    }

  return true;
}

inline bool clApprox(const ClSymbolicWeight &cl1, const ClSymbolicWeight &cl2)
{
  vector<Number>::const_iterator it1 = cl1._values.begin();
  vector<Number>::const_iterator it2 = cl2._values.begin();

  for (; it1 != cl1._values.end() && it2 != cl2._values.end(); 
       ++it1, ++it2)
    {
    if (!clApprox(*it1,*it2))
      return false;
    }

  if (it1 == cl1._values.end() && it2 == cl2._values.end())
    return true;

  return false;
}

inline ClSymbolicWeight ReciprocalOf(const ClSymbolicWeight &)
{ throw(ExCLInternalError("Cannot take ReciprocalOf symbolic weight"));
  return ClSymbolicWeight::zero(); }

#endif
