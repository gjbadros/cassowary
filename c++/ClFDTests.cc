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
   /* GJB:FIXME:: varargs inteface, with sentinel as first arg? */
   l.push_back(9);
   l.push_back(10);
   l.push_back(12);
   l.push_back(14);
   l.push_back(20);
   ClVariable x(new ClFDVariable("x",10,l));
   ClVariable y(new ClFDVariable("y",14,l));
   cout << x << ", " << y << endl;
   ClFDBinaryOneWayConstraint cn1(x,cnLT,y,2,4);
   ClFDBinaryOneWayConstraint cn2(x,cnGEQ,y);
   ClFDBinaryOneWayConstraint cn3(y,cnGEQ,x);
   ClFDBinaryOneWayConstraint cn4(y,cnGEQ,18);
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
    
    return (fAllOkResult? 0 : 255);
    
    } 
  catch (...) 
    {
    cerr << "exception!" << endl;
    }
}
