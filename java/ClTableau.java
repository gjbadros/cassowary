// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This Java Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998, All Rights Reserved.
//
// (c) 1998 Alan Borning and Greg Badros.  This code is provided for use by
// students for course projects in the course CSE 595 in the Department of
// Computer Science and Engineering, University of Washington, during winter
// quarter 1998.  Any other use requires written permission from the copyright
// holders.
//
// ClTableau

import java.util.*;

class ClTableau
{

  public void noteRemovedVariable(ClAbstractVariable v, ClAbstractVariable subject)
    { 
      //#ifndef CL_NO_TRACE
      //Tracer TRACER(__FUNCTION__);
      /** @c2j++ Replacement from cerr << "(" << v << ", " << subject << ")" << endl; */
      //System.err.println("(" + String.valueOf(v) + ", " + String.valueOf(subject) + ")");
      //#endif
      if (subject != null) {
	((Set) my_columns.get(v)).remove(subject);
      }
    }

  public void noteAddedVariable(ClAbstractVariable v, ClAbstractVariable subject)
    { 
      //#ifndef CL_NO_TRACE
      //Tracer TRACER(__FUNCTION__);
      // System.err.println("(" + String.valueOf(v) + ", " + String.valueOf(subject) + ")");
      //#endif
      if (subject != null) {
	insertColVar(v,subject);
      }
    }
  
  public String toString()
    { 
      StringBuffer bstr = new StringBuffer("Tableau:\n");
      bstr.append(my_rows.toString());

      bstr.append("Columns:\n");
      bstr.append(my_columns.toString());
      
      bstr.append("Infeasible rows:\n");
      bstr.append(my_infeasibleRows.toString());

      bstr.append("External basic variables:\n");
      bstr.append(my_externalRows.toString());

      bstr.append("External parametric variables:\n");
      bstr.append(my_externalParametricVars.toString());
      
      return bstr.toString();
    }

  private void insertColVar(ClAbstractVariable param_var, 
			    ClAbstractVariable rowvar)
  { 
    Set rowset = (Set) my_columns.get(param_var);
    if (rowset == null)
      my_columns.put(param_var,rowset = new Set());
    rowset.insert(rowvar);
  }

  protected void addRow(ClAbstractVariable var, ClLinearExpression expr)
    {
      //#ifndef CL_NO_TRACE
      // Tracer TRACER(__FUNCTION__);
      // System.err.println("(" + String.valueOf(var) + ", " + String.valueOf(expr) + ")");
      //#endif

      // for each variable in expr, add var to the set of rows which
      // have that variable in their expression

      for (Enumeration e = expr.terms().keys() ; e.hasMoreElements(); ) {
        ClVariable clv = (ClVariable) e.nextElement();
	insertColVar(clv,var);
      }
      if (var.isExternal()) {
	my_externalRows.insert(var);
      }
      //#ifndef CL_NO_TRACE
      //System.err.println(String.valueOf(*this));
      //#endif
    }

  protected void removeColumn(ClAbstractVariable var)
    {
      //#ifndef CL_NO_TRACE
      //Tracer TRACER(__FUNCTION__);
      //System.err.println("(" + String.valueOf(var) + ")");
      //#endif
      // remove the rows with the variables in varset

      Set rows = (Set) my_columns.remove(var);

      for (Enumeration e = rows.elements() ; e.hasMoreElements(); ) {
        ClVariable clv = (ClVariable) e.nextElement();
	ClLinearExpression expr = (ClLinearExpression) my_rows.get(clv);
	expr.terms().remove(clv);
      }
      
      if (var.isExternal()) {
	my_externalRows.remove(var);
	my_externalParametricVars.remove(var);
      }
    }

  protected ClLinearExpression removeRow(ClAbstractVariable var)
    {
      //#ifndef CL_NO_TRACE
      //  Tracer TRACER(__FUNCTION__);
      //System.err.println("(" + var.toString() + ")");
      //#endif

      ClLinearExpression expr = (ClLinearExpression) my_rows.remove(var);

      for (Enumeration e = expr.terms().keys() ; e.hasMoreElements(); ) {
        ClVariable clv = (ClVariable) e.nextElement();
	((Set)my_columns.get(clv)).remove(var);
	my_infeasibleRows.remove(var);
      }

      if (var.isExternal())
	{
	my_externalRows.remove(var);
	}
      //#ifndef CL_NO_TRACE
      //System.err.println("- returning " + expr.toString());
      //#endif
      return expr;
    }

  protected void substituteOut(ClAbstractVariable oldVar, ClLinearExpression expr)
    {
      //#ifndef CL_NO_TRACE
      //System.err.print("* ClTableau::");
      //Tracer TRACER(__FUNCTION__);
      //System.err.println("(" + String.valueOf(oldVar) + ", " + String.valueOf(expr) + ")");
      //System.err.println(String.valueOf((*this)));
      //#endif
      
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

  protected Hashtable columns()
    { return my_columns; }

  protected Hashtable rows()
    { return my_rows; }

  protected boolean columnsHasKey(ClAbstractVariable subject)
    { 
      return my_columns.containsKey(subject);
    }

  protected ClLinearExpression rowExpression(ClAbstractVariable v)
    {
      return (ClLinearExpression) my_rows.get(v);
    }

  private Hashtable my_columns; // From ClAbstractVariable to Set of variables
  private Hashtable my_rows;    // From ClAbstractVariable to ClLinearExpression
  private Set my_infeasibleRows; // Set of ClAbstractVariable-s
  private Set my_externalRows; // Set of ClVariable-s
  private Set my_externalParametricVars; // Set of ClVariable-s

}
