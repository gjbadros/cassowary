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
    my_columns = new Hashtable();
    my_rows = new Hashtable();
    my_infeasibleRows = new Set();
    my_externalRows = new Set();
    my_externalParametricVars = new Set();
  }

  public final void noteRemovedVariable(ClAbstractVariable v, ClAbstractVariable subject)
    { 
      if (fTraceOn) fnenterprint("noteRemovedVariable: " + v + ", " + subject);
      if (subject != null) {
	((Set) my_columns.get(v)).remove(subject);
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
    retstr.append("Rows: " + my_rows.size());
    retstr.append(" (= " + (my_rows.size() - 1) + " constraints)");
    retstr.append("\nColumns: " + my_columns.size());
    retstr.append("\nInfeasible Rows: " + my_infeasibleRows.size());
    retstr.append("\nExternal basic variables: " + my_externalRows.size());
    retstr.append("\nExternal parametric variables: ");
    retstr.append(my_externalParametricVars.size());
    retstr.append("\n");
    
    return retstr.toString();
  }

  public String toString()
    { 
      StringBuffer bstr = new StringBuffer("Tableau:\n");
      for (Enumeration e = my_rows.keys(); e.hasMoreElements(); ) {
	ClAbstractVariable clv = (ClAbstractVariable) e.nextElement();
	ClLinearExpression expr = (ClLinearExpression) my_rows.get(clv);
	bstr.append(clv.toString());
	bstr.append(" <==> ");
	bstr.append(expr.toString());
	bstr.append("\n");
      }

      bstr.append("\nColumns:\n");
      bstr.append(my_columns.toString());
      
      bstr.append("\nInfeasible rows: ");
      bstr.append(my_infeasibleRows.toString());

      bstr.append("External basic variables: ");
      bstr.append(my_externalRows.toString());

      bstr.append("External parametric variables: ");
      bstr.append(my_externalParametricVars.toString());
      
      return bstr.toString();
    }

  private final void insertColVar(ClAbstractVariable param_var, 
			    ClAbstractVariable rowvar)
  { 
    Set rowset = (Set) my_columns.get(param_var);
    if (rowset == null)
      my_columns.put(param_var,rowset = new Set());
    rowset.insert(rowvar);
  }

  protected final void addRow(ClAbstractVariable var, ClLinearExpression expr)
    {
      if (fTraceOn) fnenterprint("addRow: " + var + ", " + expr);

      // for each variable in expr, add var to the set of rows which
      // have that variable in their expression
      my_rows.put(var,expr);
      
      for (Enumeration e = expr.terms().keys() ; e.hasMoreElements(); ) {
        ClAbstractVariable clv = (ClAbstractVariable) e.nextElement();
	insertColVar(clv,var);
      }
      if (var.isExternal()) {
	my_externalRows.insert(var);
      }
      if (fTraceOn) traceprint(this.toString());
    }

  protected final void removeColumn(ClAbstractVariable var)
    {
      if (fTraceOn) fnenterprint("removeColumn:" + var);
      // remove the rows with the variables in varset

      Set rows = (Set) my_columns.remove(var);

      if (rows != null) {
        for (Enumeration e = rows.elements() ; e.hasMoreElements(); ) {
          ClAbstractVariable clv = (ClAbstractVariable) e.nextElement();
          ClLinearExpression expr = (ClLinearExpression) my_rows.get(clv);
          expr.terms().remove(var);
        }
      } else {
        if (fTraceOn) debugprint("Could not find var " + var + " in my_columns");
      }
      
      if (var.isExternal()) {
	my_externalRows.remove(var);
	my_externalParametricVars.remove(var);
      }
    }

  protected final ClLinearExpression removeRow(ClAbstractVariable var)
       throws ExCLInternalError
    {
      if (fTraceOn) fnenterprint("removeRow:" + var);

      ClLinearExpression expr = (ClLinearExpression) my_rows.get(var);
      assert(expr != null);

      // For each variable in this expression, update
      // the column mapping and remove the variable from the list
      // of rows it is known to be in
      for (Enumeration e = expr.terms().keys() ; e.hasMoreElements(); ) {
        ClAbstractVariable clv = (ClAbstractVariable) e.nextElement();
	Set varset = (Set) my_columns.get(clv);
	if (varset != null) {
          if (fTraceOn) debugprint("removing from varset " + var);
	  varset.remove(var);
        }
      }
      
      my_infeasibleRows.remove(var);

      if (var.isExternal()) {
	my_externalRows.remove(var);
      }
      my_rows.remove(var);
      if (fTraceOn) fnexitprint("returning " + expr);
      return expr;
    }

  protected final void substituteOut(ClAbstractVariable oldVar, ClLinearExpression expr)
    {
      if (fTraceOn) fnenterprint("substituteOut:" + oldVar + ", " + expr);
      if (fTraceOn) traceprint(this.toString());
      
      Set varset = (Set) my_columns.get(oldVar);
      for (Enumeration e = varset.elements(); e.hasMoreElements(); ) {
	ClAbstractVariable v = (ClAbstractVariable) e.nextElement();
	ClLinearExpression row = (ClLinearExpression) my_rows.get(v);
	row.substituteOut(oldVar,expr,v,this);
	if (v.isRestricted() && row.constant() < 0.0) {
	  my_infeasibleRows.insert(v);
	}
      }

      if (oldVar.isExternal()) {
	my_externalRows.insert(oldVar);
	my_externalParametricVars.remove(oldVar);
      }
      my_columns.remove(oldVar);
    }

  protected final Hashtable columns()
    { return my_columns; }

  protected final Hashtable rows()
    { return my_rows; }

  protected final boolean columnsHasKey(ClAbstractVariable subject)
    { 
      return my_columns.containsKey(subject);
    }

  protected final ClLinearExpression rowExpression(ClAbstractVariable v)
    {
      // if (fTraceOn) fnenterprint("rowExpression:" + v);
      return (ClLinearExpression) my_rows.get(v);
    }

  protected Hashtable my_columns; // From ClAbstractVariable to Set of variables
  protected Hashtable my_rows;    // From ClAbstractVariable to ClLinearExpression
  protected Set my_infeasibleRows; // Set of ClAbstractVariable-s
  protected Set my_externalRows; // Set of ClVariable-s
  protected Set my_externalParametricVars; // Set of ClVariable-s

}
