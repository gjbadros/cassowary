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

#ifndef ClFDSolver_H
#define ClFDSolver_H

#if defined(HAVE_CONFIG_H) && !defined(CONFIG_H_INCLUDED) && !defined(CONFIG_INLINE_H_INCLUDED)
#include <cassowary/config-inline.h>
#define CONFIG_INLINE_H_INCLUDED
#endif

#include "Cassowary.h"
#include "ClSolver.h"
#include "ClErrors.h"
#include "ClTypedefs.h"

class ClVariable;

// ClFDSolver is a concrete class
// implementing a very restricted (for now --04/23/99 gjb)
// finite-domain constraint solving algorithm
class ClFDSolver: public ClSolver {
 public:
  ClFDSolver()
    { }
  virtual ~ClFDSolver()
    { } 

 protected:

};

#endif
