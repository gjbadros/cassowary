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
#include <vector.h>
#include "Cassowary.h"
#include "ClLinearExpression.h"

class ClTableau {
 public:
  // v has been removed from the linear expression for subject
  // update column cross indices
  void noteRemovedVariable(const ClVariable &v, const ClVariable &subject);

  // v has been added to the linear expression for subject
  // update column cross indices
  void noteAddedVariable(const ClVariable &v, const ClVariable &subject);

 protected:
  ClTableau()
    { }
  
  // Add v=expr to the tableau, update column cross indices
  // v becomes a basic variable
  void addRow(const ClVariable &v, const ClLinearExpression &expr);

  // Remove v from the tableau -- remove the column cross indices for v
  // and remove v from every expression in rows in which v occurs
  void removeParametricVar(const ClVariable &v);

  // Remove the basic variable v from the tableau row v=expr
  // Then update column cross indices
  void removeRow(const ClVariable &v);

  // Replace all occurrences of oldVar with expr, and update column cross indices
  // oldVar should now be a basic variable
  void substituteOut(const ClVariable &oldVar, const ClLinearExpression &expr);

  map<ClVariable, set<ClVariable> > columns()
    { return my_columns; }  

  map<ClVariable, ClLinearExpression > rows()
    { return my_rows; }  

  // return true iff the variable subject is in the columns keys
  bool columnsHasKey(const ClVariable &subject) const
    { 
    map<ClVariable, set<ClVariable> >::const_iterator i = my_columns.find(subject);
    return (i != my_columns.end());
    }

  ClLinearExpression &rowExpression(const ClVariable &v) const
    {
    map<ClVariable, ClLinearExpression>::const_iterator i = my_rows.find(v);
    if (i != my_rows.end())
      return const_cast<ClLinearExpression &>((*i).second);
    else
      return cleNil();
    }

 private:

  // my_columns is a mapping from variables which occur in expressions to the
  // set of basic variables whose expressions contain them
  // i.e., it's a mapping from variables in expressions (a column) to the 
  // set of rows that contain them
  map<ClVariable, set<ClVariable> > my_columns;
  map<ClVariable, ClLinearExpression > my_rows;

  // the ordered collection of basic variables that have infeasible rows
  // (used when reoptimizing)
  set<ClVariable> my_infeasibleRows;

};

#endif
