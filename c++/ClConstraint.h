// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998, 1999 Alan Borning and Greg Badros
// See ../LICENSE for legal details regarding this software
//
// ClConstraint.h

#ifndef ClConstraint_H
#define ClConstraint_H

#include "debug.h"

#include "Cassowary.h"
#include "ClLinearExpression.h"
#include "ClStrength.h"

class ClSimplexSolver;

#ifdef USE_GC_CONSTRAINT
class ClConstraint : public gc {
#else
class ClConstraint {
#endif
public:

  ClConstraint(const ClStrength &strength = ClsRequired(), double weight = 1.0 ) :
    _strength(strength),
    _weight(weight),
    _pv(0),
    _times_added(0)
    { 
      CtrTracer(__FUNCTION__,this);
    }

  virtual ~ClConstraint()
    { 
      DtrTracer(__FUNCTION__,this);
    }

  // Return my linear Expression.  (For linear equations, this
  // constraint represents Expression=0; for linear inequalities it
  // represents Expression>=0.)
  virtual ClLinearExpression Expression() const = 0;

  // Returns true if this is an edit constraint
  virtual bool IsEditConstraint() const
    { return false; }

  // Return true if this is an inequality constraint and
  // false if it is an equality constraint.  The default is
  // that it is not.
  virtual bool IsInequality() const
    { return false; }

  virtual bool IsRequired() const
    { return _strength.IsRequired(); }

  virtual bool isStayConstraint() const
    { return false; }

  virtual const ClStrength &strength() const
    { return _strength; }

  virtual double weight() const
    { return _weight; }

#ifndef CL_NO_IO
  virtual ostream &PrintOn(ostream &xo) const = 0;

  friend ostream& operator<<(ostream &xos, const ClConstraint &constraint)
    { constraint.PrintOn(xos); return xos; }

#endif


  void SetPv(void *pv)
    { _pv = pv; }

  void *Pv() const
    { return _pv; }

  virtual bool FIsSatisfied() const { return false; }

  virtual bool FIsInSolver() const { return _times_added != 0; }

  void ChangeStrength( const ClStrength &strength) 
    { 
      if (_times_added == 0) {
        setStrength(strength);
      } else {
        throw ExCLTooDifficult();
      }
    }

  void ChangeWeight( double weight )
    { 
      if (_times_added == 0) {
        setWeight(weight);
      } else {
        throw ExCLTooDifficult();
      }
    }

  friend ClSimplexSolver;
private:

  void addedTo(const ClSimplexSolver &)
    { ++_times_added; }

  void removedFrom(const ClSimplexSolver &)
    { --_times_added; }

  void setStrength( const ClStrength &strength )
    { _strength = strength; }

  void setWeight( double weight )
    { _weight = weight; }

  /// instance variables
  ClStrength _strength;

  double _weight;

  void *_pv;

  int _times_added;
};

typedef ClConstraint *PClConstraint;

#endif
