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
  ClSymbolicWeight(int cLevels);

  ClSymbolicWeight(double w1 = 0.0, double w2 = 0.0, double w3 = 0.0);

  static ClSymbolicWeight &zero();

  ClSymbolicWeight times(Number n) const;
  ClSymbolicWeight plus(const ClSymbolicWeight &cl) const;
  ClSymbolicWeight minus(const ClSymbolicWeight &cl) const;
  ClSymbolicWeight divideBy(Number n) const;
  bool lessThan(const ClSymbolicWeight &cl) const;
  bool lessThanOrEqual(const ClSymbolicWeight &cl) const;
  bool equal(const ClSymbolicWeight &cl) const;
  bool greaterThan(const ClSymbolicWeight &cl) const;
  bool greaterThanOrEqual(const ClSymbolicWeight &cl) const;
  bool isNegative() const;

  ostream &printOn(ostream &xo);


 private:
  vector<double> my_values;
};
#endif
