// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998, All Rights Reserved.
//
// ClSymbolicWeight.h

#ifndef ClSymbolicWeight_H
#define ClSymbolicWeight_H

#include "Cassowary.h"
#include <vector.h>

class ClSymbolicWeight {
 public:
  ClSymbolicWeight() 
    { }

  ClSymbolicWeight(int cLevels, double value = 0.0);

  ClSymbolicWeight(double w1 = 0.0, double w2 = 0.0, double w3 = 0.0);

  ClSymbolicWeight(const vector<double> &weights);

  static ClSymbolicWeight &zero();

  ClSymbolicWeight times(Number n) const;
  ClSymbolicWeight divideBy(Number n) const;
  ClSymbolicWeight plus(const ClSymbolicWeight &cl) const;
  ClSymbolicWeight minus(const ClSymbolicWeight &cl) const;

  bool lessThan(const ClSymbolicWeight &cl) const;
  bool lessThanOrEqual(const ClSymbolicWeight &cl) const;
  bool equal(const ClSymbolicWeight &cl) const;
  bool greaterThan(const ClSymbolicWeight &cl) const;
  bool greaterThanOrEqual(const ClSymbolicWeight &cl) const;
  bool isNegative() const;

  friend operator==(const ClSymbolicWeight &cl1, const ClSymbolicWeight &cl2)
    { return cl1.equal(cl2); }

  friend operator<(const ClSymbolicWeight &cl1, const ClSymbolicWeight &cl2)
    { return cl1.lessThan(cl2); }

  // function.h provides operator>, >=, <= from operator<

  ostream &printOn(ostream &xo) const
    { 
    vector<double>::const_iterator i = my_values.begin();
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
