// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998, 1999 Alan Borning and Greg Badros
// See ../COPYRIGHT for legal details regarding this software
//
// ClStayConstraint.h

#ifndef ClStayConstraint_H
#define ClStayConstraint_H

#include "Cassowary.h"
#include "ClEditOrStayConstraint.h"

class ClAbstractVariable;

class ClStayConstraint : public ClEditOrStayConstraint {
     typedef ClEditOrStayConstraint super;
 public:

  ClStayConstraint(const ClVariable &var,
		   const ClStrength &strength = clsWeak(), double weight = 1.0 ) :
    ClEditOrStayConstraint(var,strength,weight)
    { }

  virtual bool isStayConstraint() const
    { return true; }
  
  virtual ostream &printOn(ostream &xo) const 
    { xo << "stay "; return super::printOn(xo); }
  
 private:
};

#endif
