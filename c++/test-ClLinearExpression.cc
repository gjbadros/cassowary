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
// test-ClLinearExpression.cc

#include <string>
#include "Cassowary.h"
#include "ClVariable.h"
#include "ClLinearExpression.h"
#include "ClSimplexSolver.h"

int
main( char **, int )
{
  ClVariable a("a");
  ClLinearExpression cle(a);
  cout << cle << endl;
  ClVariable b("b");
  cle.addVariable(b,2);
  cout << cle << endl;
  cout << cle.times(2) << endl;
  cout << (cle.times(2).addVariable(ClVariable("c"),3)).times(-1) << endl;
  cle = cle + 8;
  cout << cle << endl;
  cle.changeSubject(a,b);
  cout << cle << endl;
  ClLinearExpression cle2 = cle;
  cle.addExpression(cle,-1);
  cout << cle << endl;
  cout << b << endl;
  ClSimplexSolver solver;
  cout << cle2 << endl;
  ClVariable c("c");
  ClLinearExpression cleNew = cle2 * 2 + 9;
  cout << cleNew << endl;
  cle2.substituteOut(a,cleNew,c,solver);
  cout << cle2 << endl;

  return 0;
}
