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
// ClEditConstraint.h

#ifndef ClEditConstraint_H
#define ClEditConstraint_H

#include "Cassowary.h"
#include "ClEditOrStayConstraint.h"

class ClEditConstraint : public ClEditOrStayConstraint {
     typedef ClEditOrStayConstraint super;
 public:
  
  ClEditConstraint(const ClVariable &var,
		   const ClStrength &strength = clsStrong(), double weight = 1.0 ) :
    ClEditOrStayConstraint(var,strength,weight)
    { }

  // Returns true if this is an edit constraint
  virtual bool isEditConstraint() const
    { return true; }

  virtual ostream &printOn(ostream &xo) const 
    { xo << "edit "; return super::printOn(xo); }

 private:

};

#endif
