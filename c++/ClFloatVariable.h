// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998, 1999 Alan Borning and Greg Badros
// See ../LICENSE for legal details regarding this software
//
// ClFloatVariable.h

#ifndef ClFloatVariable_H
#define ClFloatVariable_H

#include <stdio.h>
#include <map>
#include <string>
#include "Cassowary.h"
#include "ClAbstractVariable.h"

class ClFloatVariable : public ClAbstractVariable {
public:
  typedef ClAbstractVariable super;

  ClFloatVariable(string name, Number value = 0.0) :
    ClAbstractVariable(name),
    _value(value),
    _pv(NULL)
    { }

  ClFloatVariable(Number value = 0.0) :
    ClAbstractVariable(""),
    _value(value),
    _pv(NULL)
    { }

  ClFloatVariable(long number, char *prefix, Number value = 0.0) :
    ClAbstractVariable(number,prefix),
    _value(value),
    _pv(NULL)
    { }

  // Return true if this a dummy variable (used as a marker variable
  // for required equality constraints).  Such variables aren't
  // allowed to enter the basis when pivoting.
  virtual bool isDummy() const
    { return false; }

  // Return true if this a variable known outside the solver.  
  // (We need to give such variables a value after solving is complete.)
  virtual bool isExternal() const
    { return true; }

  // Return true if we can pivot on this variable.
  virtual bool isPivotable() const
    { return false; }

  // Return true if this is a restricted (or slack) variable.  Such
  // variables are constrained to be non-negative and occur only
  // internally to the simplex solver.
  virtual bool isRestricted() const
    { return false; }

#ifndef CL_NO_IO
  // Prints a semi-descriptive representation to the stream, using the
  // name if there is one, and otherwise the hash number of this
  // object.
  //	EXAMPLE
  //	  [x:10.0]		-- name = "x", value = 10.0
  virtual ostream &printOn(ostream &xo) const;
#endif
  
  // Return the current value I hold.
  Number value() const
    { return _value; }

  // Round the value to an integer and return it
  int intValue() const
    { return int(_value + 0.5); }

  // change the value held -- should *not* use this if the variable is
  // in a solver -- instead use addEditVar() and suggestValue() interface
  void set_value(Number value)
    { _value = value; }

  // permit overriding in subclasses in case something needs to be
  // done when the value is changed by the solver
  // may be called when the value hasn't actually changed -- just 
  // means the solver is setting the external variable
  virtual void change_value(Number value)
    { _value = value; }

  void setPv(void *pv)
    { _pv = pv; }

  void *Pv() const
    { return _pv; }

  // Set the name of the variable
  virtual void setName(string const &name);

private:

  // similar to set_value -- see caveat above -- made private for now
  // since it's probably the wrong thing and is too easy to invoke
  Number operator=(Number value)
    { _value = value; return value; }

  // Copy constructor left undefined since we want to
  // outlaw passing by value!  Will get a link error if you
  // try to use within ClFloatVariable.c, compile-time error everywhere else
  ClFloatVariable(const ClFloatVariable &);

  Number _value;

  // C-style extension mechanism so I
  // don't have to wrap ScwmClVariables separately
  void *_pv;
};



#endif
