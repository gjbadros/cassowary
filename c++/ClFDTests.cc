// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998, 1999 Greg J. Badros and Alan Borning
// See ../LICENSE for legal details regarding this software
// 
// ClTests.cc

#include "Cl.h"
#include "ClFDConnectorVariable.h"
#include <stdlib.h>
#include "timer.h"
#include <iostream>
#include <iomanip>

inline 
double UniformRandom()
{ return double(rand())/RAND_MAX; }


bool
simple1()
{
 try
   {
   bool fOkResult = true;
   list<FDNumber> l;
   ListPushOnto(&l,6,8,10,12,14,16,20,24,30,40,60,FDN_EOL);
   ClVariable x(new ClFDVariable("x",10,l));
   ClVariable y(new ClFDVariable("y",14,l));
   cout << x << ", " << y << endl;
   ClFDBinaryOneWayConstraint cn1(x,cnGEQ,y,1,5);
   ClFDBinaryOneWayConstraint cn2(x,cnGEQ,y);
   ClFDBinaryOneWayConstraint cn3(y,cnGEQ,x);
   ClFDBinaryOneWayConstraint cn4(y,cnGEQ,10);
   cout << cn1 << endl;
   cout << cn2 << endl;
   cout << cn3 << endl;
   cout << cn4 << endl;

   ClFDSolver fdsolver;

   cerr << (fdsolver.AddConstraintNoException(&cn1)? "Added cn1" : "Failed adding cn1") << endl;
   cerr << fdsolver;
   cerr << (fdsolver.AddConstraintNoException(&cn2)? "Added cn2" : "Failed adding cn2") << endl;
   cerr << fdsolver;
   cerr << (fdsolver.AddConstraintNoException(&cn3)? "Added cn3" : "Failed adding cn3") << endl;
   cerr << fdsolver;
   cerr << (fdsolver.AddConstraintNoException(&cn4)? "Added cn4" : "Failed adding cn4") << endl;
   cerr << fdsolver;

   fdsolver.ShowSolve();
   fdsolver.Solve();

   cout << "x = " << x.Value() << "; y = " << y.Value() << endl;
   
   cerr << (fdsolver.RemoveConstraintNoException(&cn1) ? "Removed cn1" : "Failed removing cn1")
        << endl;
   cerr << fdsolver;
   cerr << (fdsolver.RemoveConstraintNoException(&cn2) ? "Removed cn2" : "Failed removing cn2")
        << endl;
   cerr << fdsolver;
   cerr << (fdsolver.RemoveConstraintNoException(&cn3) ? "Removed cn3" : "Failed removing cn3") 
        << endl;
   cerr << fdsolver;
   cerr << (fdsolver.RemoveConstraintNoException(&cn4) ? "Removed cn4" : "Failed removing cn4") 
        << endl;
   cerr << fdsolver;

   return fOkResult;
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
simple2()
{
 try
   {
   bool fOkResult = true;
   list<FDNumber> l;
   ListPushOnto(&l,6,8,10,12,14,16,20,24,30,40,60,FDN_EOL);
   ClVariable x(new ClFDVariable("x",10,l));
   cout << x << endl;
   ClFDBinaryOneWayConstraint cn1(x,cnEQ,18,ClsStrong());
   cout << cn1 << endl;

   ClFDSolver fdsolver;

   cerr << (fdsolver.AddConstraintNoException(&cn1)? "Added cn1" : "Failed adding cn1") << endl;
   cerr << fdsolver;

   fdsolver.ShowSolve();
   fdsolver.Solve();

   cout << "x = " << x.Value() << endl;
   
   cerr << (fdsolver.RemoveConstraintNoException(&cn1) ? "Removed cn1" : "Failed removing cn1")
        << endl;
   cerr << fdsolver;

   return fOkResult;
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
simple3()
{
 try
   {
   bool fOkResult = true;
   list<FDNumber> l;
   ListPushOnto(&l,6,8,10,12,14,16,20,24,30,40,60,FDN_EOL);
   ClVariable x(new ClFDVariable("x",10,l));
   cout << x << endl;
   ClFDBinaryOneWayConstraint cn1(x,cnEQ,18,ClsRequired());
   cout << cn1 << endl;

   ClFDSolver fdsolver;

   cerr << (fdsolver.AddConstraintNoException(&cn1)? "Added cn1" : "Failed adding cn1") << endl;
   cerr << fdsolver;

   fdsolver.ShowSolve();
   if (fdsolver.SolveNoException())
     cout << "Test failed... the solve should not have succeeded!" << endl;

   cout << "x = " << x.Value() << endl;
   
   cerr << (fdsolver.RemoveConstraintNoException(&cn1) ? "Removed cn1" : "Failed removing cn1")
        << endl;
   cerr << fdsolver;

   return fOkResult;
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


connect1()
{
 try
   {
   bool fOkResult = true;
   list<FDNumber> l;
   ClFDSolver fdsolver;
   ClSimplexSolver solver;
   ListPushOnto(&l,6,8,10,12,14,16,20,24,30,40,60,FDN_EOL);
   ClVariable z("z",0);
   ClVariable yf("yf",0);
   ClVariable x(new ClFDVariable("x",10,l));
   ClVariable y(new ClFDConnectorVariable("y",20,l,solver,yf));
   cout << z << "," << yf << endl;
   cout << x << "," << y << endl;
   ClFDBinaryOneWayConstraint cn1(x,cnEQ,14,ClsRequired());
   ClFDBinaryOneWayConstraint cn2(x,cnGT,y,1,0,ClsStrong());
   ClLinearInequality cn3(z,cnGEQ,yf*2+3);

   solver.AddConstraint(cn3);

   // x == 14 !required
   // x > y   !strong
   // z >= yf*2 + 3 !required

   // want: x = 14, y = 6,8,10,or 12  (copied into yf)
   //       z = y*2 + 3  = 15 when y = 6

   fdsolver.SetAutosolve(false);
   cerr << cn1 << endl;
   cerr << (fdsolver.AddConstraintNoException(&cn1)? "Added cn1" : "Failed adding cn1") << endl;
   cerr << fdsolver;
   cerr << cn2 << endl;
   cerr << (fdsolver.AddConstraintNoException(&cn2)? "Added cn2" : "Failed adding cn2") << endl;
   cerr << fdsolver;

   fdsolver.Solve();

   cout << z << "," << yf << endl;
   cout << x << "," << y << endl;
   
   cerr << (fdsolver.RemoveConstraintNoException(&cn1) ? "Removed cn1" : "Failed removing cn1")
        << endl;
   cerr << fdsolver;

   return fOkResult;
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
main( int /* argc */, char ** /* argv */ )
{
  try 
    {
    bool fAllOkResult = true;
    bool fResult;
    
    // seed the random number generator for reproducible results
    srand(123456789);

    cout << "Cassowary version: " << szCassowaryVersion << endl;

#define RUN_TEST(x) \
    cout << #x << ":" << endl; \
    fResult = x(); fAllOkResult &= fResult; \
    if (!fResult) cout << "Failed!" << endl;

    RUN_TEST(simple1);
    RUN_TEST(simple2);
    RUN_TEST(simple3);
    RUN_TEST(connect1);
    
    return (fAllOkResult? 0 : 255);
    
    } 
  catch (...) 
    {
    cerr << "exception!" << endl;
    }
}
