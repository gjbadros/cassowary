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
  ClObjectiveVariable(String name = "") :
    ClAbstractVariable(name)
    { }

  ClObjectiveVariable(long number, char *prefix, Number value = 0.0) :
    ClAbstractVariable(number,prefix)
    { }

  virtual ostream &printOn(ostream &xo) const
  {  
    xo << "[" << name() << ":obj]";
    return xo;
  }

  // We don't need to give such variables a value after solving is complete.
  virtual bool isExternal() const 
    { return false; }

  // Return true if we can pivot on this variable.
  virtual bool isPivotable() const 
    { return false; }

  // Return true if this is a restricted (or slack) variable.  Such
  // variables are constrained to be non-negative and occur only
  // internally to the simplex solver.
  virtual bool isRestricted() const 
    { return false; }

};

#endif