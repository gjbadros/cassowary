// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998, 1999 Greg J. Badros and Alan Borning
// See ../LICENSE for legal details regarding this software
//
// ClFDBinaryOneWayConstraint.cc


#ifdef HAVE_CONFIG_H
#include <cassowary/config.h>
#define CONFIG_H_INCLUDED
#endif

#include "ClFDBinaryOneWayConstraint.h"
#include "ClLinearConstraint.h"
#include "ClTypedefs.h"
#include "ClLinearExpression.h"


ClFDBinaryOneWayConstraint::ClFDBinaryOneWayConstraint(const ClLinearConstraint &cn)
    :ClFDConstraint(cn.strength(), cn.weight())
{
   list<FDNumber> l;
   /* GJB:FIXME:: varargs inteface, with sentinel as first arg? */
   l.push_back(9);
   l.push_back(10);
   l.push_back(12);
   l.push_back(14);
   l.push_back(20);

  ClVarSet setRO = cn.ReadOnlyVars();
  if (setRO.size() > 1) 
    throw ExCLEditMisuse("Only 0 or 1 read only variables are allowed");

  ClVariable clvRO = clvNil;
  ClVariable clvROLinear = clvNil;
  Number coeffRO = 0;

  ClVariable clvRW = clvNil;
  Number coeffRW = 0;

  if (setRO.size() == 1) {
    const ClVariable &clv = *(setRO.begin());
    clvRO = new ClFDVariable(clv.Name(),clv.IntValue(),l);
    clvROLinear = clv;
  }
  const ClLinearExpression &expr = cn.Expression();
  const ClVarToNumberMap &terms = expr.Terms();

  if (terms.size() > 2)
    throw ExCLEditMisuse("Cannot have more than 2 variables");
  if (terms.size() == 0)
    throw ExCLEditMisuse("Must have at least 1 variable");

  if (terms.size() == 2 && setRO.size() == 0)
    throw ExCLEditMisuse("Both variables cannot be read-write, one must be read-only");

  for (ClVarToNumberMap::const_iterator it = terms.begin();
       it != terms.end();
       ++it) {
    ClVariable clv = (*it).first;
    if (clv == clvROLinear) {
      coeffRO = (*it).second;
    } else {
      clvRW = new ClFDVariable(clv.Name(),clv.Value(),l);
      coeffRW = (*it).second;
    }
  }
  if (clvRW.IsNil()) {
    throw ExCLEditMisuse("Single read-only variable in LinearConstraint -- must not be read-only.");
  }
  if (coeffRW == 0) {
    throw ExCLEditMisuse("RW variable's coefficient must be non-zero");
  }

  bool fInequality = cn.IsInequality();
  double rhs_constant = expr.Constant();

  // now we have:
  // coeffRW * clvRW + coeffRO * clvRO <REL> rhs_constant
  //   where <REL> is >= if fInequality, or = if !fInequality
  // 
  // need:
  //   clvRW <REL> coefficient * clvRO + constant
  // 
  // so:
  // coefficient = -coeffRO/coeffRW
  // constant = rhs_constant/coeffRW

  _rel = fInequality? cnGEQ: cnEQ;
  _coefficient = -coeffRO/coeffRW;
  _constant = -rhs_constant/coeffRW;
  _vRW = clvRW;
  _vRO = clvRO;
  return;
}
