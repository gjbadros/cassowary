// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998, All Rights Reserved.
//
// ClEditOrStayConstraint.h

#ifndef ClEditOrStayConstraint_H
#define ClEditOrStayConstraint_H

#include "ClVariable.h"
#include "ClLinearExpression.h"

class ClEditOrStayConstraint : public ClConstraint {
 public:

  ClEditOrStayConstraint(const ClVariable &var,
			 const ClStrength &strength = clsRequired(), double weight = 1.0 ) :
    ClConstraint(strength,weight),
    my_variable(var)
    { }
  
  ClVariable variable() const
    { return my_variable; }

  ClLinearExpression expression() const
    { ClLinearExpression v(my_variable); return v; }

 private:

  void setVariable( const ClVariable &v)
    { my_variable = v; }

  /// instance variables
  ClVariable my_variable;


};

#endif
