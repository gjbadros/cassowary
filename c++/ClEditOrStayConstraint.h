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


class ClEditOrStayConstraint {
 public:
  
  ClExpression *new_expression()
    { ClLinearExpression *pe = new ClLinearExpression(my_variable,-1.0); return pe; }

  ClVariable variable() const
    { return my_variable; }

 private:

  void setVariable( const ClVariable &v)
    { my_variable = v; }


  /// instance variables

  ClVariable my_variable;


}

#endif
