// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998 Alan Borning and Greg Badros
// See ../COPYRIGHT for legal details regarding this software
// 
// ClSubgraphTest.cc

#include "Cl.h"
#include <stdlib.h>
#include "timer.h"
#include <iostream>
#include <iomanip>

inline 
double UniformRandom()
{ return double(rand())/RAND_MAX; }


bool
addDelSubpart(int csolver = 1, int nCns = 900, int nVars = 900, int nResolves = 10000)
{
  Timer timer;
  // FIXGJB: from where did .12 come?
  static const double ineqProb = 0.12;
  static const int maxVars = 3;

  cout << "starting timing test. nCns = " << nCns
       << ", nVars = " << nVars << ", nResolves = " << nResolves << endl;

  timer.Start();

  ClVariable **rgpclvA = new PClVariable[nVars/2];
  for (int i = 0; i < nVars/2; i++)
    rgpclvA[i] = new ClVariable(i,"a");

  ClVariable **rgpclvB = new PClVariable[nVars/2];
  for (int i = 0; i < nVars/2; i++)
    rgpclvB[i] = new ClVariable(i,"b");


  ClConstraint **rgpcnsA = new PClConstraint[nCns/2];
    { /* scope */
    int nvs = 0;
    int k;
    int j;
    double coeff;
    for (j = 0; j < nCns/2; j++)
      {
      // number of variables in this constraint
      nvs = int(UniformRandom()*maxVars) + 1;
      ClLinearExpression expr = UniformRandom()*20.0 - 10.0;
      for (k = 0; k < nvs; k++)
        {
        coeff = UniformRandom()*10 - 5;
        int index = int(UniformRandom()*(nVars/2));
        assert(index < (nVars/2));
        expr.addExpression(*(rgpclvA[index]) * coeff);
        }
      if (UniformRandom() < ineqProb)
        {
        rgpcnsA[j] = new ClLinearInequality(expr);
        }
      else
        {  
        rgpcnsA[j] = new ClLinearEquation(expr);
        }
      }
    }

  ClConstraint **rgpcnsB = new PClConstraint[nCns/2];
    { /* scope */
    int nvs = 0;
    int k;
    int j;
    double coeff;
    for (j = 0; j < nCns/2; j++)
      {
      // number of variables in this constraint
      nvs = int(UniformRandom()*maxVars) + 1;
      ClLinearExpression expr = UniformRandom()*20.0 - 10.0;
      for (k = 0; k < nvs; k++)
        {
        coeff = UniformRandom()*10 - 5;
        int index = int(UniformRandom()*(nVars/2));
        assert(index < (nVars/2));
        expr.addExpression(*(rgpclvB[index]) * coeff);
        }
      if (UniformRandom() < ineqProb)
        {
        rgpcnsB[j] = new ClLinearInequality(expr);
        }
      else
        {  
        rgpcnsB[j] = new ClLinearEquation(expr);
        }
      }
    }

  cout << "done building data structures" << endl;
  cout << "time = " << timer.ElapsedTime() << "\n" << endl;
  timer.Start();

  ClSimplexSolver *psolverA = new ClSimplexSolver();
  ClSimplexSolver *psolverB = psolverA;
  if (csolver == 2)
    psolverB = new ClSimplexSolver();

  for (int i = 0; i < nVars/2; ++i) 
    psolverA->addStay(*rgpclvA[i]);

  for (int i = 0; i < nVars/2; ++i)
    psolverB->addStay(*rgpclvB[i]);

  cout << "done adding stays" << endl;
  cout << "time = " << timer.ElapsedTime() << "\n" << endl;

  psolverA->setAutosolve(false);
  psolverB->setAutosolve(false);
  timer.Start();
  int cExceptions = 0;
  for (int j = 0; j < nCns/2; j++)
    {
    // add the constraint -- if it's incompatible, just ignore it
    try
      {
      psolverA->addConstraint(*(rgpcnsA[j]));
      }
    catch (ExCLRequiredFailure &)
      {
      cExceptions++;
      //      rgpcnsA[j] = NULL;
      }
    }

  for (int j = 0; j < nCns/2; j++)
    {
    // add the constraint -- if it's incompatible, just ignore it
    try
      {
      psolverB->addConstraint(*(rgpcnsB[j]));
      }
    catch (ExCLRequiredFailure &)
      {
      cExceptions++;
      //      rgpcnsB[j] = NULL;
      }
    }

  cout << "done adding constraints [" << cExceptions << " exceptions]" << endl;
  cout << "time = " << timer.ElapsedTime() << "\n" << endl;
  psolverA->solve();
  if (psolverA != psolverB)
    psolverB->solve();
  cout << "done initial solve" << endl;
  cout << "time = " << timer.ElapsedTime() << "\n" << endl;
  cout << "time per cn = " << timer.ElapsedTime()/nCns << "\n" << endl;
  cout << "time per actual cn = " << timer.ElapsedTime()/(nCns - cExceptions) << "\n" <<endl;
  timer.Start();

  cout << "done creating edit constraints -- about to start resolves" << endl;
  cout << "time = " << timer.ElapsedTime() << "\n" << endl;

  timer.Start();


  int e1Index = int(UniformRandom()*(nVars/2));
  int e2Index = int(UniformRandom()*(nVars/2));

  ClEditConstraint editA1(*(rgpclvA[e1Index]),clsStrong());
  ClEditConstraint editA2(*(rgpclvA[e2Index]),clsStrong());

  (*psolverA)
    .addConstraint(editA1)
    .addConstraint(editA2);

  // FIXGJB start = Timer.now();
  for (int m = 0; m < nResolves/2; m++)
    {
    vector<Number> vals;
    vals.push_back(rgpclvA[e1Index]->value() * 1.001);
    vals.push_back(rgpclvA[e2Index]->value() * 1.001);
    psolverA->resolve(vals);
    }
  psolverA->removeConstraint(editA1);
  psolverA->removeConstraint(editA2);


  e1Index = int(UniformRandom()*(nVars/2));
  e2Index = int(UniformRandom()*(nVars/2));

  ClEditConstraint editB1(*(rgpclvB[e1Index]),clsStrong());
  ClEditConstraint editB2(*(rgpclvB[e2Index]),clsStrong());

  (*psolverB)
    .addConstraint(editB1)
    .addConstraint(editB2);

  for (int m = 0; m < nResolves/2; m++)
    {
    vector<Number> vals;
    vals.push_back(rgpclvA[e1Index]->value() * 1.001);
    vals.push_back(rgpclvA[e2Index]->value() * 1.001);
    psolverB->resolve(vals);
    }
  psolverB->removeConstraint(editB1);
  psolverB->removeConstraint(editB2);

  cout << "done resolves" << endl;
  cout << "time = " << timer.ElapsedTime() << "\n" <<endl;
  cout << "time per resolve = " << timer.ElapsedTime()/nResolves << "\n" <<endl;

  cout << "done addDel timing test" << endl;
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

    int cns = 900, vars = 900, resolves = 10000;

    if (argc > 1)
      cns = atoi(argv[1]);

    if (argc > 2)
      vars = atoi(argv[2]);

    if (argc > 3)
      resolves = atoi(argv[3]);

    if (cns > 0) 
      {
      cout << "addDelSubpart1" << ":" << endl;
      fResult = addDelSubpart(1,cns,vars,resolves); fAllOkResult &= fResult;
      cout << "addDelSubpart2" << ":" << endl;
      fResult = addDelSubpart(2,cns,vars,resolves); fAllOkResult &= fResult;
      if (!fResult) cout << "Failed!" << endl;
      }
      
    return (fAllOkResult? 0 : 255);
    
    } 
  catch (...) 
    {
    cerr << "exception!" << endl;
    }
}
