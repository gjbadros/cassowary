// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998, All Rights Reserved.
//
// ClAbstractVariable.h

#ifndef ClAbstractVariable_H
#define ClAbstractVariable_H

#include "Cassowary.h"

class ClAbstractVariable {
public:
  String name() const
    { return my_name; }
  void set_name(String const &name)
    { my_name = name; }

  bool isDummy() const
    { return false; }
  bool isExternal() const = 0;
  bool isPivotable() const = 0;
  bool isRestricted() const = 0;

  ostream &printOn(ostream &xo);
    {  xo << "CV#" << name << endl;  return xo; }

private:
  String my_name;
};

#endif
