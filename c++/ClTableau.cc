// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998, All Rights Reserved.
//
// ClTableau.cc

#include "ClTableau.h"

// Add v, update column cross indices
// v becomes a basic variable
void 
ClTableau::addRow(const ClVariable &var, const ClLinearExpression &expr)
{
  my_rows[var] = expr;
  map<ClVariable,Number>::const_iterator it = expr.terms().begin();
  // for each variable in expr, add var to the multiset of variables in that column
  for (; it != expr.terms().end(); ++it)
    {
    const ClVariable &v = (*it).first;
    my_columns[v].insert(var);
    }
}

// v has been removed from the linear expression for subject
// update column cross indices 
// Variable v has been removed from an expression.  If the
// expression is in a tableau the corresponding basic variable is
// subject (or if subject is nil then it's in the objective function).
// Update the column cross-indices.

void 
ClTableau::noteRemovedVariable(const ClVariable &v, const ClVariable &subject)
{
  my_columns[v].erase(subject);
}

// v has been added to the linear expression for subject
// update column cross indices
void 
ClTableau::noteAddedVariable(const ClVariable &v, const ClVariable &subject)
{
  my_columns[v].insert(subject);
}

// Remove var from the tableau -- remove the column cross indices for var
// and remove var from every expression in rows in which v occurs
// Remove the parametric variable var, updating the appropriate column and row entries.
// (FIXGJB: is this called removeColumn in the TR?)
void 
ClTableau::removeParametricVar(const ClVariable &var)
{
  map<ClVariable, multiset<ClVariable> >::iterator it_var = my_columns.find(var);
  multiset<ClVariable> &varset = (*it_var).second;
  // remove the rows with the variables in varset
  multiset<ClVariable>::iterator it = varset.begin();
  for (; it != varset.end(); ++it)
    {
    ClVariable &v = (*it);
    map <ClVariable,Number> &terms = my_rows[v].terms();
    terms.erase(terms.find(var));
    }
  my_columns.erase(it_var);
}

// Remove the basic variable v from the tableau row v=expr
// Then update column cross indices
void 
ClTableau::removeRow(const ClVariable &var)
{
  map<ClVariable, ClLinearExpression>::iterator it = my_rows.find(var);
  assert(it != my_rows.end());
  ClLinearExpression &expr = (*it).second;
  map<ClVariable,Number> &terms = expr.terms();
  map<ClVariable,Number>::iterator it_term = terms.begin();
  for (; it_term != terms.end(); ++it_term)
    {
    ClVariable &v = (*it_term).first;
    my_columns[v].erase(var);
    }
  my_infeasibleRows.erase(my_infeasibleRows.find(var));
  my_rows.erase(it);
}

// Replace all occurrences of oldVar with expr, and update column cross indices
// oldVar should now be a basic variable
void 
ClTableau::substituteOut(const ClVariable &oldVar, const ClLinearExpression &expr)
{
  map<ClVariable, multiset<ClVariable> >::iterator it_oldVar = my_columns.find(oldVar);
  multiset<ClVariable> &varset = (*it_oldVar).second;
  multiset<ClVariable>::iterator it = varset.begin();
  for (; it != varset.end(); ++it)
    {
    ClVariable &v = (*it);
    ClLinearExpression &row = my_rows[v];
    row.substituteOut(oldVar,expr,v,*this);
    if (v.isRestricted() && row.constant() < 0.0)
      {
      my_infeasibleRows.insert(v);
      }
    }
  my_columns.erase(it_oldVar);
}
