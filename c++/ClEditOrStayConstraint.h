// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998, All Rights Reserved.
//
// (c) 1998 Alan Borning and Greg Badros.  This code is provided for use by
// students for course projects in the course CSE 595 in the Department of
// Computer Science and Engineering, University of Washington, during winter
// quarter 1998.  Any other use requires written permission from the copyright
// holders.
//
// ClEditOrStayConstraint.h

#ifndef ClEditOrStayConstraint_H
#define ClEditOrStayConstraint_H

#include "ClConstraint.h"
#include "ClLinearExpression.h"

class ClVariable;

class ClEditOrStayConstraint : public ClConstraint {
 public:

  ClEditOrStayConstraint(const ClVariable &var,
			 const ClStrength &strength = clsRequired(), double weight = 1.0 ) :
    ClConstraint(strength,weight),
    my_pvariable(&var)
    { }
  
  const ClVariable &variable() const
    { return *my_pvariable; }

  ClLinearExpression expression() const
    { return ClLinearExpression(*my_pvariable,-1,my_pvariable->value()); }

 private:

  void setVariable( const ClVariable &v)
    { my_pvariable = &v; }

  /// instance variables
  const ClVariable *my_pvariable;


};

#endif
