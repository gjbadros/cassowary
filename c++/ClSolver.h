// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998, 1999 Greg J. Badros and Alan Borning
// See ../LICENSE for legal details regarding this software
//
// ClSolver.h

#ifndef ClSolver_H
#define ClSolver_H

#if defined(HAVE_CONFIG_H) && !defined(CONFIG_H_INCLUDED) && !defined(CONFIG_INLINE_H_INCLUDED)
#include <cassowary/config-inline.h>
#define CONFIG_INLINE_H_INCLUDED
#endif

#include "Cassowary.h"
#include "ClErrors.h"
#include "ClTypedefs.h"

class ClVariable;

// ClSolver is an abstract base class
class ClSolver {
 public:
  virtual ~ClSolver()
    { } 

  void SetPv(void *pv)
    { _pv = pv; }

  void *Pv() const
    { return _pv; }

 protected:

  // C-style extension mechanism so I
  // don't have to wrap ScwmClSolver separately
  void *_pv;

};

#endif
