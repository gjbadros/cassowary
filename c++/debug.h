// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998, 1999 Alan Borning and Greg Badros
// See ../COPYRIGHT for legal details regarding this software
//
// debug.h

#ifndef CASSOWARY_DEBUG_H_
#define CASSOWARY_DEBUG_H_

#include "Cassowary.h"
#include "ClMap.h"
#include "ClSet.h"
#include <vector>
#include "ClLinearExpression_fwd.h"

class ClAbstractVariable;

ostream &operator<<(ostream &xo, const ClSet<const ClAbstractVariable *> & varset);

ostream &operator<<(ostream &xo, const ClMap<const ClAbstractVariable *, ClSet<const ClAbstractVariable *> > & varmap);

ostream &operator<<(ostream &xo, const ClMap<const ClAbstractVariable *, ClLinearExpression * > & rows);

ostream &operator<<(ostream &xo, const vector<const ClAbstractVariable *> &varlist);

class Tracer {
 public:
  Tracer(const char *const sz) : sz_(sz) { cerr << "* " << sz; }
  ~Tracer() { cerr << "x " << sz_ << " exited." << endl; }
 private:
  const char *const sz_;
};

#endif
