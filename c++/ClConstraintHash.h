// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998, 1999 Alan Borning and Greg Badros
// See ../LICENSE for legal details regarding this software
//
// ClHash.h

#ifndef CL_HASH_H__
#define CL_HASH_H__

#ifdef CL_USE_HASH_MAP_AND_SET

#include <hash_map>

class ClConstraint;

struct hash<const ClConstraint *> {
  size_t operator()(const ClConstraint * const p) const 
    { return size_t((unsigned long)p/CL_PTR_HASH_DIVISOR); }
};

struct hash<ClConstraint *> {
  size_t operator()(ClConstraint * const p) const 
    { return size_t((unsigned long)p/CL_PTR_HASH_DIVISOR); }
};

#endif // CL_USE_HASH_MAP_AND_SET


#endif
