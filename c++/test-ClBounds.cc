// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998, 1999 Greg J. Badros and Alan Borning
// See ../LICENSE for legal details regarding this software
//
// test-ClBounds.cc

#include "Cl.h"
#include <stdlib.h>

int
main( char **, int  )
{
  try 
    {
    ClSimplexSolver s;
    ClVariable x(1);
    s.AddLowerBound(x,0);
    s.AddUpperBound(x,10);
    } 
  catch (ExCLError &err)
    {
    cerr << err.description() << endl;
    }
  catch (...) 
    {
    cerr << "Random exception!" << endl;
    }
}
