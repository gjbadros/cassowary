// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998, All Rights Reserved.
//
// ClVariable.cc

#include "ClVariable.h"
#include <assert.h>

long ClVariable::iVariableNumber = 0;

ClVariable &
clvNil()
{
  static ClVariable nil_variable("Nil", CLDummyVar);
  return nil_variable;
}
