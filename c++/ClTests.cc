// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998, All Rights Reserved.
// 
// ClTests.cc

#include "Cl.h"

int
main( char **argv, int argc )
{
  bool fOkResult = true;
  ClVariable x("x");

  ClSimplexSolver solver;

  try 
    {
    solver.addConstraint( ClLinearEquation( 0-x /* = */ + 100, clsWeak() ) );
    
    ClLinearInequality c10 = ClLinearInequality(10-x);
    ClLinearInequality c20 = ClLinearInequality(20-x);
    
    solver.addConstraint(c10);
    solver.addConstraint(c20);

    fOkResult = fOkResult && clApprox(x,10.0);
    cout << "x == " << x.value() << endl;

    solver.removeConstraint(c10);
    fOkResult = fOkResult && clApprox(x,20.0);
    cout << "x == " << x.value() << endl;

    solver.removeConstraint(c20);
    fOkResult = fOkResult && clApprox(x,100.0);
    cout << "x == " << x.value() << endl;

    exit(fOkResult? 0 : -1);
    } 
  catch (ExCLError &error) 
    {
    cerr << "Exception! " << error.description() << endl;
    exit(-1);
    } 
  catch (...) 
    {
    cerr << "Unknown exception" << endl;
    }
}
