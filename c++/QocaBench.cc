// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998, 1999 Alan Borning and Greg Badros
// See ../LICENSE for legal details regarding this software
// 
// QocaBench.cc

#include "Cl.h"
#include <stdlib.h>
#include "timer.h"
#include <iostream>
#include <fstream>
#include <iomanip>


int
main( int argc, char **argv )
{
  try 
    {
    // seed the random number generator for reproducible results
    srand(123456789);

    cout << "Cassowary version: " << szCassowaryVersion << endl;

    Timer timer;
    
    timer.Start();
    ClSimplexSolver solver;
    solver.setAutosolve(false);
    StringToVarMap mapVars;
    ClVarLookupInMap LookupVar(&mapVars,true);
    map<long,ClConstraint *> mapIdToPcn;

    if (argc != 2) {
      cerr << "Usage: " << argv[0] << " [filename.qsc]" << endl;
      exit(-1);
    }
    fstream xi(argv[1],ios::in);
    string strLine;
    while (getline(xi,strLine)) {
      size_t ichSpace = strLine.find_first_of(" \t");
      string strCmd = strLine.substr(0,ichSpace);
      string strArgs = strLine.substr(ichSpace+1);
      if ("qoca_addconstraint" == strCmd) {
        cerr << "addcn" << strArgs << endl;
      }
    }
    
    cout << "done building data structures" << endl;
    cout << "time = " << timer.ElapsedTime() << "\n" << endl;
    timer.Start();

    fstream xi2(argv[1],ios::in);
    while (getline(xi2,strLine)) {
      size_t ichSpace = strLine.find_first_of(" \t");
      string strCmd = strLine.substr(0,ichSpace);
      string strArgs = strLine.substr(ichSpace+1);
      if ("qoca_solve" == strCmd) {
        cerr << "solve" << endl;
        solver.solve();
      } else if ("qoca_resolve" == strCmd) {
        cerr << "resolve" << endl;
        solver.resolve();
      } else if ("qoca_suggestvalue" == strCmd) {
        char szVarname[50];
        double n;
        sscanf(strArgs.c_str(),"%s %lg",szVarname,&n);
        cerr << "suggestvalue of " << n << " for " << szVarname << endl;
      } else if ("qoca_addvar" == strCmd) {
        size_t ichColon = strArgs.find_first_of(":");
        string strVarname = strArgs.substr(ichColon+1);
        ClVariable clv = *LookupVar(strVarname);
        cerr << "addvar of " << clv << endl;
        solver.addStay(clv);
      } else if ("qoca_addconstraint" == strCmd) {
        size_t ichSpace = strArgs.find_first_of(" \t");
        string strConstraintId = strArgs.substr(0,ichSpace);
        long id = atol(strConstraintId.c_str()+1); // skip over open brace
        cerr << "addcn with id = " << id << endl;
      } else if ("qoca_removeconstraint" == strCmd) {
        string strConstraintId = strArgs;
        long id = atol(strConstraintId.c_str()+1); // skip over open brace
        cerr << "removecn with id = " << id << endl;
      } else if ("qoca_beginedit" == strCmd) {
        cerr << "beginedit" << endl;
        solver.beginEdit();
      } else if ("qoca_endedit" == strCmd) {
        cerr << "endedit" << endl;
        solver.endEdit();
      } else if ("qoca_removevar" == strCmd) {
        ClVariable clv = *LookupVar(strArgs);
        cerr << "removevar (noop) " << clv << endl;
      } else if ("qoca_seteditvar" == strCmd) {
        ClVariable clv = *LookupVar(strArgs);
        cerr << "seteditvar " << clv << endl;
        solver.addEditVar(clv);
      } else if ("qoca_stat" == strCmd) {
        cerr << "stat (noop)" << endl;
      }
    }
    
    cout << "all done with input log." 
         << "Time processing = " << timer.ElapsedTime() << endl;

  }
  catch (ExCLError &e) {
    cerr << "Exception: " << e.description() << endl;
  }
  catch (...) {
    cerr << "exception!" << endl;
  }
}
