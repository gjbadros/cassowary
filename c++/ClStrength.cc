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

// Use the singleton pattern for the strength objects
ClStrength &clsRequired()
{
  static ClStrength required_strength("required", 1.0, 0.0, 0.0);
  return required_strength;
}

ClStrength &clsStrong()
{
  static ClStrength strong_strength("required", 0.0, 1.0, 0.0);
  return strong_strength;
}

ClStrength &clsWeak()
{
  static ClStrength weak_strength("required", 0.0, 0.0, 1.0);
  return weak_strength;
}

// special case for when nLevels = 3, should assert nLevels() == 3
ClStrength::ClStrength(const String &name, double w1, double w2, double w3) :
  my_name(name), my_symbolicWeight(w1, w2, w3)
{ 
  // just to be sure that this special case applies
  assert(nLevels() == 3);
}
