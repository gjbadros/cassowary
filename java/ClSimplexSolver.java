// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This Java Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998 Alan Borning and Greg Badros
// See ../COPYRIGHT for legal details regarding this software
//
// ClSimplexSolver
// 

import java.util.*;

class ClSimplexSolver extends ClTableau
{
  public ClSimplexSolver()
  {
    _editMinusErrorVars = new Vector();
    _editPlusErrorVars = new Vector();
    _stayMinusErrorVars = new Vector();
    _stayPlusErrorVars = new Vector();
    _prevEditConstants = new Vector();
    _errorVars = new Hashtable();
    _markerVars = new Hashtable();

    _resolve_pair = new Vector(2);
    _resolve_pair.addElement(new ClDouble(0));
    _resolve_pair.addElement(new ClDouble(0));

    _objective = new ClObjectiveVariable("Z");

    _editVarMap = new Hashtable();

    _slackCounter = 0;
    _artificialCounter = 0;
    _dummyCounter = 0;
    _epsilon = 1e-8;
    ClLinearExpression e = new ClLinearExpression();
    _rows.put(_objective,e);
    if (fTraceOn) traceprint("objective expr == " + rowExpression(_objective));
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
    optimize(_objective);
    setExternalVariables();
    if (cn.isEditConstraint()) {
      int i = _prevEditConstants.size() - 1;
      ClEditConstraint cnEdit = (ClEditConstraint) cn;
      _editVarMap.put(cnEdit.variable(), new ClConstraintAndIndex(cnEdit,i));
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
    ClConstraintAndIndex cai = (ClConstraintAndIndex) _editVarMap.get(v);
    ClConstraint cn = cai.Constraint();
    removeConstraint(cn);
    return this;
  }

  public final ClSimplexSolver beginEdit()
    throws ExCLInternalError
  {
    assert(_editVarMap.size() > 0);
    // may later want to do more in here
    _infeasibleRows.clear();
    resetStayConstants();
    return this;
  }

  public final ClSimplexSolver endEdit()
    throws ExCLInternalError
  {
    assert(_editVarMap.size() > 0);
    removeAllEditVars();
    // may later want to do more in here
    return this;
  }

  public final ClSimplexSolver removeAllEditVars()
    throws ExCLInternalError
  {
    try
      {
        for (Enumeration e = _editVarMap.keys(); e.hasMoreElements() ; ) {
          ClVariable v = (ClVariable) e.nextElement();
          removeEditVar(v);
        }
        
        _editVarMap.clear();
        
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
    
    ClLinearExpression zRow = rowExpression(_objective);
    
    Set eVars = (Set) _errorVars.get(cn);
    if (fTraceOn) traceprint("eVars == " + eVars);

    if (eVars != null) {
      for (Enumeration e = eVars.elements(); e.hasMoreElements() ; ) {
	ClAbstractVariable clv = (ClAbstractVariable) e.nextElement();
	final ClLinearExpression expr = rowExpression(clv);
	if (expr == null ) {
	  zRow.addVariable(clv, -1.0 * 
			    cn.strength().symbolicWeight().asDouble(),
			    _objective, this);
	} else { // the error variable was in the basis
	  zRow.addExpression(expr, -1.0 *
			     cn.strength().symbolicWeight().asDouble(),
			     _objective, this);
	}
      }
    }
    
    ClAbstractVariable marker = (ClAbstractVariable) _markerVars.remove(cn);
    if (marker == null) {
      throw new ExCLConstraintNotFound();
    }

    if (fTraceOn) traceprint("Looking to remove var " + marker);

    if (rowExpression(marker) == null ) {
      // not in the basis, so need to do some work
      Set col = (Set) _columns.get(marker);

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
	for (int i = 0; i < _stayPlusErrorVars.size(); i++) {
	  eVars.remove(_stayPlusErrorVars.elementAt(i));
	  eVars.remove(_stayMinusErrorVars.elementAt(i));
	}
      }
    } else if (cn.isEditConstraint()) {
      assert(eVars != null);
      for (Enumeration e = eVars.elements(); e.hasMoreElements(); ) {
	final ClAbstractVariable var = (ClAbstractVariable) e.nextElement();
	int i = 0;
	for ( ; i < _editPlusErrorVars.size(); i++) {
	  if ( _editPlusErrorVars.elementAt(i) == var) break;
	}
	if (i != _editPlusErrorVars.size()) {
	  // found it
          // the plus error vars (ep*) are the markers, so they get removed
          // elsewhere, but we need to be sure to remove the minus error vars (em*)
          removeColumn( (ClAbstractVariable) _editMinusErrorVars.elementAt(i));
	  _editPlusErrorVars.removeElementAt(i);
	  _editMinusErrorVars.removeElementAt(i);
	  _prevEditConstants.removeElementAt(i);
	  break;
	}
      }
      ClEditConstraint cnEdit = (ClEditConstraint) cn;
      _editVarMap.remove(cnEdit.variable());
    }

    // FIXGJB do the remove at top
    if (eVars != null) {
      _errorVars.remove(eVars);
    }
    marker = null;

    optimize(_objective);
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
    _infeasibleRows.clear();
    resetStayConstants();
    resetEditConstants(newEditConstants);
    dualOptimize();
    setExternalVariables();
  }

  public final void resolve(double x, double y)
       throws ExCLInternalError
  {
    ((ClDouble) _resolve_pair.elementAt(0)).setValue(x);
    ((ClDouble) _resolve_pair.elementAt(1)).setValue(y);
    resolve(_resolve_pair);
  }

  public final void resolve()
    throws ExCLInternalError
  {
    if (fTraceOn) fnenterprint("resolve()");
    dualOptimize();
    setExternalVariables();
    _infeasibleRows.clear();
    resetStayConstants();
  }

  public final ClSimplexSolver suggestValue(ClVariable v, double x)
    throws ExCLError
  {
    if (fTraceOn) fnenterprint("suggestValue(" + v + ", " + x + ")");
    ClConstraintAndIndex cai = (ClConstraintAndIndex) _editVarMap.get(v);
    if (cai == null) {
      System.err.println("suggestValue for variable " + v + ", but var is not an edit variable\n");
      throw new ExCLError();
    }
    int i = cai.Index();
    double delta = x - ((ClDouble) _prevEditConstants.elementAt(i)).doubleValue();
    _prevEditConstants.setElementAt(new ClDouble(x),i);
    deltaEditConstant(delta,
                      (ClAbstractVariable) _editPlusErrorVars.elementAt(i),
                      (ClAbstractVariable) _editMinusErrorVars.elementAt(i));
    return this;
  }

  // from Michael Noth <noth@cs>
  public final String getInternalInfo() {
    StringBuffer retstr = new StringBuffer(super.getInternalInfo());
    retstr.append("\nSolver info:\n");
    retstr.append("Edit Error Variables: ");
    retstr.append(_editPlusErrorVars.size() + _editMinusErrorVars.size());
    retstr.append(" (" + _editPlusErrorVars.size() + " +, ");
    retstr.append(_editMinusErrorVars.size() + " -)\n");
    retstr.append("Stay Error Variables: ");
    retstr.append(_stayPlusErrorVars.size() + _stayMinusErrorVars.size());
    retstr.append(" (" + _stayPlusErrorVars.size() + " +, ");
    retstr.append(_stayMinusErrorVars.size() + " -)\n");
    retstr.append("Edit Constants: " + _prevEditConstants.size());
    retstr.append("Edit Variables: " + _editVarMap.size());
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
    bstr.append("_editPlusErrorVars: ");
    bstr.append(_editPlusErrorVars);
    bstr.append("\n_editMinusErrorVars: ");
    bstr.append(_editMinusErrorVars);

    bstr.append("\n_stayPlusErrorVars: ");
    bstr.append(_stayPlusErrorVars);
    bstr.append("\n_stayMinusErrorVars: ");
    bstr.append(_stayMinusErrorVars);

    bstr.append("\n_prevEditConstants: ");
    bstr.append(_prevEditConstants);

    bstr.append("\n");
    return bstr.toString();
  }

  protected final void addWithArtificialVariable(ClLinearExpression expr)
       throws ExCLRequiredFailure, ExCLInternalError
  {
    if (fTraceOn) fnenterprint("addWithArtificialVariable: " + expr);
  
    ClSlackVariable av = new ClSlackVariable(++_artificialCounter,"a");
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
	    final Set col = (Set) _columns.get(v);
	    if (col == null || 
		( col.size() == 1 && columnsHasKey(_objective) ) ) {
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
	_infeasibleRows.insert(plusErrorVar);
      }
      return;
    }
    
    ClLinearExpression exprMinus = rowExpression(minusErrorVar);
    if (exprMinus != null) {
      exprMinus.incrementConstant(-delta);
      if (exprMinus.constant() < 0.0) {
	_infeasibleRows.insert(minusErrorVar);
      }
      return;
    }

    Set columnVars = (Set) _columns.get(minusErrorVar);
    
    for (Enumeration e = columnVars.elements(); e.hasMoreElements() ; ) {
      final ClAbstractVariable basicVar = (ClAbstractVariable) e.nextElement();
      ClLinearExpression expr = rowExpression(basicVar);
      //assert(expr != null );
      final double c = expr.coefficientFor(minusErrorVar);
      expr.incrementConstant(c * delta);
      if (basicVar.isRestricted() && expr.constant() < 0.0) {
	_infeasibleRows.insert(basicVar);
      }
    }
  }

