// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998, All Rights Reserved.
//
// ClAbstractVariable.h

#ifndef ClAbstractVariable_H
#define ClAbstractVariable_H

#include <stdio.h>
#include "Cassowary.h"
#include "auto_ptr.h"

class ClAbstractVariable {
public:
  ClAbstractVariable(string name = "") :
    my_name(name)
    { 
    iVariableNumber++;
    if (name.length() == 0)
      {
      char sz[16];
      sprintf(sz,"v%ld",iVariableNumber++);
      my_name = string(sz);
      }
    }

  ClAbstractVariable(long varnumber, char *prefix)
    {
    auto_ptr<char> pch (new char[16+strlen(prefix)]);
    iVariableNumber++;
    sprintf(pch.get(),"%s%ld",prefix,varnumber);
    my_name = string(pch.get());
    }

  virtual ~ClAbstractVariable()
    { }

  // Return the name of the variable
  string name() const
    { return my_name; }

  // Set the name of the variable
  void setName(string const &name)
    { my_name = name; }

  // Return true if this a dummy variable (used as a marker variable
  // for required equality constraints).  Such variables aren't
  // allowed to enter the basis when pivoting.
  virtual bool isDummy() const
    { return false; }

  // Return true if this a variable known outside the solver.  
  // (We need to give such variables a value after solving is complete.)
  virtual bool isExternal() const = 0;

  // Return true if we can pivot on this variable.
  virtual bool isPivotable() const = 0;

  // Return true if this is a restricted (or slack) variable.  Such
  // variables are constrained to be non-negative and occur only
  // internally to the simplex solver.
  virtual bool isRestricted() const = 0;

  // Prints a semi-descriptive representation to the stream, using the
  // name if there is one, and otherwise the hash number of this
  // object.
  //	EXAMPLES
  //	  x[10.0]		-- w/ name
  //	  x[0.0,100]		-- w/ name, bounds but no value yet
  //	  CV#345(10.0)		-- w/o name
  virtual ostream &printOn(ostream &xo) const = 0;

  friend ostream& operator<<(ostream &xos, const ClAbstractVariable &clv)
    { clv.printOn(xos); return xos; }

  friend bool operator<(const ClAbstractVariable &cl1, const ClAbstractVariable &cl2)
    { return &cl1 < &cl2; }

  friend bool operator==(const ClAbstractVariable &cl1, const ClAbstractVariable &cl2)
    { 
    return &cl1 == &cl2;
    }

  friend bool operator!=(const ClAbstractVariable &cl1, const ClAbstractVariable &cl2)
    { 
    return !(cl1 == cl2);
    }

private:
  string my_name;

  static long iVariableNumber;
};

typedef ClAbstractVariable *PClAbstractVariable;

#endif
