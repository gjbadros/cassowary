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


bool
addDelete1()
{
 try 
   {
   bool fOkResult = true; 
   ClVariable x("x");
   ClSimplexSolver solver;

   solver.addConstraint( ClLinearEquation( 0-x /* = */ + 100, clsWeak() ) );
    
   ClLinearInequality c10(10-x);
   ClLinearInequality c20(20-x);
    
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

   ClLinearInequality c10again = ClLinearInequality(10-x);

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

bool
addDelete2()
{
 try 
   {
   bool fOkResult = true; 
   ClVariable x("x");
   ClVariable y("y");
   ClSimplexSolver solver;

   solver
     .addConstraint( ClLinearEquation(x, 100.0, clsWeak()))
     .addConstraint( ClLinearEquation(y, 120.0, clsStrong()));

   ClLinearInequality c10(x,cnLEQ,10.0);
   fOkResult = fOkResult && clApprox(x,10.0);
   cout << "x == " << x.value() << endl;

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

bool
casso1()
{
 try 
   {
   bool fOkResult = true; 
   ClVariable x("x");
   ClVariable y("y");
   ClSimplexSolver solver;

   fOkResult = fOkResult && clApprox(x,10.0);
   cout << "x == " << x.value() << endl;

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
main( char **argv, int argc )
{
  bool fOkResult = true;
  fOkResult = fOkResult && addDelete1();

  
  
  exit (fOkResult? 0 : 255);
}