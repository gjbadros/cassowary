// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998, All Rights Reserved.
//
// ClLinearEquation.h

#ifndef ClLinearEquation_H
#define ClLinearEquation_H

#include <iostream.h>
#include "Cassowary.h"
#include "ClLinearConstraint.h"
#include "ClLinearExpression.h"

class ClLinearEquation : public ClLinearConstraint {
 private:
  ClLinearExpression my_expression;

 public:
  
  virtual ostream &printOn(ostream &xo)
    {  xo << strength() << "(" << expression() << "=0)"; }

 private:

  virtual void setExpression( const ClLinearExpression &expr)
    { my_expression = expr; }

};

#endif
