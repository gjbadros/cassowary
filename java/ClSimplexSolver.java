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
// ClSimplexSolver
// 

class ClSimplexSolver extends ClTableau
{
ClTableau super;

/**
* ClSimplexSolver
*/
public
ClSimplexSolver()
{
/* @c2j++: "my_objective = * = newsuper("Z");" replacement: * to " " */
 my_objective =   = newsuper("Z");
	;
	;
	my_slackCounter = 0;
	my_artificialCounter = 0;
	my_dummyCounter = 0;
	 
    my_rows[&my_objective] = new ClLinearExpression(); 
#ifndef CL_NO_TRACE
/** @c2j++ Replacement from cerr << "objective row new@ " << my_rows[&my_objective] << endl; */
System.err.println("objective row new@ " + String.valueOf(my_rows[&my_objective]));
#endif
    }

/**
* finalize
* @return 
*/
 void finalize() {
#ifndef NO_SOLVER_STATS
/** @c2j++ Replacement from cerr << "my_slackCounter == " << my_slackCounter */
System.err.print("my_slackCounter == " + String.valueOf(my_slackCounter));
       << "\nmy_artificialCounter == " << my_artificialCounter
       << "\nmy_dummyCounter == " << my_dummyCounter << endl;
/** @c2j++ Replacement from cerr << "editMinusErrorVars " << my_editMinusErrorVars.size() << ", " */
System.err.print("editMinusErrorVars " + String.valueOf(my_editMinusErrorVars.size()) + ", ");
       << "editPlusErrorVars " << my_editPlusErrorVars.size() << ", "
       << "stayMinusErrorVars " << my_stayMinusErrorVars.size() << ", "
       << "stayPlusErrorVars " << my_stayPlusErrorVars.size() << ", "
       << "errorVars " << my_errorVars.size() << ", "
       << "markerVars " << my_markerVars.size() << endl;
#endif
}

/**
* addLowerBound
* @param v
* @param lower
* @return ClSimplexSolver &
*/
public
/* @c2j++: "ClSimplexSolver & addLowerBound(ClAbstractVariable v, Number lower)" replacement:  &  to " " */
ClSimplexSolver addLowerBound(ClAbstractVariable v, Number lower)
{ 
/* @c2j++: "ClLinearInequality *pcn = new ClLinearInequality(ClLinearExpression(lower - v));" replacement: * to " " */
    ClLinearInequality  pcn = new ClLinearInequality(ClLinearExpression(lower - v));
/* @c2j++: "return addConstraint(*pcn);" replacement: * to " " */
    return addConstraint( pcn);
    }

/**
* addUpperBound
* @param v
* @param upper
* @return ClSimplexSolver &
*/
public
/* @c2j++: "ClSimplexSolver & addUpperBound(ClAbstractVariable v, Number upper)" replacement:  &  to " " */
ClSimplexSolver addUpperBound(ClAbstractVariable v, Number upper)
{
/* @c2j++: "ClLinearInequality *pcn = new ClLinearInequality(ClLinearExpression(v - upper));" replacement: * to " " */
    ClLinearInequality  pcn = new ClLinearInequality(ClLinearExpression(v - upper));
/* @c2j++: "return addConstraint(*pcn);" replacement: * to " " */
    return addConstraint( pcn);
    }

/**
* addBounds
* @param v
* @param lower
* @param upper
* @return ClSimplexSolver &
*/
public
/* @c2j++: "ClSimplexSolver & addBounds(ClAbstractVariable v, Number lower, Number upper)" replacement:  &  to " " */
ClSimplexSolver addBounds(ClAbstractVariable v, Number lower, Number upper)
/* @c2j++: "{ addLowerBound(v,lower); addUpperBound(v,upper); return *this; }" replacement: * to " " */
{ addLowerBound(v,lower); addUpperBound(v,upper); return  this; }

/**
* addConstraint
* @param cn
* @return ClSimplexSolver
*/
public
ClSimplexSolver addConstraint(ClConstraint cn)
{
#ifndef CL_NO_TRACE
  Tracer TRACER(__FUNCTION__);
/** @c2j++ Replacement from cerr << "(" << cn << ")" << endl; */
System.err.println("(" + String.valueOf(cn) + ")");
#endif
    
/* @c2j++: "ClLinearExpression *pexpr = newExpression(cn);" replacement: * to " " */
  ClLinearExpression  pexpr = newExpression(cn);

/* @c2j++: "if (!tryAddingDirectly(*pexpr))" replacement: * to " " */
  if (!tryAddingDirectly( pexpr))
    { // could not add directly
/* @c2j++: "addWithArtificialVariable(*pexpr);" replacement: * to " " */
    addWithArtificialVariable( pexpr);
    }
  optimize(my_objective);
  setExternalVariables();
/* @c2j++: "return *this;" replacement: * to " " */
  return  this;
}

/**
* addPointStays
* @param listOfPoints
* @return ClSimplexSolver
*/
public
ClSimplexSolver addPointStays(>& listOfPoints)
{
#ifndef CL_NO_TRACE
  Tracer TRACER(__FUNCTION__);
#endif

/* @c2j++: "vector<const ClPoint *>::const_iterator it = listOfPoints.begin();" replacement: * to " " */
/* @c2j++: "vector<const ClPoint  >::const_iterator it = listOfPoints.begin();" replacement: const  to static final  */
  vector<static final ClPoint  >::const_iterator it = listOfPoints.begin();
  double weight = 1.0;
/* @c2j++: "static const double multiplier = 2.0;" replacement: const  to static final  */
  static static final double multiplier = 2.0;
  for ( ; it != listOfPoints.end(); it++ )
    {
/* @c2j++: "addPointStay((*it)->X(),(*it)->Y(),weight);" replacement: * to " " */
/* @c2j++: "addPointStay(( it)->X(),(*it)->Y(),weight);" replacement: * to " " */
/* @c2j++: "addPointStay(( it)->X(),( it)->Y(),weight);" replacement: -> to . */
/* @c2j++: "addPointStay(( it).X(),( it)->Y(),weight);" replacement: -> to . */
    addPointStay(( it).X(),( it).Y(),weight);
/* @c2j++: "weight *= multiplier;" replacement: * to " " */
    weight  = multiplier;
    }
/* @c2j++: "return *this;" replacement: * to " " */
  return  this;
}

/**
* addPointStay
* @param vx
* @param vy
* @param weight
* @return ClSimplexSolver &
*/
public
/* @c2j++: "ClSimplexSolver & addPointStay(ClVariable vx, ClVariable vy, double weight)" replacement:  &  to " " */
ClSimplexSolver addPointStay(ClVariable vx, ClVariable vy, double weight)
/* @c2j++: "{ addStay(vx,clsWeak(),weight); addStay(vy,clsWeak(),weight); return *this; }" replacement: * to " " */
{ addStay(vx,clsWeak(),weight); addStay(vy,clsWeak(),weight); return  this; }

/**
* addPointStay
* @param clp
* @param weight
* @return ClSimplexSolver
*/
public
ClSimplexSolver addPointStay(ClPoint clp, double weight)
{ 
  addStay(clp.X(),clsWeak(),weight);
  addStay(clp.Y(),clsWeak(),weight);
/* @c2j++: "return *this;" replacement: * to " " */
  return  this;
}

/**
* addStay
* @return ClSimplexSolver &
*/
public
/* @c2j++: "ClSimplexSolver & addStay()" replacement:  &  to " " */
ClSimplexSolver addStay()
{ 
/* @c2j++: "ClStayConstraint *pcn = new ClStayConstraint(v,strength,weight);" replacement: * to " " */
    ClStayConstraint  pcn = new ClStayConstraint(v,strength,weight); 
/* @c2j++: "return addConstraint(*pcn);" replacement: * to " " */
    return addConstraint( pcn); 
    }

/**
* removeConstraint
* @param cnconst
* @return ClSimplexSolver
*/
public
ClSimplexSolver removeConstraint(ClConstraint cnconst)
{
#ifndef CL_NO_TRACE
  Tracer TRACER(__FUNCTION__);
/** @c2j++ Replacement from cerr << "(" << cnconst << ")" << endl; */
System.err.println("(" + String.valueOf(cnconst) + ")");
#endif


  ClConstraint &cn = const_cast<ClConstraint &>(cnconst);

  resetStayConstants();

/* @c2j++: "ClLinearExpression *pzRow = rowExpression(my_objective);" replacement: * to " " */
  ClLinearExpression  pzRow = rowExpression(my_objective);
  ClConstraintToVarSetMap::iterator 
    it_eVars = my_errorVars.find(&cn);
  if (it_eVars != my_errorVars.end())
    {
/* @c2j++: "ClTableauVarSet &eVars = (*it_eVars).second;" replacement: * to " " */
    ClTableauVarSet &eVars = ( it_eVars).second;
    ClTableauVarSet::iterator it = eVars.begin();
    for ( ; it != eVars.end(); ++it )
      {
/* @c2j++: "const ClLinearExpression *pexpr = rowExpression(*(*it));" replacement: * to " " */
/* @c2j++: "const ClLinearExpression  pexpr = rowExpression(*(*it));" replacement: * to " " */
/* @c2j++: "const ClLinearExpression  pexpr = rowExpression( (*it));" replacement: * to " " */
/* @c2j++: "const ClLinearExpression  pexpr = rowExpression( ( it));" replacement: const  to static final  */
      static final ClLinearExpression  pexpr = rowExpression( ( it));
/* @c2j++: "if (pexpr == NULL )" replacement: NULL to null */
      if (pexpr == null )
	{
/* @c2j++: "pzRow->addVariable(*(*it),-1.0 * cnconst.strength().symbolicWeight().asDouble()," replacement: * to " " */
/* @c2j++: "pzRow->addVariable( (*it),-1.0 * cnconst.strength().symbolicWeight().asDouble()," replacement: * to " " */
/* @c2j++: "pzRow->addVariable( ( it),-1.0 * cnconst.strength().symbolicWeight().asDouble()," replacement: * to " " */
/* @c2j++: "pzRow->addVariable( ( it),-1.0   cnconst.strength().symbolicWeight().asDouble()," replacement: -> to . */
	pzRow.addVariable( ( it),-1.0   cnconst.strength().symbolicWeight().asDouble(),
/* @c2j++: "my_objective,*this);" replacement: * to " " */
			   my_objective, this);
	}
      else
	{ // the error variable was in the basis
/* @c2j++: "pzRow->addExpression(*pexpr,-1.0 * cnconst.strength().symbolicWeight().asDouble()," replacement: * to " " */
/* @c2j++: "pzRow->addExpression( pexpr,-1.0 * cnconst.strength().symbolicWeight().asDouble()," replacement: * to " " */
/* @c2j++: "pzRow->addExpression( pexpr,-1.0   cnconst.strength().symbolicWeight().asDouble()," replacement: -> to . */
	pzRow.addExpression( pexpr,-1.0   cnconst.strength().symbolicWeight().asDouble(),
/* @c2j++: "my_objective,*this);" replacement: * to " " */
			     my_objective, this);
	}
      }
    }

/* @c2j++: "map<const ClConstraint *, const ClAbstractVariable *>::iterator" replacement: * to " " */
/* @c2j++: "map<const ClConstraint  , const ClAbstractVariable *>::iterator" replacement: * to " " */
/* @c2j++: "map<const ClConstraint  , const ClAbstractVariable  >::iterator" replacement: const  to static final  */
/* @c2j++: "map<static final ClConstraint  , const ClAbstractVariable  >::iterator" replacement: const  to static final  */
  map<static final ClConstraint  , static final ClAbstractVariable  >::iterator 
    it_marker = my_markerVars.find(&cn);
  if (it_marker == my_markerVars.end())
    { // could not find the constraint
    throw ExCLConstraintNotFound();
    }
/* @c2j++: "const ClAbstractVariable &marker = *((*it_marker).second);" replacement: * to " " */
/* @c2j++: "const ClAbstractVariable &marker =  ((*it_marker).second);" replacement: * to " " */
/* @c2j++: "const ClAbstractVariable &marker =  (( it_marker).second);" replacement: const  to static final  */
  static final ClAbstractVariable &marker =  (( it_marker).second);
  my_markerVars.erase(it_marker);
#ifndef CL_NO_TRACE
/** @c2j++ Replacement from cerr << "Looking to remove var " << marker << endl; */
System.err.println("Looking to remove var " + String.valueOf(marker));
#endif
/* @c2j++: "if (rowExpression(marker) == NULL )" replacement: NULL to null */
  if (rowExpression(marker) == null )
    { // not in the basis, so need to do some work
    ClTableauVarSet &col = my_columns[&marker];
    ClTableauVarSet::iterator it_col = col.begin();
#ifndef CL_NO_TRACE
/** @c2j++ Replacement from cerr << "Must pivot -- columns are " << col << endl; */
System.err.println("Must pivot -- columns are " + String.valueOf(col));
#endif

/* @c2j++: "const ClAbstractVariable *pexitVar = NULL;" replacement: * to " " */
/* @c2j++: "const ClAbstractVariable  pexitVar = NULL;" replacement: const  to static final  */
/* @c2j++: "static final ClAbstractVariable  pexitVar = NULL;" replacement: NULL to null */
    static final ClAbstractVariable  pexitVar = null;
    double minRatio = 0.0;
    for ( ; it_col != col.end(); ++it_col) 
      {
/* @c2j++: "const ClAbstractVariable *pv = *it_col;" replacement: * to " " */
/* @c2j++: "const ClAbstractVariable  pv = *it_col;" replacement: * to " " */
/* @c2j++: "const ClAbstractVariable  pv =  it_col;" replacement: const  to static final  */
      static final ClAbstractVariable  pv =  it_col;
/* @c2j++: "if (pv->isRestricted() )" replacement: -> to . */
      if (pv.isRestricted() )
	{
/* @c2j++: "const ClLinearExpression *pexpr = rowExpression(*pv);" replacement: * to " " */
/* @c2j++: "const ClLinearExpression  pexpr = rowExpression(*pv);" replacement: * to " " */
/* @c2j++: "const ClLinearExpression  pexpr = rowExpression( pv);" replacement: const  to static final  */
	static final ClLinearExpression  pexpr = rowExpression( pv);
/* @c2j++: "assert(pexpr != NULL );" replacement: NULL to null */
	assert(pexpr != null );
/* @c2j++: "Number coeff = pexpr->coefficientFor(marker);" replacement: -> to . */
	Number coeff = pexpr.coefficientFor(marker);
#ifndef CL_NO_TRACE
/** @c2j++ Replacement from cerr << "Marker " << marker << "'s coefficient in " << *pexpr << " is " */
System.err.print("Marker " + String.valueOf(marker) + "'s coefficient in " + String.valueOf(*pexpr) + " is ");
	     << coeff << endl;
#endif
	if (coeff < 0.0) 
	  {
/* @c2j++: "Number r = - pexpr->constant() / coeff;" replacement: -> to . */
	  Number r = - pexpr.constant() / coeff;
/* @c2j++: "if (pexitVar == NULL || r < minRatio)" replacement: NULL to null */
	  if (pexitVar == null || r < minRatio)
	    {
	    minRatio = r;
	    pexitVar = pv;
	    }
	  }
	}
      }
/* @c2j++: "if (pexitVar == NULL )" replacement: NULL to null */
    if (pexitVar == null ) 
      {
#ifndef CL_NO_TRACE
/** @c2j++ Replacement from cerr << "pexitVar is still NULL" << endl; */
System.err.println("pexitVar is still NULL");
#endif
      it_col = col.begin();
      for ( ; it_col != col.end(); ++it_col) 
	{
/* @c2j++: "const ClAbstractVariable *pv = *it_col;" replacement: * to " " */
/* @c2j++: "const ClAbstractVariable  pv = *it_col;" replacement: * to " " */
/* @c2j++: "const ClAbstractVariable  pv =  it_col;" replacement: const  to static final  */
	static final ClAbstractVariable  pv =  it_col;
/* @c2j++: "if (pv->isRestricted() )" replacement: -> to . */
	if (pv.isRestricted() )
	  {
/* @c2j++: "const ClLinearExpression *pexpr = rowExpression(*pv);" replacement: * to " " */
/* @c2j++: "const ClLinearExpression  pexpr = rowExpression(*pv);" replacement: * to " " */
/* @c2j++: "const ClLinearExpression  pexpr = rowExpression( pv);" replacement: const  to static final  */
	  static final ClLinearExpression  pexpr = rowExpression( pv);
/* @c2j++: "assert(pexpr != NULL);" replacement: NULL to null */
	  assert(pexpr != null);
/* @c2j++: "Number coeff = pexpr->coefficientFor(marker);" replacement: -> to . */
	  Number coeff = pexpr.coefficientFor(marker);
/* @c2j++: "Number r = pexpr->constant() / coeff;" replacement: -> to . */
	  Number r = pexpr.constant() / coeff;
/* @c2j++: "if (pexitVar == NULL || r < minRatio)" replacement: NULL to null */
	  if (pexitVar == null || r < minRatio)
	    {
	    minRatio = r;
	    pexitVar = pv;
	    }
	  }
	}
      }

/* @c2j++: "if (pexitVar == NULL)" replacement: NULL to null */
    if (pexitVar == null)
      { // exitVar is still nil
      if (col.size() == 0)
	{
	removeColumn(marker);
	}
      else
	{
/* @c2j++: "pexitVar = *(col.begin());" replacement: * to " " */
	pexitVar =  (col.begin());
	}
      }
    
/* @c2j++: "if (pexitVar != NULL)" replacement: NULL to null */
    if (pexitVar != null)
      {
/* @c2j++: "pivot(marker,*pexitVar);" replacement: * to " " */
      pivot(marker, pexitVar);
      }
    }
  
/* @c2j++: "if (rowExpression(marker) != NULL )" replacement: NULL to null */
  if (rowExpression(marker) != null )
    {
/* @c2j++: "ClLinearExpression *pexpr = removeRow(marker);" replacement: * to " " */
    ClLinearExpression  pexpr = removeRow(marker);
#ifndef CL_NO_TRACE
/** @c2j++ Replacement from cerr << "delete@ " << pexpr << endl; */
System.err.println("delete@ " + String.valueOf(pexpr));
#endif
/** @c2j++ Replacement from delete pexpr; */
pexpr = null;
    }

  if (it_eVars != my_errorVars.end())
    {
/* @c2j++: "ClTableauVarSet &eVars = (*it_eVars).second;" replacement: * to " " */
    ClTableauVarSet &eVars = ( it_eVars).second;
    ClTableauVarSet::iterator it = eVars.begin();
    for ( ; it != eVars.end(); ++it )
      {
/* @c2j++: "const ClAbstractVariable *pv = (*it);" replacement: * to " " */
/* @c2j++: "const ClAbstractVariable  pv = (*it);" replacement: * to " " */
/* @c2j++: "const ClAbstractVariable  pv = ( it);" replacement: const  to static final  */
      static final ClAbstractVariable  pv = ( it);
/* @c2j++: "if (*pv != marker)" replacement: * to " " */
      if ( pv != marker)
	{
/* @c2j++: "removeColumn(*pv);" replacement: * to " " */
	removeColumn( pv);
/** @c2j++ Replacement from delete pv; */
pv = null;
	}
      }
    }

  if (cn.isStayConstraint())
    {
    if (it_eVars != my_errorVars.end())
      {
/* @c2j++: "ClTableauVarSet &eVars = (*it_eVars).second;" replacement: * to " " */
      ClTableauVarSet &eVars = ( it_eVars).second;
      ClVarVector::iterator itStayPlusErrorVars = my_stayPlusErrorVars.begin();
      ClVarVector::iterator itStayMinusErrorVars = my_stayMinusErrorVars.begin();
      for (; itStayMinusErrorVars != my_stayMinusErrorVars.end();
	   ++itStayPlusErrorVars, ++itStayMinusErrorVars)
	{
/* @c2j++: "if (eVars.find(*itStayPlusErrorVars) != eVars.end())" replacement: * to " " */
	if (eVars.find( itStayPlusErrorVars) != eVars.end())
	  {
	  my_stayMinusErrorVars.erase(itStayPlusErrorVars);
	  }
/* @c2j++: "if (eVars.find(*itStayMinusErrorVars) != eVars.end())" replacement: * to " " */
	if (eVars.find( itStayMinusErrorVars) != eVars.end())
	  {
	  my_stayMinusErrorVars.erase(itStayMinusErrorVars);
	  }
	}
      }
    }
  else if (cn.isEditConstraint())
    {
    assert(it_eVars != my_errorVars.end());
/* @c2j++: "ClTableauVarSet &eVars = (*it_eVars).second;" replacement: * to " " */
    ClTableauVarSet &eVars = ( it_eVars).second;
    ClTableauVarSet::iterator it_v = eVars.begin();
    for ( ; it_v != eVars.end(); ++it_v)
      {
/* @c2j++: "const ClAbstractVariable *pvar = *it_v;" replacement: * to " " */
/* @c2j++: "const ClAbstractVariable  pvar = *it_v;" replacement: * to " " */
/* @c2j++: "const ClAbstractVariable  pvar =  it_v;" replacement: const  to static final  */
      static final ClAbstractVariable  pvar =  it_v;
      ClVarVector::iterator 
	itEditPlusErrorVars = my_editPlusErrorVars.begin();
      for ( ; itEditPlusErrorVars != my_editPlusErrorVars.end(); ++itEditPlusErrorVars )
	{
/* @c2j++: "if (*itEditPlusErrorVars == pvar) break;" replacement: * to " " */
	if ( itEditPlusErrorVars == pvar) break;
	}
      if (itEditPlusErrorVars != my_editPlusErrorVars.end())
	{ // found it
	int index = itEditPlusErrorVars - my_editPlusErrorVars.begin();
	ClVarVector::iterator 
	  itEditMinusErrorVars = my_editMinusErrorVars.begin() + index;
	vector<Number>::iterator 
	  itPrevEditConstants = my_prevEditConstants.begin() + index;
	my_editPlusErrorVars.erase(itEditPlusErrorVars);
	my_editMinusErrorVars.erase(itEditMinusErrorVars);
	my_prevEditConstants.erase(itPrevEditConstants);
	break;
	}
      }
    }

  if (it_eVars != my_errorVars.end())
    {
    my_errorVars.erase(it_eVars);
    }
/** @c2j++ Replacement from delete &marker; */
&marker = null;

  optimize(my_objective);
  setExternalVariables();
/* @c2j++: "return *this;" replacement: * to " " */
  return  this;
}

/**
* reset
*/
public
void reset()
{
#ifndef CL_NO_TRACE
  Tracer TRACER(__FUNCTION__);
/** @c2j++ Replacement from cerr << "()" << endl; */
System.err.println("()");
#endif
  assert(false);
}

/**
* resolve
* @param newEditConstants
*/
public
void resolve(>& newEditConstants)
{ // CODE DUPLICATED BELOW
#ifndef CL_NO_TRACE
  Tracer TRACER(__FUNCTION__);
/** @c2j++ Replacement from cerr << "(" << newEditConstants << ")" << endl; */
System.err.println("(" + String.valueOf(newEditConstants) + ")");
#endif
  my_infeasibleRows.clear();
  resetStayConstants();
  resetEditConstants(newEditConstants);
  dualOptimize();
  setExternalVariables();
}

/**
* resolve
* @param x
* @param y
*/
public
void resolve(Number x, Number y)
{
    vector<Number> vals;
    vals.push_back(x);
    vals.push_back(y);
    resolve(vals);
    }

/**
* <

public


/**
* printOn
* @param xo
* @return ostream
*/
public
ostream printOn(ostream xo)
{
  super::printOn(xo);

  xo << "my_editPlusErrorVars: "
     << my_editPlusErrorVars << endl;
  xo << "my_editMinusErrorVars: "
     << my_editMinusErrorVars << endl;

  xo << "my_stayPlusErrorVars: "
     << my_stayPlusErrorVars << endl;
  xo << "my_stayMinusErrorVars: "
     << my_stayMinusErrorVars << endl;

  xo << "my_prevEditConstants: " 
     << my_prevEditConstants << endl;

  return xo;
}

/**
* addWithArtificialVariable
* @param expr
*/
protected
void addWithArtificialVariable(ClLinearExpression expr)
{
#ifndef CL_NO_TRACE
  Tracer TRACER(__FUNCTION__);
/** @c2j++ Replacement from cerr << "(" << expr << ")" << endl; */
System.err.println("(" + String.valueOf(expr) + ")");
#endif
  
/* @c2j++: "ClSlackVariable *pav = new ClSlackVariable(++my_artificialCounter,"a");" replacement: * to " " */
  ClSlackVariable  pav = new ClSlackVariable(++my_artificialCounter,"a");
/* @c2j++: "ClObjectiveVariable *paz = new ClObjectiveVariable("az");" replacement: * to " " */
  ClObjectiveVariable  paz = new ClObjectiveVariable("az");
/* @c2j++: "ClLinearExpression *pazRow = new ClLinearExpression(expr);" replacement: * to " " */
  ClLinearExpression  pazRow = new ClLinearExpression(expr);

#ifndef CL_NO_TRACE
/** @c2j++ Replacement from cerr << __FUNCTION__ << " before addRow-s:" << endl; */
System.err.println(String.valueOf(__FUNCTION__) + " before addRow-s:");
/** @c2j++ Replacement from cerr << (*this) << endl; */
System.err.println(String.valueOf((*this)));
#endif

/* @c2j++: "addRow(*paz,*pazRow);" replacement: * to " " */
/* @c2j++: "addRow( paz,*pazRow);" replacement: * to " " */
  addRow( paz, pazRow);
/* @c2j++: "addRow(*pav,expr);" replacement: * to " " */
  addRow( pav,expr);

#ifndef CL_NO_TRACE
/** @c2j++ Replacement from cerr << __FUNCTION__ << " after addRow-s:" << endl; */
System.err.println(String.valueOf(__FUNCTION__) + " after addRow-s:");
/** @c2j++ Replacement from cerr << (*this) << endl; */
System.err.println(String.valueOf((*this)));
#endif

/* @c2j++: "optimize(*paz);" replacement: * to " " */
  optimize( paz);

/* @c2j++: "ClLinearExpression *pazTableauRow = rowExpression(*paz);" replacement: * to " " */
/* @c2j++: "ClLinearExpression  pazTableauRow = rowExpression(*paz);" replacement: * to " " */
  ClLinearExpression  pazTableauRow = rowExpression( paz);
#ifndef CL_NO_TRACE
/** @c2j++ Replacement from cerr << "pazTableauRow->constant() == " << pazTableauRow->constant() << endl; */
System.err.println("pazTableauRow->constant() == " + String.valueOf(pazTableauRow->constant()));
#endif

/* @c2j++: "if (!clApprox(pazTableauRow->constant(),0.0))" replacement: -> to . */
  if (!clApprox(pazTableauRow.constant(),0.0))
    {
    throw ExCLRequiredFailure();
    }

/* @c2j++: "const ClLinearExpression *pe = rowExpression(*pav);" replacement: * to " " */
/* @c2j++: "const ClLinearExpression  pe = rowExpression(*pav);" replacement: * to " " */
/* @c2j++: "const ClLinearExpression  pe = rowExpression( pav);" replacement: const  to static final  */
  static final ClLinearExpression  pe = rowExpression( pav);
/* @c2j++: "if (pe != NULL )" replacement: NULL to null */
  if (pe != null )
    {
/* @c2j++: "if (pe->isConstant())" replacement: -> to . */
    if (pe.isConstant())
      {
/** @c2j++ Replacement from delete removeRow(*pav); */
removeRow(*pav) = null;
/** @c2j++ Replacement from delete pav; */
pav = null;
      return;
      }
/* @c2j++: "const ClAbstractVariable *pentryVar = pe->anyVariable();" replacement: * to " " */
/* @c2j++: "const ClAbstractVariable  pentryVar = pe->anyVariable();" replacement: -> to . */
/* @c2j++: "const ClAbstractVariable  pentryVar = pe.anyVariable();" replacement: const  to static final  */
    static final ClAbstractVariable  pentryVar = pe.anyVariable();
/* @c2j++: "pivot(*pentryVar, *pav);" replacement: * to " " */
/* @c2j++: "pivot( pentryVar, *pav);" replacement: * to " " */
    pivot( pentryVar,  pav);
    }
/* @c2j++: "assert(rowExpression(*pav) == NULL);" replacement: * to " " */
/* @c2j++: "assert(rowExpression( pav) == NULL);" replacement: NULL to null */
  assert(rowExpression( pav) == null);
/* @c2j++: "removeColumn(*pav);" replacement: * to " " */
  removeColumn( pav);
/** @c2j++ Replacement from delete pav; */
pav = null;
/** @c2j++ Replacement from delete removeRow(*paz); */
removeRow(*paz) = null;
}

/**
* tryAddingDirectly
* @param expr
* @return boolean
*/
protected
boolean tryAddingDirectly(ClLinearExpression expr)
{
#ifndef CL_NO_TRACE
  Tracer TRACER(__FUNCTION__);
/** @c2j++ Replacement from cerr << "(" << expr << ")" << endl; */
System.err.println("(" + String.valueOf(expr) + ")");
#endif
/* @c2j++: "const ClAbstractVariable *psubject = chooseSubject(expr);" replacement: * to " " */
/* @c2j++: "const ClAbstractVariable  psubject = chooseSubject(expr);" replacement: const  to static final  */
  static final ClAbstractVariable  psubject = chooseSubject(expr);
/* @c2j++: "if (psubject == NULL )" replacement: NULL to null */
  if (psubject == null )
    {
#ifndef CL_NO_TRACE
/** @c2j++ Replacement from cerr << "- returning false" << endl; */
System.err.println("- returning false");
#endif
    return false;
    }
/* @c2j++: "expr.newSubject(*psubject);" replacement: * to " " */
  expr.newSubject( psubject);
/* @c2j++: "if (columnsHasKey(*psubject))" replacement: * to " " */
  if (columnsHasKey( psubject))
    {
/* @c2j++: "substituteOut(*psubject,expr);" replacement: * to " " */
    substituteOut( psubject,expr);
    }
/* @c2j++: "addRow(*psubject,expr);" replacement: * to " " */
  addRow( psubject,expr);
#ifndef CL_NO_TRACE
/** @c2j++ Replacement from cerr << "- returning true" << endl; */
System.err.println("- returning true");
#endif
  return true; // successfully added directly
}

/**
* chooseSubject
* @param expr
* @return ClAbstractVariable*
*/
protected
/* @c2j++: "ClAbstractVariable* chooseSubject(ClLinearExpression expr)" replacement: * to " " */
ClAbstractVariable  chooseSubject(ClLinearExpression expr)
{
#ifndef CL_NO_TRACE
  Tracer TRACER(__FUNCTION__);
/** @c2j++ Replacement from cerr << "(" << expr << ")" << endl; */
System.err.println("(" + String.valueOf(expr) + ")");
#endif
/* @c2j++: "const ClAbstractVariable *psubject = NULL; // the current best subject, if any" replacement: * to " " */
/* @c2j++: "const ClAbstractVariable  psubject = NULL; // the current best subject, if any" replacement: const  to static final  */
/* @c2j++: "static final ClAbstractVariable  psubject = NULL; // the current best subject, if any" replacement: NULL to null */
  static final ClAbstractVariable  psubject = null; // the current best subject, if any

  boolean foundUnrestricted = false; 

  boolean foundNewRestricted = false;

/* @c2j++: "const ClVarToNumberMap &terms = expr.terms();" replacement: const  to static final  */
  static final ClVarToNumberMap &terms = expr.terms();
  ClVarToNumberMap::const_iterator it = terms.begin();
  for ( ; it != terms.end(); ++it )
    {
/* @c2j++: "const ClAbstractVariable *pv = (*it).first;" replacement: * to " " */
/* @c2j++: "const ClAbstractVariable  pv = (*it).first;" replacement: * to " " */
/* @c2j++: "const ClAbstractVariable  pv = ( it).first;" replacement: const  to static final  */
    static final ClAbstractVariable  pv = ( it).first;
/* @c2j++: "Number c = (*it).second;" replacement: * to " " */
    Number c = ( it).second;

    if (foundUnrestricted)
      {
/* @c2j++: "if (!pv->isRestricted())" replacement: -> to . */
      if (!pv.isRestricted())
	{
/* @c2j++: "if (!columnsHasKey(*pv))" replacement: * to " " */
	if (!columnsHasKey( pv))
	  return pv;
	}
      }
    else
      { // we haven't found an restricted variable yet
/* @c2j++: "if (pv->isRestricted())" replacement: -> to . */
      if (pv.isRestricted())
	{
/* @c2j++: "if (!foundNewRestricted && !pv->isDummy() && c < 0.0)" replacement: -> to . */
	if (!foundNewRestricted && !pv.isDummy() && c < 0.0)
	  {
/* @c2j++: "const ClTableauColumnsMap &col = columns();" replacement: const  to static final  */
	  static final ClTableauColumnsMap &col = columns();
	  ClTableauColumnsMap::const_iterator it_col = col.find(pv);
	  if (it_col == col.end() || 
	      ( col.size() == 1 && columnsHasKey(my_objective) ) )
	    {
	    psubject = pv;
	    foundNewRestricted = true;
	    }
	  }
	}
      else
	{
	psubject = pv;
	foundUnrestricted = true;
	}
      }
    }
  if (psubject)
    return psubject;

  Number coeff = 0;
  it = terms.begin();
  for ( ; it != terms.end(); ++it )
    {
/* @c2j++: "const ClAbstractVariable *pv = (*it).first;" replacement: * to " " */
/* @c2j++: "const ClAbstractVariable  pv = (*it).first;" replacement: * to " " */
/* @c2j++: "const ClAbstractVariable  pv = ( it).first;" replacement: const  to static final  */
    static final ClAbstractVariable  pv = ( it).first;
/* @c2j++: "Number c = (*it).second;" replacement: * to " " */
    Number c = ( it).second;
/* @c2j++: "if (!pv->isDummy())" replacement: -> to . */
    if (!pv.isDummy())
/* @c2j++: "return NULL; // nope, no luck" replacement: NULL to null */
      return null; // nope, no luck
/* @c2j++: "if (!columnsHasKey(*pv))" replacement: * to " " */
    if (!columnsHasKey( pv))
      {
      psubject = pv;
      coeff = c;
      }
    }

  if (!clApprox(expr.constant(),0.0))
    {
    throw ExCLRequiredFailure();
    }
  if (coeff > 0.0)
    {
    expr.multiplyMe(-1);
    }
  return psubject;
}

/**
* deltaEditConstant
* @param delta
* @param plusErrorVar
* @param minusErrorVar
*/
protected
void deltaEditConstant(Number delta, ClAbstractVariable plusErrorVar, ClAbstractVariable minusErrorVar)
{
#ifndef CL_NO_TRACE
  Tracer TRACER(__FUNCTION__);
/** @c2j++ Replacement from cerr << "(" << delta << ", " << plusErrorVar << ", " << minusErrorVar << ")" << endl; */
System.err.println("(" + String.valueOf(delta) + ", " + String.valueOf(plusErrorVar) + ", " + String.valueOf(minusErrorVar) + ")");
#endif
/* @c2j++: "ClLinearExpression *pexprPlus = rowExpression(plusErrorVar);" replacement: * to " " */
  ClLinearExpression  pexprPlus = rowExpression(plusErrorVar);
/* @c2j++: "if (pexprPlus != NULL )" replacement: NULL to null */
  if (pexprPlus != null )
    {
/* @c2j++: "pexprPlus->incrementConstant(delta);" replacement: -> to . */
    pexprPlus.incrementConstant(delta);
/* @c2j++: "if (pexprPlus->constant() < 0.0)" replacement: -> to . */
    if (pexprPlus.constant() < 0.0)
      {
      my_infeasibleRows.insert(&plusErrorVar);
      }
    return;
    }
/* @c2j++: "ClLinearExpression *pexprMinus = rowExpression(minusErrorVar);" replacement: * to " " */
  ClLinearExpression  pexprMinus = rowExpression(minusErrorVar);
/* @c2j++: "if (pexprMinus != NULL)" replacement: NULL to null */
  if (pexprMinus != null)
    {
/* @c2j++: "pexprMinus->incrementConstant(-delta);" replacement: -> to . */
    pexprMinus.incrementConstant(-delta);
/* @c2j++: "if (pexprMinus->constant() < 0.0)" replacement: -> to . */
    if (pexprMinus.constant() < 0.0)
      {
      my_infeasibleRows.insert(&minusErrorVar);
      }
    return;
    }
  ClTableauVarSet &columnVars = my_columns[&minusErrorVar];
  ClTableauVarSet::iterator it = columnVars.begin();
  for (; it != columnVars.end(); ++it)
    {
/* @c2j++: "const ClAbstractVariable *pbasicVar = *it;" replacement: * to " " */
/* @c2j++: "const ClAbstractVariable  pbasicVar = *it;" replacement: * to " " */
/* @c2j++: "const ClAbstractVariable  pbasicVar =  it;" replacement: const  to static final  */
    static final ClAbstractVariable  pbasicVar =  it;
/* @c2j++: "ClLinearExpression *pexpr = rowExpression(*pbasicVar);" replacement: * to " " */
/* @c2j++: "ClLinearExpression  pexpr = rowExpression(*pbasicVar);" replacement: * to " " */
    ClLinearExpression  pexpr = rowExpression( pbasicVar);
/* @c2j++: "assert(pexpr != NULL );" replacement: NULL to null */
    assert(pexpr != null );
/* @c2j++: "double c = pexpr->coefficientFor(minusErrorVar);" replacement: -> to . */
    double c = pexpr.coefficientFor(minusErrorVar);
/* @c2j++: "pexpr->incrementConstant(c*delta);" replacement: * to " " */
/* @c2j++: "pexpr->incrementConstant(c delta);" replacement: -> to . */
    pexpr.incrementConstant(c delta);
/* @c2j++: "if (pbasicVar->isRestricted() && pexpr->constant() < 0.0)" replacement: -> to . */
/* @c2j++: "if (pbasicVar.isRestricted() && pexpr->constant() < 0.0)" replacement: -> to . */
    if (pbasicVar.isRestricted() && pexpr.constant() < 0.0)
      {
      my_infeasibleRows.insert(pbasicVar);
      }
    }
}

/**
* dualOptimize
*/
protected
void dualOptimize()
{
#ifndef CL_NO_TRACE
  Tracer TRACER(__FUNCTION__);
/** @c2j++ Replacement from cerr << "()" << endl; */
System.err.println("()");
#endif
/* @c2j++: "const ClLinearExpression *pzRow = rowExpression(my_objective);" replacement: * to " " */
/* @c2j++: "const ClLinearExpression  pzRow = rowExpression(my_objective);" replacement: const  to static final  */
  static final ClLinearExpression  pzRow = rowExpression(my_objective);
  while (!my_infeasibleRows.empty())
    {
    ClTableauVarSet::iterator it_exitVar = my_infeasibleRows.begin();
/* @c2j++: "const ClAbstractVariable *pexitVar = *it_exitVar;" replacement: * to " " */
/* @c2j++: "const ClAbstractVariable  pexitVar = *it_exitVar;" replacement: * to " " */
/* @c2j++: "const ClAbstractVariable  pexitVar =  it_exitVar;" replacement: const  to static final  */
    static final ClAbstractVariable  pexitVar =  it_exitVar;
    my_infeasibleRows.erase(it_exitVar);
/* @c2j++: "const ClAbstractVariable *pentryVar = NULL;" replacement: * to " " */
/* @c2j++: "const ClAbstractVariable  pentryVar = NULL;" replacement: const  to static final  */
/* @c2j++: "static final ClAbstractVariable  pentryVar = NULL;" replacement: NULL to null */
    static final ClAbstractVariable  pentryVar = null;
/* @c2j++: "ClLinearExpression *pexpr = rowExpression(*pexitVar);" replacement: * to " " */
/* @c2j++: "ClLinearExpression  pexpr = rowExpression(*pexitVar);" replacement: * to " " */
    ClLinearExpression  pexpr = rowExpression( pexitVar);
/* @c2j++: "if (pexpr != NULL )" replacement: NULL to null */
    if (pexpr != null )
      {
/* @c2j++: "if (pexpr->constant() < 0.0)" replacement: -> to . */
      if (pexpr.constant() < 0.0)
	{
	double ratio = DBL_MAX;
	double r;
/* @c2j++: "ClVarToNumberMap &terms = pexpr->terms();" replacement: -> to . */
	ClVarToNumberMap &terms = pexpr.terms();
	ClVarToNumberMap::iterator it = terms.begin();
	for ( ; it != terms.end(); ++it )
	  {
/* @c2j++: "const ClAbstractVariable *pv = (*it).first;" replacement: * to " " */
/* @c2j++: "const ClAbstractVariable  pv = (*it).first;" replacement: * to " " */
/* @c2j++: "const ClAbstractVariable  pv = ( it).first;" replacement: const  to static final  */
	  static final ClAbstractVariable  pv = ( it).first;
/* @c2j++: "Number c = (*it).second;" replacement: * to " " */
	  Number c = ( it).second;
/* @c2j++: "if (c > 0.0 && pv->isPivotable())" replacement: -> to . */
	  if (c > 0.0 && pv.isPivotable())
	    {
/* @c2j++: "Number zc = pzRow->coefficientFor(*pv);" replacement: * to " " */
/* @c2j++: "Number zc = pzRow->coefficientFor( pv);" replacement: -> to . */
	    Number zc = pzRow.coefficientFor( pv);
	    r = zc/c; // FIXGJB r:= zc/c or zero, as ClSymbolicWeight-s
	    if (r < ratio)
	      {
	      pentryVar = pv;
	      ratio = r;
	      }
	    }
	  }
	if (ratio == DBL_MAX)
	  {
/** @c2j++ Replacement from cerr << "ratio == nil (DBL_MAX)" << endl; */
System.err.println("ratio == nil (DBL_MAX)");
	  throw ExCLInternalError();
	  }
/* @c2j++: "pivot(*pentryVar,*pexitVar);" replacement: * to " " */
/* @c2j++: "pivot( pentryVar,*pexitVar);" replacement: * to " " */
	pivot( pentryVar, pexitVar);
	}
      }
    }
}

/**
* newExpression
* @param cn
* @return ClLinearExpression*
*/
protected
/* @c2j++: "ClLinearExpression* newExpression(ClConstraint cn)" replacement: * to " " */
ClLinearExpression  newExpression(ClConstraint cn)
{
#ifndef CL_NO_TRACE
  Tracer TRACER(__FUNCTION__);
/** @c2j++ Replacement from cerr << "(" << cn << ")" << endl; */
System.err.println("(" + String.valueOf(cn) + ")");
/** @c2j++ Replacement from cerr << "cn.isInequality() == " << cn.isInequality() << endl; */
System.err.println("cn.isInequality() == " + String.valueOf(cn.isInequality()));
/** @c2j++ Replacement from cerr << "cn.isRequired() == " << cn.isRequired() << endl; */
System.err.println("cn.isRequired() == " + String.valueOf(cn.isRequired()));
#endif
/* @c2j++: "const ClLinearExpression &cnExpr = cn.expression();" replacement: const  to static final  */
  static final ClLinearExpression &cnExpr = cn.expression();
  auto_ptr<ClLinearExpression> pexpr ( new ClLinearExpression(cnExpr.constant()) );
  auto_ptr<ClSlackVariable> pslackVar;
  auto_ptr<ClDummyVariable> pdummyVar;
  auto_ptr<ClSlackVariable> peminus;
  auto_ptr<ClSlackVariable> peplus;
/* @c2j++: "const ClVarToNumberMap &cnTerms = cnExpr.terms();" replacement: const  to static final  */
  static final ClVarToNumberMap &cnTerms = cnExpr.terms();
  ClVarToNumberMap::const_iterator it = cnTerms.begin();
  for ( ; it != cnTerms.end(); ++it)
    {
/* @c2j++: "const ClAbstractVariable *pv = (*it).first;" replacement: * to " " */
/* @c2j++: "const ClAbstractVariable  pv = (*it).first;" replacement: * to " " */
/* @c2j++: "const ClAbstractVariable  pv = ( it).first;" replacement: const  to static final  */
    static final ClAbstractVariable  pv = ( it).first;
/* @c2j++: "Number c = (*it).second;" replacement: * to " " */
    Number c = ( it).second;
/* @c2j++: "const ClLinearExpression *pe = rowExpression(*pv);" replacement: * to " " */
/* @c2j++: "const ClLinearExpression  pe = rowExpression(*pv);" replacement: * to " " */
/* @c2j++: "const ClLinearExpression  pe = rowExpression( pv);" replacement: const  to static final  */
    static final ClLinearExpression  pe = rowExpression( pv);
/* @c2j++: "if (pe == NULL)" replacement: NULL to null */
    if (pe == null)
      {
/* @c2j++: "pexpr->addVariable(*pv,c);" replacement: * to " " */
/* @c2j++: "pexpr->addVariable( pv,c);" replacement: -> to . */
      pexpr.addVariable( pv,c);
      }
    else
      {
/* @c2j++: "pexpr->addExpression(*pe,c);" replacement: * to " " */
/* @c2j++: "pexpr->addExpression( pe,c);" replacement: -> to . */
      pexpr.addExpression( pe,c);
      }
    }

  if (cn.isInequality())
    {
    ++my_slackCounter;
    ReinitializeAutoPtr(pslackVar,new ClSlackVariable (my_slackCounter, "s"));
/* @c2j++: "pexpr->setVariable(*pslackVar,-1);" replacement: * to " " */
/* @c2j++: "pexpr->setVariable( pslackVar,-1);" replacement: -> to . */
    pexpr.setVariable( pslackVar,-1);
    my_markerVars[&cn] = pslackVar.get();
    if (!cn.isRequired())
      {
      ++my_slackCounter;
      ReinitializeAutoPtr(peminus,new ClSlackVariable (my_slackCounter, "em"));
/* @c2j++: "pexpr->setVariable(*peminus,1.0);" replacement: * to " " */
/* @c2j++: "pexpr->setVariable( peminus,1.0);" replacement: -> to . */
      pexpr.setVariable( peminus,1.0);
/* @c2j++: "ClLinearExpression *pzRow = rowExpression(my_objective);" replacement: * to " " */
      ClLinearExpression  pzRow = rowExpression(my_objective);
      ClSymbolicWeight sw = cn.strength().symbolicWeight().times(cn.weight());
/* @c2j++: "pzRow->setVariable(*peminus,sw.asDouble());" replacement: * to " " */
/* @c2j++: "pzRow->setVariable( peminus,sw.asDouble());" replacement: -> to . */
      pzRow.setVariable( peminus,sw.asDouble());
      my_errorVars[&cn].insert(peminus.get());
/* @c2j++: "noteAddedVariable(*peminus,my_objective);" replacement: * to " " */
      noteAddedVariable( peminus,my_objective);
      }
    }
  else
    { // cn is an equality
    if (cn.isRequired())
      {
      ++my_dummyCounter;
      ReinitializeAutoPtr(pdummyVar,new ClDummyVariable (my_dummyCounter, "d"));
/* @c2j++: "pexpr->setVariable(*pdummyVar,1.0);" replacement: * to " " */
/* @c2j++: "pexpr->setVariable( pdummyVar,1.0);" replacement: -> to . */
      pexpr.setVariable( pdummyVar,1.0);
      my_markerVars[&cn] = pdummyVar.get();
#ifndef CL_NO_TRACE
/** @c2j++ Replacement from cerr << "Adding dummyVar == d" << my_dummyCounter << endl; */
System.err.println("Adding dummyVar == d" + String.valueOf(my_dummyCounter));
#endif
      }
    else
      {
      ++my_slackCounter;
      ReinitializeAutoPtr(peplus,new ClSlackVariable (my_slackCounter, "ep"));
      ReinitializeAutoPtr(peminus,new ClSlackVariable (my_slackCounter, "em"));

/* @c2j++: "pexpr->setVariable(*peplus,-1.0);" replacement: * to " " */
/* @c2j++: "pexpr->setVariable( peplus,-1.0);" replacement: -> to . */
      pexpr.setVariable( peplus,-1.0);
/* @c2j++: "pexpr->setVariable(*peminus,1.0);" replacement: * to " " */
/* @c2j++: "pexpr->setVariable( peminus,1.0);" replacement: -> to . */
      pexpr.setVariable( peminus,1.0);
      my_markerVars[&cn] = peplus.get();
/* @c2j++: "ClLinearExpression *pzRow = rowExpression(my_objective);" replacement: * to " " */
      ClLinearExpression  pzRow = rowExpression(my_objective);
      ClSymbolicWeight sw = cn.strength().symbolicWeight().times(cn.weight());
      double swCoeff = sw.asDouble();
#ifndef CL_NO_TRACE
      if (swCoeff == 0) 
	{
/** @c2j++ Replacement from cerr << "sw == " << sw << endl */
System.err.print("sw == " + String.valueOf(sw));
	     << "cn == " << cn << endl;
/** @c2j++ Replacement from cerr << "adding " << *peplus << " and " << *peminus */
System.err.print("adding " + String.valueOf(*peplus) + " and " + String.valueOf(*peminus));
	     << " with swCoeff == " << swCoeff << endl;
	}
#endif      
/* @c2j++: "pzRow->setVariable(*peplus,swCoeff);" replacement: * to " " */
/* @c2j++: "pzRow->setVariable( peplus,swCoeff);" replacement: -> to . */
      pzRow.setVariable( peplus,swCoeff);
/* @c2j++: "noteAddedVariable(*peplus,my_objective);" replacement: * to " " */
      noteAddedVariable( peplus,my_objective);
/* @c2j++: "pzRow->setVariable(*peminus,swCoeff);" replacement: * to " " */
/* @c2j++: "pzRow->setVariable( peminus,swCoeff);" replacement: -> to . */
      pzRow.setVariable( peminus,swCoeff);
/* @c2j++: "noteAddedVariable(*peminus,my_objective);" replacement: * to " " */
      noteAddedVariable( peminus,my_objective);
      my_errorVars[&cn].insert(peminus.get());
      my_errorVars[&cn].insert(peplus.get());
      if (cn.isStayConstraint()) 
	{
	my_stayPlusErrorVars.push_back(peplus.get());
	my_stayMinusErrorVars.push_back(peminus.get());
	}
      if (cn.isEditConstraint())
	{
	my_editPlusErrorVars.push_back(peplus.get());
	my_editMinusErrorVars.push_back(peminus.get());
	my_prevEditConstants.push_back(cnExpr.constant());
	}
      }
    }

/* @c2j++: "if (pexpr->constant() < 0)" replacement: -> to . */
  if (pexpr.constant() < 0)
    {
/* @c2j++: "pexpr->multiplyMe(-1);" replacement: -> to . */
    pexpr.multiplyMe(-1);
    }
#ifndef CL_NO_TRACE
/** @c2j++ Replacement from cerr << "- returning " << *pexpr << endl; */
System.err.println("- returning " + String.valueOf(*pexpr));
#endif
  pslackVar.release();
  pdummyVar.release();
  peminus.release();
  peplus.release();
  return pexpr.release();
}

/**
* optimize
* @param zVar
*/
protected
void optimize(ClObjectiveVariable zVar)
{
#ifndef CL_NO_TRACE
  Tracer TRACER(__FUNCTION__);
/** @c2j++ Replacement from cerr << "(" << zVar << ")" << endl; */
System.err.println("(" + String.valueOf(zVar) + ")");
/** @c2j++ Replacement from cerr << *this << endl; */
System.err.println(String.valueOf(*this));
#endif
/* @c2j++: "ClLinearExpression *pzRow = rowExpression(zVar);" replacement: * to " " */
  ClLinearExpression  pzRow = rowExpression(zVar);
/* @c2j++: "assert(pzRow != NULL);" replacement: NULL to null */
  assert(pzRow != null);
/* @c2j++: "const ClAbstractVariable *pentryVar = NULL;" replacement: * to " " */
/* @c2j++: "const ClAbstractVariable  pentryVar = NULL;" replacement: const  to static final  */
/* @c2j++: "static final ClAbstractVariable  pentryVar = NULL;" replacement: NULL to null */
  static final ClAbstractVariable  pentryVar = null;
/* @c2j++: "const ClAbstractVariable *pexitVar = NULL;" replacement: * to " " */
/* @c2j++: "const ClAbstractVariable  pexitVar = NULL;" replacement: const  to static final  */
/* @c2j++: "static final ClAbstractVariable  pexitVar = NULL;" replacement: NULL to null */
  static final ClAbstractVariable  pexitVar = null;
  while (true)
    {
    Number objectiveCoeff = 0;
/* @c2j++: "ClVarToNumberMap &terms = pzRow->terms();" replacement: -> to . */
    ClVarToNumberMap &terms = pzRow.terms();
    ClVarToNumberMap::iterator it = terms.begin();
    for (; it != terms.end(); ++it)
      {
/* @c2j++: "const ClAbstractVariable *pv = (*it).first;" replacement: * to " " */
/* @c2j++: "const ClAbstractVariable  pv = (*it).first;" replacement: * to " " */
/* @c2j++: "const ClAbstractVariable  pv = ( it).first;" replacement: const  to static final  */
      static final ClAbstractVariable  pv = ( it).first;
/* @c2j++: "Number c = (*it).second;" replacement: * to " " */
      Number c = ( it).second;
/* @c2j++: "if (pv->isPivotable() && c < objectiveCoeff)" replacement: -> to . */
      if (pv.isPivotable() && c < objectiveCoeff)
	{
	objectiveCoeff = c;
	pentryVar = pv;
	}
      }
    if (objectiveCoeff == 0)
      return;
#ifndef CL_NO_TRACE
/** @c2j++ Replacement from cerr << "*pentryVar == " << *pentryVar << ", " */
System.err.print("*pentryVar == " + String.valueOf(*pentryVar) + ", ");
	 << "objectiveCoeff == " << objectiveCoeff
	 << endl;
#endif

    double minRatio = DBL_MAX;
    ClTableauVarSet &columnVars = my_columns[pentryVar];
    ClTableauVarSet::iterator it_rowvars = columnVars.begin();
    Number r = 0.0;
    for (; it_rowvars != columnVars.end(); ++it_rowvars)
      {
/* @c2j++: "const ClAbstractVariable *pv = *it_rowvars;" replacement: * to " " */
/* @c2j++: "const ClAbstractVariable  pv = *it_rowvars;" replacement: * to " " */
/* @c2j++: "const ClAbstractVariable  pv =  it_rowvars;" replacement: const  to static final  */
      static final ClAbstractVariable  pv =  it_rowvars;
#ifndef CL_NO_TRACE
/** @c2j++ Replacement from cerr << "Checking " << *pv << endl; */
System.err.println("Checking " + String.valueOf(*pv));
#endif
/* @c2j++: "if (pv->isPivotable())" replacement: -> to . */
      if (pv.isPivotable()) 
	{
/* @c2j++: "const ClLinearExpression *pexpr = rowExpression(*pv);" replacement: * to " " */
/* @c2j++: "const ClLinearExpression  pexpr = rowExpression(*pv);" replacement: * to " " */
/* @c2j++: "const ClLinearExpression  pexpr = rowExpression( pv);" replacement: const  to static final  */
	static final ClLinearExpression  pexpr = rowExpression( pv);
/* @c2j++: "Number coeff = pexpr->coefficientFor(*pentryVar);" replacement: * to " " */
/* @c2j++: "Number coeff = pexpr->coefficientFor( pentryVar);" replacement: -> to . */
	Number coeff = pexpr.coefficientFor( pentryVar);
	if (coeff < 0.0)
	  {
/* @c2j++: "r = - pexpr->constant() / coeff;" replacement: -> to . */
	  r = - pexpr.constant() / coeff;
	  if (r < minRatio)
	    {
#ifndef CL_NO_TRACE
/** @c2j++ Replacement from cerr << "New minRatio == " << r << endl; */
System.err.println("New minRatio == " + String.valueOf(r));
#endif
	    minRatio = r;
	    pexitVar = pv;
	    }
	  }
	}
      }
    if (minRatio == DBL_MAX)
      {
/** @c2j++ Replacement from cerr << "objective function is unbounded!" << endl; */
System.err.println("objective function is unbounded!");
      throw ExCLInternalError();
      }
/* @c2j++: "pivot(*pentryVar, *pexitVar);" replacement: * to " " */
/* @c2j++: "pivot( pentryVar, *pexitVar);" replacement: * to " " */
    pivot( pentryVar,  pexitVar);
#ifndef CL_NO_TRACE
/** @c2j++ Replacement from cerr << *this << endl; */
System.err.println(String.valueOf(*this));
#endif
    }
}

/**
* pivot
* @param entryVar
* @param exitVar
*/
protected
void pivot(ClAbstractVariable entryVar, ClAbstractVariable exitVar)
{
#ifndef CL_NO_TRACE
  Tracer TRACER(__FUNCTION__);
/** @c2j++ Replacement from cerr << "(" << entryVar << ", " << exitVar << ")" << endl; */
System.err.println("(" + String.valueOf(entryVar) + ", " + String.valueOf(exitVar) + ")");
#endif
  
/* @c2j++: "ClLinearExpression *pexpr = removeRow(exitVar);" replacement: * to " " */
  ClLinearExpression  pexpr = removeRow(exitVar);

/* @c2j++: "pexpr->changeSubject(exitVar,entryVar);" replacement: -> to . */
  pexpr.changeSubject(exitVar,entryVar);
/* @c2j++: "substituteOut(entryVar,*pexpr);" replacement: * to " " */
  substituteOut(entryVar, pexpr);
/* @c2j++: "addRow(entryVar,*pexpr);" replacement: * to " " */
  addRow(entryVar, pexpr);
}

/**
* resetEditConstants
* @param newEditConstants
*/
protected
void resetEditConstants(>& newEditConstants)
{
#ifndef CL_NO_TRACE
  Tracer TRACER(__FUNCTION__);
/** @c2j++ Replacement from cerr << "(" << newEditConstants << ")" << endl; */
System.err.println("(" + String.valueOf(newEditConstants) + ")");
#endif
  if (newEditConstants.size() != my_editPlusErrorVars.size())
    { // number of edit constants doesn't match the number of edit error variables
/** @c2j++ Replacement from cerr << "newEditConstants == " << newEditConstants << endl */
System.err.print("newEditConstants == " + String.valueOf(newEditConstants));
	 << "my_editPlusErrorVars == " << my_editPlusErrorVars << endl
	 << "Sizes don't match!" << endl;
    throw ExCLInternalError();
    }
  vector<Number>::const_iterator itNew = newEditConstants.begin();
  vector<Number>::iterator itPrev = my_prevEditConstants.begin();
  ClVarVector::const_iterator 
    itEditPlusErrorVars = my_editPlusErrorVars.begin();
  ClVarVector::const_iterator
    itEditMinusErrorVars = my_editMinusErrorVars.begin();

  for ( ; itNew != newEditConstants.end(); 
	++itNew, ++itPrev, ++itEditPlusErrorVars, ++itEditMinusErrorVars )
    {
/* @c2j++: "Number delta = (*itNew) - (*itPrev);" replacement: * to " " */
/* @c2j++: "Number delta = ( itNew) - (*itPrev);" replacement: * to " " */
    Number delta = ( itNew) - ( itPrev);
/* @c2j++: "(*itPrev) = (*itNew);" replacement: * to " " */
/* @c2j++: "( itPrev) = (*itNew);" replacement: * to " " */
    ( itPrev) = ( itNew);
/* @c2j++: "deltaEditConstant(delta,*(*itEditPlusErrorVars),*(*itEditMinusErrorVars));" replacement: * to " " */
/* @c2j++: "deltaEditConstant(delta, (*itEditPlusErrorVars),*(*itEditMinusErrorVars));" replacement: * to " " */
/* @c2j++: "deltaEditConstant(delta, ( itEditPlusErrorVars),*(*itEditMinusErrorVars));" replacement: * to " " */
/* @c2j++: "deltaEditConstant(delta, ( itEditPlusErrorVars), (*itEditMinusErrorVars));" replacement: * to " " */
    deltaEditConstant(delta, ( itEditPlusErrorVars), ( itEditMinusErrorVars));
    }
}

/**
* resetStayConstants
*/
protected
void resetStayConstants()
{
#ifndef CL_NO_TRACE
  Tracer TRACER(__FUNCTION__);
/** @c2j++ Replacement from cerr << "()" << endl; */
System.err.println("()");
#endif
  ClVarVector::const_iterator 
    itStayPlusErrorVars = my_stayPlusErrorVars.begin();
  ClVarVector::const_iterator 
    itStayMinusErrorVars = my_stayMinusErrorVars.begin();

  for ( ; itStayPlusErrorVars != my_stayPlusErrorVars.end();
	++itStayPlusErrorVars, ++itStayMinusErrorVars )
    {
/* @c2j++: "ClLinearExpression *pexpr = rowExpression(*(*itStayPlusErrorVars));" replacement: * to " " */
/* @c2j++: "ClLinearExpression  pexpr = rowExpression(*(*itStayPlusErrorVars));" replacement: * to " " */
/* @c2j++: "ClLinearExpression  pexpr = rowExpression( (*itStayPlusErrorVars));" replacement: * to " " */
    ClLinearExpression  pexpr = rowExpression( ( itStayPlusErrorVars));
/* @c2j++: "if (pexpr == NULL )" replacement: NULL to null */
    if (pexpr == null )
      {
/* @c2j++: "pexpr = rowExpression(*(*itStayMinusErrorVars));" replacement: * to " " */
/* @c2j++: "pexpr = rowExpression( (*itStayMinusErrorVars));" replacement: * to " " */
      pexpr = rowExpression( ( itStayMinusErrorVars));
      }
/* @c2j++: "if (pexpr != NULL)" replacement: NULL to null */
    if (pexpr != null)
      {
/* @c2j++: "pexpr->set_constant(0.0);" replacement: -> to . */
      pexpr.set_constant(0.0);
      }
    }
}

/**
* setExternalVariables
*/
protected
void setExternalVariables()
{
#ifndef CL_NO_TRACE
  Tracer TRACER(__FUNCTION__);
/** @c2j++ Replacement from cerr << "()" << endl; */
System.err.println("()");
/** @c2j++ Replacement from cerr << *this << endl; */
System.err.println(String.valueOf(*this));
#endif


  ClExternalVarSet::iterator itParVars = my_externalParametricVars.begin();
  for ( ; itParVars != my_externalParametricVars.end(); ++itParVars )
    {
/* @c2j++: "ClVariable *pv = const_cast<ClVariable *>(*itParVars);" replacement: * to " " */
/* @c2j++: "ClVariable  pv = const_cast<ClVariable *>(*itParVars);" replacement: * to " " */
/* @c2j++: "ClVariable  pv = const_cast<ClVariable  >(*itParVars);" replacement: * to " " */
    ClVariable  pv = const_cast<ClVariable  >( itParVars);
/* @c2j++: "pv->set_value(0.0);" replacement: -> to . */
    pv.set_value(0.0);
    }

  ClExternalVarSet::iterator itRowVars = my_externalRows.begin();
  for ( ; itRowVars != my_externalRows.end() ; ++itRowVars )
    {
/* @c2j++: "ClVariable *pv = const_cast<ClVariable *>(*itRowVars);" replacement: * to " " */
/* @c2j++: "ClVariable  pv = const_cast<ClVariable *>(*itRowVars);" replacement: * to " " */
/* @c2j++: "ClVariable  pv = const_cast<ClVariable  >(*itRowVars);" replacement: * to " " */
    ClVariable  pv = const_cast<ClVariable  >( itRowVars);
/* @c2j++: "ClLinearExpression *pexpr = rowExpression(*pv);" replacement: * to " " */
/* @c2j++: "ClLinearExpression  pexpr = rowExpression(*pv);" replacement: * to " " */
    ClLinearExpression  pexpr = rowExpression( pv);
/* @c2j++: "pv->set_value(pexpr->constant());" replacement: -> to . */
/* @c2j++: "pv.set_value(pexpr->constant());" replacement: -> to . */
    pv.set_value(pexpr.constant());
    }
}
ClVarVector my_editMinusErrorVars;
ClVarVector my_editPlusErrorVars;
ClVarVector my_stayMinusErrorVars;
ClVarVector my_stayPlusErrorVars;
> my_prevEditConstants;
ClConstraintToVarSetMap my_errorVars;
ClConstraintToVarMap my_markerVars;
/* @c2j++: "ClObjectiveVariable & my_objective;" replacement:  &  to " " */
ClObjectiveVariable my_objective;
int my_slackCounter;
int my_artificialCounter;
int my_dummyCounter;

/*@c2j++ The following variable used to be declared global */
/* @c2j++: "ClPoint ; typedef map < static final ClConstraint * , ClTableauVarSet > ClConstraintToVarSetMap ;" replacement: * to " " */
ClPoint ; typedef map < static final ClConstraint   , ClTableauVarSet > ClConstraintToVarSetMap ; 

/*@c2j++ The following variable used to be declared global */
/* @c2j++: "typedef map < static final ClConstraint * , static final ClAbstractVariable * > ClConstraintToVarMap ;" replacement: * to " " */
/* @c2j++: "typedef map < static final ClConstraint   , static final ClAbstractVariable * > ClConstraintToVarMap ;" replacement: * to " " */
typedef map < static final ClConstraint   , static final ClAbstractVariable   > ClConstraintToVarMap ; 

/*@c2j++ The following variable used to be declared global */
/* @c2j++: "typedef vector < static final ClAbstractVariable * > ClVarVector ;" replacement: * to " " */
typedef vector < static final ClAbstractVariable   > ClVarVector ; 

/**
* @param xo
* @param varlist
* @return ostream
*/
static ostream printTo(ostream xo, ClVarVector varlist)
{
  ClVarVector::const_iterator it = varlist.begin();
  xo << varlist.size() << ":" << "[ ";
  if (it != varlist.end())
    {
/* @c2j++: "xo << *(*it);" replacement: * to " " */
/* @c2j++: "xo <<  (*it);" replacement: * to " " */
    xo <<  ( it);
    ++it;
    }
  for (; it != varlist.end(); ++it) 
    {
/* @c2j++: "xo << ", " << *(*it);" replacement: * to " " */
/* @c2j++: "xo << ", " <<  (*it);" replacement: * to " " */
    xo << ", " <<  ( it);
    }
  xo << " ]" << endl;
  return xo;
}
/**
* @param xo
* @param varlist
* @return <
*/
static < <(ostream xo, ClVarVector varlist)
{
  return printTo(xo,varlist);
}
/**
* @param xo
* @param clss
* @return <
*/
static < <(ostream xo, ClSimplexSolver clss)
{
  return clss.printOn(xo);
}
}
