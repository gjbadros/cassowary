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

    my_resolve_pair = new Vector(2);
    my_resolve_pair.addElement(new ClDouble(0));
    my_resolve_pair.addElement(new ClDouble(0));

    my_objective = new ClObjectiveVariable("Z");

    my_editVarMap = new Hashtable();

    my_slackCounter = 0;
    my_artificialCounter = 0;
    my_dummyCounter = 0;
    my_epsilon = 1e-8;
    ClLinearExpression e = new ClLinearExpression();
    my_rows.put(my_objective,e);
    if (fTraceOn) traceprint("objective expr == " + rowExpression(my_objective));
  }

  public final ClSimplexSolver addLowerBound(ClAbstractVariable v, double lower)
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

  public final ClSimplexSolver addUpperBound(ClAbstractVariable v, double upper)
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

  public final ClSimplexSolver addBounds(ClAbstractVariable v,
				   double lower, double upper)
  { addLowerBound(v,lower); addUpperBound(v,upper); return  this; }

  public final ClSimplexSolver addConstraint(ClConstraint cn)
       throws ExCLRequiredFailure, ExCLInternalError
  {
    if (fTraceOn) fnenterprint("addConstraint: " + cn);
    
    ClLinearExpression expr = newExpression(cn);
  
    if (!tryAddingDirectly(expr)) {
      // could not add directly
      addWithArtificialVariable(expr);
    }
    optimize(my_objective);
    setExternalVariables();
    if (cn.isEditConstraint()) {
      int i = my_prevEditConstants.size() - 1;
      ClEditConstraint cnEdit = (ClEditConstraint) cn;
      my_editVarMap.put(cnEdit.variable(), new ClConstraintAndIndex(cnEdit,i));
    }
    return  this;
  }

  public final boolean addConstraintNoException(ClConstraint cn)
       throws ExCLInternalError
  {
    if (fTraceOn) fnenterprint("addConstraintNoException: " + cn);

    try
      {
      addConstraint(cn);
      return true;
      }
    catch (ExCLRequiredFailure e)
      {
      return false;
      }
  }

  public final ClSimplexSolver addEditVar(ClVariable v, ClStrength strength)
    throws ExCLInternalError
  {
    try 
      {
        ClEditConstraint cnEdit = new ClEditConstraint(v, strength);
        return addConstraint(cnEdit);
      }
    catch (ExCLRequiredFailure e)
      {
        // should not get this
        throw new ExCLInternalError();
      }
  }

  public final ClSimplexSolver addEditVar(ClVariable v)
    throws ExCLInternalError
  { return addEditVar(v, ClStrength.strong); }

  public final ClSimplexSolver removeEditVar(ClVariable v)
    throws ExCLInternalError, ExCLConstraintNotFound
  {
    ClConstraintAndIndex cai = (ClConstraintAndIndex) my_editVarMap.get(v);
    ClConstraint cn = cai.Constraint();
    removeConstraint(cn);
    return this;
  }

  public final ClSimplexSolver beginEdit()
    throws ExCLInternalError
  {
    assert(my_editVarMap.size() > 0);
    // may later want to do more in here
    my_infeasibleRows.clear();
    resetStayConstants();
    return this;
  }

  public final ClSimplexSolver endEdit()
    throws ExCLInternalError
  {
    assert(my_editVarMap.size() > 0);
    removeAllEditVars();
    // may later want to do more in here
    return this;
  }

  public final ClSimplexSolver removeAllEditVars()
    throws ExCLInternalError
  {
    try
      {
        for (Enumeration e = my_editVarMap.keys(); e.hasMoreElements() ; ) {
          ClVariable v = (ClVariable) e.nextElement();
          removeEditVar(v);
        }
        
        my_editVarMap.clear();
        
        return this;
      }
    catch (ExCLConstraintNotFound e)
      {
        // should not get this 
        throw new ExCLInternalError();
      }
  }

  public final ClSimplexSolver addPointStays(Vector listOfPoints)
       throws ExCLRequiredFailure, ExCLInternalError
  {
    if (fTraceOn) fnenterprint("addPointStays" + listOfPoints);
    double weight = 1.0;
    final double multiplier = 2.0;
    for (int i = 0; i < listOfPoints.size(); i++) {
      addPointStay((ClPoint) listOfPoints.elementAt(i),weight);
      weight *= multiplier;
    }
    return this;
  }

  public final ClSimplexSolver addPointStay(ClVariable vx, 
				      ClVariable vy, 
				      double weight)
       throws ExCLRequiredFailure, ExCLInternalError
  { 
    addStay(vx,ClStrength.weak,weight);
    addStay(vy,ClStrength.weak,weight);
    return  this;
  }

  public final ClSimplexSolver addPointStay(ClVariable vx, 
				      ClVariable vy)
       throws ExCLRequiredFailure, ExCLInternalError
  { addPointStay(vx,vy,1.0); return this; }
  
  public final ClSimplexSolver addPointStay(ClPoint clp, double weight)
       throws ExCLRequiredFailure, ExCLInternalError
  { 
    addStay(clp.X(),ClStrength.weak,weight);
    addStay(clp.Y(),ClStrength.weak,weight);
    return this;
  }

  public final ClSimplexSolver addPointStay(ClPoint clp)
       throws ExCLRequiredFailure, ExCLInternalError
  {
    addPointStay(clp,1.0); 
    return this;
  }

  public final ClSimplexSolver addStay(ClVariable v, 
				 ClStrength strength,
				 double weight)
       throws ExCLRequiredFailure, ExCLInternalError
  { 
    ClStayConstraint cn = new ClStayConstraint(v,strength,weight);
    return addConstraint(cn); 
  }

  public final ClSimplexSolver addStay(ClVariable v, 
				 ClStrength strength)
       throws ExCLRequiredFailure, ExCLInternalError
  { 
    addStay(v,strength,1.0); return this;
  }

  public final ClSimplexSolver addStay(ClVariable v)
       throws ExCLRequiredFailure, ExCLInternalError
  { 
    addStay(v,ClStrength.weak,1.0); return this;
  }


  public final ClSimplexSolver removeConstraint(ClConstraint cn)
       throws ExCLConstraintNotFound, ExCLInternalError
  {
    if (fTraceOn) fnenterprint("removeConstraint: " + cn);
    if (fTraceOn) traceprint(this.toString());

    resetStayConstants();
    
    ClLinearExpression zRow = rowExpression(my_objective);
    
    Set eVars = (Set) my_errorVars.get(cn);
    if (fTraceOn) traceprint("eVars == " + eVars);

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

    if (fTraceOn) traceprint("Looking to remove var " + marker);

    if (rowExpression(marker) == null ) {
      // not in the basis, so need to do some work
      Set col = (Set) my_columns.get(marker);

      if (fTraceOn) traceprint("Must pivot -- columns are " + col);

      ClAbstractVariable exitVar = null;
      double minRatio = 0.0;
      for (Enumeration e = col.elements(); e.hasMoreElements() ; ) {
	final ClAbstractVariable v = (ClAbstractVariable) e.nextElement();
	if (v.isRestricted() ) {
	  final ClLinearExpression expr = rowExpression( v);
	  double coeff = expr.coefficientFor(marker);
	  if (fTraceOn) traceprint("Marker " + marker + "'s coefficient in " + expr + " is " + coeff);
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
	if (fTraceOn) traceprint("exitVar is still null");
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
        // FIXGJBNOW != or equals?
	if ( v != marker ) {
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
          // the plus error vars (ep*) are the markers, so they get removed
          // elsewhere, but we need to be sure to remove the minus error vars (em*)
          removeColumn( (ClAbstractVariable) my_editMinusErrorVars.elementAt(i));
	  my_editPlusErrorVars.removeElementAt(i);
	  my_editMinusErrorVars.removeElementAt(i);
	  my_prevEditConstants.removeElementAt(i);
	  break;
	}
      }
      ClEditConstraint cnEdit = (ClEditConstraint) cn;
      my_editVarMap.remove(cnEdit.variable());
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
  
  public final void reset()
       throws ExCLInternalError
  {
    if (fTraceOn) fnenterprint("reset");
    throw new ExCLInternalError();
  }
  
  public final void resolve(Vector newEditConstants)
       throws ExCLInternalError
  {
    if (fTraceOn) fnenterprint("resolve" + newEditConstants);
    my_infeasibleRows.clear();
    resetStayConstants();
    resetEditConstants(newEditConstants);
    dualOptimize();
    setExternalVariables();
  }

  public final void resolve(double x, double y)
       throws ExCLInternalError
  {
    ((ClDouble) my_resolve_pair.elementAt(0)).setValue(x);
    ((ClDouble) my_resolve_pair.elementAt(1)).setValue(y);
    resolve(my_resolve_pair);
  }

  public final void resolve()
    throws ExCLInternalError
  {
    if (fTraceOn) fnenterprint("resolve()");
    dualOptimize();
    setExternalVariables();
    my_infeasibleRows.clear();
    resetStayConstants();
  }

  public final ClSimplexSolver suggestValue(ClVariable v, double x)
    throws ExCLError
  {
    if (fTraceOn) fnenterprint("suggestValue(" + v + ", " + x + ")");
    ClConstraintAndIndex cai = (ClConstraintAndIndex) my_editVarMap.get(v);
    if (cai == null) {
      System.err.println("suggestValue for variable " + v + ", but var is not an edit variable\n");
      throw new ExCLError();
    }
    int i = cai.Index();
    double delta = x - ((ClDouble) my_prevEditConstants.elementAt(i)).doubleValue();
    my_prevEditConstants.setElementAt(new ClDouble(x),i);
    deltaEditConstant(delta,
                      (ClAbstractVariable) my_editPlusErrorVars.elementAt(i),
                      (ClAbstractVariable) my_editMinusErrorVars.elementAt(i));
    return this;
  }

  // from Michael Noth <noth@cs>
  public final String getInternalInfo() {
    StringBuffer retstr = new StringBuffer(super.getInternalInfo());
    retstr.append("\nSolver info:\n");
    retstr.append("Edit Error Variables: ");
    retstr.append(my_editPlusErrorVars.size() + my_editMinusErrorVars.size());
    retstr.append(" (" + my_editPlusErrorVars.size() + " +, ");
    retstr.append(my_editMinusErrorVars.size() + " -)\n");
    retstr.append("Stay Error Variables: ");
    retstr.append(my_stayPlusErrorVars.size() + my_stayMinusErrorVars.size());
    retstr.append(" (" + my_stayPlusErrorVars.size() + " +, ");
    retstr.append(my_stayMinusErrorVars.size() + " -)\n");
    retstr.append("Edit Constants: " + my_prevEditConstants.size());
    retstr.append("Edit Variables: " + my_editVarMap.size());
    retstr.append("\n");
    return retstr.toString();
  }

  public final String getDebugInfo() {
    StringBuffer bstr = new StringBuffer(toString());
    bstr.append(getInternalInfo());
    bstr.append("\n");
    return bstr.toString();
  }

  public final String toString()
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

  protected final void addWithArtificialVariable(ClLinearExpression expr)
       throws ExCLRequiredFailure, ExCLInternalError
  {
    if (fTraceOn) fnenterprint("addWithArtificialVariable: " + expr);
  
    ClSlackVariable av = new ClSlackVariable(++my_artificialCounter,"a");
    ClObjectiveVariable az = new ClObjectiveVariable("az");
    ClLinearExpression azRow = (ClLinearExpression) expr.clone();

    if (fTraceOn) traceprint("before addRows:\n" + this);

    addRow( az, azRow);
    addRow( av, expr);

    if (fTraceOn) traceprint("after addRows:\n" + this);
    optimize(az);
    
    ClLinearExpression azTableauRow = rowExpression(az);

    if (fTraceOn) traceprint("azTableauRow.constant() == " + azTableauRow.constant());
    
    if (!CL.approx(azTableauRow.constant(),0.0)) {
      removeRow(az);
      removeColumn(av);
      throw new ExCLRequiredFailure();
    }

    // See if av is a basic variable
    final ClLinearExpression e = rowExpression(av);

    if (e != null ) {
      // find another variable in this row and pivot,
      // so that av becomes parametric
      if (e.isConstant()) {
        // if there isn't another variable in the row
        // then the tableau contains the equation av=0 --
        // just delete av's row
	removeRow(av);
	removeRow(az);
	return;
      }
      ClAbstractVariable entryVar = e.anyVariable();
      pivot( entryVar, av);
    }
    assert(rowExpression(av) == null);
    removeColumn(av);
    removeRow(az);
  }

  protected final boolean tryAddingDirectly(ClLinearExpression expr)
       throws ExCLRequiredFailure
  {
    if (fTraceOn) fnenterprint("tryAddingDirectly: " + expr );
    final ClAbstractVariable subject = chooseSubject(expr);
    if (subject == null ) {
      if (fTraceOn) fnexitprint("returning false");
      return false;
    }
    expr.newSubject( subject);
    if (columnsHasKey( subject)) {
      substituteOut( subject,expr);
    }
    addRow( subject,expr);
    if (fTraceOn) fnexitprint("returning true");
    return true; // successfully added directly
  }

  protected final ClAbstractVariable chooseSubject(ClLinearExpression expr)
       throws ExCLRequiredFailure
  {
    if (fTraceOn) fnenterprint("chooseSubject: " + expr);
    ClAbstractVariable subject = null; // the current best subject, if any
    
    boolean foundUnrestricted = false; 
    boolean foundNewRestricted = false;
    
    final Hashtable terms = expr.terms();
    
    for (Enumeration e = terms.keys(); e.hasMoreElements() ; ) {
      final ClAbstractVariable v = (ClAbstractVariable) e.nextElement();
      final double c = ((ClDouble) terms.get(v)).doubleValue();
      
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
      final double c = ((ClDouble) terms.get(v)).doubleValue();
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
  
  protected final void deltaEditConstant(double delta, 
                                         ClAbstractVariable plusErrorVar, 
                                         ClAbstractVariable minusErrorVar)
  {
    if (fTraceOn) fnenterprint("deltaEditConstant :" + delta + ", " + plusErrorVar + ", " + minusErrorVar);
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

  protected final void dualOptimize()
       throws ExCLInternalError
  {
    if (fTraceOn) fnenterprint("dualOptimize:");
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
	    double c = ((ClDouble)terms.get(v)).doubleValue();
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

  protected final ClLinearExpression newExpression(ClConstraint cn)
  {
    if (fTraceOn) fnenterprint("newExpression: " + cn);
    if (fTraceOn) traceprint("cn.isInequality() == " + cn.isInequality());
    if (fTraceOn) traceprint("cn.isRequired() == " + cn.isRequired());

    final ClLinearExpression cnExpr = cn.expression();
    ClLinearExpression expr = new ClLinearExpression(cnExpr.constant());
    ClSlackVariable slackVar = new ClSlackVariable();
    ClDummyVariable dummyVar = new ClDummyVariable();
    ClSlackVariable eminus = new ClSlackVariable();
    ClSlackVariable eplus = new ClSlackVariable();
    final Hashtable cnTerms = cnExpr.terms();
    for (Enumeration en = cnTerms.keys(); en.hasMoreElements(); ) {
      final ClAbstractVariable v = (ClAbstractVariable) en.nextElement();
      double c = ((ClDouble) cnTerms.get(v)).doubleValue();
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
	if (fTraceOn) traceprint("Adding dummyVar == d" + my_dummyCounter);
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
	  if (fTraceOn) traceprint("sw == " + sw);
	  if (fTraceOn) traceprint("cn == " + cn);
	  if (fTraceOn) traceprint("adding " + eplus + " and " + eminus + " with swCoeff == " + swCoeff);
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
	  my_prevEditConstants.addElement(new ClDouble(cnExpr.constant()));
	}
      }
    }
    
    if (expr.constant() < 0)
      expr.multiplyMe(-1);

    if (fTraceOn) fnexitprint("returning " + expr);
    return expr;
  }

  protected final void optimize(ClObjectiveVariable zVar)
       throws ExCLInternalError
  {
    if (fTraceOn) fnenterprint("optimize: " + zVar);
    if (fTraceOn) traceprint(this.toString());

    ClLinearExpression zRow = rowExpression(zVar);
    assert(zRow != null);
    ClAbstractVariable entryVar = null;
    ClAbstractVariable exitVar = null;
    while (true) {
      double objectiveCoeff = 0;
      Hashtable terms = zRow.terms();
      for (Enumeration e = terms.keys(); e.hasMoreElements() ; ) {
	ClAbstractVariable v = (ClAbstractVariable) e.nextElement();
	double c = ((ClDouble) terms.get(v)).doubleValue();
	if (v.isPivotable() && c < objectiveCoeff) {
	  objectiveCoeff = c;
	  entryVar = v;
	}
      }
      if (objectiveCoeff >= -my_epsilon || entryVar == null)
	return;
      if (fTraceOn) traceprint("entryVar == " + entryVar + ", objectiveCoeff == " + objectiveCoeff);

      double minRatio = Double.MAX_VALUE;
      Set columnVars = (Set) my_columns.get(entryVar);
      double r = 0.0;
      for (Enumeration e = columnVars.elements(); e.hasMoreElements() ; ) {
	ClAbstractVariable v = (ClAbstractVariable) e.nextElement();
	if (fTraceOn) traceprint("Checking " + v);
	if (v.isPivotable()) {
	  final ClLinearExpression expr = rowExpression(v);
	  double coeff = expr.coefficientFor(entryVar);
	  if (fTraceOn) traceprint("pivotable, coeff = " + coeff);
	  if (coeff < 0.0) {
	    r = - expr.constant() / coeff;
	    if (r < minRatio) {
	      if (fTraceOn) traceprint("New minratio == " + r);
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
      if (fTraceOn) traceprint(this.toString());
    }
  }

  protected final void pivot(ClAbstractVariable entryVar, 
		       ClAbstractVariable exitVar)
       throws ExCLInternalError
  {
    if (fTraceOn) fnenterprint("pivot: " + entryVar + ", " + exitVar);

    ClLinearExpression  pexpr = removeRow(exitVar);

    pexpr.changeSubject(exitVar,entryVar);
    substituteOut(entryVar, pexpr);
    addRow(entryVar, pexpr);
  }
  
  protected final void resetEditConstants(Vector newEditConstants)
       throws ExCLInternalError
  {
    if (fTraceOn) fnenterprint("resetEditConstants:" + newEditConstants);

    if (newEditConstants.size() != my_editPlusErrorVars.size()) {
      // number of edit constants doesn't match the number of edit error variables
      System.err.println("newEditConstants == " + newEditConstants.toString());
      System.err.println("my_editPlusErrorVars == " + my_editPlusErrorVars.toString());
      System.err.println("sizes don't match");
      throw new ExCLInternalError();
    }

    for (int i = 0 ; i < newEditConstants.size(); i++) {
      double delta = (((ClDouble)newEditConstants.elementAt(i)).doubleValue() - 
		      ((ClDouble)my_prevEditConstants.elementAt(i)).doubleValue());
      my_prevEditConstants.setElementAt(((ClDouble)newEditConstants.elementAt(i)).clone(),i);
      deltaEditConstant(delta, 
			(ClAbstractVariable) my_editPlusErrorVars.elementAt(i),
			(ClAbstractVariable) my_editMinusErrorVars.elementAt(i));
    }
  }
  
  protected final void resetStayConstants()
  {
    if (fTraceOn) fnenterprint("resetStayConstants");

    for (int i = 0; i < my_stayPlusErrorVars.size(); i++) {
      ClLinearExpression expr = 
	rowExpression((ClAbstractVariable) my_stayPlusErrorVars.elementAt(i) );
      if (expr == null )
	expr = rowExpression((ClAbstractVariable) my_stayMinusErrorVars.elementAt(i));
      if (expr != null)
	expr.set_constant(0.0);
    }
  }

  protected final void setExternalVariables()
  {
    if (fTraceOn) fnenterprint("setExternalVariables:");
    if (fTraceOn) traceprint(this.toString());
    
    for (Enumeration e = my_externalParametricVars.elements(); 
	 e.hasMoreElements() ; ) {
      ClVariable v = (ClVariable) e.nextElement();
      v.set_value(0.0);
    }
    
    for (Enumeration e = my_externalRows.elements(); e.hasMoreElements(); ) {
      ClVariable v = (ClVariable) e.nextElement();
      ClLinearExpression expr = rowExpression(v);
      if (fTraceOn) debugprint("v == " + v);
      if (fTraceOn) debugprint("expr == " + expr);
      v.set_value(expr.constant());
    }
  }

  protected final void insertErrorVar(ClConstraint cn, ClAbstractVariable var)
  { 
    if (fTraceOn) fnenterprint("insertErrorVar:" + cn + ", " + var);

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

  // Map edit variables to their constraints and the index into
  // the parallel vectors for error vars and constants, above
  private Hashtable my_editVarMap; // map ClVariable to a ConstraintAndIndex

  private long my_slackCounter;
  private long my_artificialCounter;
  private long my_dummyCounter;

  private Vector my_resolve_pair;

  private double my_epsilon;
}

class ClConstraintAndIndex {
  public ClConstraintAndIndex(ClConstraint cn_, int i_)
  { cn = cn_; i = i_; }

  public int Index()
  { return i; }

  public ClConstraint Constraint()
  { return cn; }

  private ClConstraint cn;
  private int i;
}
