// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998, All Rights Reserved.
//
// ClSymbolicWeight.cc

#include "Cassowary.h"
#include "ClSymbolicWeight.h"

// Help g++ out, tell it to instantiate this
template vector<double> &vector<double>::operator =(const vector<double> &);

ClSymbolicWeight::ClSymbolicWeight(int cLevels)
{
  // FIXGJB
}

ClSymbolicWeight::ClSymbolicWeight(double w1, double w2, double w3)
{
  my_values.push_back(w1);
  my_values.push_back(w2);
  my_values.push_back(w3);
}


ClSymbolicWeight &
ClSymbolicWeight::zero()
{
  static ClSymbolicWeight Zero;
  return Zero;
}


ClSymbolicWeight 
ClSymbolicWeight::times(Number n) const
{
  
}

ClSymbolicWeight 
ClSymbolicWeight::plus(const ClSymbolicWeight &cl) const
{
  
}

ClSymbolicWeight 
ClSymbolicWeight::minus(const ClSymbolicWeight &cl) const
{
  
}


ClSymbolicWeight 
ClSymbolicWeight::divideBy(Number n) const
{
  
}
