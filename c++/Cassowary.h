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

#include <string>
#include <assert.h>

// FIXGJB what is the right thing to use to test for VC++ 5.0
#ifdef _WIN32
#include <iostream>
#pragma warning(disable : 4786)
using namespace std;
#define __FUNCTION__ "WIN"
#else
#include <iostream.h>
#endif

typedef double Number;

#endif
