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
    my_value(value)
    { }

  ClVariable(Number value) :
    ClAbstractVariable(""),
    my_value(value)
    { }

  ClVariable(long number, char *prefix, Number value = 0.0) :
    ClAbstractVariable(number,prefix),
    my_value(value)
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
    xo << "[" << name() << ":" << my_value << "]";
    return xo;
  }
  
  // Return the current value I hold.
  Number value() const
    { return my_value; }

  void set_value(Number value)
    { my_value = value; }

  Number operator=(Number value)
    { my_value = value; return value; }

private:
  // Copy constructor left undefined since we want to
  // outlaw passing by value!  Will get a link error if you
  // try to use
  ClVariable(const ClVariable &);

  Number my_value;
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
