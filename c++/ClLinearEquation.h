// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998 Alan Borning and Greg Badros
// See ../COPYRIGHT for legal details regarding this software
//
// ClLinearEquation.h

#ifndef ClLinearEquation_H
#define ClLinearEquation_H

#include "Cassowary.h"
#include "ClLinearConstraint.h"
#include "ClLinearExpression.h"

class ClStrength;
class ClVariable;

class ClLinearEquation : public ClLinearConstraint {
 private: typedef ClLinearConstraint super;

 public:
 //// Constructors

 // ClLinearEquation(expr,...)    is   expr == 0
 ClLinearEquation(const ClLinearExpression &cle,
		  const ClStrength strength = clsRequired(),
		  double weight = 1.0) :
   ClLinearConstraint(cle,strength, weight)
   { }

 // ClLinearEquation(var,expr,...)  is   var == expr
 ClLinearEquation(const ClAbstractVariable &clv,
		  const ClLinearExpression &cle,
		  const ClStrength strength = clsRequired(),
		  double weight = 1.0) :
   ClLinearConstraint(cle,strength,weight)
   { _expression.addVariable(clv,-1.0); }

 // ClLinearEquation(expr,var,...) is   var == expr
 ClLinearEquation(const ClLinearExpression &cle,
		  const ClAbstractVariable &clv,
		  const ClStrength strength = clsRequired(),
		  double weight = 1.0) :
   ClLinearConstraint(cle,strength,weight)
   { _expression.addVariable(clv,-1.0); }

 // ClLinearEquation(expr,expr,...) is   expr == expr
 ClLinearEquation(const ClLinearExpression &cle1,
		  const ClLinearExpression &cle2,
		  const ClStrength strength = clsRequired(),
		  double weight = 1.0) :
   ClLinearConstraint(cle1,strength,weight)
   { _expression.addExpression(cle2,-1.0); }
 
 virtual ostream &printOn(ostream &xo) const
   {  super::printOn(xo); xo << " = 0 )"; return xo; }

};

#endif
