// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998, All Rights Reserved.
//
// (c) 1998 Alan Borning and Greg Badros.  This code is provided for use by
// students for course projects in the course CSE 595 in the Department of
// Computer Science and Engineering, University of Washington, during winter
// quarter 1998.  Any other use requires written permission from the copyright
// holders.
//
// ClSymbolicWeight.h

#ifndef ClSymbolicWeight_H
#define ClSymbolicWeight_H

#include "Cassowary.h"
#include <vector>

class ClSymbolicWeight {
 public:
  ClSymbolicWeight() 
    { }

  ClSymbolicWeight(int cLevels, double value = 0.0);

  ClSymbolicWeight(double w1, double w2 = 0.0, double w3 = 0.0);

  ClSymbolicWeight(const vector<double> &weights);

  static ClSymbolicWeight &zero();

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

  // FIXGJB: can we express this statically?
  ClSymbolicWeight operator*(const ClSymbolicWeight &) const
    { assert(false); }
  ClSymbolicWeight &operator*=(const ClSymbolicWeight &)
    { assert(false); }

  // FIXGJB: can we express this statically?
  ClSymbolicWeight operator-() const
    { assert(false); }

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
    vector<double>::const_reverse_iterator i = my_values.rbegin();
    double sum  = 0;
    double factor = 1;
    double multiplier = 1000;
    for ( ; i != my_values.rend(); ++i) 
      {
      sum += *i * factor;
      factor *= multiplier;
      }
    return sum;
    }

  ostream &printOn(ostream &xo) const
    { 
    vector<double>::const_iterator i = my_values.begin();
    if (i == my_values.end())
      return xo;

    xo << *i;
    for (++i; i != my_values.end(); ++i) 
      {
      xo << "," << *i;
      }
    return xo;
    }

  int cLevels() const
    { return my_values.size(); }

  // FIXGJB: use a template function to generate these automatically
  friend ostream& operator<<(ostream &xos, const ClSymbolicWeight &clsw)
    { clsw.printOn(xos); return xos; }

  friend bool clApprox(const ClSymbolicWeight &cl, Number n);
  friend bool clApprox(const ClSymbolicWeight &cl1, const ClSymbolicWeight &cl2);

 private:
  vector<Number> my_values;

  void push_back(double d) 
    { my_values.push_back(d); }

};

inline bool clApprox(const ClSymbolicWeight &cl, Number n)
{
  vector<Number>::const_iterator it = cl.my_values.begin();
  if (!clApprox(*it,n))
    return false;

  ++it;
  for (; it != cl.my_values.end(); ++it)
    {
    if (!clApprox(*it,0))
      return false;
    }

  return true;
}

inline bool clApprox(const ClSymbolicWeight &cl1, const ClSymbolicWeight &cl2)
{
  vector<Number>::const_iterator it1 = cl1.my_values.begin();
  vector<Number>::const_iterator it2 = cl2.my_values.begin();

  for (; it1 != cl1.my_values.end() && it2 != cl2.my_values.end(); 
       ++it1, ++it2)
    {
    if (!clApprox(*it1,*it2))
      return false;
    }

  if (it1 == cl1.my_values.end() && it2 == cl2.my_values.end())
    return true;

  return false;
}

inline ClSymbolicWeight ReciprocalOf(const ClSymbolicWeight &)
{ assert(false); return ClSymbolicWeight::zero(); }

#endif
