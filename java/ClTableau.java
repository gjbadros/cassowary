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

/**
* noteRemovedVariable
* @param v
* @param subject
*/
public
void noteRemovedVariable(ClAbstractVariable& v, ClAbstractVariable& subject)
{ 
#ifndef CL_NO_TRACE
    Tracer TRACER(__FUNCTION__);
/** @c2j++ Replacement from cerr << "(" << v << ", " << subject << ")" << endl; */
System.err.println("(" + String.valueOf(v) + ", " + String.valueOf(subject) + ")");
#endif
    ClTableauVarSet::const_iterator it = my_columns[&v].find(&subject);
    assert(it != my_columns[&v].end());
    my_columns[&v].erase(it); 
    }

/**
* noteAddedVariable
* @param v
* @param subject
*/
public
void noteAddedVariable(ClAbstractVariable& v, ClAbstractVariable& subject)
{ 
#ifndef CL_NO_TRACE
    Tracer TRACER(__FUNCTION__);
/** @c2j++ Replacement from cerr << "(" << v << ", " << subject << ")" << endl; */
System.err.println("(" + String.valueOf(v) + ", " + String.valueOf(subject) + ")");
#endif
    my_columns[&v].insert(&subject); 
    if (v.isExternal())
      {
/* @c2j++: "my_externalParametricVars.insert(static_cast<const ClVariable *>(&v));" replacement: * to " " */
/* @c2j++: "my_externalParametricVars.insert(static_cast<const ClVariable  >(&v));" replacement: const  to static final  */
      my_externalParametricVars.insert(static_cast<static final ClVariable  >(&v));
      }
    }

/**
* printOn
* @param xo
* @return ostream&
*/
public
ostream& printOn(ostream& xo)
{
  xo << "Tableau:\n" 
     << my_rows << endl;
  xo << "Columns:\n" 
     << my_columns << endl;
  xo << "Infeasible rows: " 
     << my_infeasibleRows << endl;
  xo << "External basic variables: "
     << my_externalRows << endl;
  xo << "External parametric variables: "
     << my_externalParametricVars << endl;
  return xo;
}

/**
* ClTableau
*/
protected
ClTableau()
{ }

/**
* finalize
* @return 
*/
 void finalize() {
#ifndef CL_NO_TRACE
/** @c2j++ Replacement from cerr << *this << endl; */
System.err.println(String.valueOf(*this));
#endif
  ClTableauRowsMap::iterator it = my_rows.begin();
  for (; it != my_rows.end(); ++it)
    {
#ifndef CL_NO_TRACE
/** @c2j++ Replacement from cerr << "Deleting row  delete@ " << ((*it).second) << endl; */
System.err.println("Deleting row  delete@ " + String.valueOf(((*it).second)));
#endif
/** @c2j++ Replacement from delete (*it).second; */
(*it).second = null;
    }
  
}

/**
* addRow
* @param var
* @param expr
*/
protected
void addRow(ClAbstractVariable& var, ClLinearExpression& expr)
{
#ifndef CL_NO_TRACE
  Tracer TRACER(__FUNCTION__);
/** @c2j++ Replacement from cerr << "(" << var << ", " << expr << ")" << endl; */
System.err.println("(" + String.valueOf(var) + ", " + String.valueOf(expr) + ")");
#endif
/* @c2j++: "my_rows[&var] = const_cast<ClLinearExpression *>(&expr);" replacement: * to " " */
  my_rows[&var] = const_cast<ClLinearExpression  >(&expr);
  ClVarToNumberMap::const_iterator it = expr.terms().begin();
  for (; it != expr.terms().end(); ++it)
    {
/* @c2j++: "const ClAbstractVariable *pv = (*it).first;" replacement: * to " " */
/* @c2j++: "const ClAbstractVariable  pv = (*it).first;" replacement: * to " " */
/* @c2j++: "const ClAbstractVariable  pv = ( it).first;" replacement: const  to static final  */
    static final ClAbstractVariable  pv = ( it).first;
    my_columns[pv].insert(&var);
    }
  if (var.isExternal())
    {
/* @c2j++: "my_externalRows.insert(static_cast<const ClVariable *>(&var));" replacement: * to " " */
/* @c2j++: "my_externalRows.insert(static_cast<const ClVariable  >(&var));" replacement: const  to static final  */
    my_externalRows.insert(static_cast<static final ClVariable  >(&var));
    }
#ifndef CL_NO_TRACE
/** @c2j++ Replacement from cerr << *this << endl; */
System.err.println(String.valueOf(*this));
#endif
}

