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
#include "ClDummyVariable.h"
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
   ClVariable x(167);
   ClVariable y(2);
   ClSimplexSolver solver;

   ClLinearEquation eq(x,y+0.0);
   solver.addStay(x);
   solver.addStay(y);
   solver.addConstraint(eq);
   cout << "x = " << x.value() << endl
        << "y = " << y.value() << endl;
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


/* add an edit variable to an empty solver */
bool
simple2()
{
 try
   {
   ClVariable x(167);
   ClSimplexSolver solver;

   solver.addEditVar(x);
   solver.beginEdit();
   solver.suggestValue(x,100);
   solver.endEdit();

   cout << "x = " << x.value() << endl;
   } 
 catch (ExCLEditMisuse &error)
   {
   cout << "Success: got the exception" << endl;
   return true;
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
 cerr << "Should have gotten an exception!" << endl;
 return false;
}


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

   solver.addConstraint(new ClLinearEquation( x, 100, clsWeak() ));
    
   ClLinearInequality c10(x,cnLEQ,10.0);
   ClLinearInequality c20(x,cnLEQ,20.0);
   solver
     .addConstraint(c10)
     .addConstraint(c20);

   fOkResult = fOkResult && clApprox(x,10.0);
   cout << "x == " << x.value() << endl;

   cout << endl << solver << endl;

   solver.removeConstraint(c10);

   cout << endl << solver << endl;

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
     .addConstraint(new ClLinearEquation(x, 100.0, clsWeak()))
     .addConstraint(new ClLinearEquation(y, 120.0, clsStrong()));

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
     .addConstraint(new ClLinearInequality(x,cnLEQ,y))
     .addConstraint(new ClLinearEquation(y, x+3.0))
     .addConstraint(new ClLinearEquation(x,10.0,clsWeak()))
     .addConstraint(new ClLinearEquation(y,10.0,clsWeak()))
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
     .addConstraint(new ClLinearInequality(x,cnGEQ,10.0))
     .addConstraint(new ClLinearInequality(x,cnLEQ, 5.0));

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
   ClVariable w("w");
   ClVariable x("x");
   ClVariable y("y");
   ClVariable z("z");
   ClSimplexSolver solver;

   solver
     .addConstraint(new ClLinearInequality(w,cnGEQ,10.0))
     .addConstraint(new ClLinearInequality(x,cnGEQ,w))
     .addConstraint(new ClLinearInequality(y,cnGEQ,x))
     .addConstraint(new ClLinearInequality(z,cnGEQ,y))
     .addConstraint(new ClLinearInequality(z,cnLEQ,4.0));

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
multiedit()
{
 try
   {
   bool fOkResult = true;

   ClVariable x("x",0);
   ClVariable y("y",0);
   ClVariable w("w",0);
   ClVariable h("h",0);
   ClSimplexSolver solver;

   solver
     .addStay(x)
     .addStay(y)
     .addStay(w)
     .addStay(h);

   solver
     .addEditVar(x)
     .addEditVar(y)
     .beginEdit();

   solver
     .suggestValue(x,10)
     .suggestValue(y,20)
     .resolve();

   cout << "x = " << x.value() << "; y = " << y.value() << endl
        << "w = " << w.value() << "; h = " << h.value() << endl;

   fOkResult = fOkResult &&
     clApprox(x,10) && clApprox(y,20) && clApprox(w,0) && clApprox(h,0);

   solver
     .addEditVar(w)
     .addEditVar(h)
     .beginEdit();

   solver
     .suggestValue(w,30)
     .suggestValue(h,40)
     .endEdit();

   cout << "x = " << x.value() << "; y = " << y.value() << endl
        << "w = " << w.value() << "; h = " << h.value() << endl;

   fOkResult = fOkResult &&
     clApprox(x,10) && clApprox(y,20) && clApprox(w,30) && clApprox(h,40);

   solver
     .suggestValue(x,50)
     .suggestValue(y,60)
     .endEdit();

   cout << "x = " << x.value() << "; y = " << y.value() << endl
        << "w = " << w.value() << "; h = " << h.value() << endl;

   fOkResult = fOkResult &&
     clApprox(x,50) && clApprox(y,60) && clApprox(w,30) && clApprox(h,40);

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
 cerr << "Should have gotten an exception!" << endl;
 return false;
}


// From a bug report from Steve Wolfman on his
// SAT project using "blackbox"
bool
blackboxsat()
{
  try
    {
    ClSimplexSolver solver;

    ClVariable r1("r1");
    ClVariable r2("r2");
    ClVariable r3("r3");
    ClVariable r4("r4");
    ClVariable r5("r5");
    ClVariable r6("r6");
    ClVariable r7("r7");
    ClVariable r8("r8");

    ClConstraint *rgpcn[30];
    for (int i=0; i<int(sizeof(rgpcn)/sizeof(rgpcn[0])); ++i)
      rgpcn[i] = NULL;

    rgpcn[1] = new ClLinearEquation(r1,60);
    rgpcn[2] = new ClLinearEquation(r2,30);
    rgpcn[12] = new ClLinearEquation(r3,2.5);
    rgpcn[13] = new ClLinearEquation(r6,0);
    rgpcn[14] = new ClLinearInequality(r5, cnGEQ, 0);
    rgpcn[15] = new ClLinearInequality(r8, cnLEQ, 2.5);
    rgpcn[16] = new ClLinearInequality(r7, cnGEQ, r6);
    rgpcn[17] = new ClLinearInequality(r8, cnGEQ, r7);
    rgpcn[18] = new ClLinearEquation(r4, r3 - r2/60.0);
    rgpcn[19] = new ClLinearEquation(r5, r4 - r1/60.0);
    rgpcn[20] = new ClLinearInequality(r4, cnGEQ, 0);
    rgpcn[21] = new ClLinearInequality(r5, cnGEQ, 0);
    rgpcn[22] = new ClLinearEquation(r7, r6 + r2/20.0);
    rgpcn[23] = new ClLinearEquation(r8, r7 + r1/20.0);
    rgpcn[24] = new ClLinearEquation(r4, r3 - r2/30.0);
    rgpcn[25] = new ClLinearEquation(r5, r4 - r1/30.0);
    rgpcn[26] = new ClLinearInequality(r4, cnGEQ, 0);
    rgpcn[27] = new ClLinearInequality(r5, cnGEQ, 0);
    rgpcn[28] = new ClLinearEquation(r7, r6 + r2/60.0);
    rgpcn[29] = new ClLinearEquation(r8, r7 + r1/60.0);

    while (true)
      {
      char szCmd[1000];
      int i;
      cin >> szCmd;
      if (!cin)
        break;
      if (szCmd[0] == '#')
        { 
        cin.getline(szCmd,900);
        continue;
        }
      if (strcasecmp(szCmd,"add") == 0)
        {
        cin >> i;
        cout << "eq" << i << ": " << solver.addConstraintNoException(rgpcn[i])
             << "\t" << *(rgpcn[i]) << endl;
        cout << r1 << " = " << r1.value() << endl;
        }
      else if (strcasecmp(szCmd,"del") == 0)
        {
        cin >> i;
        cout << "REMeq" << i << ": " << solver.removeConstraintNoException(rgpcn[i])
             << "\t" << *(rgpcn[i]) << endl;
        cout << r1 << " = " << r1.value() << endl;
        }
      else if (strcasecmp(szCmd,"dump") == 0)
        {
        cout << solver << endl;
        }
      else if (strcasecmp(szCmd,"val") == 0)
        {
        cout << r1 << " = " << r1.value() << endl;
        }
      else if (strcasecmp(szCmd,"solve") == 0)
        {
        cout << solver.solve() << endl;
        }
      else if (strcasecmp(szCmd,"autosolve") == 0)
        {
        solver.setAutosolve(true);
        }
      else if (strcasecmp(szCmd,"noautosolve") == 0)
        {
        solver.setAutosolve(true);
        }
      }

    cout << r1 << " = " << r1.value() << endl
         << r2 << " = " << r2.value() << endl
         << r3 << " = " << r3.value() << endl
         << r4 << " = " << r4.value() << endl
         << r5 << " = " << r5.value() << endl
         << r6 << " = " << r6.value() << endl
         << r7 << " = " << r7.value() << endl
         << r8 << " = " << r8.value() << endl;

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

typedef ClVariable *PClVariable;

bool
addDel(const int nCns = 900, const int nVars = 900, const int nResolves = 10000)
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
#ifdef CL_SHOW_CNS_IN_BENCHMARK
    cout << "Cn[" << j << "]: " << *rgpcns[j] << endl;
#endif
    }

  cout << "done building data structures" << endl;
  cout << "time = " << timer.ElapsedTime() << "\n" << endl;
  timer.Start();
  int cExceptions = 0;
#ifdef CL_SHOW_CNS_IN_BENCHMARK
  cout << "Exceptions on: ";
#endif
  for (j = 0; j < nCns; j++)
    {
    // add the constraint -- if it's incompatible, just ignore it
    try
      {
      solver.addConstraint(rgpcns[j]);
      }
    catch (ExCLRequiredFailure &)
      {
      cExceptions++;
      rgpcns[j] = NULL;
#ifdef CL_SHOW_CNS_IN_BENCHMARK
      cout << j << " ";
#endif
      }
    }
#ifdef CL_SHOW_CNS_IN_BENCHMARK
  cout << "\n" << endl;
#endif
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
      solver.removeConstraint(rgpcns[j]);
      }
    }

  // FIXGJB end = Timer.now();
  // cout << "Total remove time: " 
  //      << "remove time per cn"
  cout << "done removing constraints and addDel timing test" << endl;
  cout << "time = " << timer.ElapsedTime() << "\n" <<endl;
  cout << "time per cn = " << timer.ElapsedTime()/nCns << "\n" <<endl;
  cout << "time per actual cn = " << timer.ElapsedTime()/(nCns - cExceptions) << "\n" <<endl;

  for (int i = 0; i < nVars; i++)
    {
    delete rgpclv[i];
    }

  for (int j = 0; j < nCns; j++)
    {
    delete rgpcns[j];
    }

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

    RUN_TEST(simple1);
    RUN_TEST(simple2);
    RUN_TEST(justStay1);
    RUN_TEST(addDelete1);
    RUN_TEST(addDelete2);
    RUN_TEST(casso1);
    RUN_TEST(inconsistent1);
    RUN_TEST(inconsistent2);
    RUN_TEST(inconsistent3);
    RUN_TEST(multiedit);
    // RUN_TEST(blackboxsat);

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

#ifdef CL_FIND_LEAK
    cout << "ClAbstractVariables: " << ClAbstractVariable::cAbstractVariables
         << "\nClDummyVariables: " << ClDummyVariable::cDummyVariables
         << "\nClSlackVariables: " << ClSlackVariable::cSlackVariables
         << endl;
#endif

    
    return (fAllOkResult? 0 : 255);
    
    } 
  catch (...) 
    {
    cerr << "exception!" << endl;
    }
}
