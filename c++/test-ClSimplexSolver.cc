// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998, 1999 Alan Borning and Greg Badros
// See ../LICENSE for legal details regarding this software
//
// test-ClSimplexSolver.cc

#include <string>
#include "Cl.h"

int
main( char **, int )
{
  ClVariable a("a",0.0);
  ClVariable b("b",0.0);
  ClVariable c("c",0.0);

  ClLinearEquation a_b( 0-a /* = */ + 2 * b + 1);
  ClLinearEquation a_c( 0-c /* = */ + b - 1);
  ClEditConstraint edit_a(a);
  ClEditConstraint edit_b(b);
  ClEditConstraint edit_c(c);
  
  ClSimplexSolver solver;

  cerr << "Starting AddConstraint-s" << endl;
  solver.AddConstraint(a_b);
  solver.AddConstraint(a_c);
  solver.AddStay(a);
  solver.AddStay(b);
  solver.AddStay(c);
  solver.AddConstraint(edit_b);
  
  vector<double> rgedits;

  cout << a_b << endl;
  cout << a_c << endl;
  
  rgedits.push_back(6.0);
  try 
    {
    solver.Resolve(rgedits);
    cout << "a = " << a << endl;
    cout << "b = " << b << endl;
    cout << "c = " << c << endl;
    }
  catch (const ExCLError &error) 
    {
    cerr << "Exception " << error.description() << endl;
    exit(-1);
    }

  return 0;
}