/**
* removeColumn
* @param var
*/
protected
void removeColumn(ClAbstractVariable& var)
{
#ifndef CL_NO_TRACE
  Tracer TRACER(__FUNCTION__);
/** @c2j++ Replacement from cerr << "(" << var << ")" << endl; */
System.err.println("(" + String.valueOf(var) + ")");
#endif
  ClTableauColumnsMap::iterator it_var = my_columns.find(&var);
/* @c2j++: "ClTableauVarSet &varset = (*it_var).second;" replacement: * to " " */
  ClTableauVarSet &varset = ( it_var).second;
  ClTableauVarSet::iterator it = varset.begin();
  for (; it != varset.end(); ++it)
    {
/* @c2j++: "const ClAbstractVariable *pv = (*it);" replacement: * to " " */
/* @c2j++: "const ClAbstractVariable  pv = (*it);" replacement: * to " " */
/* @c2j++: "const ClAbstractVariable  pv = ( it);" replacement: const  to static final  */
    static final ClAbstractVariable  pv = ( it);
/* @c2j++: "ClVarToNumberMap &terms = my_rows[pv]->terms();" replacement: -> to . */
    ClVarToNumberMap &terms = my_rows[pv].terms();
    terms.erase(terms.find(&var));
    }
  if (var.isExternal())
    {
/* @c2j++: "my_externalRows.erase(static_cast<const ClVariable *>(&var));" replacement: * to " " */
/* @c2j++: "my_externalRows.erase(static_cast<const ClVariable  >(&var));" replacement: const  to static final  */
    my_externalRows.erase(static_cast<static final ClVariable  >(&var));
/* @c2j++: "my_externalParametricVars.erase(static_cast<const ClVariable *>(&var));" replacement: * to " " */
/* @c2j++: "my_externalParametricVars.erase(static_cast<const ClVariable  >(&var));" replacement: const  to static final  */
    my_externalParametricVars.erase(static_cast<static final ClVariable  >(&var));
    }
  my_columns.erase(it_var);
}

/**
* removeRow
* @param var
* @return ClLinearExpression*
*/
protected
/* @c2j++: "ClLinearExpression* removeRow(ClAbstractVariable& var)" replacement: * to " " */
ClLinearExpression  removeRow(ClAbstractVariable& var)
{
#ifndef CL_NO_TRACE
  Tracer TRACER(__FUNCTION__);
/** @c2j++ Replacement from cerr << "(" << var << ")" << endl; */
System.err.println("(" + String.valueOf(var) + ")");
#endif
  ClTableauRowsMap::iterator it = my_rows.find(&var);
  assert(it != my_rows.end());
/* @c2j++: "ClLinearExpression *pexpr = (*it).second;" replacement: * to " " */
/* @c2j++: "ClLinearExpression  pexpr = (*it).second;" replacement: * to " " */
  ClLinearExpression  pexpr = ( it).second;
/* @c2j++: "ClVarToNumberMap &terms = pexpr->terms();" replacement: -> to . */
  ClVarToNumberMap &terms = pexpr.terms();
  ClVarToNumberMap::iterator it_term = terms.begin();
  for (; it_term != terms.end(); ++it_term)
    {
/* @c2j++: "const ClAbstractVariable *pv = (*it_term).first;" replacement: * to " " */
/* @c2j++: "const ClAbstractVariable  pv = (*it_term).first;" replacement: * to " " */
/* @c2j++: "const ClAbstractVariable  pv = ( it_term).first;" replacement: const  to static final  */
    static final ClAbstractVariable  pv = ( it_term).first;
    my_columns[pv].erase(&var);
    }

  my_infeasibleRows.erase(&var);

  if (var.isExternal())
    {
/* @c2j++: "my_externalRows.erase(static_cast<const ClVariable *>(&var));" replacement: * to " " */
/* @c2j++: "my_externalRows.erase(static_cast<const ClVariable  >(&var));" replacement: const  to static final  */
    my_externalRows.erase(static_cast<static final ClVariable  >(&var));
    }
  my_rows.erase(it);
#ifndef CL_NO_TRACE
/** @c2j++ Replacement from cerr << "- returning " << *pexpr << endl; */
System.err.println("- returning " + String.valueOf(*pexpr));
#endif
  return pexpr;
}

