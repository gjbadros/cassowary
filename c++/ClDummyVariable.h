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

class ClTableau;
class ClSimplexSolver;

class ClDummyVariable: public ClAbstractVariable {
protected:
  friend ClTableau;
  friend ClSimplexSolver;

  ClDummyVariable(string name = "") :
    ClAbstractVariable(name)
    { }

  ClDummyVariable(long number, char *prefix) :
    ClAbstractVariable(number,prefix)
    { }

  virtual ostream &printOn(ostream &xo) const
  {  
    xo << "[" << name() << ":dummy]";
    return xo;
  }

  // Return true if this a dummy variable (used as a marker variable
  // for required equality constraints).  Such variables aren't
  // allowed to enter the basis when pivoting.
  virtual bool isDummy() const
    { return true; }

  // Return true if this a variable known outside the solver.  
  // (We need to give such variables a value after solving is complete.)
  virtual bool isExternal() const
    { return false; }

  // Return true if we can pivot on this variable.
  virtual bool isPivotable() const
    { return false; }

  // Return true if this is a restricted (or slack) variable.  Such
  // variables are constrained to be non-negative and occur only
  // internally to the simplex solver.
  virtual bool isRestricted() const
    { return true; }

};

#endif
