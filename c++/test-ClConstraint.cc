// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998, 1999 Greg J. Badros and Alan Borning
// See ../LICENSE for legal details regarding this software
//
// test-ClConstraint.cc

#include <string>
#include "Cl.h"

int
main( char **, int )
{
  ClVariable a("a");
  ClVariable b("b");
  ClVariable c("c");
  ClVariable w("w");
  //  ClVariable x("x");
  //  ClVariable y("y");

  ClLinearExpression cle = a * 3 + b * 2 + c + 5;
  ClLinearEquation constraint(cle);
  cout << constraint << endl;
  ClLinearExpression cle2 = cle / 2 + 1;
  ClLinearInequality constraint2(cle2);
  cout << constraint2 << endl;

  cout << constraint2.Expression() << endl;

  ClStayConstraint cn(w);
  cout << cn << endl;

  cerr << (cn.variable() == w? "yup" : "nope") << endl;
  cerr << (cn.variable() == a? "yup" : "nope") << endl;

  cerr << "constraint is " << constraint.IsRequired() << endl;

  return 0;
}
