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

#include <iostream.h>
#include "Cassowary.h"

class ClAbstractVariable {
public:
  ClAbstractVariable() :
    my_name(String("None"))
    { }

  virtual ~ClAbstractVariable()
    { }

  // Return the name of the variable
  String name() const
    { return my_name; }

  // Set the name of the variable
  void setName(String const &name)
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
  virtual ostream &printOn(ostream &xo) const
    {  xo << "CV#" << my_name << endl;  return xo; }

private:
  String my_name;
};

#endif
