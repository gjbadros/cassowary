// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998, 1999 Alan Borning and Greg Badros
// See ../LICENSE for legal details regarding this software
// 
// ClParseTest.cc


#include "Cl.h"
#include <strstream>

int main()
{
  StringToVarMap mapVars;
  ClSimplexSolver solver;

  ClVariable::SetVarMap(&mapVars);

  ClVariable x("x");
  ClVariable y("y");
  ClVariable z("z");

  ClConstraint *pcn;
  string szLine;

  while (getline(cin,szLine))
    {
    if (szLine == "-") {
      cerr << "Now renamed x to foo -- enter more constraints" << endl;
      x.SetName("foo");
      continue;
    }

    istrstream xiLine(szLine.c_str());
    try {
      if ((pcn = PcnParseConstraint(xiLine,ClVarLookupInMap(&mapVars,false)))
          != NULL) {
        solver.AddConstraint(*pcn);
      }
    }
    catch (const ExCLParseError &e) {
      cerr << e.description() <<endl;
    }
    catch (...) { }
    }

  cout << solver << endl;
  cout << "x (i.e., foo) = " << x << "\n"
       << "y             = " << y << "\n"
       << "z             = " << z << endl;

  return 0;
}
