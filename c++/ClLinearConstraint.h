// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998, All Rights Reserved.
//
// ClLinearConstraint.h

#ifndef ClLinearConstraint_H
#define ClLinearConstraint_H

#include "Cassowary.h"
#include "ClConstraint.h"
#include "ClLinearExpression.h"

class ClLinearConstraint : public ClConstraint {
 public:

  // Constructor
  ClLinearConstraint()
  
  // Return my linear expression.  (For linear equations, this
  // constraint represents expression=0; for linear inequalities it
  // represents expression>=0.)
  ClLinearExpression expression() 
    { return my_expression; }

 private:

  ClLinearExpression my_expression;

  virtual void setExpression( const ClLinearExpression &expr)
    { my_expression = expr; }

}

#endif
