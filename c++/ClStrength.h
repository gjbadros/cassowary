// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998, All Rights Reserved.
//
// ClStrength.h

#ifndef ClStrength_H
#define ClStrength_H

#include "Cassowary.h"
#include "ClSymbolicWeight.h"

class ClStrength;

ClStrength &clsRequired();
ClStrength &clsStrong();
ClStrength &clsMedium();
ClStrength &clsWeak();

class ClStrength {
 public:

  ClStrength(const String &name, const ClSymbolicWeight &symbolicWeight) :
    my_name(name), my_symbolicWeight(symbolicWeight)
    { }

  // special case for when nLevels = 3, should assert nLevels() == 3
  ClStrength(const String &name, double w1, double w2, double w3);

  virtual ~ClStrength()
    { }

  virtual bool isRequired() const
    { return (my_symbolicWeight == clsRequired().my_symbolicWeight); }

  virtual ostream &printOn(ostream &xo) const
    { xo << name() << ":" << symbolicWeight(); return xo; }

  virtual ClSymbolicWeight symbolicWeight() const
    { return my_symbolicWeight; }

 private:
  String name() const
    { return my_name; }

  void set_name(String name)
    { my_name = name; }

  void set_symbolicWeight(const ClSymbolicWeight &symbolicWeight)
    { my_symbolicWeight = symbolicWeight; }

  friend ostream& operator<<(ostream &xos, const ClStrength &cls)
    { cls.printOn(xos); return xos; }

  // instance variables
  String my_name;
  ClSymbolicWeight my_symbolicWeight;
  

};

#endif
