// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998, 1999 Alan Borning and Greg Badros
// See ../LICENSE for legal details regarding this software
//
// creader.h
// Original implementation contributed by Steve Wolfman
// Subsequently largely revised by Greg J. Badros

#ifndef CREADER_H
#define CREADER_H

#include <string>
#include <map>
#include "ClVariable.h"
#include "ClLinearExpression_fwd.h"

class ClConstraint;
class istream;

// Attempts to read a constraint of input stream in
// Returns constraint (freshly allocated, client responsibility to deallocate)
// if succesful. Otherwise, returns 0.
ClConstraint *PcnParseConstraint(istream &xi, StringToVarMap &mapVars, bool fAutoCreate = false);

/* the "yyerror" function */
void clerror(const char *sz);

struct ClParseData {
  ClParseData(istream &xi, StringToVarMap &mapVars, bool fAutoCreate)
      : _xi(xi), _mapVars(mapVars), _fAutoCreate(fAutoCreate) {};

  ClConstraint *Pcn() { return _pcn; }
      
  istream & _xi;
  ClConstraint * _pcn;
  StringToVarMap &_mapVars;
  bool _fAutoCreate;
};


#endif
