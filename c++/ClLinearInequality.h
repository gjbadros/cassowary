// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998, All Rights Reserved.
//
// ClLinearInequality.h

#ifndef ClLinearInequality_H
#define ClLinearInequality_H

#include "ClLinearConstraint.h"

class ClLinearInequality : public ClLinearConstraint {
 public:
  ClLinearInequality();

  // Return true if this is an inequality constraint and
  // false if it is an equality constraint.  The default is
  // that it is not.
  virtual bool isInequality() const
    { return true; }

  virtual ostream &printOn(ostream &xo) const
    { xo << strength() << "(" << expression() << ">=0)"; return xo; }

 private:

}

#endif
