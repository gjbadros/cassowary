// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998, All Rights Reserved.
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

class ExCLNotEnoughtStays : public ExCLError {
 public:
  virtual char *description() const
    { return "(ExCLNotEnoughtStays) There are not enough stays to give specific values to every variable"; }
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
