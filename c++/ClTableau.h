// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998, All Rights Reserved.
//
// ClTableau.h

#ifndef ClTableau_H
#define ClTableau_H

#include <map.h>
#include <set.h>
#include "Cassowary.h"
#include "ClLinearExpression.h"

class ClTableau {

 public:
  // Variable v has been removed from an expression.  If the
  // expression is in a tableau the corresponding basic variable is
  // subject (or if subject is nil then it's in the objective function).
  // Update the column cross-indices.
  void noteRemovedVariable(const ClAbstractVariable &v, const ClAbstractVariable &subject)
    { 
#ifndef NO_TRACE
    Tracer TRACER(__FUNCTION__);
    cerr << "(" << v << ", " << subject << ")" << endl;
#endif
    set<const ClAbstractVariable *>::const_iterator it = my_columns[&v].find(&subject);
    assert(it != my_columns[&v].end());
    my_columns[&v].erase(it); 
    }

  // v has been added to the linear expression for subject
  // update column cross indices
  void noteAddedVariable(const ClAbstractVariable &v, const ClAbstractVariable &subject)
    { 
#ifndef NO_TRACE
    Tracer TRACER(__FUNCTION__);
    cerr << "(" << v << ", " << subject << ")" << endl;
#endif
    my_columns[&v].insert(&subject); 
    }

  ostream &printOn(ostream &xo) const;

 protected:
  // Constructor -- want to start with empty objects so not much to do
  ClTableau()
    { }
  
  // Add v=expr to the tableau, update column cross indices
  // v becomes a basic variable
  void addRow(const ClAbstractVariable &v, const ClLinearExpression &expr);

  // Remove v from the tableau -- remove the column cross indices for v
  // and remove v from every expression in rows in which v occurs
  void removeColumn(const ClAbstractVariable &v);

  // Remove the basic variable v from the tableau row v=expr
  // Then update column cross indices
  ClLinearExpression *removeRow(const ClAbstractVariable &v);

  // Replace all occurrences of oldVar with expr, and update column cross indices
  // oldVar should now be a basic variable
  void substituteOut(const ClAbstractVariable &oldVar, const ClLinearExpression &expr);

  map<const ClAbstractVariable *, set<const ClAbstractVariable *> > columns()
    { return my_columns; }  

  map<const ClAbstractVariable *, ClLinearExpression * > rows()
    { return my_rows; }  

  // return true iff the variable subject is in the columns keys
  bool columnsHasKey(const ClAbstractVariable &subject) const
    { 
    map<const ClAbstractVariable *, set<const ClAbstractVariable *> >::const_iterator i = my_columns.find(&subject);
    return (i != my_columns.end());
    }

  ClLinearExpression *rowExpression(const ClAbstractVariable &v)
    {
    map<const ClAbstractVariable *, ClLinearExpression *>::const_iterator i = my_rows.find(&v);
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
  map<const ClAbstractVariable *, set<const ClAbstractVariable *> > my_columns;
  map<const ClAbstractVariable *, ClLinearExpression *> my_rows;

  // the ordered collection of basic variables that have infeasible rows
  // (used when reoptimizing)
  set<const ClAbstractVariable *> my_infeasibleRows;

};

ostream &operator<<(ostream &xo, const ClTableau &clt);

#endif
