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
//#include "ClTimedSimplexSolver.h"
#include <stdlib.h>
#include "timer.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

#ifndef NO_RANDOMS_FROM_FILE
int iRandom = 0;
int cRandom = 0;
vector<double> vRandom;

void InitializeRandoms() {
  iRandom=0;
  ifstream in("randoms.txt");
  string s;
  // skip over comment
  getline(in,s);
  // skip over number of randoms
  getline(in,s);
  double f;
  while (in >> f) {
    vRandom.push_back(f);
    ++cRandom;
  }
}

inline 
double UniformRandom()
{ 
  if (iRandom >= cRandom) {
    throw string("Out of random numbers");
  }
  //  cerr << "returning value = " << vRandom[iRandom] << endl;
  return vRandom[iRandom++]; 
}


#else

inline 
double UniformRandom()
{ return double(rand())/RAND_MAX; }

#endif

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
   solver.AddStay(x);
   solver.AddStay(y);
   solver.AddConstraint(eq);
   cout << "x = " << x.Value() << endl
        << "y = " << y.Value() << endl;
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


/* Add an edit variable to an empty solver */
bool
simple2()
{
 try
   {
   ClVariable x(167);
   ClSimplexSolver solver;

   solver.AddEditVar(x);
   solver.BeginEdit();
   solver.SuggestValue(x,100);
   solver.EndEdit();

   cout << "x = " << x.Value() << endl;
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
   solver.AddPointStay(x,y,1);
#else
   solver.AddStay(x);
   solver.AddStay(y);
#endif
   fOkResult = fOkResult && ClApprox(x,5);
   fOkResult = fOkResult && ClApprox(y,10);
   cout << "x == " << x.Value() << endl;
   cout << "y == " << y.Value() << endl;

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

   solver.AddConstraint(new ClLinearEquation( x, 100, ClsWeak() ));
    
   ClLinearInequality c10(x,cnLEQ,10.0);
   ClLinearInequality c20(x,cnLEQ,20.0);
   solver
     .AddConstraint(c10)
     .AddConstraint(c20);

   fOkResult = fOkResult && ClApprox(x,10.0);
   cout << "x == " << x.Value() << endl;

   cout << endl << solver << endl;

   solver.RemoveConstraint(c10);

   cout << endl << solver << endl;

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
     .AddConstraint(new ClLinearEquation(x, 100.0, ClsWeak()))
     .AddConstraint(new ClLinearEquation(y, 120.0, ClsStrong()));

   ClLinearInequality c10(x,cnLEQ,10.0);
   ClLinearInequality c20(x,cnLEQ,20.0);

   solver
     .AddConstraint(c10)
     .AddConstraint(c20);
   fOkResult = fOkResult && ClApprox(x,10.0) && ClApprox(y,120.0);
   cout << "x == " << x.Value() << ", y == " << y.Value() << endl;

   solver.RemoveConstraint(c10);
   fOkResult = fOkResult && ClApprox(x,20.0) && ClApprox(y,120.0);
   cout << "x == " << x.Value() << ", y == " << y.Value() << endl;
   
   ClLinearEquation cxy( 2*x, y);
   solver.AddConstraint(cxy);
   fOkResult = fOkResult && ClApprox(x,20.0) && ClApprox(y,40.0);
   cout << "x == " << x.Value() << ", y == " << y.Value() << endl;

   solver.RemoveConstraint(c20);
   fOkResult = fOkResult && ClApprox(x,60.0) && ClApprox(y,120.0);
   cout << "x == " << x.Value() << ", y == " << y.Value() << endl;

   solver.RemoveConstraint(cxy);
   fOkResult = fOkResult && ClApprox(x,100.0) && ClApprox(y,120.0);
   cout << "x == " << x.Value() << ", y == " << y.Value() << endl;


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
     .AddConstraint(new ClLinearInequality(x,cnLEQ,y))
     .AddConstraint(new ClLinearEquation(y, x+3.0))
     .AddConstraint(new ClLinearEquation(x,10.0,ClsWeak()))
     .AddConstraint(new ClLinearEquation(y,10.0,ClsWeak()))
     ;
   
   fOkResult = fOkResult && 
     ( ClApprox(x,10.0) && ClApprox(y,13.0) ||
       ClApprox(x,7.0) && ClApprox(y,10.0) );
     
   cout << "x == " << x.Value() << ", y == " << y.Value() << endl;

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
    
    solver.AddConstraint( eq1 );
    solver.AddConstraint( eq2 );
    
    // no exception, we failed!
    return(false);
    } 
  catch (ExCLRequiredFailure)
    {
    // we want this exception to get thrown
    cout << "Success -- got the exception" << endl;
    // solver.RemoveConstraint(eq2); this would throw a constraint not found exception
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
     .AddConstraint(new ClLinearInequality(x,cnGEQ,10.0))
     .AddConstraint(new ClLinearInequality(x,cnLEQ, 5.0));

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
     .AddConstraint(new ClLinearInequality(w,cnGEQ,10.0))
     .AddConstraint(new ClLinearInequality(x,cnGEQ,w))
     .AddConstraint(new ClLinearInequality(y,cnGEQ,x))
     .AddConstraint(new ClLinearInequality(z,cnGEQ,y))
     .AddConstraint(new ClLinearInequality(z,cnLEQ,4.0));

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
     .AddStay(x)
     .AddStay(y)
     .AddStay(w)
     .AddStay(h);

   solver
     .AddEditVar(x)
     .AddEditVar(y)
     .BeginEdit();

   solver
     .SuggestValue(x,10)
     .SuggestValue(y,20)
     .Resolve();

   cout << "x = " << x.Value() << "; y = " << y.Value() << endl
        << "w = " << w.Value() << "; h = " << h.Value() << endl;

   fOkResult = fOkResult &&
     ClApprox(x,10) && ClApprox(y,20) && ClApprox(w,0) && ClApprox(h,0);

   solver
     .AddEditVar(w)
     .AddEditVar(h)
     .BeginEdit();

   solver
     .SuggestValue(w,30)
     .SuggestValue(h,40)
     .EndEdit();

   cout << "x = " << x.Value() << "; y = " << y.Value() << endl
        << "w = " << w.Value() << "; h = " << h.Value() << endl;

   fOkResult = fOkResult &&
     ClApprox(x,10) && ClApprox(y,20) && ClApprox(w,30) && ClApprox(h,40);

   solver
     .SuggestValue(x,50)
     .SuggestValue(y,60)
     .EndEdit();

   cout << "x = " << x.Value() << "; y = " << y.Value() << endl
        << "w = " << w.Value() << "; h = " << h.Value() << endl;

   fOkResult = fOkResult &&
     ClApprox(x,50) && ClApprox(y,60) && ClApprox(w,30) && ClApprox(h,40);

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


bool
multiedit2()
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
     .AddStay(x)
     .AddStay(y)
     .AddStay(w)
     .AddStay(h);

   solver
     .AddEditVar(x)
     .AddEditVar(y)
     .BeginEdit();

   solver
     .SuggestValue(x,10)
     .SuggestValue(y,20)
     .Resolve();

   cout << "x = " << x.Value() << "; y = " << y.Value() << endl
        << "w = " << w.Value() << "; h = " << h.Value() << endl;

   fOkResult = fOkResult &&
     ClApprox(x,10) && ClApprox(y,20) && ClApprox(w,0) && ClApprox(h,0);

   solver
     .AddEditVar(x)
     .AddEditVar(y)
     .AddEditVar(w)
     .AddEditVar(h)
     .BeginEdit();

   solver
     .SuggestValue(w,30)
     .SuggestValue(h,40)
     .EndEdit();

   cout << "x = " << x.Value() << "; y = " << y.Value() << endl
        << "w = " << w.Value() << "; h = " << h.Value() << endl;

   fOkResult = fOkResult &&
     ClApprox(x,10) && ClApprox(y,20) && ClApprox(w,30) && ClApprox(h,40);

   solver
     .SuggestValue(x,50)
     .SuggestValue(y,60)
     .EndEdit();

   cout << "x = " << x.Value() << "; y = " << y.Value() << endl
        << "w = " << w.Value() << "; h = " << h.Value() << endl;

   fOkResult = fOkResult &&
     ClApprox(x,50) && ClApprox(y,60) && ClApprox(w,30) && ClApprox(h,40);

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
      if (strcasecmp(szCmd,"Add") == 0)
        {
        cin >> i;
        cout << "eq" << i << ": " << solver.AddConstraintNoException(rgpcn[i])
             << "\t" << *(rgpcn[i]) << endl;
        cout << r1 << " = " << r1.Value() << endl;
        }
      else if (strcasecmp(szCmd,"del") == 0)
        {
        cin >> i;
        cout << "REMeq" << i << ": " << solver.RemoveConstraintNoException(rgpcn[i])
             << "\t" << *(rgpcn[i]) << endl;
        cout << r1 << " = " << r1.Value() << endl;
        }
      else if (strcasecmp(szCmd,"dump") == 0)
        {
        cout << solver << endl;
        }
      else if (strcasecmp(szCmd,"val") == 0)
        {
        cout << r1 << " = " << r1.Value() << endl;
        }
      else if (strcasecmp(szCmd,"solve") == 0)
        {
        cout << solver.Solve() << endl;
        }
      else if (strcasecmp(szCmd,"autosolve") == 0)
        {
        solver.SetAutosolve(true);
        }
      else if (strcasecmp(szCmd,"noautosolve") == 0)
        {
        solver.SetAutosolve(true);
        }
      }

    cout << r1 << " = " << r1.Value() << endl
         << r2 << " = " << r2.Value() << endl
         << r3 << " = " << r3.Value() << endl
         << r4 << " = " << r4.Value() << endl
         << r5 << " = " << r5.Value() << endl
         << r6 << " = " << r6.Value() << endl
         << r7 << " = " << r7.Value() << endl
         << r8 << " = " << r8.Value() << endl;

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

  InitializeRandoms();

  cout << "starting timing test. nCns = " << nCns
       << ", nVars = " << nVars << ", nResolves = " << nResolves << endl;

  timer.Start();
  ClSimplexSolver solver;
  solver.SetAutosolve(false);

  ClVariable **rgpclv = new PClVariable[nVars];
  for (int i = 0; i < nVars; i++)
    {
    rgpclv[i] = new ClVariable(i,"x");
    solver.AddStay(*rgpclv[i]);
    }

  int nCnsMade = nCns*2;

  ClConstraint **rgpcns = new PClConstraint[nCnsMade];
  ClConstraint **rgpcnsAdded = new PClConstraint[nCns];
  int nvs = 0;
  int k;
  int j;
  double coeff;
  for (j = 0; j < nCnsMade; ++j)
    {
    // number of variables in this constraint
    nvs = int(UniformRandom()*maxVars) + 1;
    //    cerr << "Using nvs = " << nvs << endl;
    ClLinearExpression expr = UniformRandom()*20.0 - 10.0;
    for (k = 0; k < nvs; k++)
       {
       coeff = UniformRandom()*10 - 5;
       expr.AddExpression(*(rgpclv[int(UniformRandom()*nVars)]) * coeff);
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
  timer.Stop();
  cout << "done building data structures" << endl;
  cout << "time = " << timer.ElapsedTime() << "\n" << endl;

  timer.Reset(); timer.Start();
  int cExceptions = 0;
#ifdef CL_SHOW_CNS_IN_BENCHMARK
  cout << "Exceptions on: ";
#endif
  int cCns = 0;
  for (j = 0; j < nCnsMade && cCns < nCns; ++j)
    {
    // Add the constraint -- if it's incompatible, just ignore it
    try
      {
      solver.AddConstraint(rgpcns[j]);
      // count the constraint as having been added
      rgpcnsAdded[cCns++] = rgpcns[j];
#ifdef CL_SHOW_CNS_IN_BENCHMARK
      cout << "added cn: " << *rgpcns[j] << endl;
#endif
      }
    catch (ExCLRequiredFailure &)
      {
      cExceptions++;
#ifdef CL_SHOW_CNS_IN_BENCHMARK
      cout << "could not add cn: " << *rgpcns[j] << endl;
#endif
      rgpcns[j] = NULL;
      }
    }
#ifdef CL_SHOW_CNS_IN_BENCHMARK
  cout << "\n" << endl;
#endif
  solver.Solve();
  timer.Stop();
  cout << "done adding " << cCns << " constraints ["
       << j << " attempted, "
       << cExceptions << " exceptions]" << endl;
  cout << "time = " << timer.ElapsedTime() << "\n" << endl;
  cout << "time per Add cn = " << timer.ElapsedTime()/cCns << "\n" <<endl;

  int e1Index = int(UniformRandom()*nVars);
  int e2Index = int(UniformRandom()*nVars);

  cout << "Editing vars with indices " << e1Index << ", " << e2Index << endl;
  
  ClVariable e1 = *(rgpclv[e1Index]);
  ClVariable e2 = *(rgpclv[e2Index]);

  cout << "about to start resolves" << endl;
  timer.Reset();
  timer.Start();

  solver
    .AddEditVar(e1)
    .AddEditVar(e2);

  solver.BeginEdit();
  for (int m = 0; m < nResolves; ++m)
    {
    solver
      .SuggestValue(e1,e1->Value()*1.001)
      .SuggestValue(e2,e2->Value()*1.001)
      .Resolve();
    }
  solver.EndEdit();

  timer.Stop();

#if 0
  cout << "nResolves = " << solver._cResolve
       << " over " << solver.GetResolveTimer().ElapsedTime()
       << endl;
#endif

  cout << "done resolves -- now removing constraints" << endl;
  cout << "time = " << timer.ElapsedTime() << "\n" <<endl;
  cout << "time per Resolve = " << timer.ElapsedTime()/nResolves << "\n" <<endl;

  timer.Reset();
  timer.Start();

  for (j = 0; j < cCns; j++)
    {
      solver.RemoveConstraint(rgpcnsAdded[j]);
    }
  solver.Solve();
  timer.Stop();

  // FIXGJB end = Timer.now();
  // cout << "Total remove time: " 
  //      << "remove time per cn"
  cout << "done removing constraints and addDel timing test" << endl;
  cout << "time = " << timer.ElapsedTime() << "\n" <<endl;
  cout << "time per Remove cn = " << timer.ElapsedTime()/cCns << "\n" <<endl;

  for (int i = 0; i < nVars; i++)
    {
    delete rgpclv[i];
    }

  for (int j = 0; j < nCnsMade; j++)
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

    cout << "Cassowary version: " << szCassowaryVersion << endl;

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
    RUN_TEST(multiedit2);
    // RUN_TEST(blackboxsat);

    int cns = 90, vars = 90, resolves = 100;

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
  catch (ExCLError &e) 
    {
      cerr << "exception:" << e.description() << endl;
    }
}
