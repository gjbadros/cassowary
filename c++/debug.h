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

#ifdef CL_TRACE
class Tracer {
 public:
  Tracer(const char *const sz) : sz_(sz) { cerr << "* " << sz; }
  ~Tracer() { cerr << "x " << sz_ << " exited." << endl; }
 private:
  const char *const sz_;
};

inline void CtrTracer(const char *const sz, const void *pv) 
{ cerr << "@+ " << sz << " ctrnew@ " << pv << endl; }

inline void DtrTracer(const char *const sz, const void *pv)
{ cerr << "@- " << sz << " dtrnew@ " << pv << endl; }

#else
class Tracer {
 public:
  Tracer(const char *const) { }
};

inline void CtrTracer(const char *const, const void *) { }
inline void DtrTracer(const char *const, const void *) { }
#endif // CL_TRACE  

#endif
