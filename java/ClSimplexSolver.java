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
    my_editMinusErrorVars = new Vector();
    my_editPlusErrorVars = new Vector();
    my_stayMinusErrorVars = new Vector();
    my_stayPlusErrorVars = new Vector();
    my_prevEditConstants = new Vector();
    my_errorVars = new Hashtable();
    my_markerVars = new Hashtable();

    my_objective = new ClObjectiveVariable("Z");
    my_slackCounter = 0;
    my_artificialCounter = 0;
    my_dummyCounter = 0;
    ClLinearExpression e = new ClLinearExpression();
    my_rows.put(my_objective,e);
    traceprint("objective expr == " + rowExpression(my_objective));
  }

  public ClSimplexSolver addLowerBound(ClAbstractVariable v, double lower)
  { 
    try {
      ClLinearInequality cn = 
	new ClLinearInequality(v,CL.GEQ,new ClLinearExpression(lower));
      return this;
    }
    catch (ExCLInternalError err) {
      System.err.println("Broken CL.GEQ!");
      return this;
    }
  }

  public ClSimplexSolver addUpperBound(ClAbstractVariable v, double upper)
  { 
    try {
      ClLinearInequality cn = 
	new ClLinearInequality(v,CL.LEQ,new ClLinearExpression(upper));
      return this;
    }
    catch (ExCLInternalError err) {
      System.err.println("Broken CL.LEQ!");
      return this;
    }
  }

  public ClSimplexSolver addBounds(ClAbstractVariable v,
				   double lower, double upper)
  { addLowerBound(v,lower); addUpperBound(v,upper); return  this; }

  public ClSimplexSolver addConstraint(ClConstraint cn)
       throws ExCLRequiredFailure, ExCLInternalError
  {
    fnenterprint("addConstraint: " + cn);
    
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
       throws ExCLRequiredFailure, ExCLInternalError
  {
    fnenterprint("addPointStays" + listOfPoints);
    double weight = 1.0;
    final double multiplier = 2.0;
    for (int i = 0; i < listOfPoints.size(); i++) {
      addPointStay((ClPoint) listOfPoints.elementAt(i),weight);
      weight *= multiplier;
    }
    return this;
  }

  public ClSimplexSolver addPointStay(ClVariable vx, 
				      ClVariable vy, 
				      double weight)
       throws ExCLRequiredFailure, ExCLInternalError
  { 
    addStay(vx,ClStrength.weak,weight);
    addStay(vy,ClStrength.weak,weight);
    return  this;
  }

  public ClSimplexSolver addPointStay(ClVariable vx, 
				      ClVariable vy)
       throws ExCLRequiredFailure, ExCLInternalError
  { addPointStay(vx,vy,1.0); return this; }
  
  public ClSimplexSolver addPointStay(ClPoint clp, double weight)
       throws ExCLRequiredFailure, ExCLInternalError
  { 
    addStay(clp.X(),ClStrength.weak,weight);
    addStay(clp.Y(),ClStrength.weak,weight);
    return this;
  }

  public ClSimplexSolver addPointStay(ClPoint clp)
       throws ExCLRequiredFailure, ExCLInternalError
  {
    addPointStay(clp,1.0); 
    return this;
  }

  public ClSimplexSolver addStay(ClVariable v, 
				 ClStrength strength,
				 double weight)
       throws ExCLRequiredFailure, ExCLInternalError
  { 
    ClStayConstraint cn = new ClStayConstraint(v,strength,weight);
    return addConstraint(cn); 
  }

  public ClSimplexSolver addStay(ClVariable v, 
				 ClStrength strength)
       throws ExCLRequiredFailure, ExCLInternalError
  { 
    addStay(v,strength,1.0); return this;
  }

  public ClSimplexSolver addStay(ClVariable v)
       throws ExCLRequiredFailure, ExCLInternalError
  { 
    addStay(v,ClStrength.weak,1.0); return this;
  }


  public ClSimplexSolver removeConstraint(ClConstraint cn)
       throws ExCLConstraintNotFound, ExCLInternalError
  {
    fnenterprint("removeConstraint: " + cn);
    traceprint(this.toString());

    resetStayConstants();
    
    ClLinearExpression zRow = rowExpression(my_objective);
    
    Set eVars = (Set) my_errorVars.get(cn);
    traceprint("eVars == " + eVars);

    if (eVars != null) {
      for (Enumeration e = eVars.elements(); e.hasMoreElements() ; ) {
	ClAbstractVariable clv = (ClAbstractVariable) e.nextElement();
	final ClLinearExpression expr = rowExpression(clv);
	if (expr == null ) {
	  zRow.addVariable(clv, -1.0 * 
			    cn.strength().symbolicWeight().asDouble(),
			    my_objective, this);
	} else { // the error variable was in the basis
	  zRow.addExpression(expr, -1.0 *
			     cn.strength().symbolicWeight().asDouble(),
			     my_objective, this);
	}
      }
    }
    
    ClAbstractVariable marker = (ClAbstractVariable) my_markerVars.remove(cn);
    if (marker == null) {
      throw new ExCLConstraintNotFound();
    }

    traceprint("Looking to remove var " + marker);

    if (rowExpression(marker) == null ) {
      // not in the basis, so need to do some work
      Set col = (Set) my_columns.get(marker);

      traceprint("Must pivot -- columns are " + col);

      ClAbstractVariable exitVar = null;
      double minRatio = 0.0;
      for (Enumeration e = col.elements(); e.hasMoreElements() ; ) {
	final ClAbstractVariable v = (ClAbstractVariable) e.nextElement();
	if (v.isRestricted() ) {
	  final ClLinearExpression expr = rowExpression( v);
	  double coeff = expr.coefficientFor(marker);
	  traceprint("Marker " + marker + "'s coefficient in " + expr + " is " + coeff);
	  if (coeff < 0.0) {
	    double r = -expr.constant() / coeff;
	    if (exitVar == null || r < minRatio) {
	      minRatio = r;
	      exitVar = v;
	    }
	  }
	}
      }
      if (exitVar == null ) {
	traceprint("exitVar is still null");
	for (Enumeration e = col.elements(); e.hasMoreElements() ; ) {
	  final ClAbstractVariable v = (ClAbstractVariable) e.nextElement();
	  if (v.isRestricted() ) {
	    final ClLinearExpression expr = rowExpression(v);
	    double coeff = expr.coefficientFor(marker);
	    double r = expr.constant() / coeff;
	    if (exitVar == null || r < minRatio) {
	      minRatio = r;
	      exitVar = v;
	    }
	  }
	}
      }
      
      if (exitVar == null) {
	// exitVar is still nil
	if (col.size() == 0) {
	  removeColumn(marker);
	} else {
	  exitVar = (ClAbstractVariable) col.elements().nextElement();
	}
      }
      
      if (exitVar != null) {
	pivot(marker, exitVar);
      }
    }
  
    if (rowExpression(marker) != null ) {
      ClLinearExpression expr = removeRow(marker);
      expr = null;
    }
    
    if (eVars != null) {
      for (Enumeration e = eVars.elements(); e.hasMoreElements(); ) {
	ClAbstractVariable v = (ClAbstractVariable) e.nextElement();
	if ( v != marker) {
	  removeColumn(v);
	  v = null;
	}
      }
    }

    if (cn.isStayConstraint()) {
      if (eVars != null) {
	for (int i = 0; i < my_stayPlusErrorVars.size(); i++) {
	  eVars.remove(my_stayPlusErrorVars.elementAt(i));
	  eVars.remove(my_stayMinusErrorVars.elementAt(i));
	}
      }
    } else if (cn.isEditConstraint()) {
      assert(eVars != null);
      for (Enumeration e = eVars.elements(); e.hasMoreElements(); ) {
	final ClAbstractVariable var = (ClAbstractVariable) e.nextElement();
	int i = 0;
	for ( ; i < my_editPlusErrorVars.size(); i++) {
	  if ( my_editPlusErrorVars.elementAt(i) == var) break;
	}
	if (i != my_editPlusErrorVars.size()) {
	  // found it
	  my_editPlusErrorVars.removeElementAt(i);
	  my_editMinusErrorVars.removeElementAt(i);
	  my_prevEditConstants.removeElementAt(i);
	  break;
	}
      }
    }

    // FIXGJB do the remove at top
    if (eVars != null) {
      my_errorVars.remove(eVars);
    }
    marker = null;

    optimize(my_objective);
    setExternalVariables();
    return  this;
  }
  
  public void reset()
       throws ExCLInternalError
  {
    fnenterprint("reset");
    throw new ExCLInternalError();
  }
  
  public void resolve(Vector newEditConstants)
       throws ExCLInternalError
  { // CODE DUPLICATED BELOW
    fnenterprint("resolve" + newEditConstants);
    my_infeasibleRows.clear();
    resetStayConstants();
    resetEditConstants(newEditConstants);
    dualOptimize();
    setExternalVariables();
  }

  public void resolve(double x, double y)
       throws ExCLInternalError
  {
    Vector vals = new Vector(2);
    vals.addElement(new Double(x));
    vals.addElement(new Double(y));
    resolve(vals);
  }

  public String toString()
  { 
    StringBuffer bstr = new StringBuffer(super.toString());
    bstr.append("my_editPlusErrorVars: ");
    bstr.append(my_editPlusErrorVars);
    bstr.append("\nmy_editMinusErrorVars: ");
    bstr.append(my_editMinusErrorVars);

    bstr.append("\nmy_stayPlusErrorVars: ");
    bstr.append(my_stayPlusErrorVars);
    bstr.append("\nmy_stayMinusErrorVars: ");
    bstr.append(my_stayMinusErrorVars);

    bstr.append("\nmy_prevEditConstants: ");
    bstr.append(my_prevEditConstants);

    bstr.append("\n");
    return bstr.toString();
  }

  protected void addWithArtificialVariable(ClLinearExpression expr)
       throws ExCLRequiredFailure, ExCLInternalError
  {
    fnenterprint("addWithArtificialVariable: " + expr);
  
    ClSlackVariable av = new ClSlackVariable(++my_artificialCounter,"a");
    ClObjectiveVariable az = new ClObjectiveVariable("az");
    ClLinearExpression azRow = (ClLinearExpression) expr.clone();

    traceprint("before addRows:\n" + this);

    addRow( az, azRow);
    addRow( av, expr);

    traceprint("after addRows:\n" + this);
    optimize(az);
    
    ClLinearExpression azTableauRow = rowExpression(az);

    traceprint("azTableauRow.constant() == " + azTableauRow.constant());
    
    if (!CL.approx(azTableauRow.constant(),0.0)) {
      throw new ExCLRequiredFailure();
    }

    final ClLinearExpression e = rowExpression(av);

    if (e != null ) {
      if (e.isConstant()) {
	removeRow(av);
	return;
      }
      ClAbstractVariable entryVar = e.anyVariable();
      pivot( entryVar, av);
    }
    assert(rowExpression(av) == null);
    removeColumn(av);
    removeRow(az);
  }

  protected boolean tryAddingDirectly(ClLinearExpression expr)
       throws ExCLRequiredFailure
  {
    fnenterprint("tryAddingDirectly: " + expr );
    final ClAbstractVariable subject = chooseSubject(expr);
    if (subject == null ) {
      fnexitprint("returning false");
      return false;
    }
    expr.newSubject( subject);
    if (columnsHasKey( subject)) {
      substituteOut( subject,expr);
    }
    addRow( subject,expr);
    fnexitprint("returning true");
    return true; // successfully added directly
  }

  protected ClAbstractVariable chooseSubject(ClLinearExpression expr)
       throws ExCLRequiredFailure
  {
    fnenterprint("chooseSubject: " + expr);
    ClAbstractVariable subject = null; // the current best subject, if any
    
    boolean foundUnrestricted = false; 
    boolean foundNewRestricted = false;
    
    final Hashtable terms = expr.terms();
    
    for (Enumeration e = terms.keys(); e.hasMoreElements() ; ) {
      final ClAbstractVariable v = (ClAbstractVariable) e.nextElement();
      final double c = ((Double) terms.get(v)).doubleValue();
      
      if (foundUnrestricted){
	if (!v.isRestricted()) {
	  if (!columnsHasKey(v))
	    return v;
	}
      } else { 
	// we haven't found an restricted variable yet
	if (v.isRestricted()) {
	  if (!foundNewRestricted && !v.isDummy() && c < 0.0) {
	    final Set col = (Set) my_columns.get(v);
	    if (col == null || 
		( col.size() == 1 && columnsHasKey(my_objective) ) ) {
	    subject = v;
	    foundNewRestricted = true;
	    }
	  }
	} else {
	  subject = v;
	  foundUnrestricted = true;
	}
      }
    }
    
    if (subject != null)
      return subject;
    
    double coeff = 0.0;
    
    for (Enumeration e = terms.keys(); e.hasMoreElements() ;) {
      final ClAbstractVariable v = (ClAbstractVariable) e.nextElement();
      final double c = ((Double) terms.get(v)).doubleValue();
      if (!v.isDummy())
	return null; // nope, no luck
      if (!columnsHasKey(v)) {
	subject = v;
	coeff = c;
      }
    }
    
    if (!CL.approx(expr.constant(),0.0)) {
      throw new ExCLRequiredFailure();
    }
    if (coeff > 0.0) {
      expr.multiplyMe(-1);
    }
    return subject;
  }
  
  protected void deltaEditConstant(double delta, 
				   ClAbstractVariable plusErrorVar, 
				   ClAbstractVariable minusErrorVar)
  {
    fnenterprint("deltaEditConstant :" + delta + ", " + plusErrorVar + ", " + minusErrorVar);
    ClLinearExpression exprPlus = rowExpression(plusErrorVar);
    if (exprPlus != null ) {
      exprPlus.incrementConstant(delta);
      
      if (exprPlus.constant() < 0.0) {
	my_infeasibleRows.insert(plusErrorVar);
      }
      return;
    }
    
    ClLinearExpression exprMinus = rowExpression(minusErrorVar);
    if (exprMinus != null) {
      exprMinus.incrementConstant(-delta);
      if (exprMinus.constant() < 0.0) {
	my_infeasibleRows.insert(minusErrorVar);
      }
      return;
    }

    Set columnVars = (Set) my_columns.get(minusErrorVar);
    
    for (Enumeration e = columnVars.elements(); e.hasMoreElements() ; ) {
      final ClAbstractVariable basicVar = (ClAbstractVariable) e.nextElement();
      ClLinearExpression expr = rowExpression(basicVar);
      //assert(expr != null );
      final double c = expr.coefficientFor(minusErrorVar);
      expr.incrementConstant(c * delta);
      if (basicVar.isRestricted() && expr.constant() < 0.0) {
	my_infeasibleRows.insert(basicVar);
      }
    }
  }

  protected void dualOptimize()
       throws ExCLInternalError
  {
    fnenterprint("dualOptimize:");
    final ClLinearExpression zRow = rowExpression(my_objective);
    while (!my_infeasibleRows.isEmpty()) {
      ClAbstractVariable exitVar = 
	(ClAbstractVariable) my_infeasibleRows.elements().nextElement();
      my_infeasibleRows.remove(exitVar);
      ClAbstractVariable entryVar = null;
      ClLinearExpression expr = rowExpression(exitVar);
      if (expr != null ) {
	if (expr.constant() < 0.0) {
	  double ratio = Double.MAX_VALUE;
	  double r;
	  Hashtable terms = expr.terms();
	  for (Enumeration e = terms.keys(); e.hasMoreElements() ; ) {
	    ClAbstractVariable v = (ClAbstractVariable) e.nextElement();
	    double c = ((Double)terms.get(v)).doubleValue();
	    if (c > 0.0 && v.isPivotable()) {
	      double zc = zRow.coefficientFor(v);
	      r = zc/c; // FIXGJB r:= zc/c or zero, as ClSymbolicWeight-s
	      if (r < ratio) {
		entryVar = v;
		ratio = r;
	      }
	    }
	  }
	  if (ratio == Double.MAX_VALUE) {
	    System.err.println("ratio == nil (MAX_VALUE)");
	    throw new ExCLInternalError();
	  }
	  pivot( entryVar, exitVar);
	}
      }
    }
  }

  protected ClLinearExpression newExpression(ClConstraint cn)
  {
    fnenterprint("newExpression: " + cn);
    traceprint("cn.isInequality() == " + cn.isInequality());
    traceprint("cn.isRequired() == " + cn.isRequired());

    final ClLinearExpression cnExpr = cn.expression();
    ClLinearExpression expr = new ClLinearExpression(cnExpr.constant());
    ClSlackVariable slackVar = new ClSlackVariable();
    ClDummyVariable dummyVar = new ClDummyVariable();
    ClSlackVariable eminus = new ClSlackVariable();
    ClSlackVariable eplus = new ClSlackVariable();
    final Hashtable cnTerms = cnExpr.terms();
    for (Enumeration en = cnTerms.keys(); en.hasMoreElements(); ) {
      final ClAbstractVariable v = (ClAbstractVariable) en.nextElement();
      double c = ((Double) cnTerms.get(v)).doubleValue();
      final ClLinearExpression e = rowExpression(v);
      if (e == null)
	expr.addVariable(v,c);
      else
	expr.addExpression(e,c);
    }

    if (cn.isInequality()) {
      ++my_slackCounter;
      slackVar = new ClSlackVariable (my_slackCounter, "s");
      expr.setVariable(slackVar,-1);
      my_markerVars.put(cn,slackVar);
      if (!cn.isRequired()) {
	++my_slackCounter;
	eminus = new ClSlackVariable(my_slackCounter, "em");
	expr.setVariable(eminus,1.0);
	ClLinearExpression zRow = rowExpression(my_objective);
	ClSymbolicWeight sw = cn.strength().symbolicWeight().times(cn.weight());
	zRow.setVariable( eminus,sw.asDouble());
	insertErrorVar(cn,eminus);
	noteAddedVariable(eminus,my_objective);
      }
    }
    else {
      // cn is an equality
      if (cn.isRequired()) {
	++my_dummyCounter;
	dummyVar = new ClDummyVariable(my_dummyCounter, "d");
	expr.setVariable(dummyVar,1.0);
	my_markerVars.put(cn,dummyVar);
	traceprint("Adding dummyVar == d" + my_dummyCounter);
      } else {
	++my_slackCounter;
	eplus = new ClSlackVariable (my_slackCounter, "ep");
	eminus = new ClSlackVariable (my_slackCounter, "em");

	expr.setVariable( eplus,-1.0);
	expr.setVariable( eminus,1.0);
	my_markerVars.put(cn,eplus);
	ClLinearExpression zRow = rowExpression(my_objective);
	ClSymbolicWeight sw = cn.strength().symbolicWeight().times(cn.weight());
	double swCoeff = sw.asDouble();
	if (swCoeff == 0) {
	  traceprint("sw == " + sw);
	  traceprint("cn == " + cn);
	  traceprint("adding " + eplus + " and " + eminus + " with swCoeff == " + swCoeff);
	}
	zRow.setVariable(eplus,swCoeff);
	noteAddedVariable(eplus,my_objective);
	zRow.setVariable(eminus,swCoeff);
	noteAddedVariable(eminus,my_objective);
	insertErrorVar(cn,eminus);
	insertErrorVar(cn,eplus);
	if (cn.isStayConstraint()) {
	  my_stayPlusErrorVars.addElement(eplus);
	  my_stayMinusErrorVars.addElement(eminus);
	}
	else if (cn.isEditConstraint()) {
	  my_editPlusErrorVars.addElement(eplus);
	  my_editMinusErrorVars.addElement(eminus);
	  my_prevEditConstants.addElement(new Double(cnExpr.constant()));
	}
      }
    }
    
    if (expr.constant() < 0)
      expr.multiplyMe(-1);

    fnexitprint("returning " + expr);
    return expr;
  }

  protected void optimize(ClObjectiveVariable zVar)
       throws ExCLInternalError
  {
    fnenterprint("optimize: " + zVar);
    traceprint(this.toString());

    ClLinearExpression zRow = rowExpression(zVar);
    assert(zRow != null);
    ClAbstractVariable entryVar = null;
    ClAbstractVariable exitVar = null;
    while (true) {
      double objectiveCoeff = 0;
      Hashtable terms = zRow.terms();
      for (Enumeration e = terms.keys(); e.hasMoreElements() ; ) {
	ClAbstractVariable v = (ClAbstractVariable) e.nextElement();
	double c = ((Double) terms.get(v)).doubleValue();
	if (v.isPivotable() && c < objectiveCoeff) {
	  objectiveCoeff = c;
	  entryVar = v;
	}
      }
      if (objectiveCoeff == 0)
	return;
      traceprint("entryVar == " + entryVar + ", objectiveCoeff == " + objectiveCoeff);

      double minRatio = Double.MAX_VALUE;
      Set columnVars = (Set) my_columns.get(entryVar);
      double r = 0.0;
      for (Enumeration e = columnVars.elements(); e.hasMoreElements() ; ) {
	ClAbstractVariable v = (ClAbstractVariable) e.nextElement();
	traceprint("Checking " + v);
	if (v.isPivotable()) {
	  final ClLinearExpression expr = rowExpression(v);
	  double coeff = expr.coefficientFor(entryVar);
	  if (coeff < 0.0) {
	    r = - expr.constant() / coeff;
	    if (r < minRatio) {
	      traceprint("New minratio == " + r);
	      minRatio = r;
	      exitVar = v;
	    }
	  }
	}
      }
      if (minRatio == Double.MAX_VALUE) {
	System.err.println("objective function is unbounded!");
	throw new ExCLInternalError();
      }
      pivot(entryVar, exitVar);
      traceprint(this.toString());
    }
  }

  protected void pivot(ClAbstractVariable entryVar, 
		       ClAbstractVariable exitVar)
       throws ExCLInternalError
  {
    fnenterprint("pivot: " + entryVar + ", " + exitVar);

    ClLinearExpression  pexpr = removeRow(exitVar);

    pexpr.changeSubject(exitVar,entryVar);
    substituteOut(entryVar, pexpr);
    addRow(entryVar, pexpr);
  }
  
  protected void resetEditConstants(Vector newEditConstants)
       throws ExCLInternalError
  {
    fnenterprint("resetEditConstants:" + newEditConstants);

    if (newEditConstants.size() != my_editPlusErrorVars.size()) {
      // number of edit constants doesn't match the number of edit error variables
      System.err.println("newEditConstants == " + newEditConstants.toString());
      System.err.println("my_editPlusErrorVars == " + my_editPlusErrorVars.toString());
      System.err.println("sizes don't match");
      throw new ExCLInternalError();
    }

    for (int i = 0 ; i < newEditConstants.size(); i++) {
      double delta = (((Double)newEditConstants.elementAt(i)).doubleValue() - 
		      ((Double)my_prevEditConstants.elementAt(i)).doubleValue());
      my_prevEditConstants.setElementAt(newEditConstants.elementAt(i),i);
      deltaEditConstant(delta, 
			(ClAbstractVariable) my_editPlusErrorVars.elementAt(i),
			(ClAbstractVariable) my_editMinusErrorVars.elementAt(i));
    }
  }
  
  protected void resetStayConstants()
  {
    fnenterprint("resetStayConstants");

    for (int i = 0; i < my_stayPlusErrorVars.size(); i++) {
      ClLinearExpression expr = 
	rowExpression((ClAbstractVariable) my_stayPlusErrorVars.elementAt(i) );
      if (expr == null )
	expr = rowExpression((ClAbstractVariable) my_stayMinusErrorVars.elementAt(i));
      if (expr != null)
	expr.set_constant(0.0);
    }
  }

  protected void setExternalVariables()
  {
    fnenterprint("setExternalVariables:");
    traceprint(this.toString());
    
    for (Enumeration e = my_externalParametricVars.elements(); 
	 e.hasMoreElements() ; ) {
      ClVariable v = (ClVariable) e.nextElement();
      v.set_value(0.0);
    }
    
    for (Enumeration e = my_externalRows.elements(); e.hasMoreElements(); ) {
      ClVariable v = (ClVariable) e.nextElement();
      ClLinearExpression expr = rowExpression(v);
      debugprint("v == " + v);
      debugprint("expr == " + expr);
      v.set_value(expr.constant());
    }
  }

  private void insertErrorVar(ClConstraint cn, ClAbstractVariable var)
  { 
    fnenterprint("insertErrorVar:" + cn + ", " + var);

    Set cnset = (Set) my_errorVars.get(var);
    if (cnset == null)
      my_errorVars.put(cn,cnset = new Set());
    cnset.insert(var);
  }


  private Vector my_editMinusErrorVars;
  private Vector my_editPlusErrorVars;
  private Vector my_stayMinusErrorVars;
  private Vector my_stayPlusErrorVars;
  private Vector my_prevEditConstants;
  private Hashtable my_errorVars; // map ClConstraint to Set (of ClVariable)
  private Hashtable my_markerVars; // map ClConstraint to ClVariable

  private ClObjectiveVariable my_objective;
  private long my_slackCounter;
  private long my_artificialCounter;
  private long my_dummyCounter;
  
}
