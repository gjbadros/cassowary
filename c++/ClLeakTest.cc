// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998 Alan Borning and Greg Badros
// See ../COPYRIGHT for legal details regarding this software
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
leakTest()
{
 try
   {
   bool fOkResult = true;
   ClVariable x(167);
   ClVariable y(2);
   ClSimplexSolver solver;

   ClLinearEquation eq(x,y+0.0);
#if 0
   solver.addStay(x);
   solver.addStay(y);
#endif
   for (int i = 0; i < 10000; ++i) {
#ifdef DEBUG_ADD_REMOVE
     cerr << "i = " << i << endl;
     cerr << "before add: " << solver << "\n" << endl;
#endif
     solver.addConstraint(eq);
#ifdef DEBUG_ADD_REMOVE
     cerr << "before remove: " << solver << "\n" << endl;
#endif
     solver.removeConstraint(eq);
#ifdef DEBUG_ADD_REMOVE
     cerr << "---------------------------" << "\n\n" << endl;
#endif
   }
   fOkResult = (x.value() == y.value());
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
main( int , char ** )
{
  try 
    {
    bool fAllOkResult = true;
    bool fResult;
    
    // seed the random number generator for reproducible results
    srand(123456789);

#define RUN_TEST(x) \
    cout << #x << ":" << endl; \
    fResult = x(); fAllOkResult &= fResult; \
    if (!fResult) cout << "Failed!" << endl;

    RUN_TEST(leakTest);
#undef RUN_TEST
    return (fAllOkResult? 0 : 255);
    
    } 
  catch (...) 
    {
    cerr << "exception!" << endl;
    }
}
