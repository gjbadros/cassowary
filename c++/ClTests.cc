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
justStay1()
{
 try
   {
   bool fOkResult = true;
   ClVariable x(5);
   ClVariable y(10);
   ClSimplexSolver solver;

#if 0
   solver.addPointStay(x,y,1);
#else
   solver.addStay(x);
   solver.addStay(y);
#endif
   fOkResult = fOkResult && clApprox(x,5);
   fOkResult = fOkResult && clApprox(y,10);
   cout << "x == " << x.value() << endl;
   cout << "y == " << y.value() << endl;

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
   cerr << "next" << endl;
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
  ClSimplexSolver solver;
  ClVariable x("x");
  ClLinearEquation eq1(x,10.0);
  ClLinearEquation eq2(x,5.0);
  try 
    {
    
    solver.addConstraint( eq1 );
    solver.addConstraint( eq2 );
    
    // no exception, we failed!
    return(false);
    } 
  catch (ExCLRequiredFailure)
    {
    // we want this exception to get thrown
    cout << "Success -- got the exception" << endl;
    // solver.removeConstraint(eq2); this would throw a constraint not found exception
    //    cout << solver << endl;
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
 catch (ExCLRequiredFailure &)
   {
   // we want this exception to get thrown
   cout << "Success -- got the exception" << endl;
   //    cout << solver << endl;
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
inconsistent3()
{
  try
    {
    ClSimplexSolver solver;

    ClVariable r1("v2");
    ClVariable r2("v3");
    ClVariable r3("v4");
    ClVariable r4("v5");
    ClVariable r5("v6");
    ClVariable r6("v7");
    ClVariable r7("v8");
    ClVariable r8("v9");

#if REAL_NAMES
    ClVariable r1("r1");
    ClVariable r2("r2");
    ClVariable r3("r3");
    ClVariable r4("r4");
    ClVariable r5("r5");
    ClVariable r6("r6");
    ClVariable r7("r7");
    ClVariable r8("r8");
#endif

    ClLinearEquation eq1(r1,60);
    ClLinearEquation eq2(r2,30);
    ClLinearEquation eq12(r3,2.5);
    ClLinearEquation eq13(r6,0);
    ClLinearInequality eq14(r5, cnGEQ, 0);
    ClLinearInequality eq15(r8, cnLEQ, 2.5);
    ClLinearInequality eq16(r7, cnGEQ, r6);
    ClLinearInequality eq17(r8, cnGEQ, r7);
    ClLinearEquation eq18(r4, r3 - r2/60.0);
    ClLinearEquation eq19(r5, r4 - r1/60.0);
    ClLinearInequality eq20(r4, cnGEQ, 0);
    ClLinearInequality eq21(r5, cnGEQ, 0);
    ClLinearEquation eq22(r7, r6 + r2/20.0);
    ClLinearEquation eq23(r8, r7 + r1/20.0);
    ClLinearEquation eq24(r4, r3 - r2/30.0);
    ClLinearEquation eq25(r5, r4 - r1/30.0);
    ClLinearInequality eq26(r4, cnGEQ, 0);
    ClLinearInequality eq27(r5, cnGEQ, 0);
    ClLinearEquation eq28(r7, r6 + r2/60.0);
    ClLinearEquation eq29(r8, r7 + r1/60.0);

#if 0
    cout << "eq1:  " << solver.addConstraintNoException(eq1) << endl;
    cout << "eq2:  " << solver.addConstraintNoException(eq2) << endl;
    cout << "eq12: " << solver.addConstraintNoException(eq12) << endl;
    cout << "eq13: " << solver.addConstraintNoException(eq13) << endl;
    cout << "eq14: " << solver.addConstraintNoException(eq14) << endl;
    cout << "eq15: " << solver.addConstraintNoException(eq15) << endl;
    cout << "eq16: " << solver.addConstraintNoException(eq16) << endl;
    cout << "eq17: " << solver.addConstraintNoException(eq17) << endl;
    cout << "eq18: " << solver.addConstraintNoException(eq18) << endl;
    cout << "eq19: " << solver.addConstraintNoException(eq19) << endl;
    cout << "eq20: " << solver.addConstraintNoException(eq20) << endl;
    cout << "eq21: " << solver.addConstraintNoException(eq21) << endl;
    cout << "eq22: " << solver.addConstraintNoException(eq22) << endl;
    cout << "eq23: " << solver.addConstraintNoException(eq23) << endl;
    cout << "eq24: " << solver.addConstraintNoException(eq24) << endl;
    cout << "eq25: " << solver.addConstraintNoException(eq25) << endl;
    cout << "eq26: " << solver.addConstraintNoException(eq26) << endl;
    cout << "eq27: " << solver.addConstraintNoException(eq27) << endl;
    cout << "eq28: " << solver.addConstraintNoException(eq28) << endl;
    cout << "eq29: " << solver.addConstraintNoException(eq29) << endl;
#endif
#if 0
    solver.setAutosolve(false);

    cout << "eq1:  " << solver.addConstraintNoException(eq1) << "\t" << eq1 << endl;
    cerr << solver << endl;
    cout << "eq29: " << solver.addConstraintNoException(eq29) << "\t" << eq29 << endl;
    cerr << solver << endl;
    cout << "eq24: " << solver.addConstraintNoException(eq24) << "\t" << eq24 << endl;
    cerr << solver << endl;
    cout << "eq17: " << solver.addConstraintNoException(eq17) << "\t" << eq17 << endl;
    cerr << solver << endl;
    cout << "eq16: " << solver.addConstraintNoException(eq16) << "\t" << eq16 << endl;
    cerr << solver << endl;
    cout << "eq15: " << solver.addConstraintNoException(eq15) << "\t" << eq15 << endl;
    cerr << solver << endl;
    cout << "eq14: " << solver.addConstraintNoException(eq14) << "\t" << eq14 << endl;
    cerr << solver << endl;
    cout << "eq13: " << solver.addConstraintNoException(eq13) << "\t" << eq13 << endl;
    cerr << solver << endl;
    cout << "eq12: " << solver.addConstraintNoException(eq12) << "\t" << eq12 << endl;
    cerr << solver << endl;
    cout << "eq26: " << solver.addConstraintNoException(eq26) << "\t" << eq26 << endl;
    cerr << solver << endl;
    cout << "eq2:  " << solver.addConstraintNoException(eq2) << "\t" << eq2 << endl;
    cerr << solver << endl;
    cout << "eq23: " << solver.addConstraintNoException(eq23) << "\t" << eq23 << endl;
    cerr << solver << endl;
    cout << "eq25: " << solver.addConstraintNoException(eq25) << "\t" << eq25 << endl;
    cerr << solver << endl;
    cout << "eq27: " << solver.addConstraintNoException(eq27) << "\t" << eq27 << endl;
    cerr << solver << endl;

    cout << "-----" << endl;
    solver.solve();
    cerr << solver << endl;
    cout << "-----" << endl;

    cout << "eq27: " << solver.removeConstraintNoException(eq27) << "\t" << eq27 << endl;
    cerr << solver << endl;
    cout << "eq25: " << solver.removeConstraintNoException(eq25) << "\t" << eq25 << endl;
    cerr << solver << endl;
    cout << "eq23: " << solver.removeConstraintNoException(eq23) << "\t" << eq23 << endl;
    cerr << solver << endl;
    cout << "eq2:  " << solver.removeConstraintNoException(eq2) << "\t" << eq2 << endl;
    cerr << solver << endl;
    cout << "eq26: " << solver.removeConstraintNoException(eq26) << "\t" << eq26 << endl;
    cerr << solver << endl;
    cout << "eq12: " << solver.removeConstraintNoException(eq12) << "\t" << eq12 << endl;
    cerr << solver << endl;
    cout << "eq13: " << solver.removeConstraintNoException(eq13) << "\t" << eq13 << endl;
    cerr << solver << endl;
    cout << "eq14: " << solver.removeConstraintNoException(eq14) << "\t" << eq14 << endl;
    cerr << solver << endl;
    cout << "eq15: " << solver.removeConstraintNoException(eq15) << "\t" << eq15 << endl;
    cerr << solver << endl;
    cout << "eq16: " << solver.removeConstraintNoException(eq16) << "\t" << eq16 << endl;
    cerr << solver << endl;
    cout << "eq17: " << solver.removeConstraintNoException(eq17) << "\t" << eq17 << endl;
    cerr << solver << endl;
    cout << "eq24: " << solver.removeConstraintNoException(eq24) << "\t" << eq24 << endl;
    cerr << solver << endl;
    cout << "eq29: " << solver.removeConstraintNoException(eq29) << "\t" << eq29 << endl;
    cerr << solver << endl;
    cout << "eq1:  " << solver.removeConstraintNoException(eq1) << "\t" << eq1 << endl;
    cerr << solver << endl;

    solver.solve();

    cout << r1 << " = " << r1.value() << endl
         << r4 << " = " << r4.value() << endl
         << r5 << " = " << r5.value() << endl;
#endif

#if 1
    cout << "eq1:  " << solver.addConstraintNoException(eq1) << "\t" << eq1 << endl << solver << endl;
    cout << "eq17:  " << solver.addConstraintNoException(eq17) << "\t" << eq17 << endl << solver << endl;
    cout << "eq16:  " << solver.addConstraintNoException(eq16) << "\t" << eq16 << endl << solver << endl;
    cout << "eq15:  " << solver.addConstraintNoException(eq15) << "\t" << eq15 << endl << solver << endl;
    cout << "eq14:  " << solver.addConstraintNoException(eq14) << "\t" << eq14 << endl << solver << endl;
    cout << "eq13:  " << solver.addConstraintNoException(eq13) << "\t" << eq13 << endl << solver << endl;
    cout << "eq12:  " << solver.addConstraintNoException(eq12) << "\t" << eq12 << endl << solver << endl;
    cout << "eq2:  " << solver.addConstraintNoException(eq2) << "\t" << eq2 << endl << solver << endl;
    cout << "eq29:  " << solver.addConstraintNoException(eq29) << "\t" << eq29 << endl << solver << endl;
    cout << "eq27:  " << solver.addConstraintNoException(eq27) << "\t" << eq27 << endl << solver << endl;
    cout << "eq25:  " << solver.addConstraintNoException(eq25) << "\t" << eq25 << endl << solver << endl;
    cout << "REMeq25:  " << solver.removeConstraintNoException(eq25) << "\t" << eq25 << endl << solver << endl;
    cout << "REMeq27:  " << solver.removeConstraintNoException(eq27) << "\t" << eq27 << endl << solver << endl;
    cout << "REMeq29:  " << solver.removeConstraintNoException(eq29) << "\t" << eq29 << endl << solver << endl;
    cout << "eq23:  " << solver.addConstraintNoException(eq23) << "\t" << eq23 << endl << solver << endl;
    cout << "eq23:  " << solver.addConstraintNoException(eq23) << "\t" << eq23 << endl << solver << endl;
    cout << "eq29:  " << solver.addConstraintNoException(eq29) << "\t" << eq29 << endl << solver << endl;
    cout << "eq27:  " << solver.addConstraintNoException(eq27) << "\t" << eq27 << endl << solver << endl;
    cout << "eq25:  " << solver.addConstraintNoException(eq25) << "\t" << eq25 << endl << solver << endl;
    cout << "eq24:  " << solver.addConstraintNoException(eq24) << "\t" << eq24 << endl << solver << endl;
    cout << "eq24:  " << solver.addConstraintNoException(eq24) << "\t" << eq24 << endl << solver << endl;
    cout << "eq18:  " << solver.addConstraintNoException(eq18) << "\t" << eq18 << endl << solver << endl;
    cout << "eq22:  " << solver.addConstraintNoException(eq22) << "\t" << eq22 << endl << solver << endl;
    cout << "eq20:  " << solver.addConstraintNoException(eq20) << "\t" << eq20 << endl << solver << endl;
    cout << "eq26:  " << solver.addConstraintNoException(eq26) << "\t" << eq26 << endl << solver << endl;
    cout << "eq21:  " << solver.addConstraintNoException(eq21) << "\t" << eq21 << endl << solver << endl;
    cout << "eq19:  " << solver.addConstraintNoException(eq19) << "\t" << eq19 << endl << solver << endl;
    cout << "eq28:  " << solver.addConstraintNoException(eq28) << "\t" << eq28 << endl << solver << endl;
    cout << "eq23:  " << solver.addConstraintNoException(eq23) << "\t" << eq23 << endl << solver << endl;
    cout << "eq24:  " << solver.addConstraintNoException(eq24) << "\t" << eq24 << endl << solver << endl;
    

    cout << r1 << " = " << r1.value() << endl
         << r2 << " = " << r2.value() << endl
         << r3 << " = " << r3.value() << endl
         << r4 << " = " << r4.value() << endl
         << r5 << " = " << r5.value() << endl
         << r6 << " = " << r6.value() << endl
         << r7 << " = " << r7.value() << endl
         << r8 << " = " << r8.value() << endl;

#endif

    return false;
    }
  catch (ExCLError &error)
    {
   cerr << "Exception! " << error.description() << endl;
   return(true);
   }
 catch (...) 
   {
   cerr << "Unknown exception" << endl;
   return(false);
   }
}

bool
addDel(int nCns = 900, int nVars = 900, int nResolves = 10000)
//addDel(int nCns = 300, int nVars = 300, int nResolves = 1000)
//addDel(int nCns = 30, int nVars = 30, int nResolves = 100)
{
  Timer timer;
  // FIXGJB: from where did .12 come?
  static const double ineqProb = 0.12;
  static const int maxVars = 3;

  cout << "starting timing test. nCns = " << nCns
       << ", nVars = " << nVars << ", nResolves = " << nResolves << endl;

  timer.Start();
  ClSimplexSolver solver;
  solver.setAutosolve(false);

  ClVariable **rgpclv = new PClVariable[nVars];
  for (int i = 0; i < nVars; i++)
    {
    rgpclv[i] = new ClVariable(i,"x");
    solver.addStay(*rgpclv[i]);
    }

  ClConstraint **rgpcns = new PClConstraint[nCns];
  int nvs = 0;
  int k;
  int j;
  double coeff;
  for (j = 0; j < nCns; j++)
    {
    // number of variables in this constraint
    nvs = int(UniformRandom()*maxVars) + 1;
    ClLinearExpression expr = UniformRandom()*20.0 - 10.0;
    for (k = 0; k < nvs; k++)
       {
       coeff = UniformRandom()*10 - 5;
       expr.addExpression(*(rgpclv[int(UniformRandom()*nVars)]) * coeff);
       }
    if (UniformRandom() < ineqProb)
       {
       rgpcns[j] = new ClLinearInequality(expr);
       }
    else
       {  
       rgpcns[j] = new ClLinearEquation(expr);
       }
    }

  cout << "done building data structures" << endl;
  cout << "time = " << timer.ElapsedTime() << "\n" << endl;
  timer.Start();
  int cExceptions = 0;
  for (j = 0; j < nCns; j++)
    {
    // add the constraint -- if it's incompatible, just ignore it
    try
      {
      solver.addConstraint(*(rgpcns[j]));
      }
    catch (ExCLRequiredFailure &)
      {
      cExceptions++;
      rgpcns[j] = NULL;
      }
    }
  solver.solve();
  cout << "done adding constraints [" << cExceptions << " exceptions]" << endl;
  cout << "time = " << timer.ElapsedTime() << "\n" << endl;
  cout << "time per cn = " << timer.ElapsedTime()/nCns << "\n" << endl;
  cout << "time per actual cn = " << timer.ElapsedTime()/(nCns - cExceptions) << "\n" <<endl;
  timer.Start();

  int e1Index = int(UniformRandom()*nVars);
  int e2Index = int(UniformRandom()*nVars);

  ClEditConstraint edit1(*(rgpclv[e1Index]),clsStrong());
  ClEditConstraint edit2(*(rgpclv[e2Index]),clsStrong());

  solver
    .addConstraint(edit1)
    .addConstraint(edit2);

  cout << "done creating edit constraints -- about to start resolves" << endl;
  cout << "time = " << timer.ElapsedTime() << "\n" << endl;
  timer.Start();

  // FIXGJB start = Timer.now();
  for (int m = 0; m < nResolves; m++)
    {
    vector<Number> vals;
    vals.push_back(rgpclv[e1Index]->value() * 1.001);
    vals.push_back(rgpclv[e2Index]->value() * 1.001);
    solver.resolve(vals);
    }

  // cout << "run time: " <<

  cout << "done resolves -- now removing constraints" << endl;
  cout << "time = " << timer.ElapsedTime() << "\n" <<endl;
  cout << "time per resolve = " << timer.ElapsedTime()/nResolves << "\n" <<endl;
  solver.removeConstraint(edit1);
  solver.removeConstraint(edit2);
  
  timer.Start();

  for (j = 0; j < nCns; j++)
    {
    if (rgpcns[j])
      {
      solver.removeConstraint(*(rgpcns[j]));
      }
    }

  // FIXGJB end = Timer.now();
  // cout << "Total remove time: " 
  //      << "remove time per cn"
  cout << "done removing constraints and addDel timing test" << endl;
  cout << "time = " << timer.ElapsedTime() << "\n" <<endl;
  cout << "time per cn = " << timer.ElapsedTime()/nCns << "\n" <<endl;
  cout << "time per actual cn = " << timer.ElapsedTime()/(nCns - cExceptions) << "\n" <<endl;

  timer.Start();

  return true;
}


int
main( int argc, char **argv )
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
    
    //    RUN_TEST(justStay1);
    //    RUN_TEST(addDelete1);
    //    RUN_TEST(addDelete2);
    //    RUN_TEST(casso1);
    //    RUN_TEST(inconsistent1);
    //    RUN_TEST(inconsistent2);
    RUN_TEST(inconsistent3);

    int cns = 900, vars = 900, resolves = 10000;

    if (argc > 1)
      cns = atoi(argv[1]);

    if (argc > 2)
      vars = atoi(argv[2]);

    if (argc > 3)
      resolves = atoi(argv[3]);

    if (cns > 0) 
      {
      cout << "addDel" << ":" << endl;
      fResult = addDel(cns,vars,resolves); fAllOkResult &= fResult;
      if (!fResult) cout << "Failed!" << endl;
      }
      
#undef RUN_TEST
    return (fAllOkResult? 0 : 255);
    
    } 
  catch (...) 
    {
    cerr << "exception!" << endl;
    }
}
