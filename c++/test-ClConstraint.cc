// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998, All Rights Reserved.
//
// (c) 1998 Alan Borning and Greg Badros.  This code is provided for use by
// students for course projects in the course CSE 595 in the Department of
// Computer Science and Engineering, University of Washington, during winter
// quarter 1998.  Any other use requires written permission from the copyright
// holders.
//
// test-ClConstraint.cc

#include <string>
#include "Cassowary.h"
#include "ClVariable.h"
#include "ClLinearEquation.h"
#include "ClLinearInequality.h"
#include "ClStayConstraint.h"

int
main( char **, int )
{
  ClVariable a("a");
  ClVariable b("b");
  ClVariable c("c");
  ClVariable w("w");
  ClVariable x("x");
  ClVariable y("y");

  ClLinearExpression cle = a * 3 + b * 2 + c + 5;
  ClLinearEquation constraint(cle);
  cout << constraint << endl;
  ClLinearExpression cle2 = cle / 2 + 1;
  ClLinearInequality constraint2(cle2);
  cout << constraint2 << endl;

  cout << constraint2.expression() << endl;

  ClStayConstraint cn(w);
  cout << cn << endl;

  cerr << (cn.variable() == w? "yup" : "nope") << endl;
  cerr << (cn.variable() == a? "yup" : "nope") << endl;

  cerr << "constraint is " << constraint.isRequired() << endl;

  return 0;
}
