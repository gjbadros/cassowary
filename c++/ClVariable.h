// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998, 1999 Alan Borning and Greg Badros
// See ../LICENSE for legal details regarding this software
//
// ClVariable.h
// A handle on ClAbstractVariable-s

#ifndef ClVariable_H
#define ClVariable_H

#include <stdio.h>
#include <map>
#include <string>
#include "Cassowary.h"
#include "ClFloatVariable.h"

class ClVariable;
typedef map<string,ClVariable> StringToVarMap;


class ClVariable {
  ClAbstractVariable *pclv;
public:
  ClVariable(ClAbstractVariable *pclv_) : pclv(pclv_) { }
  ClVariable(ClAbstractVariable &clv_) : pclv(&clv_) { }

  /// These ctrs build ClFloatVariable-s
  ClVariable(string name, Number value = 0.0) 
      : pclv(new ClFloatVariable(name,value)) 
    { if (pmapSzPclv) { (*pmapSzPclv)[name] = *this; }  }
  ClVariable(Number value = 0.0) 
      : pclv(new ClFloatVariable(value)) { }
  ClVariable(long number, char *prefix, Number value = 0.0)
      : pclv(new ClFloatVariable(number,prefix,value)) { }

  /// permit ClVariables to be used as pointers to pclvs
  ClAbstractVariable *operator->() { return pclv; }

  /// and also forward the function calls along

  bool isDummy() const { return pclv->isDummy(); }
  bool isExternal() const { return pclv->isExternal(); }
  bool isPivotable() const { return pclv->isPivotable(); }
  bool isRestricted() const { return pclv->isRestricted(); }

  string name() const { return pclv->name(); }

  Number value() const { return pclv->value(); }
  int intValue() const { return pclv->intValue(); }
  void set_value(Number value) 
    { pclv->set_value(value); }
  void change_value(Number value) 
    { pclv->change_value(value); }
  void setPv(void *pv) 
    { pclv->setPv(pv); }
  void *Pv() const 
    { return pclv->Pv(); }

  void setName(string const &name) { pclv->setName(name); }

  ClAbstractVariable *get_pclv() const { return pclv; } 
  bool isNil() const { return pclv == NULL; }

  static void SetVarMap(StringToVarMap *pmap) { pmapSzPclv = pmap; }
  static StringToVarMap *VarMap() { return pmapSzPclv; }
  static StringToVarMap *pmapSzPclv;
#ifndef CL_NO_IO
  ostream &printOn(ostream &xo) const
    { return pclv->printOn(xo); }
#endif

  friend bool operator<(ClVariable cl1, ClVariable cl2)
    { return cl1.pclv < cl2.pclv; }

  friend bool operator==(ClVariable cl1, ClVariable cl2)
    { return cl1.pclv == cl2.pclv; }

  friend bool operator!=(ClVariable cl1, ClVariable cl2)
    { return !(cl1 == cl2); }

};

#ifndef CL_NO_IO
inline ostream &operator<<(ostream &xo, const ClVariable &clv)
{ return clv.printOn(xo); }
#endif

#ifdef CL_USE_HASH_MAP_AND_SET
struct hash<ClVariable> { 
  size_t operator()(const ClVariable & v) const
    { return size_t(v.get_pclv());  }
};
#endif


#include <math.h>

// Compare two double-s approximately, since equality is no good
inline bool clApprox(double a, double b)
{
  const double epsilon = 1.0e-8;
  if (a > b) {
    return (a - b) < epsilon;
  } else {
    return (b - a) < epsilon;
  }
}

// Can remove these if I decide to 
// autoconvert from ClVariable-s to double-s
inline bool clApprox(ClVariable clv, double b)
{
  return clApprox(clv->value(),b);
}

inline bool clApprox(double a, ClVariable clv)
{
  return clApprox(a,clv->value());
}

extern ClVariable clvNil;

#endif
