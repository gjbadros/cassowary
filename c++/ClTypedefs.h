// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998, 1999 Alan Borning and Greg Badros
// See ../LICENSE for legal details regarding this software
//
// ClTypedefs.h

#ifndef CL_TYPEDEFS_H__
#define CL_TYPEDEFS_H__

#include "ClMap.h"
#include "ClSet.h"
#include "ClLinearExpression_fwd.h"
#include <set> // Since ClTableauVarSet is always a set (never a hash_set)
#include <vector>

class ClVariable;
class ClConstraint;
class ClEditInfo;

// For Tableau
// ClTableauVarSet is not a ClSet, for performance reasons 
// (Steve Wolfman discovered this, and seems to be true --02/17/99 gjb)
// I have not observed any big performance gains from using the hashtable based containers 
typedef set<ClVariable> ClVarSet;  
typedef ClMap<ClVariable, ClVarSet > ClTableauColumnsMap;
typedef ClMap<ClVariable, ClLinearExpression *> ClTableauRowsMap;

// For Solver
typedef ClMap<const ClConstraint *, ClVarSet > ClConstraintToVarSetMap;
typedef ClMap<const ClConstraint *, ClVariable> ClConstraintToVarMap;
typedef ClMap<ClVariable, const ClConstraint *> ClVarToConstraintMap;
typedef vector<ClVariable> ClVarVector;


#endif