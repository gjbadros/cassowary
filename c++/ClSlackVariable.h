// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998, All Rights Reserved.
//
// ClSlackVariable.h

#ifndef ClSlackVariable_H
#define ClSlackVariable_H

#include "Cassowary.h"
#include "ClAbstractVariable.h"

class ClTableau;
class ClSimplexSolver;


class ClSlackVariable: public ClAbstractVariable {
protected:
  friend ClTableau;
  friend ClSimplexSolver;

  ClSlackVariable(String name = "") :
    ClAbstractVariable(name)
    { }

  ClSlackVariable(long number, char *prefix, Number value = 0.0) :
    ClAbstractVariable(number,prefix)
    { }

  virtual ostream &printOn(ostream &xo) const
  {  
    xo << "[" << name() << ":slack]";
    return xo;
  }

  virtual bool isExternal() const
    { return false; }

  virtual bool isPivotable() const
    { return true; }

  virtual bool isRestricted() const
    { return true; }

};


#endif
