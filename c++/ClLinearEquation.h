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

#include <iostream>
#include "Cassowary.h"
#include "ClLinearConstraint.h"
#include "ClLinearExpression.h"

class ClStrength;
class ClVariable;

class ClLinearEquation : public ClLinearConstraint {
 private: typedef ClLinearConstraint super;

 public:
  // Constructor
 ClLinearEquation(const ClLinearExpression &cle,
		  const ClStrength strength = clsRequired(),
		  double weight = 1.0) :
   ClLinearConstraint(cle,strength, weight)
   { }

 ClLinearEquation(const ClAbstractVariable &clv,
		  const ClLinearExpression &cle,
		  const ClStrength strength = clsRequired(),
		  double weight = 1.0) :
   ClLinearConstraint(cle,strength,weight)
   { my_expression.addVariable(clv,-1.0); }

 ClLinearEquation(const ClLinearExpression &cle,
		  const ClAbstractVariable &clv,
		  const ClStrength strength = clsRequired(),
		  double weight = 1.0) :
   ClLinearConstraint(cle,strength,weight)
   { my_expression.addVariable(clv,-1.0); }

 
 virtual ostream &printOn(ostream &xo) const
   {  super::printOn(xo); xo << " = 0 )"; return xo; }

};

#endif
