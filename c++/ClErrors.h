// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998, 1999 Alan Borning and Greg Badros
// See ../LICENSE for legal details regarding this software
//
// ClErrors.h

#ifndef ClErrors_H
#define ClErrors_H

#include "Cassowary.h"
#include "ClSet.h"
#include <string.h>
#include <stdexcept>

#ifdef USE_GC
class ExCLError : public exception, public gc {
#else
class ExCLError : public exception {
#endif

 public:
  ExCLError() : _msg(NULL) { }
  virtual char *description() const
    { return "(ExCLError) An error has occured in CL"; }
 protected:
  char *_msg;
};

class ExCLInternalError : public ExCLError {
 public:
  ExCLInternalError(const char *sz)
    { _msg = strdup(sz); }
  virtual char *description() const
    { 
      if (_msg) return _msg;
      else return "(ExCLInternalError) An internal error has occurred"; 
    }
};

class ExCLBadResolve : public ExCLError {
 public:
  ExCLBadResolve(const char *sz)
    { _msg = strdup(sz); }
  virtual char *description() const
    {
      if (_msg) return _msg;
      else return "(ExCLBadResolve) Number of resolve values did not match number of edit vars"; 
    }
};

class ExCLEditMisuse : public ExCLError {
 public:
  ExCLEditMisuse(const char *sz)
    { _msg = strdup(sz); }
  virtual char *description() const
    {
      if (_msg) return _msg;
      return "(ExCLEditMisuse) Edit protocol usage violation"; 
    }
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

class ClConstraint;
typedef ClSet<const ClConstraint *> ClConstraintSet;
 
class ExCLRequiredFailureWithExplanation : public ExCLRequiredFailure 
{
public:
  virtual char *description() const
    { return "(ExCLRequiredFailureWithExplanation) A required constraint cannot be satisfied"; }
  virtual void addConstraint(const ClConstraint * cnExpl)
    { _explanation.insert(cnExpl); }
  virtual const ClConstraintSet & explanation() const
    { return _explanation; }
protected:
  ClConstraintSet _explanation;
};

#endif
