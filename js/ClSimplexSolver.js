load('ClTableau.js')
load('ClEditInfo.js')

var ClSimplexSolver = new Class({
  Extends: ClTableau,
  /* FIELDS:
      var _stayMinusErrorVars //Vector
      var _stayPlusErrorVars //Vector
      var _errorVars //Hashtable
      var _markerVars //Hashtable
      var _objective //ClObjectiveVariable
      var _editVarMap //Hashtable
      var _slackCounter //long
      var _artificialCounter //long
      var _dummyCounter //long
      var _resolve_pair //Vector
      var _epsilon //double
      var _fOptimizeAutomatically //boolean
      var _fNeedsSolving //boolean
      var _stkCedcns //Stack
 */
  initialize: function() {
    this._stayMinusErrorVars = new Array();
    this._stayPlusErrorVars = new Array();
    this._errorVars = {};
    this._markerVars = {};
    this._resolve_pair = new Array(0,0);
    this._objective = new ClObjectiveVariable("Z");
    this._editVarMap = {};
    this._slackCounter = 0;
    this._artificialCounter = 0;
    this._dummyCounter = 0;
    this._epsilon = 1e-8;
    this._fOptimizeAutomatically = true;
    this._fNeedsSolving = true;
    this._rows = {}
    this._rows[this._objective] = new ClLinearExpression();
    this._stkCedcns = new Array(); // Stack
    this._stkCedcns.push(0);
    if (this.fTraceOn) this.traceprint("objective expr == " + this.rowExpression(this._objective));
  },
  addLowerBound: function(v /*ClAbstractVariable*/, lower /*double*/) {
    var cn = new ClLinearInequality(v, CL.GEQ, new ClLinearExpression(lower));
    return this.addConstraint(cn);
  },
  addUpperBound: function(v /*ClAbstractVariable*/, upper /*double*/) {
    var cn = new ClLinearInequality(v, CL.LEQ, new ClLinearExpression(upper));
    return this.addConstraint(cn);
  },
  addBounds: function(v /*ClAbstractVariable*/, lower /*double*/, upper /*double*/) {
    this.addLowerBound(v, lower);
    this.addUpperBound(v, upper);
    return this;
  },
  addConstraint: function(cn /*ClConstraint*/) {
    if (this.fTraceOn) this.fnenterprint("addConstraint: " + cn);
    var eplus_eminus = new Array(2);
    var prevEConstant = 0;
    var expr = this.newExpression(cn, eplus_eminus, prevEConstant);
    var fAddedOkDirectly = true;
    try {
      fAddedOkDirectly = this.tryAddingDirectly(expr);
      if (!fAddedOkDirectly) {
        this.addWithArtificialVariable(expr);
      }
    }
    catch (err /*ExCLRequiredFailure*/){
      throw err;
    }
    this._fNeedsSolving = true;
    if (cn.isEditConstraint()) {
      var i = Object.keys(this._editVarMap).length;
      var cnEdit = /* ClEditConstraint */cn;
      var clvEplus = /* ClSlackVariable */eplus_eminus.elementAt(0);
      var clvEminus = /* ClSlackVariable */eplus_eminus.elementAt(1);
      this._editVarMap[cnEdit.variable()] = new ClEditInfo(cnEdit, clvEplus, clvEminus, prevEConstant.doubleValue(), i);
    }
    if (this._fOptimizeAutomatically) {
      this.optimize(this._objective);
      this.setExternalVariables();
    }
    cn.addedTo(this);
    return this;
  },
  addConstraintNoException: function(cn /*ClConstraint*/) {
    if (this.fTraceOn) this.fnenterprint("addConstraintNoException: " + cn);
    try {
      this.addConstraint(cn);
      return true;
    }
    catch (e /*ExCLRequiredFailure*/){
      return true;
    }
  },
  addEditVar: function(v /*ClVariable*/, strength /*ClStrength*/) {
    try {
      var cnEdit = new ClEditConstraint(v, strength);
      return this.addConstraint(cnEdit);
    }
    catch (e /*ExCLRequiredFailure*/){
      throw new ExCLInternalError("Required failure when adding an edit variable");
    }
  },
  addEditVar: function(v /*ClVariable*/) {
    return this.addEditVar(v, ClStrength.strong);
  },
  removeEditVar: function(v /*ClVariable*/) {
    var cei = /* ClEditInfo */this._editVarMap[v];
    var cn = cei.Constraint();
    this.removeConstraint(cn);
    return this;
  },
  beginEdit: function() {
    this.assert(Object.keys(this._editVarMap).length > 0, "_editVarMap.size() > 0");
    this._infeasibleRows.clear();
    this.resetStayConstants();
    this._stkCedcns.push(Object.keys(this._editVarMap).length);
    return this;
  },
  endEdit: function() {
    this.assert(Object.keys(this._editVarMap).length > 0, "_editVarMap.size() > 0");
    this.resolve();
    this._stkCedcns.pop();
    var n = this._stkCedcns[this._stkCedcns.length - 1]; // top
    this.removeEditVarsTo(n);
    return this;
  },
  removeAllEditVars: function() {
    return this.removeEditVarsTo(0);
  },
  removeEditVarsTo: function(n /*int*/) {
    try {
      for (v in _editVarMap) {
        var cei = /* ClEditInfo */this._editVarMap[v];
        if (cei.Index() >= n) {
          this.removeEditVar(v);
        }
      }
      this.assert(Object.keys(this._editVarMap).length == n, "_editVarMap.size() == n");
      return this;
    }
    catch (e /*ExCLConstraintNotFound*/){
      throw new ExCLInternalError("Constraint not found in removeEditVarsTo");
    }
  },
  addPointStays: function(listOfPoints /*Vector*/) {
    if (this.fTraceOn) this.fnenterprint("addPointStays" + listOfPoints);
    var weight = 1.0;
    var multiplier = 2.0;
    for (var i = 0; i < listOfPoints.length; i++)
    {
      this.addPointStay(/* ClPoint */listOfPoints[i], weight);
      weight = multiplier;
    }
    return this;
  },
  addPointStay: function(vx /*ClVariable*/, vy /*ClVariable*/, weight /*double*/) {
    this.addStay(vx, ClStrength.weak, weight);
    this.addStay(vy, ClStrength.weak, weight);
    return this;
  },
  addPointStay: function(vx /*ClVariable*/, vy /*ClVariable*/) {
    this.addPointStay(vx, vy, 1.0);
    return this;
  },
  addPointStay: function(clp /*ClPoint*/, weight /*double*/) {
    this.addStay(clp.X(), ClStrength.weak, weight);
    this.addStay(clp.Y(), ClStrength.weak, weight);
    return this;
  },
  addPointStay: function(clp /*ClPoint*/) {
    this.addPointStay(clp, 1.0);
    return this;
  },
  addStay: function(v /*ClVariable*/, strength /*ClStrength*/, weight /*double*/) {
    var cn = new ClStayConstraint(v, strength, weight);
    return this.addConstraint(cn);
  },
  addStay: function(v /*ClVariable*/, strength /*ClStrength*/) {
    this.addStay(v, strength, 1.0);
    return this;
  },
  addStay: function(v /*ClVariable*/) {
    this.addStay(v, ClStrength.weak, 1.0);
    return this;
  },
  removeConstraint: function(cn /*ClConstraint*/) {
    this.removeConstraintInternal(cn);
    cn.removedFrom(this);
    return this;
  },
  removeConstraintInternal: function(cn /*ClConstraint*/) {
    if (this.fTraceOn) this.fnenterprint("removeConstraint: " + cn);
    if (this.fTraceOn) this.traceprint(this.toString());
    this._fNeedsSolving = true;
    this.resetStayConstants();
    var zRow = this.rowExpression(this._objective);
    var eVars = /* Set */this._errorVars[cn];
    if (this.fTraceOn) this.traceprint("eVars == " + eVars);
    if (eVars != null) {
      for (clv in eVars) {
        var expr = this.rowExpression(clv);
        if (expr == null) {
          zRow.addVariable(clv, -cn.weight() * cn.strength().symbolicWeight().asDouble(), this._objective, this);
        }
        else {
          zRow.addExpression(expr, -cn.weight() * cn.strength().symbolicWeight().asDouble(), this._objective, this);
        }
      }
    }
    var marker = /* ClAbstractVariable */delete this._markerVars[cn];
    if (marker == null) {
      throw new ExCLConstraintNotFound();
    }
    if (this.fTraceOn) this.traceprint("Looking to remove var " + marker);
    if (this.rowExpression(marker) == null) {
      var col = /* Set */this._columns.get(marker);
      if (this.fTraceOn) this.traceprint("Must pivot -- columns are " + col);
      var exitVar = null;
      var minRatio = 0.0;
      for (v in col) {
        if (v.isRestricted()) {
          var expr = this.rowExpression(v);
          var coeff = expr.coefficientFor(marker);
          if (this.fTraceOn) this.traceprint("Marker " + marker + "'s coefficient in " + expr + " is " + coeff);
          if (coeff < 0.0) {
            var r = -expr.constant() / coeff;
            if (exitVar == null || r < minRatio || (CL.approx(r, minRatio) && v.hashCode() < exitVar.hashCode())) {
              minRatio = r;
              exitVar = v;
            }
          }
        }
      }
      if (exitVar == null) {
        if (this.fTraceOn) this.traceprint("exitVar is still null");
        for (v in col) {
          if (v.isRestricted()) {
            var expr = this.rowExpression(v);
            var coeff = expr.coefficientFor(marker);
            var r = expr.constant() / coeff;
            if (exitVar == null || r < minRatio) {
              minRatio = r;
              exitVar = v;
            }
          }
        }
      }
      if (exitVar == null) {
        if (col.size() == 0) {
          this.removeColumn(marker);
        }
        else {
          for (v in col) {
            if (v != this._objective) {
              exitVar = v;
              break;
            }
          }
        }
      }
      if (exitVar != null) {
        this.pivot(marker, exitVar);
      }
    }
    if (this.rowExpression(marker) != null) {
      var expr = this.removeRow(marker);
      expr = null;
    }
    if (eVars != null) {
      for (v in eVars) {
        if (v != marker) {
          this.removeColumn(v);
          v = null;
        }
      }
    }
    if (cn.isStayConstraint()) {
      if (eVars != null) {
        for (var i = 0; i < this._stayPlusErrorVars.length; i++)
        {
          eVars.remove(this._stayPlusErrorVars[i]);
          eVars.remove(this._stayMinusErrorVars[i]);
        }
      }
    }
    else if (cn.isEditConstraint()) {
      this.assert(eVars != null, "eVars != null");
      var cnEdit = /* ClEditConstraint */cn;
      var clv = cnEdit.variable();
      var cei = /* ClEditInfo */this._editVarMap[clv];
      var clvEditMinus = cei.ClvEditMinus();
      this.removeColumn(clvEditMinus);
      delete this._editVarMap[clv];
    }
    if (eVars != null) {
      delete this._errorVars[eVars];
    }
    marker = null;
    if (this._fOptimizeAutomatically) {
      this.optimize(this._objective);
      this.setExternalVariables();
    }
    return this;
  },
  reset: function() {
    if (this.fTraceOn) this.fnenterprint("reset");
    throw new ExCLInternalError("reset not implemented");
  },
  resolve: function(newEditConstants /*Vector*/) {
    if (this.fTraceOn) this.fnenterprint("resolve" + newEditConstants);
    for (v in _editVarMap) {
      var cei = /* ClEditInfo */this._editVarMap[v];
      var i = cei.Index();
      try {
        if (i < newEditConstants.length) this.suggestValue(v, (/* ClDouble */newEditConstants[i]).doubleValue());
      }
      catch (err /*ExCLError*/){
        throw new ExCLInternalError("Error during resolve");
      }
    }
    this.resolve();
  },
  resolve: function(x /*double*/, y /*double*/) {
    (/* ClDouble */this._resolve_pair[0]).setValue(x);
    (/* ClDouble */this._resolve_pair[1]).setValue(y);
    this.resolve(this._resolve_pair);
  },
  resolve: function() {
    if (this.fTraceOn) this.fnenterprint("resolve()");
    this.dualOptimize();
    this.setExternalVariables();
    this._infeasibleRows.clear();
    this.resetStayConstants();
  },
  suggestValue: function(v /*ClVariable*/, x /*double*/) {
    if (this.fTraceOn) this.fnenterprint("suggestValue(" + v + ", " + x + ")");
    var cei = /* ClEditInfo */this._editVarMap[v];
    if (cei == null) {
      print("suggestValue for variable " + v + ", but var is not an edit variable\n");
      throw new ExCLError();
    }
    var i = cei.Index();
    var clvEditPlus = cei.ClvEditPlus();
    var clvEditMinus = cei.ClvEditMinus();
    var delta = x - cei.PrevEditConstant();
    cei.SetPrevEditConstant(x);
    this.deltaEditConstant(delta, clvEditPlus, clvEditMinus);
    return this;
  },
  setAutosolve: function(f /*boolean*/) {
    this._fOptimizeAutomatically = f;
    return this;
  },
  FIsAutosolving: function() {
    return this._fOptimizeAutomatically;
  },
  solve: function() {
    if (this._fNeedsSolving) {
      this.optimize(this._objective);
      this.setExternalVariables();
    }
    return this;
  },
  setEditedValue: function(v /*ClVariable*/, n /*double*/) {
    if (!this.FContainsVariable(v)) {
      v.change_value(n);
      return this;
    }
    if (!CL.approx(n, v.value())) {
      this.addEditVar(v);
      this.beginEdit();
      try {
        this.suggestValue(v, n);
      }
      catch (e /*ExCLError*/){
        throw new ExCLInternalError("Error in setEditedValue");
      }
      this.endEdit();
    }
    return this;
  },
  FContainsVariable: function(v /*ClVariable*/) {
    return this.columnsHasKey(v) || (this.rowExpression(v) != null);
  },
  addVar: function(v /*ClVariable*/) {
    if (!this.FContainsVariable(v)) {
      try {
        this.addStay(v);
      }
      catch (e /*ExCLRequiredFailure*/){
        throw new ExCLInternalError("Error in addVar -- required failure is impossible");
      }
      if (this.fTraceOn) {
        this.traceprint("added initial stay on " + v);
      }
    }
    return this;
  },
  getInternalInfo: function() {
    var retstr = this.parent.getInternalInfo();
    retstr += "\nSolver info:\n";
    retstr += "Stay Error Variables: ";
    retstr += this._stayPlusErrorVars.length + this._stayMinusErrorVars.length;
    retstr += " (" + this._stayPlusErrorVars.length + " +, ";
    retstr += this._stayMinusErrorVars.length + " -)\n";
    retstr += "Edit Variables: " + Object.keys(this._editVarMap).length;
    retstr += "\n";
    return retstr;
  },
  getDebugInfo: function() {
    var bstr = this.toString();
    bstr += this.getInternalInfo();
    bstr += "\n";
    return bstr;
  },
  toString: function() {
    var bstr = this.parent();
    bstr += "\n_stayPlusErrorVars: ";
    bstr += this._stayPlusErrorVars;
    bstr += "\n_stayMinusErrorVars: ";
    bstr += this._stayMinusErrorVars;
    bstr += "\n";
    return bstr;
  },
  getConstraintMap: function() {
    return this._markerVars;
  },
  addWithArtificialVariable: function(expr /*ClLinearExpression*/) {
    if (this.fTraceOn) this.fnenterprint("addWithArtificialVariable: " + expr);
    var av = new ClSlackVariable(++this._artificialCounter, "a");
    var az = new ClObjectiveVariable("az");
    var azRow = /* ClLinearExpression */expr.clone();
    if (this.fTraceOn) this.traceprint("before addRows:\n" + this);
    this.addRow(az, azRow);
    this.addRow(av, expr);
    if (this.fTraceOn) this.traceprint("after addRows:\n" + this);
    this.optimize(az);
    var azTableauRow = this.rowExpression(az);
    if (this.fTraceOn) this.traceprint("azTableauRow.constant() == " + azTableauRow.constant());
    if (!CL.approx(azTableauRow.constant(), 0.0)) {
      this.removeRow(az);
      this.removeColumn(av);
      throw new ExCLRequiredFailure();
    }
    var e = this.rowExpression(av);
    if (e != null) {
      if (e.isConstant()) {
        this.removeRow(av);
        this.removeRow(az);
      }
      var entryVar = e.anyPivotableVariable();
      this.pivot(entryVar, av);
    }
    this.assert(this.rowExpression(av) == null, "rowExpression(av) == null");
    this.removeColumn(av);
    this.removeRow(az);
  },
  tryAddingDirectly: function(expr /*ClLinearExpression*/) {
    if (this.fTraceOn) this.fnenterprint("tryAddingDirectly: " + expr);
    var subject = this.chooseSubject(expr);
    if (subject == null) {
      if (this.fTraceOn) this.fnexitprint("returning false");
      return true;
    }
    expr.newSubject(subject);
    if (this.columnsHasKey(subject)) {
      this.substituteOut(subject, expr);
    }
    this.addRow(subject, expr);
    if (this.fTraceOn) this.fnexitprint("returning true");
    return true;
  },
  chooseSubject: function(expr /*ClLinearExpression*/) {
    if (this.fTraceOn) this.fnenterprint("chooseSubject: " + expr);
    var subject = null;
    var foundUnrestricted = true;
    var foundNewRestricted = true;
    var terms = expr.terms();
    for (v in terms) {
      var c = (/* ClDouble */terms.get(v)).doubleValue();
      if (foundUnrestricted) {
        if (!v.isRestricted()) {
          if (!this.columnsHasKey(v)) return v;
        }
      }
      else {
        if (v.isRestricted()) {
          if (!foundNewRestricted && !v.isDummy() && c < 0.0) {
            var col = /* Set */this._columns.get(v);
            if (col == null || (col.size() == 1 && this.columnsHasKey(this._objective))) {
              subject = v;
              foundNewRestricted = true;
            }
          }
        }
        else {
          subject = v;
          foundUnrestricted = true;
        }
      }
    }
    if (subject != null) return subject;
    var coeff = 0.0;
    for (v in terms) {
      var c = (/* ClDouble */terms.get(v)).doubleValue();
      if (!v.isDummy()) return null;
      if (!this.columnsHasKey(v)) {
        subject = v;
        coeff = c;
      }
    }
    if (!CL.approx(expr.constant(), 0.0)) {
      throw new ExCLRequiredFailure();
    }
    if (coeff > 0.0) {
      expr.multiplyMe(-1);
    }
    return subject;
  },
  deltaEditConstant: function(delta /*double*/, plusErrorVar /*ClAbstractVariable*/, minusErrorVar /*ClAbstractVariable*/) {
    if (this.fTraceOn) this.fnenterprint("deltaEditConstant :" + delta + ", " + plusErrorVar + ", " + minusErrorVar);
    var exprPlus = this.rowExpression(plusErrorVar);
    if (exprPlus != null) {
      exprPlus.incrementConstant(delta);
      if (exprPlus.constant() < 0.0) {
        this._infeasibleRows.insert(plusErrorVar);
      }
    }
    var exprMinus = this.rowExpression(minusErrorVar);
    if (exprMinus != null) {
      exprMinus.incrementConstant(-delta);
      if (exprMinus.constant() < 0.0) {
        this._infeasibleRows.insert(minusErrorVar);
      }
    }
    var columnVars = /* Set */this._columns.get(minusErrorVar);
    for (basicVar in columnVars) {
      var expr = this.rowExpression(basicVar);
      var c = expr.coefficientFor(minusErrorVar);
      expr.incrementConstant(c * delta);
      if (basicVar.isRestricted() && expr.constant() < 0.0) {
        this._infeasibleRows.insert(basicVar);
      }
    }
  },
  dualOptimize: function() {
    if (this.fTraceOn) this.fnenterprint("dualOptimize:");
    var zRow = this.rowExpression(this._objective);
    while  (!this._infeasibleRows.isEmpty()) {
      var exitVar = /* ClAbstractVariable */this._infeasibleRows.elements().nextElement();
      this._infeasibleRows.remove(exitVar);
      var entryVar = null;
      var expr = this.rowExpression(exitVar);
      if (expr != null) {
        if (expr.constant() < 0.0) {
          var ratio = Double.MAX_VALUE;
          var r;
          var terms = expr.terms();
          for (v in terms) {
            var c = (/* ClDouble */terms.get(v)).doubleValue();
            if (c > 0.0 && v.isPivotable()) {
              var zc = zRow.coefficientFor(v);
              r = zc / c;
              if (r < ratio || (CL.approx(r, ratio) && v.hashCode() < entryVar.hashCode())) {
                entryVar = v;
                ratio = r;
              }
            }
          }
          if (ratio == Double.MAX_VALUE) {
            throw new ExCLInternalError("ratio == nil (MAX_VALUE) in dualOptimize");
          }
          this.pivot(entryVar, exitVar);
        }
      }
    }
  },
  newExpression: function(cn /*ClConstraint*/, eplus_eminus /*Vector*/, prevEConstant /*ClDouble*/) {
    if (this.fTraceOn) this.fnenterprint("newExpression: " + cn);
    if (this.fTraceOn) this.traceprint("cn.isInequality() == " + cn.isInequality());
    if (this.fTraceOn) this.traceprint("cn.isRequired() == " + cn.isRequired());
    var cnExpr = cn.expression();
    var expr = new ClLinearExpression(cnExpr.constant());
    var slackVar = new ClSlackVariable();
    var dummyVar = new ClDummyVariable();
    var eminus = new ClSlackVariable();
    var eplus = new ClSlackVariable();
    var cnTerms = cnExpr.terms();
    for (v in cnTerms) {
      var c = (/* ClDouble */cnTerms.get(v)).doubleValue();
      var e = this.rowExpression(v);
      if (e == null) expr.addVariable(v, c);
      else expr.addExpression(e, c);
    }
    if (cn.isInequality()) {
      ++this._slackCounter;
      slackVar = new ClSlackVariable(this._slackCounter, "s");
      expr.setVariable(slackVar, -1);
      this._markerVars[cn] = slackVar;
      if (!cn.isRequired()) {
        ++this._slackCounter;
        eminus = new ClSlackVariable(this._slackCounter, "em");
        expr.setVariable(eminus, 1.0);
        var zRow = this.rowExpression(this._objective);
        var sw = cn.strength().symbolicWeight().times(cn.weight());
        zRow.setVariable(eminus, sw.asDouble());
        this.insertErrorVar(cn, eminus);
        this.noteAddedVariable(eminus, this._objective);
      }
    }
    else {
      if (cn.isRequired()) {
        ++this._dummyCounter;
        dummyVar = new ClDummyVariable(this._dummyCounter, "d");
        expr.setVariable(dummyVar, 1.0);
        this._markerVars[cn] = dummyVar;
        if (this.fTraceOn) this.traceprint("Adding dummyVar == d" + this._dummyCounter);
      }
      else {
        ++this._slackCounter;
        eplus = new ClSlackVariable(this._slackCounter, "ep");
        eminus = new ClSlackVariable(this._slackCounter, "em");
        expr.setVariable(eplus, -1.0);
        expr.setVariable(eminus, 1.0);
        this._markerVars[cn] = eplus;
        var zRow = this.rowExpression(this._objective);
        var sw = cn.strength().symbolicWeight().times(cn.weight());
        var swCoeff = sw.asDouble();
        if (swCoeff == 0) {
          if (this.fTraceOn) this.traceprint("sw == " + sw);
          if (this.fTraceOn) this.traceprint("cn == " + cn);
          if (this.fTraceOn) this.traceprint("adding " + eplus + " and " + eminus + " with swCoeff == " + swCoeff);
        }
        zRow.setVariable(eplus, swCoeff);
        this.noteAddedVariable(eplus, this._objective);
        zRow.setVariable(eminus, swCoeff);
        this.noteAddedVariable(eminus, this._objective);
        this.insertErrorVar(cn, eminus);
        this.insertErrorVar(cn, eplus);
        if (cn.isStayConstraint()) {
          this._stayPlusErrorVars.push(eplus);
          this._stayMinusErrorVars.push(eminus);
        }
        else if (cn.isEditConstraint()) {
          eplus_eminus.push(eplus);
          eplus_eminus.push(eminus);
          prevEConstant.setValue(cnExpr.constant());
        }
      }
    }
    if (expr.constant() < 0) expr.multiplyMe(-1);
    if (this.fTraceOn) this.fnexitprint("returning " + expr);
    return expr;
  },
  optimize: function(zVar /*ClObjectiveVariable*/) {
    if (this.fTraceOn) this.fnenterprint("optimize: " + zVar);
    if (this.fTraceOn) this.traceprint(this.toString());
    var zRow = this.rowExpression(zVar);
    this.assert(zRow != null, "zRow != null");
    var entryVar = null;
    var exitVar = null;
    while  (true) {
      var objectiveCoeff = 0;
      var terms = zRow.terms();
      for (v in terms) {
        var c = (/* ClDouble */terms.get(v)).doubleValue();
        if (v.isPivotable() && c < 0.0 && (entryVar == null || v.hashCode() < entryVar.hashCode())) {
          objectiveCoeff = c;
          entryVar = v;
        }
      }
      if (objectiveCoeff >= -this._epsilon) ;
      if (this.fTraceOn) this.traceprint("entryVar == " + entryVar + ", objectiveCoeff == " + objectiveCoeff);
      var minRatio = Double.MAX_VALUE;
      var columnVars = /* Set */this._columns.get(entryVar);
      var r = 0.0;
      for (v in columnVars) {
        if (this.fTraceOn) this.traceprint("Checking " + v);
        if (v.isPivotable()) {
          var expr = this.rowExpression(v);
          var coeff = expr.coefficientFor(entryVar);
          if (this.fTraceOn) this.traceprint("pivotable, coeff = " + coeff);
          if (coeff < 0.0) {
            r = -expr.constant() / coeff;
            if (r < minRatio || (CL.approx(r, minRatio) && v.hashCode() < exitVar.hashCode())) {
              minRatio = r;
              exitVar = v;
            }
          }
        }
      }
      if (minRatio == Double.MAX_VALUE) {
        throw new ExCLInternalError("Objective function is unbounded in optimize");
      }
      this.pivot(entryVar, exitVar);
      if (this.fTraceOn) this.traceprint(this.toString());
    }
  },
  pivot: function(entryVar /*ClAbstractVariable*/, exitVar /*ClAbstractVariable*/) {
    if (this.fTraceOn) this.fnenterprint("pivot: " + entryVar + ", " + exitVar);
    var pexpr = this.removeRow(exitVar);
    pexpr.changeSubject(exitVar, entryVar);
    this.substituteOut(entryVar, pexpr);
    this.addRow(entryVar, pexpr);
  },
  resetStayConstants: function() {
    if (this.fTraceOn) this.fnenterprint("resetStayConstants");
    for (var i = 0; i < this._stayPlusErrorVars.length; i++)
    {
      var expr = this.rowExpression(/* ClAbstractVariable */this._stayPlusErrorVars[i]);
      if (expr == null) expr = this.rowExpression(/* ClAbstractVariable */this._stayMinusErrorVars[i]);
      if (expr != null) expr.set_constant(0.0);
    }
  },
  setExternalVariables: function() {
    if (this.fTraceOn) this.fnenterprint("setExternalVariables:");
    if (this.fTraceOn) this.traceprint(this.toString());
    for (v in _externalParametricVars) {
      if (this.rowExpression(v) != null) {
        print("Error: variable" + v + " in _externalParametricVars is basic");
        continue;
      }
      v.change_value(0.0);
    }
    for (v in _externalRows) {
      var expr = this.rowExpression(v);
      if (this.fTraceOn) this.debugprint("v == " + v);
      if (this.fTraceOn) this.debugprint("expr == " + expr);
      v.change_value(expr.constant());
    }
    this._fNeedsSolving = true;
  },
  insertErrorVar: function(cn /*ClConstraint*/, aVar /*ClAbstractVariable*/) {
    if (this.fTraceOn) this.fnenterprint("insertErrorVar:" + cn + ", " + aVar);
    var cnset = /* Set */this._errorVars[aVar];
    if (cnset == null) this._errorVars[cn] = cnset = {}/* SET: */;
    cnset.insert(aVar);
  },
});
