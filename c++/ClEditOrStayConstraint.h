// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998, 1999 Alan Borning and Greg Badros
// See ../LICENSE for legal details regarding this software
//
// ClEditOrStayConstraint.h

#ifndef ClEditOrStayConstraint_H
#define ClEditOrStayConstraint_H

#include "ClConstraint.h"
#include "ClLinearExpression.h"

class ClVariable;

class ClEditOrStayConstraint : public ClConstraint {
 public:

  ClEditOrStayConstraint(const ClVariable var,
			 const ClStrength &strength = ClsRequired(), double weight = 1.0 ) :
    ClConstraint(strength,weight),
    _variable(var)
    { }
  
  const ClVariable variable() const
    { return _variable; }

  ClLinearExpression Expression() const
    { return ClLinearExpression(_variable,-1,_variable.Value()); }

 private:

  void setVariable( const ClVariable v)
    { _variable = v; }

  /// instance variables
  ClVariable _variable;


};

#endif
