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
// ClStayConstraint.h

#ifndef ClStayConstraint_H
#define ClStayConstraint_H

#include "Cassowary.h"
#include "ClEditOrStayConstraint.h"

class ClAbstractVariable;

class ClStayConstraint : public ClEditOrStayConstraint {
 public:

  ClStayConstraint(const ClAbstractVariable &var,
		   const ClStrength &strength = clsWeak(), double weight = 1.0 ) :
    ClEditOrStayConstraint(var,strength,weight)
    { }

  virtual bool isStayConstraint() const
    { return true; }
  
  virtual ostream &printOn(ostream &xo) const 
    { xo << "stay" << variable(); return xo; }
  
 private:
};

#endif
