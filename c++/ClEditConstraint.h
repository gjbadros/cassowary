// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998, All Rights Reserved.
//
// ClEditConstraint.h

#ifndef ClEditConstraint_H
#define ClEditConstraint_H


class ClEditConstraint {
 public:
  
  // Returns true if this is an edit constraint
  virtual bool isEditConstraint() const
    { return false; }

 private:

}

#endif