/**
* substituteOut
* @param oldVar
* @param expr
*/
protected
void substituteOut(ClAbstractVariable& oldVar, ClLinearExpression& expr)
{
#ifndef CL_NO_TRACE
/** @c2j++ Replacement from cerr << "* ClTableau::"; */
System.err.print("* ClTableau::");
  Tracer TRACER(__FUNCTION__);
/** @c2j++ Replacement from cerr << "(" << oldVar << ", " << expr << ")" << endl; */
System.err.println("(" + String.valueOf(oldVar) + ", " + String.valueOf(expr) + ")");
/** @c2j++ Replacement from cerr << (*this) << endl; */
System.err.println(String.valueOf((*this)));
#endif

  ClTableauColumnsMap::iterator it_oldVar = my_columns.find(&oldVar);
  assert(it_oldVar != my_columns.end());
/* @c2j++: "ClTableauVarSet &varset = (*it_oldVar).second;" replacement: * to " " */
  ClTableauVarSet &varset = ( it_oldVar).second;
  ClTableauVarSet::iterator it = varset.begin();
  for (; it != varset.end(); ++it)
    {
/* @c2j++: "const ClAbstractVariable *pv = (*it);" replacement: * to " " */
/* @c2j++: "const ClAbstractVariable  pv = (*it);" replacement: * to " " */
/* @c2j++: "const ClAbstractVariable  pv = ( it);" replacement: const  to static final  */
    static final ClAbstractVariable  pv = ( it);
/* @c2j++: "ClLinearExpression *prow = my_rows[pv];" replacement: * to " " */
    ClLinearExpression  prow = my_rows[pv];
/* @c2j++: "prow->substituteOut(oldVar,expr,*pv,*this);" replacement: * to " " */
/* @c2j++: "prow->substituteOut(oldVar,expr, pv,*this);" replacement: * to " " */
/* @c2j++: "prow->substituteOut(oldVar,expr, pv, this);" replacement: -> to . */
    prow.substituteOut(oldVar,expr, pv, this);
/* @c2j++: "if (pv->isRestricted() && prow->constant() < 0.0)" replacement: -> to . */
/* @c2j++: "if (pv.isRestricted() && prow->constant() < 0.0)" replacement: -> to . */
    if (pv.isRestricted() && prow.constant() < 0.0)
      {
      my_infeasibleRows.insert(pv);
      }
    }
  if (oldVar.isExternal())
    {
/* @c2j++: "my_externalRows.insert(static_cast<const ClVariable *>(&oldVar));" replacement: * to " " */
/* @c2j++: "my_externalRows.insert(static_cast<const ClVariable  >(&oldVar));" replacement: const  to static final  */
    my_externalRows.insert(static_cast<static final ClVariable  >(&oldVar));
/* @c2j++: "my_externalParametricVars.erase(static_cast<const ClVariable *>(&oldVar));" replacement: * to " " */
/* @c2j++: "my_externalParametricVars.erase(static_cast<const ClVariable  >(&oldVar));" replacement: const  to static final  */
    my_externalParametricVars.erase(static_cast<static final ClVariable  >(&oldVar));
    }
  my_columns.erase(it_oldVar);
}

/**
* columns
* @return ClTableauColumnsMap
*/
protected
ClTableauColumnsMap columns()
{ return my_columns; }

/**
* rows
* @return ClTableauRowsMap
*/
protected
ClTableauRowsMap rows()
{ return my_rows; }

/**
* columnsHasKey
* @param subject
* @return boolean
*/
protected
boolean columnsHasKey(ClAbstractVariable& subject)
{ 
    ClTableauColumnsMap::const_iterator i = my_columns.find(&subject);
    return (i != my_columns.end());
    }

/**
* rowExpression
* @param v
* @return ClLinearExpression *
*/
protected
/* @c2j++: "ClLinearExpression * rowExpression(ClAbstractVariable& v)" replacement: * to " " */
ClLinearExpression   rowExpression(ClAbstractVariable& v)
{
    ClTableauRowsMap::const_iterator i = my_rows.find(&v);
    if (i != my_rows.end())
/* @c2j++: "return (*i).second;" replacement: * to " " */
      return ( i).second;
    else
/* @c2j++: "return NULL;" replacement: NULL to null */
      return null;
    }
ClTableauColumnsMap my_columns;
ClTableauRowsMap my_rows;
ClTableauVarSet my_infeasibleRows;
ClExternalVarSet my_externalRows;
ClExternalVarSet my_externalParametricVars;

/*@c2j++ The following variable used to be declared global */
/* @c2j++: "typedef set < static final ClAbstractVariable * > ClTableauVarSet ;" replacement: * to " " */
typedef set < static final ClAbstractVariable   > ClTableauVarSet ; 

/*@c2j++ The following variable used to be declared global */
/* @c2j++: "typedef set < static final ClVariable * > ClExternalVarSet ;" replacement: * to " " */
typedef set < static final ClVariable   > ClExternalVarSet ; 

/*@c2j++ The following variable used to be declared global */
/* @c2j++: "typedef map < static final ClAbstractVariable * , ClTableauVarSet > ClTableauColumnsMap ;" replacement: * to " " */
typedef map < static final ClAbstractVariable   , ClTableauVarSet > ClTableauColumnsMap ; 

/*@c2j++ The following variable used to be declared global */
/* @c2j++: "typedef map < static final ClAbstractVariable * , ClLinearExpression * > ClTableauRowsMap ;" replacement: * to " " */
/* @c2j++: "typedef map < static final ClAbstractVariable   , ClLinearExpression * > ClTableauRowsMap ;" replacement: * to " " */
typedef map < static final ClAbstractVariable   , ClLinearExpression   > ClTableauRowsMap ; 

