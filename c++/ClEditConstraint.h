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
  
  ClEditConstraint(const ClVariable var,
		   const ClStrength &strength = ClsStrong(), double weight = 1.0 ) :
    ClEditOrStayConstraint(var,strength,weight)
    { }

  // Returns true if this is an edit constraint
  virtual bool IsEditConstraint() const
    { return true; }

#ifndef CL_NO_IO
  virtual ostream &PrintOn(ostream &xo) const 
    { super::PrintOn(xo); return xo << "= edit)"; }
#endif

 private:

};

#endif
