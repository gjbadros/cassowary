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
#include <algorithm>
#include "ClErrors.h"
#include "ClVariable.h"
#include "ClLinearExpression_fwd.h"

class ClConstraint;
class istream;

class ClVarLookupFunction : public unary_function<const string &,ClVariable *> {
public:
  virtual ClVariable *operator()(const string &) const { return &clvNil; }
};


// Attempts to read a constraint of input stream in
// Returns constraint (freshly allocated, client responsibility to deallocate)
// if succesful. Otherwise, returns 0.
ClConstraint *PcnParseConstraint(istream &xi, const ClVarLookupFunction &lookup_func);

class ClVarLookupInMap : public ClVarLookupFunction {
public:
  ClVarLookupInMap(StringToVarMap *pmapVars, bool fAutoCreate) 
      : _pmapVars(pmapVars), _fAutoCreate(fAutoCreate) { }

  ClVariable *operator()(const string &str) const
    { 
      StringToVarMap &_mapVars = *_pmapVars;
      StringToVarMap::iterator it = _mapVars.find(str);
      if (it != _mapVars.end()) {
        return &it->second;
      } else if (_fAutoCreate) {
        // save the old symbol table, if any
        StringToVarMap *pmapOld = ClVariable::VarMap();
	// and set it to this one temporarily
        ClVariable::SetVarMap(&_mapVars);
	ClVariable *pclv = new ClVariable(str);
        // now switch it back
        ClVariable::SetVarMap(pmapOld);
        return pclv;
      } else {
        return &clvNil;
      }
    }
private:
  StringToVarMap *_pmapVars;
  bool _fAutoCreate;
};


/* the "yyerror" function */
void clerror(const char *sz);

struct ClParseData {
  ClParseData(istream &xi, const ClVarLookupFunction &lookup_func)
      : _xi(xi), _lookup_func(lookup_func) { }

  ClConstraint *Pcn() { return _pcn; }
      
  istream & _xi;
  ClConstraint * _pcn;
  const ClVarLookupFunction &_lookup_func;
};


#endif
