// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This Java Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998 Alan Borning and Greg Badros
// See ../COPYRIGHT for legal details regarding this software
//
// ClTableau

import java.util.*;

class ClTableau extends CL
{

  public ClTableau()
  {
    _columns = new Hashtable();
    _rows = new Hashtable();
    _infeasibleRows = new Set();
    _externalRows = new Set();
    _externalParametricVars = new Set();
  }

  public final void noteRemovedVariable(ClAbstractVariable v, ClAbstractVariable subject)
    { 
      if (fTraceOn) fnenterprint("noteRemovedVariable: " + v + ", " + subject);
      if (subject != null) {
	((Set) _columns.get(v)).remove(subject);
      }
    }

  public final void noteAddedVariable(ClAbstractVariable v, ClAbstractVariable subject)
    { 
      if (fTraceOn) fnenterprint("noteAddedVariable: " + v + ", " + subject);
      if (subject != null) {
	insertColVar(v,subject);
      }
    }
  
  // from Michael Noth <noth@cs>
  public String getInternalInfo() {
    StringBuffer retstr = new StringBuffer("Tableau Information:\n");
    retstr.append("Rows: " + _rows.size());
    retstr.append(" (= " + (_rows.size() - 1) + " constraints)");
    retstr.append("\nColumns: " + _columns.size());
    retstr.append("\nInfeasible Rows: " + _infeasibleRows.size());
    retstr.append("\nExternal basic variables: " + _externalRows.size());
    retstr.append("\nExternal parametric variables: ");
    retstr.append(_externalParametricVars.size());
    retstr.append("\n");
    
    return retstr.toString();
  }

  public String toString()
    { 
      StringBuffer bstr = new StringBuffer("Tableau:\n");
      for (Enumeration e = _rows.keys(); e.hasMoreElements(); ) {
	ClAbstractVariable clv = (ClAbstractVariable) e.nextElement();
	ClLinearExpression expr = (ClLinearExpression) _rows.get(clv);
	bstr.append(clv.toString());
	bstr.append(" <==> ");
	bstr.append(expr.toString());
	bstr.append("\n");
      }

      bstr.append("\nColumns:\n");
      bstr.append(_columns.toString());
      
      bstr.append("\nInfeasible rows: ");
      bstr.append(_infeasibleRows.toString());

      bstr.append("External basic variables: ");
      bstr.append(_externalRows.toString());

      bstr.append("External parametric variables: ");
      bstr.append(_externalParametricVars.toString());
      
      return bstr.toString();
    }

  private final void insertColVar(ClAbstractVariable param_var, 
			    ClAbstractVariable rowvar)
  { 
    Set rowset = (Set) _columns.get(param_var);
    if (rowset == null)
      _columns.put(param_var,rowset = new Set());
    rowset.insert(rowvar);
  }

  protected final void addRow(ClAbstractVariable var, ClLinearExpression expr)
    {
      if (fTraceOn) fnenterprint("addRow: " + var + ", " + expr);

      // for each variable in expr, add var to the set of rows which
      // have that variable in their expression
      _rows.put(var,expr);
      
      for (Enumeration e = expr.terms().keys() ; e.hasMoreElements(); ) {
        ClAbstractVariable clv = (ClAbstractVariable) e.nextElement();
	insertColVar(clv,var);
      }
      if (var.isExternal()) {
	_externalRows.insert(var);
      }
      if (fTraceOn) traceprint(this.toString());
    }

  protected final void removeColumn(ClAbstractVariable var)
    {
      if (fTraceOn) fnenterprint("removeColumn:" + var);
      // remove the rows with the variables in varset

      Set rows = (Set) _columns.remove(var);

      if (rows != null) {
        for (Enumeration e = rows.elements() ; e.hasMoreElements(); ) {
          ClAbstractVariable clv = (ClAbstractVariable) e.nextElement();
          ClLinearExpression expr = (ClLinearExpression) _rows.get(clv);
          expr.terms().remove(var);
        }
      } else {
        if (fTraceOn) debugprint("Could not find var " + var + " in _columns");
      }
      
      if (var.isExternal()) {
	_externalRows.remove(var);
	_externalParametricVars.remove(var);
      }
    }

  protected final ClLinearExpression removeRow(ClAbstractVariable var)
       throws ExCLInternalError
    {
      if (fTraceOn) fnenterprint("removeRow:" + var);

      ClLinearExpression expr = (ClLinearExpression) _rows.get(var);
      assert(expr != null);

      // For each variable in this expression, update
      // the column mapping and remove the variable from the list
      // of rows it is known to be in
      for (Enumeration e = expr.terms().keys() ; e.hasMoreElements(); ) {
        ClAbstractVariable clv = (ClAbstractVariable) e.nextElement();
	Set varset = (Set) _columns.get(clv);
	if (varset != null) {
          if (fTraceOn) debugprint("removing from varset " + var);
	  varset.remove(var);
        }
      }
      
      _infeasibleRows.remove(var);

      if (var.isExternal()) {
	_externalRows.remove(var);
      }
      _rows.remove(var);
      if (fTraceOn) fnexitprint("returning " + expr);
      return expr;
    }

  protected final void substituteOut(ClAbstractVariable oldVar, ClLinearExpression expr)
    {
      if (fTraceOn) fnenterprint("substituteOut:" + oldVar + ", " + expr);
      if (fTraceOn) traceprint(this.toString());
      
      Set varset = (Set) _columns.get(oldVar);
      for (Enumeration e = varset.elements(); e.hasMoreElements(); ) {
	ClAbstractVariable v = (ClAbstractVariable) e.nextElement();
	ClLinearExpression row = (ClLinearExpression) _rows.get(v);
	row.substituteOut(oldVar,expr,v,this);
	if (v.isRestricted() && row.constant() < 0.0) {
	  _infeasibleRows.insert(v);
	}
      }

      if (oldVar.isExternal()) {
	_externalRows.insert(oldVar);
	_externalParametricVars.remove(oldVar);
      }
      _columns.remove(oldVar);
    }

  protected final Hashtable columns()
    { return _columns; }

  protected final Hashtable rows()
    { return _rows; }

  protected final boolean columnsHasKey(ClAbstractVariable subject)
    { 
      return _columns.containsKey(subject);
    }

  protected final ClLinearExpression rowExpression(ClAbstractVariable v)
    {
      // if (fTraceOn) fnenterprint("rowExpression:" + v);
      return (ClLinearExpression) _rows.get(v);
    }

  protected Hashtable _columns; // From ClAbstractVariable to Set of variables
  protected Hashtable _rows;    // From ClAbstractVariable to ClLinearExpression
  protected Set _infeasibleRows; // Set of ClAbstractVariable-s
  protected Set _externalRows; // Set of ClVariable-s
  protected Set _externalParametricVars; // Set of ClVariable-s

}
