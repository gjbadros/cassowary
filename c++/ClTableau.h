// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998 Alan Borning and Greg Badros
// See ../COPYRIGHT for legal details regarding this software
//
// ClTableau.h

#ifndef ClTableau_H
#define ClTableau_H

#include <map>
#include <set>
#include "Cassowary.h"
#include "ClLinearExpression.h"
#include "ClVariable.h"

typedef set<const ClAbstractVariable *> ClTableauVarSet;
typedef set<const ClVariable *> ClExternalVarSet;
typedef map<const ClAbstractVariable *, ClTableauVarSet > ClTableauColumnsMap;
typedef map<const ClAbstractVariable *, ClLinearExpression *> ClTableauRowsMap;

class ClTableau {

 public:
  // No public constructor, since this does nothing but support
  // an ADT for the ClSimplexSolver

  // Variable v has been removed from an expression.  If the
  // expression is in a tableau the corresponding basic variable is
  // subject (or if subject is nil then it's in the objective function).
  // Update the column cross-indices.
  void noteRemovedVariable(const ClAbstractVariable &v, const ClAbstractVariable &subject)
    { 
#ifndef CL_NO_TRACE
    Tracer TRACER(__FUNCTION__);
    cerr << "(" << v << ", " << subject << ")" << endl;
#endif
    ClTableauVarSet::const_iterator it = _columns[&v].find(&subject);
    assert(it != _columns[&v].end());
    _columns[&v].erase(it); 
    }

  // v has been added to the linear expression for subject
  // update column cross indices
  void noteAddedVariable(const ClAbstractVariable &v, const ClAbstractVariable &subject)
    { 
#ifndef CL_NO_TRACE
    Tracer TRACER(__FUNCTION__);
    cerr << "(" << v << ", " << subject << ")" << endl;
#endif
    _columns[&v].insert(&subject); 
    if (v.isExternal())
      {
      _externalParametricVars.insert(static_cast<const ClVariable *>(&v));
      }
    }

  ostream &printOn(ostream &xo) const;

  ostream &printInternalInfo(ostream &xo) const;
  
 protected:
  // Constructor -- want to start with empty objects so not much to do
  ClTableau()
    { }

  ~ClTableau();

  // Add v=expr to the tableau, update column cross indices
  // v becomes a basic variable
  // expr is now owned by ClTableau class, 
  // and ClTableauis responsible for deleting it
  // (also, expr better be allocated on the heap!)
  void addRow(const ClAbstractVariable &v, const ClLinearExpression &expr);

  // Remove v from the tableau -- remove the column cross indices for v
  // and remove v from every expression in rows in which v occurs
  void removeColumn(const ClAbstractVariable &v);

  // Remove the basic variable v from the tableau row v=expr
  // Then update column cross indices
  // Probably want to call delete on the ClLinearExpression * returned
  // unless you're adding that same expression back into the 
  // tableau
  ClLinearExpression *removeRow(const ClAbstractVariable &v);

  // Replace all occurrences of oldVar with expr, and update column cross indices
  // oldVar should now be a basic variable
  void substituteOut(const ClAbstractVariable &oldVar, const ClLinearExpression &expr);

  ClTableauColumnsMap columns()
    { return _columns; }  

  ClTableauRowsMap rows()
    { return _rows; }  

  // return true iff the variable subject is in the columns keys
  bool columnsHasKey(const ClAbstractVariable &subject) const
    { 
    ClTableauColumnsMap::const_iterator i = _columns.find(&subject);
    return (i != _columns.end());
    }

  ClLinearExpression *rowExpression(const ClAbstractVariable &v)
    {
    ClTableauRowsMap::const_iterator i = _rows.find(&v);
    if (i != _rows.end())
      return (*i).second;
    else
      return NULL;
    }

  // private: FIXGJB: can I improve the encapsulation?

  // _columns is a mapping from variables which occur in expressions to the
  // set of basic variables whose expressions contain them
  // i.e., it's a mapping from variables in expressions (a column) to the 
  // set of rows that contain them
  ClTableauColumnsMap _columns;
  ClTableauRowsMap _rows;

  // the ordered collection of basic variables that have infeasible rows
  // (used when reoptimizing)
  ClTableauVarSet _infeasibleRows;

  // the set of rows where the basic variable is external
  // this was added to the C++ version to reduce time in setExternalVariables()
  ClExternalVarSet _externalRows;

  // the set of external variables which are parametric
  // this was added to the C++ version to reduce time in setExternalVariables()
  ClExternalVarSet _externalParametricVars;

};

ostream &operator<<(ostream &xo, const ClTableau &clt);

#endif
