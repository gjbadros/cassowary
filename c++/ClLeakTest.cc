// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998, 1999 Alan Borning and Greg Badros
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
leakTest()
{
 try
   {
   bool fOkResult = true;
   ClVariable x(167);
   ClVariable y(2);
   ClSimplexSolver solver;

   ClLinearEquation eq(x,y+0.0);
   solver.SetAutosolve(false);
   solver.AddStay(x);
   solver.AddStay(y);
   for (int i = 0; i < 100000; ++i) {
#ifdef DEBUG_ADD_REMOVE
     cerr << "i = " << i << endl;
     cerr << "before Add: " << solver << "\n" << endl;
#endif
     solver.AddConstraint(eq);
#ifdef DEBUG_ADD_REMOVE
     cerr << "before remove: " << solver << "\n" << endl;
#endif
     solver.RemoveConstraint(eq);
#ifdef DEBUG_ADD_REMOVE
     cerr << "---------------------------" << "\n\n" << endl;
#endif
   }
   solver.solve();
   fOkResult = (x.Value() == y.Value());
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
leakTest2()
{
  for (int i=0; i<100; ++i)
    {
    new ClVariable();
    }
#ifdef USE_GC
  GC_gcollect();
#endif
  return true;
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
    //    RUN_TEST(leakTest2);

#ifdef USE_GC
    GC_gcollect();
    cerr << "Num Collects = " << GC_gc_no << endl;
#endif
#undef RUN_TEST
    return (fAllOkResult? 0 : 255);
    
    } 
  catch (...) 
    {
    cerr << "exception!" << endl;
    }
}
