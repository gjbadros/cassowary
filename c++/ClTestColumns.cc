// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998, 1999 Greg J. Badros and Alan Borning
// See ../LICENSE for legal details regarding this software
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

    
   ClLinearInequality c10(x,cnLEQ,10.0);
   ClLinearInequality c20(x,cnLEQ,20.0);

   ClSimplexSolver solver;
   solver.AddConstraint( new ClLinearEquation( x, 100, ClsWeak() ) );
    
   solver
     .AddConstraint(c10)
     .AddConstraint(c20);

   fOkResult = fOkResult && ClApprox(x,10.0);
   cout << "x == " << x.Value() << endl;

   solver.RemoveConstraint(c10);
   fOkResult = fOkResult && ClApprox(x,20.0);
   cout << "x == " << x.Value() << endl;

   solver.RemoveConstraint(c20);
   fOkResult = fOkResult && ClApprox(x,100.0);
   cout << "x == " << x.Value() << endl;

   ClLinearInequality c10again(x,cnLEQ,10.0);

   solver
     .AddConstraint(c10)
     .AddConstraint(c10again);

   fOkResult = fOkResult && ClApprox(x,10.0);
   cout << "x == " << x.Value() << endl;
    
   solver.RemoveConstraint(c10);
   fOkResult = fOkResult && ClApprox(x,10.0);
   cout << "x == " << x.Value() << endl;

   solver.RemoveConstraint(c10again);
   fOkResult = fOkResult && ClApprox(x,100.0);
   cout << "x == " << x.Value() << endl;

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
