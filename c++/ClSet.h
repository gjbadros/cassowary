// $Id$
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998, 1999 Alan Borning and Greg Badros
// See ../LICENSE for legal details regarding this software
//
// ClSet.h
// Define a set templated type, either as a hash_set or as a regular set
// depending on makefile cpp flag CL_USE_HASH_MAP_AND_SET

#ifndef CL_SET_H
#define CL_SET_H

#ifdef CL_USE_HASH_MAP_AND_SET
#include <hash_set>
#define ClSet hash_set
#else
#include <set>
#define ClSet set
#endif

#endif
