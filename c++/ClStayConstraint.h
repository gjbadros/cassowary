// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998, All Rights Reserved.
//
// ClStayConstraint.h

#ifndef ClStayConstraint_H
#define ClStayConstraint_H

#include "Cassowary.h"
#include "ClConstraint.h"


class ClStayConstraint : public ClConstraint {
public:
  virtual bool isStayConstraint() const
    { return true; }
};

#endif
