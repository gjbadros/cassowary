// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998, 1999 Alan Borning and Greg Badros
// See ../LICENSE for legal details regarding this software
//
// ClFloatVariable.cc

#include "ClFloatVariable.h"

ostream &ClFloatVariable::printOn(ostream &xo) const
{  
  xo << "[" << name() << ":" << _value << "]";
  return xo;
}

void ClFloatVariable::setName(string const &name)
{ 
  super::setName(name); 
#ifndef CL_NO_IO
  cerr << "Not updating symbol table!" << endl;
#endif
}

