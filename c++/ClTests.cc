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
   ClLinearInequality c20(x,cnLEQ,20.0);

   solver
     .addConstraint(c10)
     .addConstraint(c20);
   fOkResult = fOkResult && clApprox(x,10.0) && clApprox(y,120.0);
   cout << "x == " << x.value() << ", y == " << y.value() << endl;

   solver.removeConstraint(c10);
   fOkResult = fOkResult && clApprox(x,20.0) && clApprox(y,120.0);
   cout << "x == " << x.value() << ", y == " << y.value() << endl;
   
   ClLinearEquation cxy( 2*x, y);
   solver.addConstraint(cxy);
   fOkResult = fOkResult && clApprox(x,20.0) && clApprox(y,40.0);
   cout << "x == " << x.value() << ", y == " << y.value() << endl;

   solver.removeConstraint(c20);
   fOkResult = fOkResult && clApprox(x,60.0) && clApprox(y,120.0);
   cout << "x == " << x.value() << ", y == " << y.value() << endl;

   solver.removeConstraint(cxy);
   fOkResult = fOkResult && clApprox(x,100.0) && clApprox(y,120.0);
   cout << "x == " << x.value() << ", y == " << y.value() << endl;


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

   solver
     .addConstraint( ClLinearInequality(x,cnLEQ,y) )
     .addConstraint( ClLinearEquation(y, x+3.0) )
     .addConstraint( ClLinearEquation(x,10.0,clsWeak()) )
     .addConstraint( ClLinearEquation(y,10.0,clsWeak()) )
     ;
   
   fOkResult = fOkResult && 
     ( clApprox(x,10.0) && clApprox(y,13.0) ||
       clApprox(x,7.0) && clApprox(y,10.0) );
     
   cout << "x == " << x.value() << ", y == " << y.value() << endl;

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
inconsistent1()
{
 try 
   {
   ClVariable x("x");
   ClSimplexSolver solver;

   solver
     .addConstraint( ClLinearEquation(x,10.0) )
     .addConstraint( ClLinearEquation(x, 5.0) );

   // no exception, we failed!
   return(false);
   } 
 catch (ExCLRequiredFailure &error)
   {
   // we want this exception to get thrown
   cout << "Success -- got the exception" << endl;
   return(true);
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
inconsistent2()
{
 try 
   {
   ClVariable x("x");
   ClSimplexSolver solver;

   solver
     .addConstraint( ClLinearInequality(x,cnGEQ,10.0) )
     .addConstraint( ClLinearInequality(x,cnLEQ, 5.0) );

   // no exception, we failed!
   return(false);
   } 
 catch (ExCLRequiredFailure &error)
   {
   // we want this exception to get thrown
   cout << "Success -- got the exception" << endl;
   return(true);
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
  bool fAllOkResult = true;
  bool fResult;

#define RUN_TEST(x) \
cout << #x << ":" << endl; \
fResult = x(); fAllOkResult &= fResult; \
if (!fResult) cout << "Failed!" << endl;

  RUN_TEST(addDelete1);
  RUN_TEST(addDelete2);
  RUN_TEST(casso1);
  RUN_TEST(inconsistent1);
  RUN_TEST(inconsistent2);
  
#undef RUN_TEST

  exit (fAllOkResult? 0 : 255);
}
