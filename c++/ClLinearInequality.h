// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998 Alan Borning and Greg Badros
// See ../COPYRIGHT for legal details regarding this software
//
// ClLinearInequality.h

#ifndef ClLinearInequality_H
#define ClLinearInequality_H

#include "ClLinearConstraint.h"

enum ClInequalityOperator { cnLEQ, cnGEQ };

class ClVariable;

class ClLinearInequality : public ClLinearConstraint {
 private: typedef ClLinearConstraint super;

 public:
 //// Constructors
 // ClLinearInequality(expr,...)  is expr >= 0
 ClLinearInequality(const ClLinearExpression &cle,
		    const ClStrength &strength = clsRequired(),
		    double weight = 1.0) :
   ClLinearConstraint(cle,strength, weight)
   { }

 // ClLinearInequality(var,OP,expr) is  var >= expr
 ClLinearInequality(const ClVariable &clv,
		    ClInequalityOperator op,
		    const ClLinearExpression &cle,
		    const ClStrength &strength = clsRequired(),
		    double weight = 1.0) :
   ClLinearConstraint( cle, strength, weight)
   { 
   if (op == cnGEQ)
     {
     _expression.multiplyMe(-1.0);
     _expression.addVariable(clv,1.0);
     }
   else // op == cnLEQ
     {
     _expression.addVariable(clv,-1.0);
     }
   }

#ifdef FIXGJB_AMBIGUOUS
 // ClLinearInequality(expr,OP,var) is  var ?<>? expr
 ClLinearInequality(const ClLinearExpression &cle,
		    ClInequalityOperator op,
		    const ClVariable &clv,
		    const ClStrength &strength = clsRequired(),
		    double weight = 1.0) :
   ClLinearConstraint( cle, strength, weight)
   { 
   if (op == cnLEQ)
     {
     _expression.multiplyMe(-1.0);
     _expression.addVariable(clv,1.0);
     }
   else // op == cnGEQ
     {
     _expression.addVariable(clv,-1.0);
     }
   }
#endif

 // ClLinearInequality(expr,OP,expr) is  expr >= expr
 ClLinearInequality(const ClLinearExpression &cle1,
		    ClInequalityOperator op,
		    const ClLinearExpression &cle2,
		    const ClStrength &strength = clsRequired(),
		    double weight = 1.0) :
   ClLinearConstraint( cle2, strength, weight)
   { 
   if (op == cnGEQ)
     {
     _expression.multiplyMe(-1.0);
     _expression.addExpression(cle1);
     }
   else // op == cnLEQ
     {
     _expression.addExpression(cle1,-1.0);
     }
   }

#ifdef FIXGJB_AMBIGUOUS
 // ClLinearInequality(var,OP,var) is  var ?<>? var
 ClLinearInequality(const ClVariable &clv1,
		    ClInequalityOperator op,
		    const ClVariable &clv2,
		    const ClStrength &strength = clsRequired(),
		    double weight = 1.0) :
   ClLinearConstraint( clv2, strength, weight)
   { 
   if (op == cnGEQ)
     {
     _expression.multiplyMe(-1.0);
     _expression.addVariable(clv1,1.0);
     }
   else // op == cnLEQ
     {
     _expression.addVariable(clv1,-1.0);
     }
   }
#endif

 
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
