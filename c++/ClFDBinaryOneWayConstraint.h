// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998, 1999 Greg J. Badros and Alan Borning
// See ../LICENSE for legal details regarding this software
//
// ClFDBinaryOneWayConstraint.h

#ifndef ClFDBinaryOneWayConstraint_H
#define ClFDBinaryOneWayConstraint_H

#if defined(HAVE_CONFIG_H) && !defined(CONFIG_H_INCLUDED) && !defined(CONFIG_INLINE_H_INCLUDED)
#include <cassowary/config-inline.h>
#define CONFIG_INLINE_H_INCLUDED
#endif

#include "Cassowary.h"
#include "ClFDConstraint.h"

// Just a node in the class hierarchy for now
class ClFDBinaryOneWayConstraint : public ClFDConstraint {
 private: typedef ClFDConstraint super;

 public:

   ClFDBinaryOneWayConstraint(ClVariable vRW, enum ClCnRelation rel, ClVariable vRO, 
                              double coefficient = 1.0, double constant = 0.0)
       : ClFDConstraint(), _vRW(vRW), _rel(rel), _vRO(vRO),
         _coefficient(coefficient), _constant(constant)
    { }

   ClFDBinaryOneWayConstraint(ClVariable vRW, enum ClCnRelation rel, double constant)
       : ClFDConstraint(), _vRW(vRW), _rel(rel), _vRO(clvNil),
         _coefficient(0), _constant(constant)
    { }

#ifndef CL_NO_IO
  virtual ostream &PrintOn(ostream &xo) const
    { 
      xo << "FDCn: " << _vRW << " " << StrCnRelation(_rel) << " ";
      if (_coefficient != 0) {
        if (_coefficient != 1) xo << _coefficient << "*";
        if (_vRO != clvNil) xo << _vRO;
      }
      if (_constant != 0) xo << " + " << _constant;
      return xo;
    }

  friend ostream& operator<<(ostream &xos, const ClFDBinaryOneWayConstraint &constraint)
    { return constraint.PrintOn(xos); }

#endif

  ClVariable ClvRW() const
    { return _vRW; }
  ClVariable ClvRO() const 
    { return _vRO; }
  ClCnRelation Relation() const 
    { return _rel; }
  double Coefficient() const 
    { return _coefficient; }
  double Constant() const 
    { return _constant; }

 protected:
  ClVariable _vRW;
  ClCnRelation _rel;
  ClVariable _vRO;
  double _coefficient;
  double _constant;
};

#endif
