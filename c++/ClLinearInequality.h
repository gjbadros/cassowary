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
 private: typedef ClLinearConstraint super;

 public:
  // Constructor
 ClLinearInequality(const ClLinearExpression &cle,
		    const ClStrength strength = clsRequired(),
		    double weight = 1.0) :
   ClLinearConstraint(cle,strength, weight)
   { }
 
 // Return true if this is an inequality constraint and
 // false if it is an equality constraint.  The default is
 // that it is not.
 virtual bool isInequality() const
   { return true; }
 
 virtual ostream &printOn(ostream &xo) const
   {  super::printOn(xo); xo << " >= 0 )"; return xo; }

 private:

};

#endif
