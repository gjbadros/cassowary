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
ClTableau::addRow(const ClAbstractVariable &var, const ClLinearExpression &expr)
{
#ifndef NO_TRACE
  Tracer TRACER(__FUNCTION__);
  cerr << "(" << var << ", " << expr << ")" << endl;
#endif
  // FIXGJB: gotta delete this somewhere
  ClLinearExpression *pexpr = new ClLinearExpression(expr);
  my_rows[&var] = pexpr;
  map<const ClAbstractVariable *,Number>::const_iterator it = pexpr->terms().begin();
  // for each variable in *pexpr, add var to the set of rows which have that variable
  // in their expression
  for (; it != pexpr->terms().end(); ++it)
    {
    const ClAbstractVariable *pv = (*it).first;
    my_columns[pv].insert(&var);
    }
#ifndef NO_TRACE
  cerr << *this << endl;
#endif
}

// Remove var from the tableau -- remove the column cross indices for var
// and remove var from every expression in rows in which v occurs
// Remove the parametric variable var, updating the appropriate column and row entries.
// (Renamed from Smalltalk implementation's `removeParametricVar')
void 
ClTableau::removeColumn(const ClAbstractVariable &var)
{
#ifndef NO_TRACE
  Tracer TRACER(__FUNCTION__);
  cerr << "(" << var << ")" << endl;
#endif
  map<const ClAbstractVariable *, set<const ClAbstractVariable *> >::iterator it_var = my_columns.find(&var);
  set<const ClAbstractVariable *> &varset = (*it_var).second;
  // remove the rows with the variables in varset
  set<const ClAbstractVariable *>::iterator it = varset.begin();
  for (; it != varset.end(); ++it)
    {
    const ClAbstractVariable *pv = (*it);
    map <const ClAbstractVariable *,Number> &terms = my_rows[pv]->terms();
    terms.erase(terms.find(&var));
    }
  my_columns.erase(it_var);
}

// Remove the basic variable v from the tableau row v=expr
// Then update column cross indices
ClLinearExpression *
ClTableau::removeRow(const ClAbstractVariable &var)
{
#ifndef NO_TRACE
  Tracer TRACER(__FUNCTION__);
  cerr << "(" << var << ")" << endl;
#endif
  map<const ClAbstractVariable *, ClLinearExpression *>::iterator it = my_rows.find(&var);
  assert(it != my_rows.end());
  ClLinearExpression *pexpr = (*it).second;
  map<const ClAbstractVariable *,Number> &terms = pexpr->terms();
  map<const ClAbstractVariable *,Number>::iterator it_term = terms.begin();
  for (; it_term != terms.end(); ++it_term)
    {
    const ClAbstractVariable *pv = (*it_term).first;
    my_columns[pv].erase(&var);
    }
  set<const ClAbstractVariable *>::iterator itVar = my_infeasibleRows.find(&var);
  if (itVar != my_infeasibleRows.end())
    {
    my_infeasibleRows.erase(itVar);
    }
  my_rows.erase(it);
#ifndef NO_TRACE
  cerr << "- returning " << *pexpr << endl;
#endif
  return pexpr;
}

// Replace all occurrences of oldVar with expr, and update column cross indices
// oldVar should now be a basic variable
// Uses the columns data structure and calls substituteOut on each
// row that has oldVar in it
void 
ClTableau::substituteOut(const ClAbstractVariable &oldVar, const ClLinearExpression &expr)
{
#ifndef NO_TRACE
  cerr << "* ClTableau::";
  Tracer TRACER(__FUNCTION__);
  cerr << "(" << oldVar << ", " << expr << ")" << endl;
  cerr << (*this) << endl;
#endif

  map<const ClAbstractVariable *, set<const ClAbstractVariable *> >::iterator it_oldVar = my_columns.find(&oldVar);
  assert(it_oldVar != my_columns.end());
  set<const ClAbstractVariable *> &varset = (*it_oldVar).second;
  set<const ClAbstractVariable *>::iterator it = varset.begin();
  for (; it != varset.end(); ++it)
    {
    const ClAbstractVariable *pv = (*it);
    ClLinearExpression *prow = my_rows[pv];
    prow->substituteOut(oldVar,expr,*pv,*this);
    if (pv->isRestricted() && prow->constant() < 0.0)
      {
      my_infeasibleRows.insert(pv);
      }
    }
  my_columns.erase(it_oldVar);
}


ostream &
ClTableau::printOn(ostream &xo) const
{
  xo << "Tableau:\n" 
     << my_rows << endl;
  xo << "Columns:\n" 
     << my_columns << endl;
  xo << "Infeasible rows: " 
     << my_infeasibleRows << endl;
  return xo;
}

ostream &operator<<(ostream &xo, const ClTableau &clt)
{ return clt.printOn(xo); }

ostream &
printTo(ostream &xo, const set<const ClAbstractVariable *> & varset)
{
  set<const ClAbstractVariable *>::const_iterator it = varset.begin();
  xo << "{ ";
  if (it != varset.end())
    {
    xo << *(*it);
    ++it;
    }
  for (; it != varset.end(); ++it) 
    {
    xo << ", " << *(*it);
    }
  xo << " }";
  return xo;
}  

ostream &operator<<(ostream &xo, const set<const ClAbstractVariable *> & varset)
{ return printTo(xo,varset); }


ostream &
printTo(ostream &xo, const map<const ClAbstractVariable *, set<const ClAbstractVariable *> > & varmap)
{
  map<const ClAbstractVariable *, set<const ClAbstractVariable *> >::const_iterator it = varmap.begin();
  for (; it != varmap.end(); ++it) 
    {
    xo << *((*it).first) << " -> " << (*it).second << endl;
    }
  return xo;
}

ostream &operator<<(ostream &xo, const map<const ClAbstractVariable *, set<const ClAbstractVariable *> > & varmap)
{ return printTo(xo,varmap); }

ostream &
printTo(ostream &xo, const map<const ClAbstractVariable *, ClLinearExpression * > & rows)
{
  map<const ClAbstractVariable *, ClLinearExpression * >::const_iterator it = rows.begin();
  for (; it != rows.end(); ++it) 
    {
    const ClAbstractVariable *pv = it->first;
    const ClLinearExpression *pe = it->second;
    if (pv) xo << *pv; else xo << "NilVar";
    xo << " <-=-> ";
    if (pe) xo << *pe; else xo << "NilExpr";
    xo << endl;
    }
  return xo;
}

ostream &operator<<(ostream &xo, const map<const ClAbstractVariable *, ClLinearExpression * > & rows)
{ return printTo(xo,rows); }