/**
* @param xo
* @param varset
* @return ostream&
*/
static ostream& printTo(ostream& xo, ClTableauVarSet& varset)
{
  ClTableauVarSet::const_iterator it = varset.begin();
  xo << "{ ";
  if (it != varset.end())
    {
/* @c2j++: "xo << *(*it);" replacement: * to " " */
/* @c2j++: "xo <<  (*it);" replacement: * to " " */
    xo <<  ( it);
    ++it;
    }
  for (; it != varset.end(); ++it) 
    {
/* @c2j++: "xo << ", " << *(*it);" replacement: * to " " */
/* @c2j++: "xo << ", " <<  (*it);" replacement: * to " " */
    xo << ", " <<  ( it);
    }
  xo << " }";
  return xo;
}
/**
* @param xo
* @param varset
* @return <
*/
static < <(ostream& xo, ClTableauVarSet& varset)
{ return printTo(xo,varset); }
/**
* @param xo
* @param varset
* @return ostream&
*/
static ostream& printTo(ostream& xo, ClExternalVarSet& varset)
{
  ClExternalVarSet::const_iterator it = varset.begin();
  xo << "{ ";
  if (it != varset.end())
    {
/* @c2j++: "xo << *(*it);" replacement: * to " " */
/* @c2j++: "xo <<  (*it);" replacement: * to " " */
    xo <<  ( it);
    ++it;
    }
  for (; it != varset.end(); ++it) 
    {
/* @c2j++: "xo << ", " << *(*it);" replacement: * to " " */
/* @c2j++: "xo << ", " <<  (*it);" replacement: * to " " */
    xo << ", " <<  ( it);
    }
  xo << " }";
  return xo;
}
/**
* @param xo
* @param varset
* @return <
*/
static < <(ostream& xo, ClExternalVarSet& varset)
{ return printTo(xo,varset); }
/**
* @param xo
* @param varmap
* @return ostream&
*/
static ostream& printTo(ostream& xo, ClTableauColumnsMap& varmap)
{
  ClTableauColumnsMap::const_iterator it = varmap.begin();
  for (; it != varmap.end(); ++it) 
    {
/* @c2j++: "xo << *((*it).first) << " -> " << (*it).second << endl;" replacement: * to " " */
/* @c2j++: "xo <<  ((*it).first) << " -> " << (*it).second << endl;" replacement: * to " " */
/* @c2j++: "xo <<  (( it).first) << " -> " << (*it).second << endl;" replacement: * to " " */
/* @c2j++: "xo <<  (( it).first) << " -> " << ( it).second << endl;" replacement: -> to . */
    xo <<  (( it).first) << " . " << ( it).second << endl;
    }
  return xo;
}
/**
* @param xo
* @param varmap
* @return <
*/
static < <(ostream& xo, ClTableauColumnsMap& varmap)
{ return printTo(xo,varmap); }
/**
* @param xo
* @param rows
* @return ostream&
*/
static ostream& printTo(ostream& xo, ClTableauRowsMap& rows)
{
  ClTableauRowsMap::const_iterator it = rows.begin();
  for (; it != rows.end(); ++it) 
    {
/* @c2j++: "const ClAbstractVariable *pv = it->first;" replacement: * to " " */
/* @c2j++: "const ClAbstractVariable  pv = it->first;" replacement: -> to . */
/* @c2j++: "const ClAbstractVariable  pv = it.first;" replacement: const  to static final  */
    static final ClAbstractVariable  pv = it.first;
/* @c2j++: "const ClLinearExpression *pe = it->second;" replacement: * to " " */
/* @c2j++: "const ClLinearExpression  pe = it->second;" replacement: -> to . */
/* @c2j++: "const ClLinearExpression  pe = it.second;" replacement: const  to static final  */
    static final ClLinearExpression  pe = it.second;
/* @c2j++: "if (pv) xo << *pv; else xo << "NilVar";" replacement: * to " " */
    if (pv) xo <<  pv; else xo << "NilVar";
/* @c2j++: "xo << " <-=-> ";" replacement: -> to . */
    xo << " <-=. ";
/* @c2j++: "if (pe) xo << *pe; else xo << "NilExpr";" replacement: * to " " */
    if (pe) xo <<  pe; else xo << "NilExpr";
    xo << endl;
    }
  return xo;
}
/**
* @param xo
* @param rows
* @return <
*/
static < <(ostream& xo, ClTableauRowsMap& rows)
{ return printTo(xo,rows); }
/**
* @param xo
* @param clt
* @return <
*/
static < <(ostream& xo, ClTableau& clt)
{ return clt.printOn(xo); }
}