  protected final void dualOptimize()
       throws ExCLInternalError
  {
    if (fTraceOn) fnenterprint("dualOptimize:");
    final ClLinearExpression zRow = rowExpression(_objective);
    while (!_infeasibleRows.isEmpty()) {
      ClAbstractVariable exitVar = 
	(ClAbstractVariable) _infeasibleRows.elements().nextElement();
      _infeasibleRows.remove(exitVar);
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
      ++_slackCounter;
      slackVar = new ClSlackVariable (_slackCounter, "s");
      expr.setVariable(slackVar,-1);
      _markerVars.put(cn,slackVar);
      if (!cn.isRequired()) {
	++_slackCounter;
	eminus = new ClSlackVariable(_slackCounter, "em");
	expr.setVariable(eminus,1.0);
	ClLinearExpression zRow = rowExpression(_objective);
	ClSymbolicWeight sw = cn.strength().symbolicWeight().times(cn.weight());
	zRow.setVariable( eminus,sw.asDouble());
	insertErrorVar(cn,eminus);
	noteAddedVariable(eminus,_objective);
      }
    }
    else {
      // cn is an equality
      if (cn.isRequired()) {
	++_dummyCounter;
	dummyVar = new ClDummyVariable(_dummyCounter, "d");
	expr.setVariable(dummyVar,1.0);
	_markerVars.put(cn,dummyVar);
	if (fTraceOn) traceprint("Adding dummyVar == d" + _dummyCounter);
      } else {
	++_slackCounter;
	eplus = new ClSlackVariable (_slackCounter, "ep");
	eminus = new ClSlackVariable (_slackCounter, "em");

	expr.setVariable( eplus,-1.0);
	expr.setVariable( eminus,1.0);
	_markerVars.put(cn,eplus);
	ClLinearExpression zRow = rowExpression(_objective);
	ClSymbolicWeight sw = cn.strength().symbolicWeight().times(cn.weight());
	double swCoeff = sw.asDouble();
	if (swCoeff == 0) {
	  if (fTraceOn) traceprint("sw == " + sw);
	  if (fTraceOn) traceprint("cn == " + cn);
	  if (fTraceOn) traceprint("adding " + eplus + " and " + eminus + " with swCoeff == " + swCoeff);
	}
	zRow.setVariable(eplus,swCoeff);
	noteAddedVariable(eplus,_objective);
	zRow.setVariable(eminus,swCoeff);
	noteAddedVariable(eminus,_objective);
	insertErrorVar(cn,eminus);
	insertErrorVar(cn,eplus);
	if (cn.isStayConstraint()) {
	  _stayPlusErrorVars.addElement(eplus);
	  _stayMinusErrorVars.addElement(eminus);
	}
	else if (cn.isEditConstraint()) {
	  _editPlusErrorVars.addElement(eplus);
	  _editMinusErrorVars.addElement(eminus);
	  _prevEditConstants.addElement(new ClDouble(cnExpr.constant()));
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
      if (objectiveCoeff >= -_epsilon || entryVar == null)
	return;
      if (fTraceOn) traceprint("entryVar == " + entryVar + ", objectiveCoeff == " + objectiveCoeff);

      double minRatio = Double.MAX_VALUE;
      Set columnVars = (Set) _columns.get(entryVar);
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

    if (newEditConstants.size() != _editPlusErrorVars.size()) {
      // number of edit constants doesn't match the number of edit error variables
      System.err.println("newEditConstants == " + newEditConstants.toString());
      System.err.println("_editPlusErrorVars == " + _editPlusErrorVars.toString());
      System.err.println("sizes don't match");
      throw new ExCLInternalError();
    }

    for (int i = 0 ; i < newEditConstants.size(); i++) {
      double delta = (((ClDouble)newEditConstants.elementAt(i)).doubleValue() - 
		      ((ClDouble)_prevEditConstants.elementAt(i)).doubleValue());
      _prevEditConstants.setElementAt(((ClDouble)newEditConstants.elementAt(i)).clone(),i);
      deltaEditConstant(delta, 
			(ClAbstractVariable) _editPlusErrorVars.elementAt(i),
			(ClAbstractVariable) _editMinusErrorVars.elementAt(i));
    }
  }
  
  protected final void resetStayConstants()
  {
    if (fTraceOn) fnenterprint("resetStayConstants");

    for (int i = 0; i < _stayPlusErrorVars.size(); i++) {
      ClLinearExpression expr = 
	rowExpression((ClAbstractVariable) _stayPlusErrorVars.elementAt(i) );
      if (expr == null )
	expr = rowExpression((ClAbstractVariable) _stayMinusErrorVars.elementAt(i));
      if (expr != null)
	expr.set_constant(0.0);
    }
  }

  protected final void setExternalVariables()
  {
    if (fTraceOn) fnenterprint("setExternalVariables:");
    if (fTraceOn) traceprint(this.toString());
    
    for (Enumeration e = _externalParametricVars.elements(); 
	 e.hasMoreElements() ; ) {
      ClVariable v = (ClVariable) e.nextElement();
      v.set_value(0.0);
    }
    
    for (Enumeration e = _externalRows.elements(); e.hasMoreElements(); ) {
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

    Set cnset = (Set) _errorVars.get(var);
    if (cnset == null)
      _errorVars.put(cn,cnset = new Set());
    cnset.insert(var);
  }


  private Vector _editMinusErrorVars;
  private Vector _editPlusErrorVars;
  private Vector _stayMinusErrorVars;
  private Vector _stayPlusErrorVars;
  private Vector _prevEditConstants;
  private Hashtable _errorVars; // map ClConstraint to Set (of ClVariable)
  private Hashtable _markerVars; // map ClConstraint to ClVariable

  private ClObjectiveVariable _objective;

  // Map edit variables to their constraints and the index into
  // the parallel vectors for error vars and constants, above
  private Hashtable _editVarMap; // map ClVariable to a ConstraintAndIndex

  private long _slackCounter;
  private long _artificialCounter;
  private long _dummyCounter;

  private Vector _resolve_pair;

  private double _epsilon;
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
