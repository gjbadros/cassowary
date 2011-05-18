load('CL.js')
load('ClLinearExpression.js')
load('ClVariable.js')


var ClTableau = new Class({
  /* FIELDS:
      var _columns //Hashtable
      var _rows //Hashtable
      var _infeasibleRows //Set
      var _externalRows //Set
      var _externalParametricVars //Set
 */
  initialize: function() {
    this._columns = {};
    this._rows = {};
    this._infeasibleRows = {}/* SET: */;
    this._externalRows = {}/* SET: */;
    this._externalParametricVars = {}/* SET: */;
  },
  noteRemovedVariable: function(v /*ClAbstractVariable*/, subject /*ClAbstractVariable*/) {
    if (this.fTraceOn) this.fnenterprint("noteRemovedVariable: " + v + ", " + subject);
    if (subject != null) {
      delete (/* Set */this._columns[v])[subject];
    }
  },
  noteAddedVariable: function(v /*ClAbstractVariable*/, subject /*ClAbstractVariable*/) {
    if (this.fTraceOn) this.fnenterprint("noteAddedVariable: " + v + ", " + subject);
    if (subject != null) {
      this.insertColVar(v, subject);
    }
  },
  getInternalInfo: function() {
    var retstr = "Tableau Information:\n";
    retstr += "Rows: " + Object.keys(this._rows).length;
    retstr += " (= " + (Object.keys(this._rows).length - 1) + " constraints)";
    retstr += "\nColumns: " + Object.keys(this._columns).length;
    retstr += "\nInfeasible Rows: " + Object.keys(this._infeasibleRows).length;
    retstr += "\nExternal basic variables: " + Object.keys(this._externalRows).length;
    retstr += "\nExternal parametric variables: ";
    retstr += Object.keys(this._externalParametricVars).length;
    retstr += "\n";
    return retstr;
  },
  toString: function() {
    var bstr = "Tableau:\n";
    for (clv in this._rows) {
      var expr = /* ClLinearExpression */this._rows[clv];
      bstr += clv;
      bstr += " <==> ";
      bstr += expr;
      bstr += "\n";
    }
    bstr += "\nColumns:\n";
    bstr += CL.hashToString(this._columns);
    bstr += "\nInfeasible rows: ";
    bstr += CL.setToString(this._infeasibleRows);
    bstr += "External basic variables: ";
    bstr += CL.setToString(this._externalRows);
    bstr += "External parametric variables: ";
    bstr += CL.setToString(this._externalParametricVars);
    return bstr;
  },
  insertColVar: function(param_var /*ClAbstractVariable*/, rowvar /*ClAbstractVariable*/) {
    var rowset = /* Set */this._columns[param_var];
    if (rowset == null) this._columns[param_var] = rowset = {}/* SET: */;
    rowset.insert(rowvar);
  },
  addRow: function(aVar /*ClAbstractVariable*/, expr /*ClLinearExpression*/) {
    if (this.fTraceOn) this.fnenterprint("addRow: " + aVar + ", " + expr);
    this._rows[aVar] = expr;
    for (clv in expr) {
      this.insertColVar(clv, aVar);
      if (clv.isExternal()) {
        this._externalParametricVars[clv] = true;
      }
    }
    if (aVar.isExternal()) {
      this._externalRows[aVar] = true;
    }
    if (this.fTraceOn) this.traceprint(this.toString());
  },
  removeColumn: function(aVar /*ClAbstractVariable*/) {
    if (this.fTraceOn) this.fnenterprint("removeColumn:" + aVar);
    var rows = /* Set */delete this._columns[aVar];
    if (rows != null) {
      for (clv in rows) {
        var expr = /* ClLinearExpression */this._rows[clv];
        expr.terms().remove(aVar);
      }
    }
    else {
      if (this.fTraceOn) this.debugprint("Could not find var " + aVar + " in _columns");
    }
    if (aVar.isExternal()) {
      delete this._externalRows[aVar];
      delete this._externalParametricVars[aVar];
    }
  },
  removeRow: function(aVar /*ClAbstractVariable*/) {
    if (this.fTraceOn) this.fnenterprint("removeRow:" + aVar);
    var expr = /* ClLinearExpression */this._rows[aVar];
    this.assert(expr != null);
    for (clv in expr) {
      var varset = /* Set */this._columns[clv];
      if (varset != null) {
        if (this.fTraceOn) this.debugprint("removing from varset " + aVar);
        varset.remove(aVar);
      }
    }
    delete this._infeasibleRows[aVar];
    if (aVar.isExternal()) {
      delete this._externalRows[aVar];
    }
    delete this._rows[aVar];
    if (this.fTraceOn) this.fnexitprint("returning " + expr);
    return expr;
  },
  substituteOut: function(oldVar /*ClAbstractVariable*/, expr /*ClLinearExpression*/) {
    if (this.fTraceOn) this.fnenterprint("substituteOut:" + oldVar + ", " + expr);
    if (this.fTraceOn) this.traceprint(this.toString());
    var varset = /* Set */this._columns[oldVar];
    for (v in varset) {
      var row = /* ClLinearExpression */this._rows[v];
      row.substituteOut(oldVar, expr, v, this);
      if (v.isRestricted() && row.constant() < 0.0) {
        this._infeasibleRows[v] = true;
      }
    }
    if (oldVar.isExternal()) {
      this._externalRows[oldVar] = true;
      delete this._externalParametricVars[oldVar];
    }
    delete this._columns[oldVar];
  },
  columns: function() {
    return this._columns;
  },
  rows: function() {
    return this._rows;
  },
  columnsHasKey: function(subject /*ClAbstractVariable*/) {
    return (this._columns[subject] != undefined);
  },
  rowExpression: function(v /*ClAbstractVariable*/) {
    return /* ClLinearExpression */this._rows[v];
  },
});


