// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998, All Rights Reserved.
//
// ClStrength.cc

#include "ClStrength.h"
#include <assert.h>
#include <values.h>

// Use the singleton pattern for the strength objects
ClStrength &clsRequired()
{
  // required is distinct by equality to this static object,
  // but I still use an especially high symbolic weight, just in case
  // FIXGJB: hack?
  static ClStrength required_strength("<Required>", 1000, 1000, 1000);
  return required_strength;
}

ClStrength &clsStrong()
{
  static ClStrength strong_strength("strong", 1.0, 0.0, 0.0);
  return strong_strength;
}

ClStrength &clsMedium()
{
  static ClStrength medium_strength("medium", 0.0, 1.0, 0.0);
  return medium_strength;
}


ClStrength &clsWeak()
{
  static ClStrength weak_strength("weak", 0.0, 0.0, 1.0);
  return weak_strength;
}

// special case for when nLevels = 3, should assert nLevels() == 3
ClStrength::ClStrength(const string &name, double w1, double w2, double w3) :
  my_name(name), my_symbolicWeight(w1, w2, w3)
{ 
}
