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

#if defined(HAVE_CONFIG_H) && !defined(CONFIG_H_INCLUDED) && !defined(CONFIG_INLINE_H_INCLUDED)
#include <cassowary/config-inline.h>
#define CONFIG_INLINE_H_INCLUDED
#endif

#include <stdio.h> /* for sprintf */
#include "Cassowary.h"
#include "auto_ptr.h"

#ifdef USE_GC
class ClAbstractVariable : public gc {
#else
class ClAbstractVariable {
#endif
public:
  ClAbstractVariable(string Name = "") :
    _name(Name)
    { 
    ++iVariableNumber;
#ifdef CL_FIND_LEAK
    ++cAbstractVariables;
#endif
    if (Name.length() == 0)
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

  // Return the Name of the variable
  string Name() const
    { return _name; }

  // Set the Name of the variable
  virtual void SetName(string const &Name)
    { _name = Name; }

  // Return true if this a dummy variable (used as a marker variable
  // for required equality constraints).  Such variables aren't
  // allowed to enter the basis when pivoting.
  virtual bool IsDummy() const
    { return false; }

  // Return true if this a variable known outside the solver.  
  // (We need to give such variables a Value after solving is complete.)
  virtual bool IsExternal() const = 0;

  // Return true if we can Pivot on this variable.
  virtual bool IsPivotable() const = 0;

  // Return true if this is a restricted (or slack) variable.  Such
  // variables are constrained to be non-negative and occur only
  // internally to the simplex solver.
  virtual bool IsRestricted() const = 0;

#ifndef CL_NO_IO
  // Prints a semi-descriptive representation to the stream, using the
  // Name if there is one, and otherwise the hash number of this
  // object.
  //	EXAMPLES
  //	  x[10.0]		-- w/ Name
  //	  x[0.0,100]		-- w/ Name, bounds but no Value yet
  //	  CV#345(10.0)		-- w/o Name
  virtual ostream &PrintOn(ostream &xo) const = 0;

  friend ostream& operator<<(ostream &xos, const ClAbstractVariable &clv)
    { clv.PrintOn(xos); return xos; }

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

  virtual Number Value() const { return 0; }
  virtual int IntValue() const { return 0; }

  virtual void SetValue(Number) 
    { assert(false); }

  virtual void ChangeValue(Number)
    { assert(false); }

  virtual void SetPv(void *)
    { assert(false); }

  virtual void *Pv() const
    { assert(false); }


private:
  string _name;

  static long iVariableNumber;
};

typedef ClAbstractVariable *PClAbstractVariable;

#endif
