// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998, All Rights Reserved.
//
// ClDummyVariable.h

#ifndef ClDummyVariable_H
#define ClDummyVariable_H

#include "Cassowary.h"
#include "ClAbstractVariable.h"

class ClDummyVariable: public ClAbstractVariable {
public:
  bool isDummy() const
    { return true; }
  bool isExternal() const
    { return false; }
  bool isPivotable() const
    { return false; }
  bool isRestricted() const
    { return true; }
};

#endif
