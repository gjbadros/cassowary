// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998, All Rights Reserved.
//
// ClSimplexSolver.h

#ifndef ClSimplexSolver_H
#define ClSimplexSolver_H

#include "Cassowary.h"

class ClVariable;

class ClSimplexSolver {
 public:
  ClSimplexSolver()
    { }

  void noteRemovedVariable(const ClVariable &v, const ClVariable &subject)
    { }

  void noteAddedVariable(const ClVariable &v, const ClVariable &subject)
    { }

 private:
};

#endif
