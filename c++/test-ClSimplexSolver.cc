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
// test-ClSimplexSolver.cc

#include <string>
#include "ClErrors.h"
#include "ClLinearEquation.h"
#include "ClLinearInequality.h"
#include "ClEditConstraint.h"
#include "ClStayConstraint.h"
#include "ClSimplexSolver.h"
#include "ClVariable.h"

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

  cerr << "Starting addConstraint-s" << endl;
  solver.addConstraint(a_b);
  solver.addConstraint(a_c);
  solver.addStay(a);
  solver.addStay(b);
  solver.addStay(c);
  solver.addConstraint(edit_b);
  
  vector<double> rgedits;

  cout << a_b << endl;
  cout << a_c << endl;
  
  rgedits.push_back(6.0);
  try 
    {
    solver.resolve(rgedits);
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
