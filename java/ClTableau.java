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

class ClTableau
{

  public void noteRemovedVariable(ClAbstractVariable v, ClAbstractVariable subject)
    { 
      //#ifndef CL_NO_TRACE
      //Tracer TRACER(__FUNCTION__);
      /** @c2j++ Replacement from cerr << "(" << v << ", " << subject << ")" << endl; */
      //System.err.println("(" + String.valueOf(v) + ", " + String.valueOf(subject) + ")");
      //#endif
      ClTableauVarSet::const_iterator it = my_columns[&v].find(&subject);
      assert(it != my_columns[&v].end());
      my_columns[&v].erase(it); 
    }

  public void noteAddedVariable(ClAbstractVariable v, ClAbstractVariable subject)
    { 
      //#ifndef CL_NO_TRACE
      //Tracer TRACER(__FUNCTION__);
      // System.err.println("(" + String.valueOf(v) + ", " + String.valueOf(subject) + ")");
      //#endif
      my_columns[&v].insert(&subject); 
      if (v.isExternal())
	{
	my_externalParametricVars.insert(static_cast<static final ClVariable  >(&v));
	}
    }
  
  public String toString()
    { 
      StringBuffer bstr("Tableau:\n");
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

  protected void addRow(ClAbstractVariable var, ClLinearExpression expr)
    {
      //#ifndef CL_NO_TRACE
      // Tracer TRACER(__FUNCTION__);
      // System.err.println("(" + String.valueOf(var) + ", " + String.valueOf(expr) + ")");
      //#endif
      my_rows[&var] = const_cast<ClLinearExpression  >(&expr);
      ClVarToNumberMap::const_iterator it = expr.terms().begin();
      for (; it != expr.terms().end(); ++it)
	{
	static final ClAbstractVariable  pv = ( it).first;
	my_columns[pv].insert(&var);
	}
      if (var.isExternal())
	{
	my_externalRows.insert(static_cast<static final ClVariable  >(&var));
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
      ClTableauColumnsMap::iterator it_var = my_columns.find(&var);
      ClTableauVarSet &varset = ( it_var).second;
      ClTableauVarSet::iterator it = varset.begin();
      for (; it != varset.end(); ++it)
	{
	static final ClAbstractVariable  pv = ( it);
	ClVarToNumberMap &terms = my_rows[pv].terms();
	terms.erase(terms.find(&var));
	}
      if (var.isExternal())
	{
	my_externalRows.erase(static_cast<static final ClVariable  >(&var));
	my_externalParametricVars.erase(static_cast<static final ClVariable  >(&var));
	}
      my_columns.erase(it_var);
    }

  protected ClLinearExpression removeRow(ClAbstractVariable var)
    {
      //#ifndef CL_NO_TRACE
      //  Tracer TRACER(__FUNCTION__);
      //System.err.println("(" + String.valueOf(var) + ")");
      //#endif

      ClTableauRowsMap::iterator it = my_rows.find(&var);

      // assert(it != my_rows.end());
      //  ClLinearExpression  pexpr = ( it).second;
      ClVarToNumberMap &terms = pexpr.terms();
      ClVarToNumberMap::iterator it_term = terms.begin();
      for (; it_term != terms.end(); ++it_term)
	{
	static final ClAbstractVariable  pv = ( it_term).first;
	my_columns[pv].erase(&var);
	}
      
      my_infeasibleRows.erase(&var);
      
      if (var.isExternal())
	{
	my_externalRows.erase(static_cast<static final ClVariable  >(&var));
	}
      my_rows.erase(it);
      //#ifndef CL_NO_TRACE
      //System.err.println("- returning " + String.valueOf(*pexpr));
      //#endif
      return pexpr;
    }

  protected void substituteOut(ClAbstractVariable oldVar, ClLinearExpression expr)
    {
      //#ifndef CL_NO_TRACE
      //System.err.print("* ClTableau::");
      //Tracer TRACER(__FUNCTION__);
      //System.err.println("(" + String.valueOf(oldVar) + ", " + String.valueOf(expr) + ")");
      //System.err.println(String.valueOf((*this)));
      //#endif

      ClTableauColumnsMap::iterator it_oldVar = my_columns.find(&oldVar);
      assert(it_oldVar != my_columns.end());
      ClTableauVarSet &varset = ( it_oldVar).second;
      ClTableauVarSet::iterator it = varset.begin();
      for (; it != varset.end(); ++it)
	{
	static final ClAbstractVariable  pv = ( it);
	ClLinearExpression  prow = my_rows[pv];
	prow.substituteOut(oldVar,expr, pv, this);
	if (pv.isRestricted() && prow.constant() < 0.0)
	  {
	  my_infeasibleRows.insert(pv);
	  }
	}
      if (oldVar.isExternal())
	{
	my_externalRows.insert(static_cast<static final ClVariable  >(&oldVar));
	my_externalParametricVars.erase(static_cast<static final ClVariable  >(&oldVar));
	}
      my_columns.erase(it_oldVar);
    }

  protected ClTableauColumnsMap columns()
    { return my_columns; }

  protected ClTableauRowsMap rows()
    { return my_rows; }

  protected boolean columnsHasKey(ClAbstractVariable subject)
    { 
      ClTableauColumnsMap::const_iterator i = my_columns.find(&subject);
      return (i != my_columns.end());
    }

  protected ClLinearExpression rowExpression(ClAbstractVariable v)
    {
      ClTableauRowsMap::const_iterator i = my_rows.find(&v);
      if (i != my_rows.end())
	return ( i).second;
      else
	return null;
    }

  private Hashtable my_columns; // From ClAbstractVariable to Set of variables
  private Hashtable my_rows;    // From ClAbstractVariable to ClLinearExpression
  private Set my_infeasibleRows; // Set of ClAbstractVariable-s
  private Set my_externalRows; // Set of ClVariable-s
  private Set my_externalParametricVars; // Set of ClVariable-s

}
