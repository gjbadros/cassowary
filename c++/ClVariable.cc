// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998, 1999 Alan Borning and Greg Badros
// See ../COPYRIGHT for legal details regarding this software
//
// ClVariable.cc

#include "ClVariable.h"

StringToVarMap *ClVariable::pmapSzPclv = NULL;
