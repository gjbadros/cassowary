// $Id$
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998, 1999 Alan Borning and Greg Badros
// See ../COPYRIGHT for legal details regarding this software
//
// ClMap.h
// Define a map templated type, either as a hash_map or as a regular map
// depending on makefile cpp flag USE_HASH_MAP_AND_SET

#ifndef CL_MAP_H
#define CL_MAP_H

#ifdef USE_HASH_MAP_AND_SET
#include <hash_map>
#define ClMap hash_map
#else
#include <map>
#define ClMap map
#endif

#endif
