// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998, 1999 Alan Borning and Greg Badros
// See ../LICENSE for legal details regarding this software
//
// ClConstraint.cc

#include "ClConstraint.h"

#ifdef HAVE_CONFIG_H
#include <cassowary/config.h>
#define CONFIG_H_INCLUDED
#endif

#ifndef CL_NO_IO

ostream &
ClConstraint::PrintOn(ostream &xo) const 
{
  // Note that the trailing "= 0)" or ">= 0)" is missing, as derived classes will
  // print the right thing after calling this function
  xo << strength() << " {" << weight() << "} [" 
     << _times_added << "] (" << Expression();
  return xo;
}

#endif
