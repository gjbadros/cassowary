// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998, All Rights Reserved.
//
// ClVariable.h

#ifndef ClVariable_H
#define ClVariable_H

#include "Cassowary.h"
#include "ClAbstractVariable.h"
#include "ClLinearExpression.h"
#include "ClLinearEquation.h"

class ClVariable: public ClAbstractVariable {
public:
  ClVariable(const String &name, Number value = 0.0);


  // Return true if this a variable known outside the solver.  
  // (We need to give such variables a value after solving is complete.)
  bool isExternal() const
    { return true; }

  // Return true if we can pivot on this variable.
  bool isPivotable() const
    { return false; }

  // Return true if this is a restricted (or slack) variable.  Such
  // variables are constrained to be non-negative and occur only
  // internally to the simplex solver.
  bool isRestricted() const
    { return false; }

  ostream &printOn(ostream &xo)
  {  
    xo << "[" << name << ":";
    value.printOn(xo);
    xo << "]" << endl;
    return xo;
  }
  
  // Return the current value I hold.
  Number value() const
    { return my_value; }
  void set_value(Number const &value)
    { my_value = value; }
  
  ClLinearExpression asLinearExpression() const;
  
  ClLinearExpression times(Number n) const
    { return asLinearExpression().times(n); }

  ClLinearExpression plus(const ClLinearExpression &expr) const
    { return asLinearExpression().plus(expr); }

  ClLinearExpression minus(const ClLinearExpression &expr) const
    { return asLinearExpression().minus(expr); }

  ClLinearExpression divide(Number n) const
    { return asLinearExpression().divide(n); }

  // We should return a new linear expression (aNumber/self) -- but
  // since the result must be linear, this is always an error.
  ClLinearExpression divFrom(Number n) const
    { throw ExCLNonlinearExpression; }

  ClLinearExpression subtractFrom(const ClLinearExpression &expr) const
    { times(-1.0).plus(expr); }

  // Return a linear constraint self=expr with given strength and weight
  ClLinearEquation cnEqual(const ClLinearExpression &expr, 
			   const ClStrength &strength = ClStrength::required(),
			   double weight = 1.0);
  ClLinearEquation cnEqual(const ClVariable &expr,
			   const ClStrength &strength = ClStrength::required(),
			   double weight = 1.0);
  ClLinearEquation cnEqual(Number expr,
			   const ClStrength &strength = ClStrength::required(),
			   double weight = 1.0);

  // Return a linear constraint self>=expr with given strength and weight
  ClLinearInequality cnGEQ(const ClLinearExpression &expr, 
			   const ClStrength &strength = ClStrength::required(), 
			   double weight = 1.0);
  ClLinearInequality cnGEQ(const ClVariable &expr,
			   const ClStrength &strength = ClStrength::required(),
			   double weight = 1.0);
  ClLinearInequality cnGEQ(Number expr,
			   const ClStrength &strength = ClStrength::required(),
			   double weight = 1.0);


  // Return a linear constraint self<=expr with given strength and weight
  ClLinearInequality cnLEQ(const ClLinearExpression &expr, 
			   const ClStrength &strength = ClStrength::required(),
			   double weight = 1.0);
  ClLinearInequality cnLEQ(const ClVariable &expr,
			   const ClStrength &strength = ClStrength::required(),
			   double weight = 1.0);
  ClLinearInequality cnLEQ(Number expr,
			   const ClStrength &strength = ClStrength::required(),
			   double weight = 1.0);


  /* Also has "@" for creating point pairs */
  /* arithmetic linear expression builders */
  /* constraint creators */ 

private:
  String my_name;
  Number my_value;
};

#endif
