// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998, All Rights Reserved.
//
// ClObjectVariable.h

#ifndef ClObjectVariable_H
#define ClObjectVariable_H

#include "Cassowary.h"
#include "ClAbstractVariable.h"

class ClObjectVariable: public ClAbstractVariable {
public:
  bool isExternal() const
    { return false; }
  bool isPivotable() const
    { return false; }
  bool isRestricted() const
    { return false; }
};


#endif

