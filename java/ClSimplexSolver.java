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

import java.util.*;

class ClSimplexSolver extends ClTableau
{
  public ClSimplexSolver()
  {
    my_objective = new ClObjectiveVariable("Z");
    my_slackCounter = 0;
    my_artificialCounter = 0;
    my_dummyCounter = 0;
    my_rows.put(my_objective,new ClLinearExpression());
  }

  public ClSimplexSolver addLowerBound(ClAbstractVariable v, double lower)
  { 
    ClLinearInequality cn = 
      new ClLinearInequality(v,cnGEQ,ClLinearExpression(lower));
    return this;
  }

  public ClSimplexSolver addUpperBound(ClAbstractVariable v, double upper)
  { 
    ClLinearInequality cn = 
      new ClLinearInequality(v,cnLEQ,ClLinearExpression(upper));
    return this;
  }

  public ClSimplexSolver addBounds(ClAbstractVariable v,
				   double lower, double upper)
  { addLowerBound(v,lower); addUpperBound(v,upper); return  this; }

  public ClSimplexSolver addConstraint(ClConstraint cn)
  {
    //#ifndef CL_NO_TRACE
    //Tracer TRACER(__FUNCTION__);
    // System.err.println("(" + cn.toString() + ")");
    //#endif
    
    ClLinearExpression expr = newExpression(cn);
  
    if (!tryAddingDirectly(expr)) {
      // could not add directly
      addWithArtificialVariable(expr);
    }
    optimize(my_objective);
    setExternalVariables();
    return  this;
  }

  public ClSimplexSolver addPointStays(Vector listOfPoints)
  {
    //#ifndef CL_NO_TRACE
    //Tracer TRACER(__FUNCTION__);
    //#endif
    double weight = 1.0;
    final double multiplier = 2.0;
    for (int i = 0; i < listOfPoints.size(); i++) {
      addPointStay((ClPoint) listOfPoints[i],weight);
      weight *= multiplier;
    }
    return this;
  }

  public ClSimplexSolver addPointStay(ClVariable vx, 
				      ClVariable vy, 
				      double weight)
  { 
    addStay(vx,ClStrength.clsWeak,weight);
    addStay(vy,ClStrength.clsWeak,weight);
    return  this;
  }

  public ClSimplexSolver addPointStay(ClVariable vx, 
				      ClVariable vy)
  { addPointStay(vx,vy,1.0); }
  
  public ClSimplexSolver addPointStay(ClPoint clp, double weight)
  { 
    addStay(clp.X(),ClStrength.clsWeak,weight);
    addStay(clp.Y(),ClStrength.clsWeak,weight);
    return  this;
  }

  public ClSimplexSolver addPointStay(ClPoint clp)
  {
    addPointStay(clp,1.0);
  }

  public ClSimplexSolver addStay(ClVariable v, 
				 ClStrength strength,
				 double weight)
  { 
    ClStayConstraint cn = new ClStayConstraint(v,strength,weight);
    return addConstraint( pcn); 
  }

  public ClSimplexSolver addStay(ClVariable v, 
				 ClStrength strength)
  { 
    addStay(v,strength,1.0);
  }

  public ClSimplexSolver addStay(ClVariable v)
  { 
    addStay(v,ClStrength.clsWeak,1.0);
  }


