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

  // Add the constraint cn to the solver
  virtual ClSolver &AddConstraint(ClConstraint *const pcn) = 0;

  // Remove the constraint cn from the solver
  virtual ClSolver &RemoveConstraint(ClConstraint *const pcn) = 0;

  // Same as above, but returns false if the constraint cannot be solved
  // (i.e., the resulting system would be unsatisfiable)
  // The above function "AddConstraint" throws an exception in that case
  // which may be inconvenient
  virtual bool AddConstraintNoException(ClConstraint *const pcn)
    {
      try {
          AddConstraint(pcn);
          return true;
      }
      catch (const ExCLRequiredFailure &e)
        { return false; }
      catch (const ExCLTooDifficult &e)
        { return false; }
    }

#ifndef CL_NO_DEPRECATED
  // Deprecated --02/22/99 gjb
  bool AddConstraintNoException(ClConstraint &cn)
    { return AddConstraintNoException(&cn); }
#endif

  virtual bool RemoveConstraintNoException(ClConstraint *const pcn)
    {
      try {
        RemoveConstraint(pcn);
        return true;
      }
      catch (const ExCLConstraintNotFound &e)
        { return false; }
    }

#ifndef CL_NO_DEPRECATED
  // Deprecated --02/22/99 gjb
  bool RemoveConstraintNoException(ClConstraint &cn)
    { return RemoveConstraintNoException(&cn); }
#endif

  virtual ClSolver &Solve()
    { assert(false); return *this; }

  void SetPv(void *pv)
    { _pv = pv; }

  void *Pv() const
    { return _pv; }

 protected:

  // C-style extension mechanism so I
  // don't have to wrap ScwmClSolver separately
  void *_pv;

};


#ifndef CL_NO_IO
ostream &PrintTo(ostream &xo, const ClVarVector &varlist);
ostream &operator<<(ostream &xo, const ClVarVector &varlist);

ostream &PrintTo(ostream &xo, const ClConstraintToVarSetMap &mapCnToVarSet);
ostream &operator<<(ostream &xo, const ClConstraintToVarSetMap &mapCnToVarSet);

ostream &PrintTo(ostream &xo, const ClConstraintSet &setCn);
ostream &operator<<(ostream &xo, const ClConstraintSet &setCn);

#endif

#endif
