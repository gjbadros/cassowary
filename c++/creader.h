// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998 Alan Borning and Greg Badros
// See ../COPYRIGHT for legal details regarding this software
//
// creader.h
// Contributed by Steve Wolfman

#ifndef CREADER_H
#define CREADER_H

#include <string>
#include <map>
class istream;
class ClConstraint;
class ClVariable;

typedef map<string,const ClVariable *> StringToVarMap;

// Attempts to read a constraint of input stream in
// Returns constraint (freshly allocated, client responsibility to deallocate)
// if succesful. Otherwise, returns 0.
ClConstraint * parseConstraint(istream &xi, StringToVarMap &mapVars);

#endif
