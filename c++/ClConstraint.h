// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998 Alan Borning and Greg Badros
// See ../COPYRIGHT for legal details regarding this software
//
// ClConstraint.h

#ifndef ClConstraint_H
#define ClConstraint_H

#include "Cassowary.h"
#include "ClLinearExpression.h"
#include "ClStrength.h"


class ClConstraint {
public:

  ClConstraint(const ClStrength &strength = clsRequired(), double weight = 1.0 ) :
    _strength(strength),
    _weight(weight),
    _pv(0)
    { }

  virtual ~ClConstraint()
    { }

  // Return my linear expression.  (For linear equations, this
  // constraint represents expression=0; for linear inequalities it
  // represents expression>=0.)
  virtual ClLinearExpression expression() const = 0;

  // Returns true if this is an edit constraint
  virtual bool isEditConstraint() const
    { return false; }

  // Return true if this is an inequality constraint and
  // false if it is an equality constraint.  The default is
  // that it is not.
  virtual bool isInequality() const
    { return false; }

  virtual bool isRequired() const
    { return _strength.isRequired(); }

  virtual bool isStayConstraint() const
    { return false; }

  virtual const ClStrength &strength() const
    { return _strength; }

  virtual double weight() const
    { return _weight; }

  virtual ostream &printOn(ostream &xo) const = 0;

  friend ostream& operator<<(ostream &xos, const ClConstraint &constraint)
    { constraint.printOn(xos); return xos; }

  void setPv(void *pv)
    { _pv = pv; }

  void *Pv() const
    { return _pv; }

  virtual bool FIsSatisfied() const { return false; }

private:

  void setStrength( const ClStrength &strength )
    { _strength = strength; }

  void setWeight( double weight )
    { _weight = weight; }

  /// instance variables
  ClStrength _strength;

  double _weight;

  void *_pv;
};

typedef ClConstraint *PClConstraint;

#endif
