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
    ClTableauVarSet::const_iterator it = my_columns[&v].find(&subject);
    assert(it != my_columns[&v].end());
    my_columns[&v].erase(it); 
    }

  // v has been added to the linear expression for subject
  // update column cross indices
  void noteAddedVariable(const ClAbstractVariable &v, const ClAbstractVariable &subject)
    { 
#ifndef CL_NO_TRACE
    Tracer TRACER(__FUNCTION__);
    cerr << "(" << v << ", " << subject << ")" << endl;
#endif
    my_columns[&v].insert(&subject); 
    if (v.isExternal())
      {
      my_externalParametricVars.insert(static_cast<const ClVariable *>(&v));
      }
    }

  ostream &printOn(ostream &xo) const;

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
    { return my_columns; }  

  ClTableauRowsMap rows()
    { return my_rows; }  

  // return true iff the variable subject is in the columns keys
  bool columnsHasKey(const ClAbstractVariable &subject) const
    { 
    ClTableauColumnsMap::const_iterator i = my_columns.find(&subject);
    return (i != my_columns.end());
    }

  ClLinearExpression *rowExpression(const ClAbstractVariable &v)
    {
    ClTableauRowsMap::const_iterator i = my_rows.find(&v);
    if (i != my_rows.end())
      return (*i).second;
    else
      return NULL;
    }

  // private: FIXGJB: can I improve the encapsulation?

  // my_columns is a mapping from variables which occur in expressions to the
  // set of basic variables whose expressions contain them
  // i.e., it's a mapping from variables in expressions (a column) to the 
  // set of rows that contain them
  ClTableauColumnsMap my_columns;
  ClTableauRowsMap my_rows;

  // the ordered collection of basic variables that have infeasible rows
  // (used when reoptimizing)
  ClTableauVarSet my_infeasibleRows;

  // the set of rows where the basic variable is external
  // this was added to the C++ version to reduce time in setExternalVariables()
  ClExternalVarSet my_externalRows;

  // the set of external variables which are parametric
  // this was added to the C++ version to reduce time in setExternalVariables()
  ClExternalVarSet my_externalParametricVars;

};

ostream &operator<<(ostream &xo, const ClTableau &clt);

#endif
