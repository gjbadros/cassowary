// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998, All Rights Reserved.
//
// ClVariable.h

#ifndef ClVariable_H
#define ClVariable_H

#include "Cassowary.h"
#include "ClAbstractVariable.h"

class ClVariable: public ClAbstractVariable {
public:
  bool isExternal() const
    { return true; }
  bool isPivotable() const
    { return false; }
  bool isRestricted() const
    { return false; }

  ostream &printOn(ostream &xo);
    {  
      xo << "[" << name << ":";
      value.printOn(xo);
      xo << "]" << endl;
      return xo;
    }


  Number value() const
    { return my_value; }
  void set_value(Number const &value)
    { my_value = value; }


  ClLinearExpression asLinearExpression() const;

  /* Also has "@" for creating point pairs */
  /* arithmetic linear expression builders */
  /* constraint creators */ 

private:
  Number my_value;
};


#endif

