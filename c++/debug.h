// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998, 1999 Alan Borning and Greg Badros
// See ../LICENSE for legal details regarding this software
//
// debug.h

#ifndef CASSOWARY_DEBUG_H_
#define CASSOWARY_DEBUG_H_

#include <vector>
#include "Cassowary.h"
#include "ClTypedefs.h"

class ClAbstractVariable;

#ifndef CL_NO_IO

ostream &operator<<(ostream &xo, const ClTableauVarSet &varset);

ostream &operator<<(ostream &xo, const ClTableauColumnsMap &varmap);

ostream &operator<<(ostream &xo, const ClTableauRowsMap &rows);

ostream &operator<<(ostream &xo, const ClVarVector &varlist);

class Tracer {
 public:
  Tracer(const char *const sz) : sz_(sz) { cerr << "* " << sz; }
  ~Tracer() { cerr << "x " << sz_ << " exited." << endl; }
 private:
  const char *const sz_;
};

#endif

#endif
