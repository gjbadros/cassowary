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
// ClSymbolicWeight.cc

#include "Cassowary.h"
#include "ClSymbolicWeight.h"

// Help g++ out, tell it to instantiate this
//template vector<double> &vector<double>::operator =(const vector<double> &);

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

ClSymbolicWeight::ClSymbolicWeight(const vector<double> &weights) :
  my_values(weights)
{ }

ClSymbolicWeight &
ClSymbolicWeight::zero()
{
  static ClSymbolicWeight Zero(0.0, 0.0, 0.0);
  return Zero;
}


ClSymbolicWeight &
ClSymbolicWeight::multiplyMe(Number n)
{
  vector<double>::iterator it = my_values.begin();
  for (; it != my_values.end(); ++it)
    {
    *it *= n;
    }
  return *this;
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

ClSymbolicWeight &
ClSymbolicWeight::addtoMe(const ClSymbolicWeight &cl)
{
  assert(cl.cLevels() == cLevels());

  vector<double>::iterator i1 = my_values.begin();
  vector<double>::const_iterator i2 = cl.my_values.begin();
  for (; i1 != my_values.end(); ++i1, ++i2)
    {
    *i1 += *i2;
    }
  return *this;
}

ClSymbolicWeight 
ClSymbolicWeight::subtract(const ClSymbolicWeight &cl) const
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
