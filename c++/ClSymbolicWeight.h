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

  ClSymbolicWeight times(Number n) const;
  ClSymbolicWeight divideBy(Number n) const;
  ClSymbolicWeight add(const ClSymbolicWeight &cl) const;
  ClSymbolicWeight subtract(const ClSymbolicWeight &cl) const;

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

 private:
  vector<double> my_values;

  void push_back(double d) 
    { my_values.push_back(d); }

};
#endif
