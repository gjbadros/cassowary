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
#include "debug.h"

// Add v, update column cross indices
// v becomes a basic variable
void 
ClTableau::addRow(const ClVariable &var, ClLinearExpression expr)
{
#ifndef NO_TRACE
  Tracer TRACER(__FUNCTION__);
  cerr << "(" << var << ")" << endl;
#endif
  my_rows[var] = expr;
  map<ClVariable,Number>::const_iterator it = expr.terms().begin();
  // for each variable in expr, add var to the set of rows which have that variable
  // in their expression
  for (; it != expr.terms().end(); ++it)
    {
    const ClVariable &v = (*it).first;
    my_columns[v].insert(var);
    }
}

// Remove var from the tableau -- remove the column cross indices for var
// and remove var from every expression in rows in which v occurs
// Remove the parametric variable var, updating the appropriate column and row entries.
// (Renamed from Smalltalk implementation's `removeParametricVar')
void 
ClTableau::removeColumn(const ClVariable &var)
{
#ifndef NO_TRACE
  Tracer TRACER(__FUNCTION__);
  cerr << "(" << var << ")" << endl;
#endif
  map<ClVariable, set<ClVariable> >::iterator it_var = my_columns.find(var);
  set<ClVariable> &varset = (*it_var).second;
  // remove the rows with the variables in varset
  set<ClVariable>::iterator it = varset.begin();
  for (; it != varset.end(); ++it)
    {
    const ClVariable &v = (*it);
    map <ClVariable,Number> &terms = my_rows[v].terms();
    terms.erase(terms.find(var));
    }
  my_columns.erase(it_var);
}

// Remove the basic variable v from the tableau row v=expr
// Then update column cross indices
ClLinearExpression
ClTableau::removeRow(const ClVariable &var)
{
#ifndef NO_TRACE
  Tracer TRACER(__FUNCTION__);
  cerr << "(" << var << ")" << endl;
#endif
  map<ClVariable, ClLinearExpression>::iterator it = my_rows.find(var);
  assert(it != my_rows.end());
  ClLinearExpression expr = (*it).second;
  map<ClVariable,Number> &terms = expr.terms();
  map<ClVariable,Number>::iterator it_term = terms.begin();
  for (; it_term != terms.end(); ++it_term)
    {
    ClVariable &v = (*it_term).first;
    my_columns[v].erase(var);
    }
  set<ClVariable>::iterator itVar = my_infeasibleRows.find(var);
  if (itVar != my_infeasibleRows.end())
    {
    my_infeasibleRows.erase(itVar);
    }
  my_rows.erase(it);
#ifndef NO_TRACE
  cerr << "- returning " << expr << endl;
#endif
  return expr;
}

// Replace all occurrences of oldVar with expr, and update column cross indices
// oldVar should now be a basic variable
void 
ClTableau::substituteOut(const ClVariable &oldVar, const ClLinearExpression &expr)
{
#ifndef NO_TRACE
  Tracer TRACER(__FUNCTION__);
  cerr << "(" << oldVar << ", " << expr << ")" << endl;
  cerr << (*this) << endl;
#endif

  map<ClVariable, set<ClVariable> >::iterator it_oldVar = my_columns.find(oldVar);
  assert(it_oldVar != my_columns.end());
  set<ClVariable> &varset = (*it_oldVar).second;
  set<ClVariable>::iterator it = varset.begin();
  for (; it != varset.end(); ++it)
    {
    const ClVariable &v = (*it);
    ClLinearExpression &row = my_rows[v];
    row.substituteOut(oldVar,expr,v,*this);
    if (v.isRestricted() && row.constant() < 0.0)
      {
      my_infeasibleRows.insert(v);
      }
    }
  my_columns.erase(it_oldVar);
}

ostream &operator<<(ostream &xo, const ClTableau &clt)
{
  xo << "Tableau:\n" 
     << clt.my_rows << endl;
  xo << "Columns:\n" 
     << clt.my_columns << endl;
  xo << "Infeasible rows:\n" 
     << clt.my_infeasibleRows << endl;
  return xo;
}


ostream &operator<<(ostream &xo, const set<ClVariable> & varset)
{
  set<ClVariable>::const_iterator it = varset.begin();
  xo << "{ ";
  if (it != varset.end())
    {
    xo << (*it);
    ++it;
    }
  for (; it != varset.end(); ++it) 
    {
    xo << ", " << (*it);
    }
  xo << " }" << endl;
  return xo;
}  


ostream &operator<<(ostream &xo, const map<ClVariable, set<ClVariable> > & varmap)
{
  map<ClVariable, set<ClVariable> >::const_iterator it = varmap.begin();
  for (; it != varmap.end(); ++it) 
    {
    xo << (*it).first << " -> " << (*it).second << endl;
    }
  return xo;
}

ostream &operator<<(ostream &xo, const map<ClVariable, ClLinearExpression > & rows)
{
  map<ClVariable, ClLinearExpression >::const_iterator it = rows.begin();
  for (; it != rows.end(); ++it) 
    {
    xo << (*it).first << " <-=-> " << (*it).second << endl;
    }
  return xo;
}
