// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998 Alan Borning and Greg Badros
// See ../COPYRIGHT for legal details regarding this software
// 
// ClParseTest.cc


#include "Cl.h"
#include "creader.h"
#include <strstream>

int main()
{
  StringToVarMap mapVars;
  ClSimplexSolver solver;

  ClVariable x("x");
  ClVariable y("y");
  ClVariable z("z");

  mapVars[x.name()] = &x;
  mapVars[y.name()] = &y;
  mapVars[z.name()] = &z;

  ClConstraint *pcn;
  string szLine;
  while (getline(cin,szLine))
    {
    istrstream xiLine(szLine.c_str());
    try {
      if ((pcn = parseConstraint(xiLine,mapVars)) != NULL) {
        solver.addConstraint(*pcn);
      }
    }
    catch (const char *szError) { }
    catch (...) { }
    }

  cout << solver << endl;
  cout << "x = " << x << "\n"
       << "y = " << y << "\n"
       << "z = " << z << endl;

  return 0;
}
