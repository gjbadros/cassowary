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
// ClConstraint.cc

#include "ClConstraint.h"

ostream &
ClConstraint::printOn(ostream &xo) const 
{
  // Note that the trailing "= 0)" or ">= 0)" is missing, as derived classes will
  // print the right thing after calling this function
  xo << strength() << " {" << weight() << "} (" << expression();
  return xo;
}
