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


#ifndef CL_NO_IO
class ClTableau;

ostream &operator<<(ostream &xo, const ClTableau &clt);

ostream &operator<<(ostream &xo, const ClVarSet &varset);

ostream &operator<<(ostream &xo, const ClTableauColumnsMap &varmap);

ostream &operator<<(ostream &xo, const ClTableauRowsMap &rows);

ostream &operator<<(ostream &xo, const ClVarVector &varlist);
#endif // CL_NO_IO

class ClTableau {

 public:
  // No public constructor, since this does nothing but support
  // an ADT for the ClSimplexSolver

  // Variable v has been removed from an expression.  If the
  // expression is in a tableau the corresponding basic variable is
  // subject (or if subject is nil then it's in the objective function).
  // Update the column cross-indices.
  void noteRemovedVariable(ClVariable v, ClVariable subject)
    { 
#ifdef CL_TRACE
    Tracer TRACER(__FUNCTION__);
    cerr << "(" << v << ", " << subject << ")" << endl;
#endif
    ClVarSet &column = _columns[v];
    ClVarSet::const_iterator it = column.find(subject);
    assert(it != column.end());
    column.erase(it);
#ifdef CL_TRACE_VERBOSE
    cerr << "v = " << v << " and columns[v].size() = "
         << column.size() << endl;
#endif
    if (column.size() == 0)
      {
      _columns.erase(v);
      _externalRows.erase(v);
      _externalParametricVars.erase(v);
      }
    }

  // v has been added to the linear expression for subject
  // update column cross indices
  void noteAddedVariable(ClVariable v, ClVariable subject)
    { 
#ifdef CL_TRACE
    Tracer TRACER(__FUNCTION__);
    cerr << "(" << v << ", " << subject << ")" << endl;
#endif
    _columns[v].insert(subject); 
    if (v.isExternal() && !FIsBasicVar(v))
      {
      _externalParametricVars.insert(v);
      }
    }

#ifndef CL_NO_IO
  ostream &printOn(ostream &xo) const;

  ostream &printInternalInfo(ostream &xo) const;

  ostream &printExternalVariablesTo(ostream &xo) const;

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
      const ClVariable clv = (*itRow).first;
      if (clv.isExternal())
        {
        if (_externalRows.find(clv) == _externalRows.end()) 
          {
#ifndef CL_NO_IO
          cerr << "External basic variable " << clv
               << " is not in _externalRows" << endl;
#endif
          }
        }
      const ClLinearExpression *pcle = rowExpression(clv);
      assert(pcle);
      ClVarToNumberMap::const_iterator it = pcle->terms().begin();
      for (; it != pcle->terms().end(); ++it)
        {
        ClVariable clv = (*it).first;
        if (clv.isExternal()) 
          {
          if (_externalParametricVars.find(clv) == _externalParametricVars.end())
            {
#ifndef CL_NO_IO
            cerr << "External parametric variable " << clv 
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
  void addRow(ClVariable v, const ClLinearExpression &expr);

  // Remove v from the tableau -- remove the column cross indices for v
  // and remove v from every expression in rows in which v occurs
  // returns a pointer to the variable (since we often want to delete
  // the variable)
  ClVariable removeColumn(ClVariable v);

  // Remove the basic variable v from the tableau row v=expr
  // Then update column cross indices
  // Probably want to call delete on the ClLinearExpression * returned
  // unless you're adding that same expression back into the 
  // tableau
  ClLinearExpression *removeRow(ClVariable v);

  // Replace all occurrences of oldVar with expr, and update column cross indices
  // oldVar should now be a basic variable
  void substituteOut(ClVariable oldVar, const ClLinearExpression &expr);

  ClTableauColumnsMap columns()
    { return _columns; }  

  ClTableauRowsMap rows()
    { return _rows; }  

  // return true iff the variable subject is in the columns keys
  bool columnsHasKey(ClVariable subject) const
    { 
    ClTableauColumnsMap::const_iterator i = _columns.find(subject);
    return (i != _columns.end());
    }

  const ClLinearExpression *rowExpression(ClVariable v) const
    {
    ClTableauRowsMap::const_iterator i = _rows.find(v);
    if (i != _rows.end())
      return (*i).second;
    else
      return NULL;
    }

  ClLinearExpression *rowExpression(ClVariable v)
    {
      const ClTableau *pthis = const_cast<const ClTableau *>(this);
      return const_cast<ClLinearExpression *>(pthis->rowExpression(v));
    }


  bool FIsBasicVar(ClVariable v)
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
  ClVarSet _infeasibleRows;

  // the set of rows where the basic variable is external
  // this was added to the C++ version to reduce time in setExternalVariables()
  ClVarSet _externalRows;

  // the set of external variables which are parametric
  // this was added to the C++ version to reduce time in setExternalVariables()
  ClVarSet _externalParametricVars;

};

#endif
