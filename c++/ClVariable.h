// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998 Alan Borning and Greg Badros
// See ../COPYRIGHT for legal details regarding this software
//
// ClVariable.h

#ifndef ClVariable_H
#define ClVariable_H

#include <stdio.h>
#include "Cassowary.h"
#include "ClAbstractVariable.h"

class ClVariable : public ClAbstractVariable {
public:
  ClVariable(string name = "", Number value = 0.0) :
    ClAbstractVariable(name),
    _value(value)
    { }

  ClVariable(Number value) :
    ClAbstractVariable(""),
    _value(value)
    { }

  ClVariable(long number, char *prefix, Number value = 0.0) :
    ClAbstractVariable(number,prefix),
    _value(value)
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

  // Prints a semi-descriptive representation to the stream, using the
  // name if there is one, and otherwise the hash number of this
  // object.
  //	EXAMPLES
  //	  x[10.0]		-- w/ name
  //	  x[0.0,100]		-- w/ name, bounds but no value yet
  //	  CV#345(10.0)		-- w/o name
  virtual ostream &printOn(ostream &xo) const
  {  
    xo << "[" << name() << ":" << _value << "]";
    return xo;
  }
  
  // Return the current value I hold.
  Number value() const
    { return _value; }

  // Round the value to an integer and return it
  int intValue() const
    { int i = int(_value + 0.5); 
#ifdef CL_TRACE_VERBOSE
    cerr << "intValue() returning i = " << i << endl;
#endif
    return i;
    }

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

private:

  // similar to set_value -- see caveat above -- made private for now
  // since it's probably the wrong thing and is too easy to invoke
  Number operator=(Number value)
    { _value = value; return value; }

  // Copy constructor left undefined since we want to
  // outlaw passing by value!  Will get a link error if you
  // try to use within ClVariable.c, compile-time error everywhere else
  ClVariable(const ClVariable &);

  Number _value;
};

typedef ClVariable *PClVariable;

#include <math.h>

// Compare two double-s approximately, since equality is no good
inline bool clApprox(double a, double b)
{
  const double epsilon = 1.0e-8;
  if (a > b) {
    return (a - b) < epsilon;
  } else {
    return (b - a) < epsilon;
  }
}

// Can remove these if I decide to 
// autoconvert from ClVariable-s to double-s
inline bool clApprox(const ClVariable &clv, double b)
{
  return clApprox(clv.value(),b);
}

inline bool clApprox(double a, const ClVariable &clv)
{
  return clApprox(a,clv.value());
}

#endif
