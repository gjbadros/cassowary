// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998, 1999 Alan Borning and Greg Badros
// See ../LICENSE for legal details regarding this software
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
