// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998, All Rights Reserved.
//
// Cassowary.h

#ifndef Cassowary_H
#define Cassowary_H

#include <string>
#include <math.h>

typedef string String;
typedef double Number;

// Compare two double-s approximately, since equality is no good
inline bool clApprox(double a, double b)
{
  float epsilon = 1.0e-8;
  if (a == 0.0) {
    return (fabs(b) < epsilon);
  } else if (b == 0.0) {
    return (fabs(a) < epsilon);
  } else {
    return (fabs(a-b) < fabs(a) * epsilon);
  }
}

#endif
