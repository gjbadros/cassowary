// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998, 1999 Greg J. Badros and Alan Borning
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
        cerr << "Got constraint: " << *pcn << "... ";
        if (solver.AddConstraintNoException(*pcn))
          cerr << "Added!";
        else {
          // could be inconsistent req'd constraints,
          // or could be a strict inequality.
          cerr << "Inconsistent or not permitted!";
        }
        cerr << endl;
        try {
          ClLinearConstraint *pcnLin = dynamic_cast<ClLinearConstraint *>(pcn);
          ClFDBinaryOneWayConstraint cnfd(*pcnLin);
          cerr << "as an fd constraint: " << cnfd << endl;
        } catch (const ExCLError &e) {
          cerr << e.description() << endl;
        }
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
