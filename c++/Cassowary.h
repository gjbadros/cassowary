// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998 Alan Borning and Greg Badros
// See ../COPYRIGHT for legal details regarding this software
//
// Cassowary.h

#ifndef Cassowary_H
#define Cassowary_H

#ifdef USE_GC
#include "../../gc/gc_cpp.h"
#endif

#include <string>
#include <assert.h>

#ifdef _MSC_VER
#include <iostream>
#pragma warning(disable : 4786)
using namespace std;
#define __FUNCTION__ "WIN"
#else
#include <iostream.h>
#endif

typedef double Number;

#define NEWVAR(x) do { cerr << "line " << __LINE__ << ": new " << x << endl; } while (0)
#define DELVAR(x) do { cerr << "line " << __LINE__ << ": del " << x << endl; } while (0)

#endif
