// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998 Alan Borning and Greg Badros
// See ../COPYRIGHT for legal details regarding this software
//
// ClObjectiveVariable.h

#ifndef ClObjectiveVariable_H
#define ClObjectiveVariable_H

#include "Cassowary.h"
#include "ClAbstractVariable.h"

class ClTableau;
class ClSimplexSolver;

class ClObjectiveVariable : public ClAbstractVariable {
protected:
  friend ClTableau;
  friend ClSimplexSolver;

  ClObjectiveVariable(string name = "") :
    ClAbstractVariable(name)
    { }

  ClObjectiveVariable(long number, char *prefix) :
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
