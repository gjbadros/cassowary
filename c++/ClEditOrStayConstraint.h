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

#include "ClLinearExpression.h"

class ClAbstractVariable;

class ClEditOrStayConstraint : public ClConstraint {
 public:

  ClEditOrStayConstraint(const ClAbstractVariable &var,
			 const ClStrength &strength = clsRequired(), double weight = 1.0 ) :
    ClConstraint(strength,weight),
    my_pvariable(&var)
    { }
  
  const ClAbstractVariable *variable() const
    { return my_pvariable; }

  ClLinearExpression expression() const
    { ClLinearExpression expr(*my_pvariable); return expr; }

 private:

  void setVariable( const ClAbstractVariable &v)
    { my_pvariable = &v; }

  /// instance variables
  const ClAbstractVariable *my_pvariable;


};

#endif
