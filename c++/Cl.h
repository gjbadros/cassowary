// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998, 1999 Alan Borning and Greg Badros
// See ../LICENSE for legal details regarding this software
//
// Cl.h

#ifndef CL_H
#define CL_H

#ifdef CL_NO_IO
#ifndef CL_NO_TRACE /* prevent re-def warning */
#define CL_NO_TRACE
#endif
#undef CL_SOLVER_STATS
#undef CL_DEBUG_FAILURES
#endif

#include "ClVariable.h"
#include "ClSimplexSolver.h"
#include "ClLinearEquation.h"
#include "ClLinearInequality.h"
#include "ClErrors.h"
#include "ClEditConstraint.h"
#include "ClStayConstraint.h"

#endif
