// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998 Alan Borning and Greg Badros
// See ../COPYRIGHT for legal details regarding this software
//
// ClTableau.cc

#include "ClTableau.h"
#include "debug.h"

// delete the linear expressions
// let ClSimplexSolver worry about deleting the variables
ClTableau::~ClTableau()
{
  ClTableauRowsMap::iterator it = _rows.begin();
  for (; it != _rows.end(); ++it)
    {
    // free the ClLinearExpression that we new-ed 
#ifndef CL_NO_TRACE
    cerr << "Deleting row  delete@ " << ((*it).second) << endl;
#endif
    delete (*it).second;
    }
}

// Some extra debugging info
ostream &
ClTableau::printInternalInfo(ostream &xo) const
{
  xo << "Tableau Information:" << endl
     << "Rows: " << _rows.size()
     << " (= " << _rows.size() - 1 << " constraints)" << endl
     << "Columns: " << _columns.size() << endl
     << "Infeasible Rows: " << _infeasibleRows.size() << endl
     << "External basic variables: " << _externalRows.size() << endl
     << "External parameteric variables: " << _externalParametricVars.size() << endl;
  return xo;
}


// Add v, update column cross indices
// v becomes a basic variable
// expr is now owned by ClTableau class, 
// and ClTableauis responsible for deleting it
// (also, expr better be allocated on the heap!)
void 
ClTableau::addRow(const ClAbstractVariable &var, const ClLinearExpression &expr)
{
#ifndef CL_NO_TRACE
  Tracer TRACER(__FUNCTION__);
  cerr << "(" << var << ", " << expr << ")" << endl;
#endif
  _rows[&var] = const_cast<ClLinearExpression *>(&expr);
  ClVarToNumberMap::const_iterator it = expr.terms().begin();
  // for each variable in expr, add var to the set of rows which have that variable
  // in their expression
  for (; it != expr.terms().end(); ++it)
    {
    const ClAbstractVariable *pv = (*it).first;
    _columns[pv].insert(&var);
    if (pv->isExternal())
      {
      _externalParametricVars.insert(static_cast<const ClVariable *>(pv));
      }
    }
  if (var.isExternal())
    {
    _externalRows.insert(static_cast<const ClVariable *>(&var));
    }
#ifndef CL_NO_TRACE
  cerr << *this << endl;
#endif
}

// Remove var from the tableau -- remove the column cross indices for var
// and remove var from every expression in rows in which v occurs
// Remove the parametric variable var, updating the appropriate column and row entries.
// (Renamed from Smalltalk implementation's `removeParametricVar')
const ClAbstractVariable *
ClTableau::removeColumn(const ClAbstractVariable &var)
{
#ifndef CL_NO_TRACE
  Tracer TRACER(__FUNCTION__);
  cerr << "(" << var << ")" << endl;
#endif
  ClTableauColumnsMap::iterator it_var = _columns.find(&var);
  if (it_var == _columns.end())
    return &var;  // nothing to do

  ClTableauVarSet &varset = (*it_var).second;
  // remove the rows with the variables in varset
  ClTableauVarSet::iterator it = varset.begin();
  for (; it != varset.end(); ++it)
    {
    const ClAbstractVariable *pv = (*it);
    ClVarToNumberMap &terms = _rows[pv]->terms();
    terms.erase(terms.find(&var));
    }
  if (var.isExternal())
    {
    _externalRows.erase(static_cast<const ClVariable *>(&var));
    _externalParametricVars.erase(static_cast<const ClVariable *>(&var));
    }
  _columns.erase(it_var);
  return &var;
}

// Remove the basic variable v from the tableau row v=expr
// Then update column cross indices
ClLinearExpression *
ClTableau::removeRow(const ClAbstractVariable &var)
{
#ifndef CL_NO_TRACE
  Tracer TRACER(__FUNCTION__);
  cerr << "(" << var << ")" << endl;
#endif
  ClTableauRowsMap::iterator it = _rows.find(&var);
  assert(it != _rows.end());
  ClLinearExpression *pexpr = (*it).second;
  ClVarToNumberMap &terms = pexpr->terms();
  ClVarToNumberMap::iterator it_term = terms.begin();
  for (; it_term != terms.end(); ++it_term)
    {
    const ClAbstractVariable *pv = (*it_term).first;
    _columns[pv].erase(&var);
    if (_columns[pv].size() == 0)
      _columns.erase(pv);
    }

  _infeasibleRows.erase(&var);

  if (var.isExternal())
    {
    _externalRows.erase(static_cast<const ClVariable *>(&var));
    }
  _rows.erase(it);
#ifndef CL_NO_TRACE
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
#ifndef CL_NO_TRACE
  cerr << "* ClTableau::";
  Tracer TRACER(__FUNCTION__);
  cerr << "(" << oldVar << ", " << expr << ")" << endl;
  cerr << (*this) << endl;
#endif

  ClTableauColumnsMap::iterator it_oldVar = _columns.find(&oldVar);
  if (it_oldVar == _columns.end())
    return;

  ClTableauVarSet &varset = (*it_oldVar).second;
  ClTableauVarSet::iterator it = varset.begin();
  for (; it != varset.end(); ++it)
    {
    const ClAbstractVariable *pv = (*it);
    ClLinearExpression *prow = _rows[pv];
    prow->substituteOut(oldVar,expr,*pv,*this);
    if (pv->isRestricted() && prow->constant() < 0.0)
      {
      _infeasibleRows.insert(pv);
      }
    }
  if (oldVar.isExternal())
    {
    _externalRows.insert(static_cast<const ClVariable *>(&oldVar));
    _externalParametricVars.erase(static_cast<const ClVariable *>(&oldVar));
    }
  _columns.erase(it_oldVar);
}


ostream &
printTo(ostream &xo, const ClTableauVarSet & varset)
{
  ClTableauVarSet::const_iterator it = varset.begin();
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

ostream &operator<<(ostream &xo, const ClTableauVarSet & varset)
{ return printTo(xo,varset); }

ostream &
printTo(ostream &xo, const ClExternalVarSet & varset)
{
  ClExternalVarSet::const_iterator it = varset.begin();
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

ostream &operator<<(ostream &xo, const ClExternalVarSet & varset)
{ return printTo(xo,varset); }


ostream &
printTo(ostream &xo, const ClTableauColumnsMap & varmap)
{
  ClTableauColumnsMap::const_iterator it = varmap.begin();
  for (; it != varmap.end(); ++it) 
    {
    xo << *((*it).first) << " -> " << (*it).second << endl;
    }
  return xo;
}

ostream &operator<<(ostream &xo, const ClTableauColumnsMap & varmap)
{ return printTo(xo,varmap); }

ostream &
printTo(ostream &xo, const ClTableauRowsMap & rows)
{
  ClTableauRowsMap::const_iterator it = rows.begin();
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

ostream &operator<<(ostream &xo, const ClTableauRowsMap & rows)
{ return printTo(xo,rows); }

ostream &
ClTableau::printOn(ostream &xo) const
{
  xo << "Tableau:\n" 
     << _rows << endl;
  xo << "Columns:\n" 
     << _columns << endl;
  xo << "Infeasible rows: " 
     << _infeasibleRows << endl;
  xo << "External basic variables: "
     << _externalRows << endl;
  xo << "External parametric variables: "
     << _externalParametricVars << endl;
  return xo;
}

ostream &operator<<(ostream &xo, const ClTableau &clt)
{ return clt.printOn(xo); }

