// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998, All Rights Reserved.
//
// ClVariable.h

#ifndef ClVariable_H
#define ClVariable_H

#include <iostream.h>
#include <stdio.h>
#include <assert.h>
#include "Cassowary.h"

typedef enum { CLAbstractVar, CLSlackVar, CLObjectiveVar, CLDummyVar, CLVar } ClVariableKind;

class ClVariable;

ClVariable &clvNil();

class ClVariable {
public:
  ClVariable(String name = "", ClVariableKind kind = CLVar, Number value = 0.0) :
    my_kind(kind),
    my_name(name),
    my_value(value),
    my_fDummy(false),
    my_fExternal(false),
    my_fPivotable(false),
    my_fRestricted(false)
    { 
    if (name.length() == 0)
      {
      char sz[16];
      sprintf(sz,"v%ld",iVariableNumber++);
      my_name = String(sz);
      }
    initializeFor(kind);
    assert (isCLVar() || my_value == 0.0);
    }

  ClVariable(long number, char *prefix, ClVariableKind kind = CLVar, Number value = 0.0) :
    my_kind(kind),
    my_value(value),
    my_fDummy(false),
    my_fExternal(false),
    my_fPivotable(false),
    my_fRestricted(false)
    { 
    char sz[32];
    sprintf(sz,"%s%ld",prefix,number);
    my_name = String(sz);
    initializeFor(kind);
    assert (isCLVar() || my_value == 0.0);
    }

  bool isCLVar() const
    { return my_kind == CLVar; }

  bool isCLObjective() const
    { return my_kind == CLObjectiveVar; }
    
  // Return the name of the variable
  String name() const
    { return my_name; }

  // Set the name of the variable
  void setName(String const &name)
    { my_name = name; }

  // Return true if this a dummy variable (used as a marker variable
  // for required equality constraints).  Such variables aren't
  // allowed to enter the basis when pivoting.
  bool isDummy() const
    { return my_fDummy; }

  // Return true if this a variable known outside the solver.  
  // (We need to give such variables a value after solving is complete.)
  bool isExternal() const
    { return my_fExternal; }

  // Return true if we can pivot on this variable.
  bool isPivotable() const
    { return my_fPivotable; }

  // Return true if this is a restricted (or slack) variable.  Such
  // variables are constrained to be non-negative and occur only
  // internally to the simplex solver.
  bool isRestricted() const
    { return my_fRestricted; }

  // Prints a semi-descriptive representation to the stream, using the
  // name if there is one, and otherwise the hash number of this
  // object.
  //	EXAMPLES
  //	  x[10.0]		-- w/ name
  //	  x[0.0,100]		-- w/ name, bounds but no value yet
  //	  CV#345(10.0)		-- w/o name
  ostream &printOn(ostream &xo) const
  {  
    xo << "[" << my_name << ":" << my_value << "]";
    return xo;
  }
  
  // Return the current value I hold.
  Number value() const
    { assert(isCLVar()); return my_value; }

  void set_value(Number const &value)
    { assert(isCLVar()); my_value = value; }

  friend ostream& operator<<(ostream &xos, const ClVariable &clv)
    { clv.printOn(xos); return xos; }

  friend bool operator<(const ClVariable &cl1, const ClVariable &cl2)
    { return cl1.my_name < cl2.my_name; }

  friend bool operator==(const ClVariable &cl1, const ClVariable &cl2)
    { return cl1.my_name == cl2.my_name; }

private:
  void initializeFor(ClVariableKind kind)
    {
    switch (kind)
      {
      case CLAbstractVar:
	break;
      case CLSlackVar:
	my_fPivotable = true;
	my_fRestricted = true;
	break;
      case CLObjectiveVar:
	break;
      case CLDummyVar:
	my_fDummy = true;
	my_fRestricted = true;
	break;
      case CLVar:
	my_fExternal = true;
	break;
      }
    }

  ClVariableKind my_kind;
  String my_name;
  Number my_value;
  bool my_fDummy;
  bool my_fExternal;
  bool my_fPivotable;
  bool my_fRestricted;
  
  static long iVariableNumber;
};

#endif
