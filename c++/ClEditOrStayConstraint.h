// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998 Alan Borning and Greg Badros
// See ../COPYRIGHT for legal details regarding this software
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
    _pvariable(&var)
    { }
  
  const ClVariable &variable() const
    { return *_pvariable; }

  ClLinearExpression expression() const
    { return ClLinearExpression(*_pvariable,-1,_pvariable->value()); }

 private:

  void setVariable( const ClVariable &v)
    { _pvariable = &v; }

  /// instance variables
  const ClVariable *_pvariable;


};

#endif
