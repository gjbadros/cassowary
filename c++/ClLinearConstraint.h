// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998, 1999 Alan Borning and Greg Badros
// See ../LICENSE for legal details regarding this software
//
// ClLinearConstraint.h

#ifndef ClLinearConstraint_H
#define ClLinearConstraint_H

#include "Cassowary.h"
#include "ClConstraint.h"
#include "ClLinearExpression.h"


// Add the ClLinearExpression member variable needed for both
// ClLinearEquation and ClLinearInequality
class ClLinearConstraint : public ClConstraint {
 private: typedef ClLinearConstraint super;

 public:

  // Constructor
  ClLinearConstraint(const ClLinearExpression &cle,
		     const ClStrength &strength = clsRequired(),
		     double weight = 1.0) :
    ClConstraint(strength, weight),
    _expression(cle)
    { }

  // Return my linear expression.  (For linear equations, this
  // constraint represents expression=0; for linear inequalities it
  // represents expression>=0.)
  ClLinearExpression expression() const
    { return _expression; }

 protected:

  ClLinearExpression _expression;

  virtual void setExpression( const ClLinearExpression &expr)
    { _expression = expr; }

};

#endif