  public ClSimplexSolver removeConstraint(ClConstraint cnconst)
  {
    //#ifndef CL_NO_TRACE
    //Tracer TRACER(__FUNCTION__);
    //System.err.println("(" + String.valueOf(cnconst) + ")");
    //#endif

    resetStayConstants();

    ClLinearExpression zRow = rowExpression(my_objective);


  ClConstraintToVarSetMap::iterator 
    it_eVars = my_errorVars.find(cn);
  if (it_eVars != my_errorVars.end())
    {
    ClTableauVarSet eVars = ( it_eVars).second;
    ClTableauVarSet::iterator it = eVars.begin();
    for ( ; it != eVars.end(); ++it )
      {
      final ClLinearExpression  pexpr = rowExpression( ( it));
      if (pexpr == null )
	{
	pzRow.addVariable( ( it),-1.0   cnconst.strength().symbolicWeight().asDouble(),
			   my_objective, this);
	}
      else
	{ // the error variable was in the basis
	pzRow.addExpression( pexpr,-1.0   cnconst.strength().symbolicWeight().asDouble(),
			     my_objective, this);
	}
      }
    }

  map<static final ClConstraint  , static final ClAbstractVariable  >::iterator 
    it_marker = my_markerVars.find(cn);
  if (it_marker == my_markerVars.end())
    { // could not find the constraint
    throw new ExCLConstraintNotFound();
    }
  final ClAbstractVariable marker =  (( it_marker).second);
  my_markerVars.erase(it_marker);
  //#ifndef CL_NO_TRACE
  //System.err.println("Looking to remove var " + String.valueOf(marker));
  //#endif
  if (rowExpression(marker) == null )
    { // not in the basis, so need to do some work
    ClTableauVarSet col = my_columns[marker];
    ClTableauVarSet::iterator it_col = col.begin();
    //#ifndef CL_NO_TRACE
    //System.err.println("Must pivot -- columns are " + String.valueOf(col));
    //#endif

    final ClAbstractVariable  pexitVar = null;
    double minRatio = 0.0;
    for ( ; it_col != col.end(); ++it_col) 
      {
      final ClAbstractVariable  pv =  it_col;
      if (pv.isRestricted() )
	{
	final ClLinearExpression  pexpr = rowExpression( pv);
	assert(pexpr != null );
	Number coeff = pexpr.coefficientFor(marker);
	//#ifndef CL_NO_TRACE
	//System.err.print("Marker " + String.valueOf(marker) + "'s coefficient in " + String.valueOf(*pexpr) + " is ");
	//#endif
	if (coeff < 0.0) 
	  {
	  Number r = - pexpr.constant() / coeff;
	  if (pexitVar == null || r < minRatio)
	    {
	    minRatio = r;
	    pexitVar = pv;
	    }
	  }
	}
      }
    if (pexitVar == null ) {
      //#ifndef CL_NO_TRACE
      //System.err.println("pexitVar is still NULL");
      //#endif
      it_col = col.begin();
      for ( ; it_col != col.end(); ++it_col) {
	final ClAbstractVariable  pv =  it_col;
	if (pv.isRestricted() ) {
	  final ClLinearExpression  pexpr = rowExpression( pv);
	  // assert(pexpr != null);
	  Number coeff = pexpr.coefficientFor(marker);
	  Number r = pexpr.constant() / coeff;
	  if (pexitVar == null || r < minRatio)
	    {
	      minRatio = r;
	      pexitVar = pv;
	    }
	}
      }
    }
    
    if (pexitVar == null)
      { // exitVar is still nil
      if (col.size() == 0)
	{
	removeColumn(marker);
	}
      else
	{
	pexitVar =  (col.begin());
	}
      }
    
    if (pexitVar != null)
      {
      pivot(marker, pexitVar);
      }
    }
  
  if (rowExpression(marker) != null )
    {
    ClLinearExpression  pexpr = removeRow(marker);
    //#ifndef CL_NO_TRACE
    //System.err.println("delete@ " + String.valueOf(pexpr));
    //#endif
    pexpr = null;
    }

  if (it_eVars != my_errorVars.end())
    {
    ClTableauVarSet eVars = ( it_eVars).second;
    ClTableauVarSet::iterator it = eVars.begin();
    for ( ; it != eVars.end(); ++it )
      {
      final ClAbstractVariable  pv = ( it);
      if ( pv != marker)
	{
	removeColumn( pv);
	pv = null;
	}
      }
    }

  if (cn.isStayConstraint())
    {
    if (it_eVars != my_errorVars.end())
      {
      ClTableauVarSet eVars = ( it_eVars).second;
      ClVarVector::iterator itStayPlusErrorVars = my_stayPlusErrorVars.begin();
      ClVarVector::iterator itStayMinusErrorVars = my_stayMinusErrorVars.begin();
      for (; itStayMinusErrorVars != my_stayMinusErrorVars.end();
	   ++itStayPlusErrorVars, ++itStayMinusErrorVars)
	{
	if (eVars.find( itStayPlusErrorVars) != eVars.end())
	  {
	  my_stayMinusErrorVars.erase(itStayPlusErrorVars);
	  }
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
    ClTableauVarSet eVars = ( it_eVars).second;
    ClTableauVarSet::iterator it_v = eVars.begin();
    for ( ; it_v != eVars.end(); ++it_v)
      {
      final ClAbstractVariable  pvar =  it_v;
      ClVarVector::iterator 
	itEditPlusErrorVars = my_editPlusErrorVars.begin();
      for ( ; itEditPlusErrorVars != my_editPlusErrorVars.end(); ++itEditPlusErrorVars )
	{
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
  marker = null;

  optimize(my_objective);
  setExternalVariables();
  return  this;
  }
  
  public void reset()
  {
    //#ifndef CL_NO_TRACE
    //Tracer TRACER(__FUNCTION__);
    //System.err.println("()");
    throw new ExCLInternalError();
  }
  
  public void resolve(Vector newEditConstants)
  { // CODE DUPLICATED BELOW
    //#ifndef CL_NO_TRACE
    //  Tracer TRACER(__FUNCTION__);
    //System.err.println("(" + String.valueOf(newEditConstants) + ")");
    //#endif
    my_infeasibleRows.clear();
    resetStayConstants();
    resetEditConstants(newEditConstants);
    dualOptimize();
    setExternalVariables();
  }

  public void resolve(Number x, Number y)
  {
    vector<Number> vals;
    vals.push_back(x);
    vals.push_back(y);
    resolve(vals);
  }

  public String toString()
  { 
    StringBuffer bstr = new StringBuffer(super.toString());
    bstr.append("my_editPlusErrorVars: ");
    bstr.append(my_editPlusErrorVars);
    bstr.append("my_editMinusErrorVars: ");
    bstr.append(my_editMinusErrorVars);

    bstr.append("my_stayPlusErrorVars: ");
    bstr.append(my_stayPlusErrorVars);
    bstr.append("my_stayMinusErrorVars: ");
    bstr.append(my_stayMinusErrorVars);

    bstr.append("my_prevEditConstants: ");
    bstr.append(my_prevEditConstants);

    bstr.append("\n");
    return bstr.toString();
  }

  protected void addWithArtificialVariable(ClLinearExpression expr)
  {
    //#ifndef CL_NO_TRACE
    // Tracer TRACER(__FUNCTION__);
    // System.err.println("(" + String.valueOf(expr) + ")");
    // #endif
  
  ClSlackVariable pav = new ClSlackVariable(++my_artificialCounter,"a");
  ClObjectiveVariable paz = new ClObjectiveVariable("az");
  ClLinearExpression pazRow = new ClLinearExpression(expr);

  //#ifndef CL_NO_TRACE
  //System.err.println(String.valueOf(__FUNCTION__) + " before addRow-s:");
  //System.err.println(String.valueOf((*this)));
  //#endif

  addRow( paz, pazRow);
  addRow( pav,expr);

  //#ifndef CL_NO_TRACE
  //System.err.println(String.valueOf(__FUNCTION__) + " after addRow-s:");
  //System.err.println(String.valueOf((*this)));
  //#endif

  optimize( paz);

  ClLinearExpression  pazTableauRow = rowExpression( paz);
  //#ifndef CL_NO_TRACE
  //System.err.println("pazTableauRow->constant() == " + String.valueOf(pazTableauRow->constant()));
  //#endif

  if (!ClVariable.clApprox(pazTableauRow.constant(),0.0))
    {
    throw new ExCLRequiredFailure();
    }

  final ClLinearExpression  pe = rowExpression( pav);

  if (pe != null )
    {
    if (pe.isConstant())
      {
	/** @c2j++ Replacement from delete removeRow(*pav); */
	removeRow(*pav) = null;
	/** @c2j++ Replacement from delete pav; */
	pav = null;
      return;
      }
    final ClAbstractVariable  pentryVar = pe.anyVariable();
    pivot( pentryVar,  pav);
    }
  assert(rowExpression( pav) == null);
  removeColumn( pav);
  /** @c2j++ Replacement from delete pav; */
  pav = null;
  /** @c2j++ Replacement from delete removeRow(*paz); */
  removeRow(*paz) = null;
  }

  protected boolean tryAddingDirectly(ClLinearExpression expr)
  {
    //#ifndef CL_NO_TRACE
    //Tracer TRACER(__FUNCTION__);
    //System.err.println("(" + String.valueOf(expr) + ")");
    //#endif
  final ClAbstractVariable  psubject = chooseSubject(expr);
  if (psubject == null )
    {
      //#ifndef CL_NO_TRACE
      //System.err.println("- returning false");
      //#endif
    return false;
    }
  expr.newSubject( psubject);
  if (columnsHasKey( psubject))
    {
    substituteOut( psubject,expr);
    }
  addRow( psubject,expr);
  //#ifndef CL_NO_TRACE
  //System.err.println("- returning true");
  //#endif
  return true; // successfully added directly
  }

  protected ClAbstractVariable chooseSubject(ClLinearExpression expr)
  {
    //#ifndef CL_NO_TRACE
    //Tracer TRACER(__FUNCTION__);
    //System.err.println("(" + String.valueOf(expr) + ")");
    //#endif
  final ClAbstractVariable  psubject = null; // the current best subject, if any

  boolean foundUnrestricted = false; 

  boolean foundNewRestricted = false;

  final ClVarToNumberMap terms = expr.terms();
  ClVarToNumberMap::const_iterator it = terms.begin();
  for ( ; it != terms.end(); ++it )
    {
    final ClAbstractVariable  pv = ( it).first;
    Number c = ( it).second;

    if (foundUnrestricted)
      {
      if (!pv.isRestricted())
	{
	if (!columnsHasKey( pv))
	  return pv;
	}
      }
    else
      { // we haven't found an restricted variable yet
      if (pv.isRestricted())
	{
	if (!foundNewRestricted && !pv.isDummy() && c < 0.0)
	  {
	  final ClTableauColumnsMap col = columns();
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
    final ClAbstractVariable  pv = ( it).first;
    Number c = ( it).second;
    if (!pv.isDummy())
      return null; // nope, no luck
    if (!columnsHasKey( pv))
      {
      psubject = pv;
      coeff = c;
      }
    }

  if (!ClVariable.clApprox(expr.constant(),0.0))
    {
    throw new ExCLRequiredFailure();
    }
  if (coeff > 0.0)
    {
    expr.multiplyMe(-1);
    }
  return psubject;
}

  protected void deltaEditConstant(Number delta, 
				   ClAbstractVariable plusErrorVar, 
				   ClAbstractVariable minusErrorVar)
  {
    //#ifndef CL_NO_TRACE
    //  Tracer TRACER(__FUNCTION__);
    // System.err.println("(" + String.valueOf(delta) + ", " + String.valueOf(plusErrorVar) + ", " + String.valueOf(minusErrorVar) + ")");
    // #endif
    ClLinearExpression  pexprPlus = rowExpression(plusErrorVar);
    if (pexprPlus != null )
      {
      pexprPlus.incrementConstant(delta);
	
      if (pexprPlus.constant() < 0.0)
	{
	  my_infeasibleRows.insert(plusErrorVar);
	}
      return;
      }
  ClLinearExpression  pexprMinus = rowExpression(minusErrorVar);
  if (pexprMinus != null)
    {
    pexprMinus.incrementConstant(-delta);
    if (pexprMinus.constant() < 0.0)
      {
      my_infeasibleRows.insert(minusErrorVar);
      }
    return;
    }
  ClTableauVarSet columnVars = my_columns[minusErrorVar];
  ClTableauVarSet::iterator it = columnVars.begin();
  for (; it != columnVars.end(); ++it)
    {
    final ClAbstractVariable  pbasicVar =  it;
    ClLinearExpression  pexpr = rowExpression( pbasicVar);
    //    assert(pexpr != null );
    double c = pexpr.coefficientFor(minusErrorVar);
    pexpr.incrementConstant(c * delta);
    if (pbasicVar.isRestricted() && pexpr.constant() < 0.0)
      {
      my_infeasibleRows.insert(pbasicVar);
      }
    }
  }

  protected void dualOptimize()
  {
    //#ifndef CL_NO_TRACE
    // Tracer TRACER(__FUNCTION__);
    //System.err.println("()");
    //#endif
    final ClLinearExpression  pzRow = rowExpression(my_objective);
    while (!my_infeasibleRows.empty()) {
      ClTableauVarSet::iterator it_exitVar = my_infeasibleRows.begin();
      final ClAbstractVariable  pexitVar =  it_exitVar;
      my_infeasibleRows.erase(it_exitVar);
      final ClAbstractVariable  pentryVar = null;
      ClLinearExpression  pexpr = rowExpression( pexitVar);
      if (pexpr != null ) {
	if (pexpr.constant() < 0.0) {
	  double ratio = DBL_MAX;
	  double r;
	  ClVarToNumberMap terms = pexpr.terms();
	  ClVarToNumberMap::iterator it = terms.begin();
	  for ( ; it != terms.end(); ++it ) {
	    final ClAbstractVariable  pv = ( it).first;
	    Number c = ( it).second;
	    if (c > 0.0 && pv.isPivotable()) {
	      Number zc = pzRow.coefficientFor( pv);
	      r = zc/c; // FIXGJB r:= zc/c or zero, as ClSymbolicWeight-s
	      if (r < ratio) {
		pentryVar = pv;
		ratio = r;
	      }
	    }
	  }
	  if (ratio == DBL_MAX) {
	    System.err.println("ratio == nil (DBL_MAX)");
	    throw new ExCLInternalError();
	  }
	  pivot( pentryVar, pexitVar);
	}
      }
    }
  }

  protected ClLinearExpression newExpression(ClConstraint cn)
  {
    //#ifndef CL_NO_TRACE
    // Tracer TRACER(__FUNCTION__);
    // System.err.println("(" + String.valueOf(cn) + ")");
    // System.err.println("cn.isInequality() == " + String.valueOf(cn.isInequality()));
    //System.err.println("cn.isRequired() == " + String.valueOf(cn.isRequired()));
    //#endif
    final ClLinearExpression cnExpr = cn.expression();
    auto_ptr<ClLinearExpression> pexpr ( new ClLinearExpression(cnExpr.constant()) );
    auto_ptr<ClSlackVariable> pslackVar;
    auto_ptr<ClDummyVariable> pdummyVar;
    auto_ptr<ClSlackVariable> peminus;
    auto_ptr<ClSlackVariable> peplus;
    final ClVarToNumberMap cnTerms = cnExpr.terms();
    ClVarToNumberMap::const_iterator it = cnTerms.begin();
    for ( ; it != cnTerms.end(); ++it)
      {
    final ClAbstractVariable  pv = ( it).first;
    Number c = ( it).second;
    final ClLinearExpression  pe = rowExpression( pv);
    if (pe == null)
      {
      pexpr.addVariable( pv,c);
      }
    else
      {
      pexpr.addExpression( pe,c);
      }
    }

  if (cn.isInequality())
    {
    ++my_slackCounter;
    ReinitializeAutoPtr(pslackVar,new ClSlackVariable (my_slackCounter, "s"));
    pexpr.setVariable( pslackVar,-1);
    my_markerVars[cn] = pslackVar.get();
    if (!cn.isRequired())
      {
      ++my_slackCounter;
      ReinitializeAutoPtr(peminus,new ClSlackVariable (my_slackCounter, "em"));
      pexpr.setVariable( peminus,1.0);
      ClLinearExpression  pzRow = rowExpression(my_objective);
      ClSymbolicWeight sw = cn.strength().symbolicWeight().times(cn.weight());
      pzRow.setVariable( peminus,sw.asDouble());
      my_errorVars[cn].insert(peminus.get());
      noteAddedVariable( peminus,my_objective);
      }
    }
  else
    { // cn is an equality
    if (cn.isRequired())
      {
      ++my_dummyCounter;
      ReinitializeAutoPtr(pdummyVar,new ClDummyVariable (my_dummyCounter, "d"));
      pexpr.setVariable( pdummyVar,1.0);
      my_markerVars[cn] = pdummyVar.get();
      //#ifndef CL_NO_TRACE
      //System.err.println("Adding dummyVar == d" + String.valueOf(my_dummyCounter));
      //#endif
      }
    else
      {
      ++my_slackCounter;
      ReinitializeAutoPtr(peplus,new ClSlackVariable (my_slackCounter, "ep"));
      ReinitializeAutoPtr(peminus,new ClSlackVariable (my_slackCounter, "em"));

      pexpr.setVariable( peplus,-1.0);
      pexpr.setVariable( peminus,1.0);
      my_markerVars[cn] = peplus.get();
      ClLinearExpression  pzRow = rowExpression(my_objective);
      ClSymbolicWeight sw = cn.strength().symbolicWeight().times(cn.weight());
      double swCoeff = sw.asDouble();
      //#ifndef CL_NO_TRACE
      if (swCoeff == 0) 
	{
	  //System.err.print("sw == " + String.valueOf(sw));
	  //	     << "cn == " << cn << endl;
	  // System.err.print("adding " + String.valueOf(*peplus) + " and " + String.valueOf(*peminus));
	  // << " with swCoeff == " << swCoeff << endl;
	}
      //#endif      
      pzRow.setVariable( peplus,swCoeff);
      noteAddedVariable( peplus,my_objective);
      pzRow.setVariable( peminus,swCoeff);
      noteAddedVariable( peminus,my_objective);
      my_errorVars[cn].insert(peminus.get());
      my_errorVars[cn].insert(peplus.get());
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

  if (pexpr.constant() < 0)
    {
    pexpr.multiplyMe(-1);
    }
  //#ifndef CL_NO_TRACE
  //System.err.println("- returning " + String.valueOf(*pexpr));
  //#endif
  pslackVar.release();
  pdummyVar.release();
  peminus.release();
  peplus.release();
  return pexpr.release();
  }

  protected void optimize(ClObjectiveVariable zVar)
  {
    //#ifndef CL_NO_TRACE
    //Tracer TRACER(__FUNCTION__);
    //System.err.println("(" + String.valueOf(zVar) + ")");
    //System.err.println(String.valueOf(*this));
    //#endif
  ClLinearExpression  pzRow = rowExpression(zVar);
  //  assert(pzRow != null);
  final ClAbstractVariable  pentryVar = null;
  final ClAbstractVariable  pexitVar = null;
  while (true)
    {
    Number objectiveCoeff = 0;
    ClVarToNumberMap terms = pzRow.terms();
    ClVarToNumberMap::iterator it = terms.begin();
    for (; it != terms.end(); ++it)
      {
      final ClAbstractVariable  pv = ( it).first;
      Number c = ( it).second;
      if (pv.isPivotable() && c < objectiveCoeff)
	{
	objectiveCoeff = c;
	pentryVar = pv;
	}
      }
    if (objectiveCoeff == 0)
      return;
    //#ifndef CL_NO_TRACE
    //System.err.print("*pentryVar == " + String.valueOf(*pentryVar) + ", ");
    // << "objectiveCoeff == " << objectiveCoeff
    // << endl;
    //#endif

    double minRatio = DBL_MAX;
    ClTableauVarSet columnVars = my_columns[pentryVar];
    ClTableauVarSet::iterator it_rowvars = columnVars.begin();
    Number r = 0.0;
    for (; it_rowvars != columnVars.end(); ++it_rowvars)
      {
      final ClAbstractVariable  pv =  it_rowvars;
      //#ifndef CL_NO_TRACE
      //System.err.println("Checking " + String.valueOf(*pv));
      //#endif
      if (pv.isPivotable()) 
	{
	final ClLinearExpression  pexpr = rowExpression( pv);
	Number coeff = pexpr.coefficientFor( pentryVar);
	if (coeff < 0.0)
	  {
	  r = - pexpr.constant() / coeff;
	  if (r < minRatio)
	    {
	      //#ifndef CL_NO_TRACE
	      //System.err.println("New minRatio == " + String.valueOf(r));
	      //#endif
	    minRatio = r;
	    pexitVar = pv;
	    }
	  }
	}
      }
    if (minRatio == DBL_MAX)
      {
	//System.err.println("objective function is unbounded!");
	throw new ExCLInternalError();
      }
    pivot( pentryVar,  pexitVar);
    //#ifndef CL_NO_TRACE
    //System.err.println(String.valueOf(*this));
    //#endif
    }
  }

  protected void pivot(ClAbstractVariable entryVar, 
		       ClAbstractVariable exitVar)
  {
  //#ifndef CL_NO_TRACE
  //  Tracer TRACER(__FUNCTION__);
  //System.err.println("(" + String.valueOf(entryVar) + ", " + String.valueOf(exitVar) + ")");
  //#endif
  
  ClLinearExpression  pexpr = removeRow(exitVar);

  pexpr.changeSubject(exitVar,entryVar);
  substituteOut(entryVar, pexpr);
  addRow(entryVar, pexpr);
  }
  
  protected void resetEditConstants(Vector newEditConstants)
  {
  //#ifndef CL_NO_TRACE
  //  Tracer TRACER(__FUNCTION__);
  //System.err.println("(" + String.valueOf(newEditConstants) + ")");
  //#endif
  if (newEditConstants.size() != my_editPlusErrorVars.size())
    { // number of edit constants doesn't match the number of edit error variables
      //System.err.print("newEditConstants == " + String.valueOf(newEditConstants));
      // << "my_editPlusErrorVars == " << my_editPlusErrorVars << endl
      // << "Sizes don't match!" << endl;
    throw new ExCLInternalError();
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
    Number delta = ( itNew) - ( itPrev);
    ( itPrev) = ( itNew);
    deltaEditConstant(delta, ( itEditPlusErrorVars), ( itEditMinusErrorVars));
    }
  }
  
  protected void resetStayConstants()
  {
  //#ifndef CL_NO_TRACE
  //  Tracer TRACER(__FUNCTION__);
  //System.err.println("()");
  //#endif
  ClVarVector::const_iterator 
    itStayPlusErrorVars = my_stayPlusErrorVars.begin();
  ClVarVector::const_iterator 
    itStayMinusErrorVars = my_stayMinusErrorVars.begin();

  for ( ; itStayPlusErrorVars != my_stayPlusErrorVars.end();
	++itStayPlusErrorVars, ++itStayMinusErrorVars )
    {
    ClLinearExpression  pexpr = rowExpression( ( itStayPlusErrorVars));
    if (pexpr == null )
      {
      pexpr = rowExpression( ( itStayMinusErrorVars));
      }
    if (pexpr != null)
      {
      pexpr.set_constant(0.0);
      }
    }
  }

  protected void setExternalVariables()
  {
  //#ifndef CL_NO_TRACE
  //  Tracer TRACER(__FUNCTION__);
  //System.err.println("()");
  //System.err.println(String.valueOf(*this));
  //#endif


  ClExternalVarSet::iterator itParVars = my_externalParametricVars.begin();
  for ( ; itParVars != my_externalParametricVars.end(); ++itParVars )
    {
    ClVariable  pv = const_cast<ClVariable  >( itParVars);
    pv.set_value(0.0);
    }

  ClExternalVarSet::iterator itRowVars = my_externalRows.begin();
  for ( ; itRowVars != my_externalRows.end() ; ++itRowVars )
    {
    ClVariable  pv = const_cast<ClVariable  >( itRowVars);
    ClLinearExpression  pexpr = rowExpression( pv);
    pv.set_value(pexpr.constant());
    }
  }

  Vector my_editMinusErrorVars;
  Vector my_editPlusErrorVars;
  Vector my_stayMinusErrorVars;
  Vector my_stayPlusErrorVars;
  Vector my_prevEditConstants;
  Hashtable my_errorVars; // map ClConstraint to Set (of ClVariable)
  Hashtable my_markerVars; // map ClConstraint to ClVariable

  ClObjectiveVariable my_objective;
  long my_slackCounter;
  long my_artificialCounter;
  long my_dummyCounter;
  
}
