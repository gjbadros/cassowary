// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998, 1999 Greg J. Badros and Alan Borning
// See ../LICENSE for legal details regarding this software
//
// test-ClLinearExpression.cc

#include <string>
#include "Cl.h"

int
main( char **, int )
{
  ClVariable a("a");
  ClLinearExpression cle(a);
  cout << cle << endl;
  ClVariable b("b");
  cle.AddVariable(b,2);
  cout << cle << endl;
  cout << cle.Times(2) << endl;
  cout << (cle.Times(2).AddVariable(ClVariable("c"),3)).Times(-1) << endl;
  cle = cle + 8;
  cout << cle << endl;
  cle.ChangeSubject(a,b);
  cout << cle << endl;
  ClLinearExpression cle2 = cle;
  cle.AddExpression(cle,-1);
  cout << cle << endl;
  cout << b << endl;
  ClSimplexSolver solver;
  cout << cle2 << endl;
  ClVariable c("c");
  ClLinearExpression cleNew = cle2 * 2 + 9;
  cout << cleNew << endl;
  cle2.SubstituteOut(a,cleNew,c,solver);
  cout << cle2 << endl;

  return 0;
}
