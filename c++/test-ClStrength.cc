// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998, 1999 Greg J. Badros and Alan Borning
// See ../LICENSE for legal details regarding this software
//
// test-ClStrength.cc

#include <string>
#include "Cl.h"

int
main( char **, int  )
{
  ClSymbolicWeight clsw(0.0,1.0,0.0);
  ClSymbolicWeight clsw2(2.0,0.5,0.5);
  ClSymbolicWeight clsw3(2.0,0.5,0.5);
  ClSymbolicWeight clsw4(2.0,0.4,0.5);

  cout << clsw2.AsDouble() << endl;

  ClStrength cls(string("cls"),clsw);
  cout << cls << endl;

  ClStrength cls2(string("cls2"),clsw.Times(2));
  cout << cls2 << endl;

  ClStrength cls3(string("cls3"),clsw.Add(clsw2));
  cout << cls3 << endl;

  ClStrength cls4(string("cls4"),clsw.Subtract(clsw2));
  cout << cls4 << endl;

  ClStrength cls5(string("cls5"),clsw.DivideBy(2));
  cout << cls5 << endl;

  ClStrength a(ClsRequired());
  cout << "a is " << a.IsRequired() << endl;

  cout << clsw << (clsw == clsw2? "==" : "!=") << clsw2 << endl;
  cout << clsw2 << (clsw2 == clsw3? "==" : "!=") << clsw3 << endl;
  cout << clsw3 << (clsw3 == clsw4? "==" : "!=") << clsw4 << endl;

  cout << clsw << (clsw < clsw2? "<" : "!<") << clsw2 << endl;
  cout << clsw2 << (clsw2 < clsw3? "<" : "!<") << clsw3 << endl;
  cout << clsw3 << (clsw3 < clsw4? "<" : "!<") << clsw4 << endl;

  cout << clsw << (clsw > clsw2? ">" : "!>") << clsw2 << endl;
  cout << clsw2 << (clsw2 > clsw3? ">" : "!>") << clsw3 << endl;
  cout << clsw3 << (clsw3 > clsw4? ">" : "!>") << clsw4 << endl;

  return 0;
}
