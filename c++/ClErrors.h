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
// ClErrors.h

#ifndef ClErrors_H
#define ClErrors_H

#include "Cassowary.h"
#include <stdexcept>

class ExCLError : public exception {
 public:
  virtual char *description() const
    { return "(ExCLError) An error has occured in CL"; }
};

class ExCLInternalError : public ExCLError {
 public:
  virtual char *description() const
    { return "(ExCLInternalError) An internal error has occurred"; }
};

class ExCLTooDifficult : public ExCLError {
 public:
  virtual char *description() const
    { return "(ExCLTooDifficult) The constraints are too difficult to solve"; }
};

class ExCLRequiredFailure : public ExCLError {
 public:
  virtual char *description() const
    { return "(ExCLRequiredFailure) A required constraint cannot be satisfied"; }
};

class ExCLNotEnoughStays : public ExCLError {
 public:
  virtual char *description() const
    { return "(ExCLNotEnoughStays) There are not enough stays to give specific values to every variable"; }
};

class ExCLNonlinearExpression : public ExCLError {
 public:
  virtual char *description() const
    { return "(ExCLNonlinearExpression) The resulting expression would be nonlinear"; }
};
  
class ExCLConstraintNotFound : public ExCLError {
 public:
  virtual char *description() const
    { return "(ExCLConstraintNotFound) Tried to remove a constraint never added to the tableu"; }
};
  

#endif
