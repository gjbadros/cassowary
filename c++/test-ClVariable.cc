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
// test-ClVariable.cc

#include "ClVariable.h"

int
main( char **, int )
{
  ClVariable clv1("clv1");
  ClVariable clv2("clv2",2.4);
  cout << clv1 << endl;
  cout << clv2 << endl;

  return 0;
}
