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
// ClTestColumns.cc

#include "Cl.h"
#include <stdlib.h>
#include "timer.h"

bool
addDelete1()
{
 try 
   {
   bool fOkResult = true; 
   ClVariable x("x");
   ClSimplexSolver solver;

   solver.addConstraint( ClLinearEquation( x, 100, clsWeak() ) );
    
   ClLinearInequality c10(x,cnLEQ,10.0);
   ClLinearInequality c20(x,cnLEQ,20.0);
    
   solver
     .addConstraint(c10)
     .addConstraint(c20);

   fOkResult = fOkResult && clApprox(x,10.0);
   cout << "x == " << x.value() << endl;

   solver.removeConstraint(c10);
   fOkResult = fOkResult && clApprox(x,20.0);
   cout << "x == " << x.value() << endl;

   solver.removeConstraint(c20);
   fOkResult = fOkResult && clApprox(x,100.0);
   cout << "x == " << x.value() << endl;

   ClLinearInequality c10again(x,cnLEQ,10.0);

   solver
     .addConstraint(c10)
     .addConstraint(c10again);

   fOkResult = fOkResult && clApprox(x,10.0);
   cout << "x == " << x.value() << endl;
    
   solver.removeConstraint(c10);
   fOkResult = fOkResult && clApprox(x,10.0);
   cout << "x == " << x.value() << endl;

   solver.removeConstraint(c10again);
   fOkResult = fOkResult && clApprox(x,100.0);
   cout << "x == " << x.value() << endl;

   cerr << "Solver == " << solver << endl;

   return(fOkResult);
   } 
 catch (ExCLError &error) 
   {
   cerr << "Exception! " << error.description() << endl;
   return(false);
   } 
 catch (...) 
   {
   cerr << "Unknown exception" << endl;
   return(false);
   }
}

int
main( int /* argc */ , char ** /* argv */ )
{
  try 
    {
    bool fAllOkResult = true;
    bool fResult;
    
#define RUN_TEST(x) \
    cout << #x << ":" << endl; \
    fResult = x(); fAllOkResult &= fResult; \
    if (!fResult) cout << "Failed!" << endl;
    
    RUN_TEST(addDelete1);

#undef RUN_TEST

    return (fAllOkResult? 0 : 255);
    
    } 
  catch (...) 
    {
    cerr << "Random exception!" << endl;
    }
}
