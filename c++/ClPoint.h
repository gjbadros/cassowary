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
// ClPoint.h

#ifndef ClPoint_H
#define ClPoint_H

#include "Cassowary.h"
#include "ClVariable.h"

class ClPoint {
 public:
  ClPoint(Number x, Number y)
    : clv_x(x), clv_y(y)
    { }

  ClPoint()
    { }


  ClVariable &Xvar()
    { return clv_x; }

  ClVariable &Yvar()
    { return clv_y; }

  const ClVariable &Xvar() const
    { return clv_x; }

  const ClVariable &Yvar() const
    { return clv_y; }


  Number X() const
    { return Xvar().value(); }

  Number Y() const
    { return Yvar().value(); }

 private:
  ClVariable clv_x;
  ClVariable clv_y;

  friend ostream &operator<<(ostream &xo, const ClPoint &clp);
  
};

inline ostream &
operator<<(ostream &xo, const ClPoint &clp)
{
  xo << "(" << clp.clv_x << ", " << clp.clv_y << ")";
  return xo;
}

#endif
