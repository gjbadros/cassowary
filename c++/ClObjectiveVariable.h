// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998, All Rights Reserved.
//
// ClObjectiveVariable.h

#ifndef ClObjectiveVariable_H
#define ClObjectiveVariable_H

#include "Cassowary.h"
#include "ClAbstractVariable.h"

class ClObjectiveVariable : public ClAbstractVariable {
public:
  // (We need to give such variables a value after solving is complete.)
  virtual bool isExternal()
    { return false; }

  // Return true if we can pivot on this variable.
  virtual bool isPivotable() 
    { return false; }

  // Return true if this is a restricted (or slack) variable.  Such
  // variables are constrained to be non-negative and occur only
  // internally to the simplex solver.
  virtual bool isRestricted() 
    { return false; }

};

#endif
