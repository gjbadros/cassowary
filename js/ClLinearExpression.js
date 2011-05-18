// FILE: EDU.Washington.grad.gjb.cassowary
// package EDU.Washington.grad.gjb.cassowary;

load("mootools-core-1.3.2-server.js");

var ClLinearExpression = new Class({
  /* FIELDS:
     private ClDouble _constant
     private Hashtable _terms
 */
  initialize: function(clv /*ClAbstractVariable*/, value /*double*/, constant /*double*/) {
    if (CL.fGC) print("new ClLinearExpression");
    this._constant = constant || 0;
    this._terms = {};
    if (clv instanceof ClAbstractVariable) this._terms[clv] = value || 1;
    else if (typeof(clv) == 'number') this._constant = clv;
  },

  initializeFromHash: function(constant /*ClDouble*/, terms /*Hashtable*/) {
    if (CL.fGC) print("clone ClLinearExpression");
    this._constant = constant;
    this._terms = Object.clone(terms);
    return this;
  },
  
  multiplyMe: function(x /*double*/) {
    this._constant *= x;
    for (var clv in this._terms) {
      this._terms[clv] *= x;
    }
    return this;
  },

  clone: function() {
    return new ClLinearExpression().initializeFromHash(this._constant, this._terms);
  },

  times: function(x) {
    if (typeof(x) == 'number') {
      return (this.clone()).multiplyMe(x);
    } else {
      if (this.isConstant()) {
        expr = x;
        return expr.times(this._constant);
      } else if (expr.isConstant()) {
        return this.times(expr._constant);
      } else {
        throw new ExCLNonlinearExpression();
      }
    }
  },

  plus: function(expr /*ClLinearExpression*/) {
    if (expr instanceof ClLinearExpression) {
      return this.clone().addExpression(expr, 1.0);
    } else if (expr instanceof ClVariable) {
      return this.clone().addVariable(expr, 1.0);
    }
  },

  minus: function(expr /*ClLinearExpression*/) {
    if (expr instanceof ClLinearExpression) {
      return this.clone().addExpression(expr, -1.0);
    } else if (expr instanceof ClVariable) {
      return this.clone().addVariable(expr, -1.0);
    }
  },


  divide: function(x) {
    if (typeof(x) == 'number') {
      if (CL.approx(x, 0.0)) {
        throw new ExCLNonlinearExpression();
      }
      return times(1.0 / x);
    } else if (x instanceof ClLinearExpression) {
      if (!x.isConstant) {
        throw new ExCLNonlinearExpression();
      }
      return this.times(1.0 / x._constant);
    }
  },

  divFrom: function(expr) {
    if (!this.isConstant() || CL.approx(this._constant, 0.0)) {
        throw new ExCLNonlinearExpression();
    }
    return x.divide(this._constant);
  },

  subtractFrom: function(expr /*ClLinearExpression*/) {
    return expr.minus(this);
  },

  addExpression: function(expr /*ClLinearExpression*/, n /*double*/, subject /*ClAbstractVariable*/, solver /*ClTableau*/) {
    this.incrementConstant(n * expr.constant());
    n = n || 1;
    for (clv in expr.terms()) {
      coeff = expr.terms()[clv];
      this.addVariable(clv, coeff*n, subject, solver);
    }
    return this;
  },

  addVariable: function(v /*ClAbstractVariable*/, c /*double*/, subject, solver) {
    c = c || 1.0;
    if (CL.fTraceOn) CL.fnenterprint("addVariable:" + v + ", " + c);
    coeff = this._terms[v];
    if (coeff) {
      new_coefficient = coeff + c;
      if (CL.approx(new_coefficient, 0.0)) {
        if (solver) {
          solver.noteRemovedVariable(v, subject);
        }
        delete this._terms[v];
      } else {
        this._terms[v] = new_coefficient;
      }
    } else {
      if (!CL.approx(c, 0.0)) {
        this._terms[v] = c;
        if (solver) {
          solver.noteAddedVariable(v, subject);
        }
      }
    }
    return this;
  },

  setVariable: function(v /*ClAbstractVariable*/, c /*double*/) {
    this._terms[v] = c;
    return this;
  },

  anyPivotableVariable: function() {
    if (this.isConstant()) {
      throw new ExCLInternalError("anyPivotableVariable called on a constant");
    } 
    
    for (clv in this._terms) {
      if (clv.isPivotable()) return clv;
    }
    return null;
  },
  
  substituteOut: function(outvar /*ClAbstractVariable*/, expr /*ClLinearExpression*/, subject /*ClAbstractVariable*/, solver /*ClTableau*/) {
    if (this.fTraceOn) CL.fnenterprint("CLE:substituteOut: " + outvar + ", " + expr + ", " + subject + ", ...");
    if (this.fTraceOn) this.traceprint("this = " + this);
    var multiplier = this._terms[outvar];
    delete this._terms[outvar];
    this.incrementConstant(multiplier * expr.constant());
    for (var clv in expr.terms()) {
      var coeff = expr.terms()[clv];
      var old_coeff = this._terms[clv];
      if (old_coeff) {
        var newCoeff = old_coeff + multiplier * coeff;
        if (CL.approx(newCoeff, 0.0)) {
          solver.noteRemovedVariable(clv, subject);
          delete this._terms[clv];
        } else {
          this._terms[clv] = newCoeff;
        }
      } else {
        this._terms[clv] = multiplier * coeff;
        solver.noteAddedVariable(clv, subject);
      }
    }
    if (this.fTraceOn) this.traceprint("Now this is " + this);
  },

  changeSubject: function(old_subject /*ClAbstractVariable*/, new_subject /*ClAbstractVariable*/) {
    this._terms[old_subject] = this.newSubject(new_subject);
  },

  newSubject: function(subject /*ClAbstractVariable*/) {
    if (this.fTraceOn) CL.fnenterprint("newSubject:" + subject);
    
    var reciprocal = 1.0 / this._terms[subject];
    delete this._terms[subject];
    this.multiplyMe(-reciprocal);
    return reciprocal;
  },

  coefficientFor: function(clv /*ClAbstractVariable*/) {
    return this._terms[clv] || 0;
  },

  constant: function() {
    return this._constant;
  },

  set_constant: function(c /*double*/) {
    this._constant = c;
  },

  terms: function() {
    return this._terms;
  },

  incrementConstant: function(c /*double*/) {
    this._constant += c;
  },

  isConstant: function() {
    return Object.keys(this._terms).length == 0;
  },

  toString: function() {
    var bstr = ''; // answer
    var needsplus = false;
    if (!CL.approx(this._constant, 0.0) || this.isConstant()) {
      bstr += this._constant;
      if (this.isConstant()) {
        return bstr;
      } else {
        needsplus = true;
      }
    } 
    for (clv in this._terms) {
      var coeff = this._terms[clv];
      if (needsplus) {
        bstr += " + ";
      }
      bstr += coeff + "*" + clv;
      needsplus = true;
    }
    return bstr;
  },

  Plus: function(e1 /*ClLinearExpression*/, e2 /*ClLinearExpression*/) {
    return e1.plus(e2);
  },
  Minus: function(e1 /*ClLinearExpression*/, e2 /*ClLinearExpression*/) {
    return e1.minus(e2);
  },
  Times: function(e1 /*ClLinearExpression*/, e2 /*ClLinearExpression*/) {
    return e1.times(e2);
  },
  Divide: function(e1 /*ClLinearExpression*/, e2 /*ClLinearExpression*/) {
    return e1.divide(e2);
  },
});


