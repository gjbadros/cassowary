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
#include <assert.h>

// Help g++ out, tell it to instantiate this
template vector<double> &vector<double>::operator =(const vector<double> &);

ClSymbolicWeight::ClSymbolicWeight(int cLevels, double value) :
  my_values(cLevels, value)
{ }

ClSymbolicWeight::ClSymbolicWeight(double w1, double w2, double w3)
{
  my_values.push_back(w1);
  my_values.push_back(w2);
  my_values.push_back(w3);
  assert(my_values.size() == 3);
}

ClSymbolicWeight &
ClSymbolicWeight::zero()
{
  static ClSymbolicWeight Zero(0.0, 0.0, 0.0);
  return Zero;
}


ClSymbolicWeight 
ClSymbolicWeight::times(Number n) const
{
  ClSymbolicWeight clsw(0);
  vector<double>::const_iterator i = my_values.begin();
  for (; i != my_values.end(); ++i)
    {
    clsw.push_back(*i * n);
    }
  return clsw;
}


ClSymbolicWeight 
ClSymbolicWeight::divideBy(Number n) const
{
  assert(n!=0);
  ClSymbolicWeight clsw(0);
  vector<double>::const_iterator i = my_values.begin();
  for (; i != my_values.end(); ++i)
    {
    clsw.push_back(*i / n);
    }
  return clsw;
}

ClSymbolicWeight 
ClSymbolicWeight::plus(const ClSymbolicWeight &cl) const
{
  assert(cl.cLevels() == cLevels());

  ClSymbolicWeight clsw(0);
  vector<double>::const_iterator i1 = my_values.begin();
  vector<double>::const_iterator i2 = cl.my_values.begin();
  for (; i1 != my_values.end(); ++i1, ++i2)
    {
    clsw.push_back(*i1 + *i2);
    }
  return clsw;
}

ClSymbolicWeight 
ClSymbolicWeight::minus(const ClSymbolicWeight &cl) const
{
  assert(cl.cLevels() == cLevels());

  ClSymbolicWeight clsw(0);
  vector<double>::const_iterator i1 = my_values.begin();
  vector<double>::const_iterator i2 = cl.my_values.begin();
  for (; i1 != my_values.end(); ++i1, ++i2)
    {
    clsw.push_back(*i1 - *i2);
    }
  return clsw;
}


bool 
ClSymbolicWeight::lessThan(const ClSymbolicWeight &cl) const
{
  return my_values < cl.my_values;
}

bool 
ClSymbolicWeight::lessThanOrEqual(const ClSymbolicWeight &cl) const
{
  return my_values <= cl.my_values;
}

bool 
ClSymbolicWeight::equal(const ClSymbolicWeight &cl) const
{
  return my_values == cl.my_values;
}

bool 
ClSymbolicWeight::greaterThan(const ClSymbolicWeight &cl) const
{
  return my_values > cl.my_values;
}

bool 
ClSymbolicWeight::greaterThanOrEqual(const ClSymbolicWeight &cl) const
{
  return my_values >= cl.my_values;
}

bool 
ClSymbolicWeight::isNegative() const
{
  return my_values < zero().my_values;
}
