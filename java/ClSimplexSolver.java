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

package EDU.Washington.grad.gjb.cassowary;

import java.util.*;

public class ClSimplexSolver extends ClTableau
{
  // Ctr initializes the fields, and creates the objective row
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
    
    _fOptimizeAutomatically = true;
    _fNeedsSolving = false;
    
    ClLinearExpression e = new ClLinearExpression();
    _rows.put(_objective,e);
    if (fTraceOn) traceprint("objective expr == " + rowExpression(_objective));
  }

  // Convenience function for creating a linear inequality constraint
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

  // Convenience function for creating a linear inequality constraint
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

  // Convenience function for creating a pair of linear inequality constraint
  public final ClSimplexSolver addBounds(ClAbstractVariable v,
				   double lower, double upper)
  { addLowerBound(v,lower); addUpperBound(v,upper); return  this; }

  // Add constraint "cn" to the solver
  public final ClSimplexSolver addConstraint(ClConstraint cn)
       throws ExCLRequiredFailure, ExCLInternalError
  {
    if (fTraceOn) fnenterprint("addConstraint: " + cn);
    
    ClLinearExpression expr = newExpression(cn);
  
    boolean fAddedOkDirectly = false;

    try {
      fAddedOkDirectly = tryAddingDirectly(expr);
      if (!fAddedOkDirectly) {
        // could not add directly
        addWithArtificialVariable(expr);
      }
    } catch (ExCLRequiredFailure err) {
      ///try {
        ///        removeConstraint(cn); // FIXGJB
        //      } catch (ExCLConstraintNotFound errNF) {
        // This should not possibly happen
        /// System.err.println("ERROR: could not find a constraint just added\n");
        ///}
      throw err;
    }

    _fNeedsSolving = true;

    if (cn.isEditConstraint()) {
      int i = _prevEditConstants.size() - 1;
      ClEditConstraint cnEdit = (ClEditConstraint) cn;
      _editVarMap.put(cnEdit.variable(), new ClConstraintAndIndex(cnEdit,i));
    }

    if (_fOptimizeAutomatically) {
      optimize(_objective);
      setExternalVariables();
    }

    return  this;
  }

  // Same as addConstraint, except returns false if the constraint
  // resulted in an unsolvable system (instead of throwing an exception)
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

  // Add an edit constraint for "v" with given strength
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

  // default to strength = strong
  public final ClSimplexSolver addEditVar(ClVariable v)
    throws ExCLInternalError
  { return addEditVar(v, ClStrength.strong); }

  // Remove the edit constraint previously added for variable v
  public final ClSimplexSolver removeEditVar(ClVariable v)
    throws ExCLInternalError, ExCLConstraintNotFound
  {
    ClConstraintAndIndex cai = (ClConstraintAndIndex) _editVarMap.get(v);
    ClConstraint cn = cai.Constraint();
    removeConstraint(cn);
    return this;
  }

  // beginEdit() should be called before sending
  // resolve() messages, after adding the appropriate edit variables
  public final ClSimplexSolver beginEdit()
    throws ExCLInternalError
  {
    assert(_editVarMap.size() > 0);
    // may later want to do more in here
    _infeasibleRows.clear();
    resetStayConstants();
    return this;
  }

  // endEdit should be called after editing has finished
  // for now, it just removes all edit variables
  public final ClSimplexSolver endEdit()
    throws ExCLInternalError
  {
    assert(_editVarMap.size() > 0);
    removeAllEditVars();
    // may later want to do more in here
    return this;
  }

  // removeAllEditVars() just eliminates all the edit constraints
  // that were added
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

  // Add weak stays to the x and y parts of each point. These have
  // increasing weights so that the solver will try to satisfy the x
  // and y stays on the same point, rather than the x stay on one and
  // the y stay on another.
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

  // Add a stay of the given strength (default to weak) of v to the tableau
  public final ClSimplexSolver addStay(ClVariable v, 
				 ClStrength strength,
				 double weight)
       throws ExCLRequiredFailure, ExCLInternalError
  { 
    ClStayConstraint cn = new ClStayConstraint(v,strength,weight);
    return addConstraint(cn); 
  }

  // default to weight == 1.0
  public final ClSimplexSolver addStay(ClVariable v, 
				 ClStrength strength)
       throws ExCLRequiredFailure, ExCLInternalError
  { 
    addStay(v,strength,1.0); return this;
  }

  // default to strength = weak
  public final ClSimplexSolver addStay(ClVariable v)
       throws ExCLRequiredFailure, ExCLInternalError
  { 
    addStay(v,ClStrength.weak,1.0); return this;
  }


  // Remove the constraint cn from the tableau
  // Also remove any error variable associated with cn
  public final ClSimplexSolver removeConstraint(ClConstraint cn)
       throws ExCLConstraintNotFound, ExCLInternalError
  {
    if (fTraceOn) fnenterprint("removeConstraint: " + cn);
    if (fTraceOn) traceprint(this.toString());

    _fNeedsSolving = true;

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

    if (_fOptimizeAutomatically) {
      optimize(_objective);
      setExternalVariables();
    }

    return  this;
  }
  
  // Re-initialize this solver from the original constraints, thus
  // getting rid of any accumulated numerical problems.  (Actually, we
  // haven't definitely observed any such problems yet)
  public final void reset()
       throws ExCLInternalError
  {
    if (fTraceOn) fnenterprint("reset");
    throw new ExCLInternalError();
  }
  
  // Re-solve the current collection of constraints for new values for
  // the constants of the edit variables.
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

  // Convenience function for resolve-s of two variables
  public final void resolve(double x, double y)
       throws ExCLInternalError
  {
    ((ClDouble) _resolve_pair.elementAt(0)).setValue(x);
    ((ClDouble) _resolve_pair.elementAt(1)).setValue(y);
    resolve(_resolve_pair);
  }

  // Re-solve the cuurent collection of constraints, given the new
  // values for the edit variables that have already been
  // suggested (see suggestValue() method)
  public final void resolve()
    throws ExCLInternalError
  {
    if (fTraceOn) fnenterprint("resolve()");
    dualOptimize();
    setExternalVariables();
    _infeasibleRows.clear();
    resetStayConstants();
  }

  // Suggest a new value for an edit variable
  // the variable needs to be added as an edit variable
  // and beginEdit() needs to be called before this is called.
  // The tableau will not be solved completely until
  // after resolve() has been called
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

  // Control whether optimization and setting of external variables
  // is done automatically or not.  By default it is done
  // automatically and solve() never needs to be explicitly
  // called by client code; if setAutosolve is put to false,
  // then solve() needs to be invoked explicitly before using
  // variables' values
  // (Turning off autosolve while adding lots and lots of
  // constraints [ala the addDel test in ClTests] saved
  // about 20% in runtime, from 68sec to 54sec for 900 constraints,
  // with 126 failed adds)
  public final ClSimplexSolver setAutosolve(boolean f)
  { _fOptimizeAutomatically = f; return this; }

  // Tell whether we are autosolving
  public final boolean FIsAutosolving()
  { return _fOptimizeAutomatically; }

  // If autosolving has been turned off, client code needs
  // to explicitly call solve() before accessing variables
  // values
  public final ClSimplexSolver solve()
    throws ExCLInternalError
  {
    if (_fNeedsSolving) {
      optimize(_objective);
      setExternalVariables();
    }
    return this;
  }
        
  // Originally from Michael Noth <noth@cs>
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

  //// END PUBLIC INTERFACE
  
  // Add the constraint expr=0 to the inequality tableau using an
  // artificial variable.  To do this, create an artificial variable
  // av and add av=expr to the inequality tableau, then make av be 0.
  // (Raise an exception if we can't attain av=0.)
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
      ClAbstractVariable entryVar = e.anyPivotableVariable();
      pivot( entryVar, av);
    }
    assert(rowExpression(av) == null);
    removeColumn(av);
    removeRow(az);
  }

  // We are trying to add the constraint expr=0 to the appropriate
  // tableau.  Try to add expr directly to the tableax without
  // creating an artificial variable.  Return true if successful and
  // false if not.
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

  // We are trying to add the constraint expr=0 to the tableaux.  Try
  // to choose a subject (a variable to become basic) from among the
  // current variables in expr.  If expr contains any unrestricted
  // variables, then we must choose an unrestricted variable as the
  // subject.  Also, if the subject is new to the solver we won't have
  // to do any substitutions, so we prefer new variables to ones that
  // are currently noted as parametric.  If expr contains only
  // restricted variables, if there is a restricted variable with a
  // negative coefficient that is new to the solver we can make that
  // the subject.  Otherwise we can't find a subject, so return nil.
  // (In this last case we have to add an artificial variable and use
  // that variable as the subject -- this is done outside this method
  // though.)
  // 
  // Note: in checking for variables that are new to the solver, we
  // ignore whether a variable occurs in the objective function, since
  // new slack variables are added to the objective function by
  // 'newExpression:', which is called before this method.
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

  // We have set new values for the constants in the edit constraints.
  // Re-optimize using the dual simplex algorithm.
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

  // Make a new linear expression representing the constraint cn,
  // replacing any basic variables with their defining expressions.
  // Normalize if necessary so that the constant is non-negative.  If
  // the constraint is non-required give its error variables an
  // appropriate weight in the objective function.
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

  // Minimize the value of the objective.  (The tableau should already
  // be feasible.)
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

  // Do a pivot.  Move entryVar into the basis (i.e. make it a basic variable),
  // and move exitVar out of the basis (i.e., make it a parametric variable)
  protected final void pivot(ClAbstractVariable entryVar, 
                             ClAbstractVariable exitVar)
       throws ExCLInternalError
  {
    if (fTraceOn) fnenterprint("pivot: " + entryVar + ", " + exitVar);
    assert(entryVar.isPivotable());

    ClLinearExpression  pexpr = removeRow(exitVar);

    pexpr.changeSubject(exitVar,entryVar);
    substituteOut(entryVar, pexpr);
    addRow(entryVar, pexpr);
  }
  
  // Each of the non-required edits will be represented by an equation
  // of the form
  //    v = c + eplus - eminus 
  // where v is the variable with the edit, c is the previous edit
  // value, and eplus and eminus are slack variables that hold the
  // error in satisfying the edit constraint.  We are about to change
  // something, and we want to fix the constants in the equations
  // representing the edit constraints.  If one of eplus and eminus is
  // basic, the other must occur only in the expression for that basic
  // error variable.  (They can't both be basic.)  Fix the constant in
  // this expression.  Otherwise they are both nonbasic.  Find all of
  // the expressions in which they occur, and fix the constants in
  // those.  See the UIST paper for details.  void
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
  
  // Each of the non-required stays will be represented by an equation
  // of the form
  //     v = c + eplus - eminus
  // where v is the variable with the stay, c is the previous value of
  // v, and eplus and eminus are slack variables that hold the error
  // in satisfying the stay constraint.  We are about to change
  // something, and we want to fix the constants in the equations
  // representing the stays.  If both eplus and eminus are nonbasic
  // they have value 0 in the current solution, meaning the previous
  // stay was exactly satisfied.  In this case nothing needs to be
  // changed.  Otherwise one of them is basic, and the other must
  // occur only in the expression for that basic error variable.
  // Reset the constant in this expression to 0.
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

  // Set the external variables known to this solver to their appropriate values.
  // Set each external basic variable to its value, and set each
  // external parametric variable to 0.  (It isn't clear that we will
  // ever have external parametric variables -- every external
  // variable should either have a stay on it, or have an equation
  // that defines it in terms of other external variables that do have
  // stays.  For the moment I'll put this in though.)  Variables that
  // are internal to the solver don't actually store values -- their
  // values are just implicit in the tableu -- so we don't need to set
  // them.
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

    _fNeedsSolving = false;
  }

  // Protected convenience function to insert an error variable into
  // the _errorVars set, creating the mapping with put as necessary
  protected final void insertErrorVar(ClConstraint cn, ClAbstractVariable var)
  { 
    if (fTraceOn) fnenterprint("insertErrorVar:" + cn + ", " + var);

    Set cnset = (Set) _errorVars.get(var);
    if (cnset == null)
      _errorVars.put(cn,cnset = new Set());
    cnset.insert(var);
  }


  //// BEGIN PRIVATE INSTANCE FIELDS

  // the arrays of positive and negative error vars for the edit constraints
  // (need both positive and negative since they have only non-negative values)
  private Vector _editMinusErrorVars;
  private Vector _editPlusErrorVars;


  // the arrays of positive and negative error vars for the stay constraints
  // (need both positive and negative since they have only non-negative values)
  private Vector _stayMinusErrorVars;
  private Vector _stayPlusErrorVars;


  // The array of constants for the edit constraints on the previous
  // iteration.  These must be in the same order as editPlusErrorVars
  // and editMinusErrorVars
  private Vector _prevEditConstants;

  // give error variables for a non required constraint,
  // maps to ClSlackVariable-s
  private Hashtable _errorVars; // map ClConstraint to Set (of ClVariable)


  // Return a lookup table giving the marker variable for each
  // constraint (used when deleting a constraint).
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

  boolean _fOptimizeAutomatically;
  boolean _fNeedsSolving;
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
