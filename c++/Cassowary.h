// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998, 1999 Alan Borning and Greg Badros
// See ../LICENSE for legal details regarding this software
//
// Cassowary.h

#ifndef Cassowary_H
#define Cassowary_H

#ifdef CL_USE_HASH_MAP_AND_SET
#include <hash_map>
#include <hash_set>
class ClConstraint;
struct hash<const ClConstraint *>
{
  size_t operator()(const ClConstraint * const p) const {
    return (size_t)p;
  }
};
struct hash<ClConstraint *>
{
  size_t operator()(ClConstraint * const p) const {
    return (size_t)p;
  }
};
#endif /* CL_USE_HASH_MAP_AND_SET */

#ifdef USE_GC
#include "../../gc/gc_cpp.h"
#endif

#include <string>
#include <assert.h>

#ifdef _MSC_VER
#ifndef CL_NO_IO
#include <iostream>
#endif
#pragma warning(disable : 4786)
using namespace std;
#define __FUNCTION__ "WIN"
#else
#ifndef CL_NO_IO
#include <iostream.h>
#endif
#endif

typedef double Number;

#define NEWVAR(x) do { cerr << "line " << __LINE__ << ": new " << x << endl; } while (0)
#define DELVAR(x) do { cerr << "line " << __LINE__ << ": del " << x << endl; } while (0)

#endif
