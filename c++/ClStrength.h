// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998 Alan Borning and Greg Badros
// See ../COPYRIGHT for legal details regarding this software
//
// ClStrength.h

#ifndef ClStrength_H
#define ClStrength_H

#include "Cassowary.h"
#include "ClSymbolicWeight.h"

class ClStrength;

const ClStrength &clsRequired();
const ClStrength &clsStrong();
const ClStrength &clsMedium();
const ClStrength &clsWeak();

class ClStrength {
 public:

  ClStrength(const string &name, const ClSymbolicWeight &symbolicWeight) :
    _name(name), _symbolicWeight(symbolicWeight)
    { }

  // special case for when nLevels = 3, should assert nLevels() == 3
  ClStrength(const string &name, double w1, double w2, double w3);

  virtual ~ClStrength()
    { }

  virtual bool isRequired() const
    { return (_symbolicWeight == clsRequired()._symbolicWeight); }

  virtual ostream &printOn(ostream &xo) const
    { 
    xo << name(); 
    if (!isRequired())
      xo << ":" << symbolicWeight(); 
    return xo; 
    }

  virtual const ClSymbolicWeight &symbolicWeight() const
    { return _symbolicWeight; }

 private:
  string name() const
    { return _name; }

  void set_name(string name)
    { _name = name; }

  void set_symbolicWeight(const ClSymbolicWeight &symbolicWeight)
    { _symbolicWeight = symbolicWeight; }

  friend ostream& operator<<(ostream &xos, const ClStrength &cls)
    { cls.printOn(xos); return xos; }

  // instance variables
  string _name;
  ClSymbolicWeight _symbolicWeight;
  

};

#endif
