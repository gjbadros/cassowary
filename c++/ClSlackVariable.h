// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998, 1999 Alan Borning and Greg Badros
// See ../LICENSE for legal details regarding this software
//
// ClSlackVariable.h

#ifndef ClSlackVariable_H
#define ClSlackVariable_H

#include "Cassowary.h"
#include "ClAbstractVariable.h"

class ClTableau;
class ClSimplexSolver;


class ClSlackVariable : public ClAbstractVariable {
public:
#ifdef CL_FIND_LEAK
  ~ClSlackVariable() { --cSlackVariables; };

  static long cSlackVariables;
#endif

protected:
  friend ClTableau;
  friend ClSimplexSolver;

  ClSlackVariable(string name = "") :
    ClAbstractVariable(name)
    {
#ifdef CL_FIND_LEAK
      ++cSlackVariables; 
#endif
    }

  ClSlackVariable(long number, char *prefix) :
    ClAbstractVariable(number,prefix)
    { 
#ifdef CL_FIND_LEAK
      ++cSlackVariables; 
#endif
    }

#ifndef CL_NO_IO
  virtual ostream &printOn(ostream &xo) const
  {  
    xo << "[" << name() << ":slack]";
    return xo;
  }
#endif

  virtual bool isExternal() const
    { return false; }

  virtual bool isPivotable() const
    { return true; }

  virtual bool isRestricted() const
    { return true; }

};


#endif
