// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998, All Rights Reserved.
//
// (c) 1998 Alan Borning and Greg Badros.  This code is provided for use by
// students for course projects in the course CSE 595 in the Department of
// Computer Science and Engineering, University of Washington, during winter
// quarter 1998.  Any other use requires written permission from the copyright
// holders.
//
// test-ClStrength.cc

#include <string>
#include "Cassowary.h"
#include "ClStrength.h"
#include "debug.h"

int
main( char **, int  )
{
  ClSymbolicWeight clsw(0.0,1.0,0.0);
  ClSymbolicWeight clsw2(2.0,0.5,0.5);
  ClSymbolicWeight clsw3(2.0,0.5,0.5);
  ClSymbolicWeight clsw4(2.0,0.4,0.5);

  cout << clsw2.asDouble() << endl;

  ClStrength cls(string("cls"),clsw);
  cout << cls << endl;

  ClStrength cls2(string("cls2"),clsw.times(2));
  cout << cls2 << endl;

  ClStrength cls3(string("cls3"),clsw.add(clsw2));
  cout << cls3 << endl;

  ClStrength cls4(string("cls4"),clsw.subtract(clsw2));
  cout << cls4 << endl;

  ClStrength cls5(string("cls5"),clsw.divideBy(2));
  cout << cls5 << endl;

  ClStrength a(clsRequired());
  cout << "a is " << a.isRequired() << endl;

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
