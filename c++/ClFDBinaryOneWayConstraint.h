// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998, 1999 Greg J. Badros and Alan Borning
// See ../LICENSE for legal details regarding this software
//
// ClFDBinaryOneWayConstraint.h

#ifndef ClFDBinaryOneWayConstraint_H
#define ClFDBinaryOneWayConstraint_H

#if defined(HAVE_CONFIG_H) && !defined(CONFIG_H_INCLUDED) && !defined(CONFIG_INLINE_H_INCLUDED)
#include <cassowary/config-inline.h>
#define CONFIG_INLINE_H_INCLUDED
#endif

#include "Cassowary.h"
#include "ClFDConstraint.h"

// Just a node in the class hierarchy for now
class ClFDBinaryOneWayConstraint : public ClFDConstraint {
 private: typedef ClFDConstraint super;

 public:

   ClFDBinaryOneWayConstraint(ClVariable vRW, ClVariable vRO)
       : ClFDConstraint(), _vRW(vRW), _vRO(vRO)
    { }

#ifndef CL_NO_IO
  virtual ostream &PrintOn(ostream &xo) const
    { xo << "FDCn: " << _vRW << " <- " << _vRO << endl; }

  friend ostream& operator<<(ostream &xos, const ClConstraint &constraint)
    { constraint.PrintOn(xos); return xos; }

#endif

 protected:
  ClVariable _vRW;
  ClVariable _vRO;
};

#endif
