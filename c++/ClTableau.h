// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998, 1999 Alan Borning and Greg Badros
// See ../LICENSE for legal details regarding this software
//
// ClTableau.h

#ifndef ClTableau_H
#define ClTableau_H

#include "Cassowary.h"
#include "ClLinearExpression.h"
#include "ClVariable.h"
#include "ClTypedefs.h"

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
#ifdef CL_TRACE
    Tracer TRACER(__FUNCTION__);
    cerr << "(" << v << ", " << subject << ")" << endl;
#endif
    ClTableauVarSet &column = _columns[&v];
    ClTableauVarSet::const_iterator it = column.find(&subject);
    assert(it != column.end());
    column.erase(it);
#ifdef CL_TRACE_VERBOSE
    cerr << "v = " << v << " and columns[&v].size() = "
         << column.size() << endl;
#endif
    if (column.size() == 0)
      {
      const ClVariable *pclv = static_cast<const ClVariable *>(&v);  
      _columns.erase(pclv);
      _externalRows.erase(pclv);
      _externalParametricVars.erase(pclv);
      }
    }

  // v has been added to the linear expression for subject
  // update column cross indices
  void noteAddedVariable(const ClAbstractVariable &v, const ClAbstractVariable &subject)
    { 
#ifdef CL_TRACE
    Tracer TRACER(__FUNCTION__);
    cerr << "(" << v << ", " << subject << ")" << endl;
#endif
    _columns[&v].insert(&subject); 
    if (v.isExternal() && !FIsBasicVar(v))
      {
      _externalParametricVars.insert(static_cast<const ClVariable *>(&v));
      }
    }

#ifndef CL_NO_IO
  ostream &printOn(ostream &xo) const;

  ostream &printInternalInfo(ostream &xo) const;
#endif

  // Check integrity of the tableau
  // not complete, yet, but a start, at least
  // Guard calls to AssertValid with CL_SOLVER_CHECK_INTEGRITY,
  // since this is expensive
  virtual void AssertValid() const {
#ifndef NDEBUG
    // all external basic variables are in _externalRows
    // and all external parametric variables are in _externalParametricVars
    ClTableauRowsMap::const_iterator itRow = _rows.begin();
    for (; itRow != _rows.end(); ++itRow)
      {
      const ClAbstractVariable *pclv = (*itRow).first;
      if (pclv->isExternal())
        {
        const ClVariable *pv = dynamic_cast<const ClVariable *>(pclv);
        if (_externalRows.find(pv) == _externalRows.end()) 
          {
#ifndef CL_NO_IO
          cerr << "External basic variable " << *pclv
               << " is not in _externalRows" << endl;
#endif
          }
        }
      const ClLinearExpression *pcle = rowExpression(*pclv);
      assert(pcle);
      ClVarToNumberMap::const_iterator it = pcle->terms().begin();
      for (; it != pcle->terms().end(); ++it)
        {
        const ClAbstractVariable *pclv = (*it).first;
        if (pclv->isExternal()) 
          {
          const ClVariable *pv = dynamic_cast<const ClVariable *>(pclv);
          if (_externalParametricVars.find(pv) == _externalParametricVars.end())
            {
#ifndef CL_NO_IO
            cerr << "External parametric variable " << *pclv 
                 << " is not in _externalParametricVars" << endl;
#endif
            }
          }
        }
      }
#endif /* !NDEBUG */
  }

  
 protected:
  // Constructor -- want to start with empty objects so not much to do
  ClTableau()
    { }

  virtual ~ClTableau();

  // Add v=expr to the tableau, update column cross indices
  // v becomes a basic variable
  // expr is now owned by ClTableau class, 
  // and ClTableauis responsible for deleting it
  // (also, expr better be allocated on the heap!)
  void addRow(const ClAbstractVariable &v, const ClLinearExpression &expr);

  // Remove v from the tableau -- remove the column cross indices for v
  // and remove v from every expression in rows in which v occurs
  // returns a pointer to the variable (since we often want to delete
  // the variable)
  const ClAbstractVariable *removeColumn(const ClAbstractVariable &v);

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

  const ClLinearExpression *rowExpression(const ClAbstractVariable &v) const
    {
    ClTableauRowsMap::const_iterator i = _rows.find(&v);
    if (i != _rows.end())
      return (*i).second;
    else
      return NULL;
    }

  ClLinearExpression *rowExpression(const ClAbstractVariable &v)
    {
      const ClTableau *pthis = const_cast<const ClTableau *>(this);
      return const_cast<ClLinearExpression *>(pthis->rowExpression(v));
    }


  bool FIsBasicVar(const ClAbstractVariable &v)
    { return rowExpression(v) != NULL; }

  // private: FIXGJB: can I improve the encapsulation?

  // _columns is a mapping from variables which occur in expressions to the
  // set of basic variables whose expressions contain them
  // i.e., it's a mapping from variables in expressions (a column) to the 
  // set of rows that contain them
  ClTableauColumnsMap _columns;

  // _rows maps basic variables to the expressions for that row in the tableau
  ClTableauRowsMap _rows;

  // the collection of basic variables that have infeasible rows
  // (used when reoptimizing)
  ClTableauVarSet _infeasibleRows;

  // the set of rows where the basic variable is external
  // this was added to the C++ version to reduce time in setExternalVariables()
  ClExternalVarSet _externalRows;

  // the set of external variables which are parametric
  // this was added to the C++ version to reduce time in setExternalVariables()
  ClExternalVarSet _externalParametricVars;

};

#ifndef CL_NO_IO
ostream &operator<<(ostream &xo, const ClTableau &clt);
#endif

#endif
