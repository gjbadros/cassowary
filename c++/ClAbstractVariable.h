// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998, 1999 Alan Borning and Greg Badros
// See ../LICENSE for legal details regarding this software
//
// ClAbstractVariable.h

#ifndef ClAbstractVariable_H
#define ClAbstractVariable_H

#include <stdio.h> /* for sprintf */
#include "Cassowary.h"
#include "auto_ptr.h"

#ifdef USE_GC
class ClAbstractVariable : public gc {
#else
class ClAbstractVariable {
#endif
public:
  ClAbstractVariable(string name = "") :
    _name(name)
    { 
    ++iVariableNumber;
#ifdef CL_FIND_LEAK
    ++cAbstractVariables;
#endif
    if (name.length() == 0)
      {
      char sz[16];
      sprintf(sz,"v%ld",iVariableNumber);
      _name = string(sz);
      }
    }

  ClAbstractVariable(long varnumber, char *prefix)
    {
    auto_ptr<char> pch (new char[16+strlen(prefix)]);
    iVariableNumber++;
#ifdef CL_FIND_LEAK
    ++cAbstractVariables;
#endif
    sprintf(pch.get(),"%s%ld",prefix,varnumber);
    _name = string(pch.get());
    }

  virtual ~ClAbstractVariable()
#ifdef CL_FIND_LEAK
  { --cAbstractVariables; }

  static long cAbstractVariables;
#else
  { }
#endif

  // Return the name of the variable
  string name() const
    { return _name; }

  // Set the name of the variable
  virtual void setName(string const &name)
    { _name = name; }

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

#ifndef CL_NO_IO
  // Prints a semi-descriptive representation to the stream, using the
  // name if there is one, and otherwise the hash number of this
  // object.
  //	EXAMPLES
  //	  x[10.0]		-- w/ name
  //	  x[0.0,100]		-- w/ name, bounds but no value yet
  //	  CV#345(10.0)		-- w/o name
  virtual ostream &printOn(ostream &xo) const = 0;

  virtual void gdb_print() const { printOn(cerr); };

  friend ostream& operator<<(ostream &xos, const ClAbstractVariable &clv)
    { clv.printOn(xos); return xos; }

#endif // CL_NO_IO

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

  virtual Number value() const { return 0; }

private:
  string _name;

  static long iVariableNumber;
};

typedef ClAbstractVariable *PClAbstractVariable;

#endif
