
// ABSTRACT
var ClLinearConstraint = new Class({
  Extends: ClConstraint,
  /* FIELDS:
     var _expression
   */
  initialize: function(cle /*ClLinearExpression*/, strength /*ClStrength*/, weight /*double*/) {
    this.parent(strength, weight);
    this._expression = cle;
  },

  expression: function() {
    return this._expression;
  },

  setExpression: function(expr /*ClLinearExpression*/) {
    this._expression = expr;
  },
});


var ClLinearInequality = new Class({
  Extends: ClLinearConstraint,

  initialize: function(a1, a2, a3, a4, a5) {
    if (a1 instanceof ClLinearExpression) {
      return this.parent(a1, a2, a3);
    } else if (a2 == CL.GEQ) {
      this.parent(new ClLinearExpression(a3), a4, a5);
      this._expression.multiplyMe(-1.0);
      this._expression.addVariable(a1);
    } else if (a2 == CL.LEQ) {
      this.parent(new ClLinearExpression(a3), a4, a5);
      this._expression.addVariable(a1,-1.0);
    } else {
      throw new ExCLInternalError("Invalid operator in ClLinearInequality constructor");
    }
  },

  toString: function() {
    return this.parent() + " >= 0 )";
  },
});


var ClLinearEquation = new Class({
  Extends: ClLinearConstraint,

  initialize: function(a1, a2, a3, a4) {
    if (a1 instanceof ClLinearExpression && (a2 == undefined || a2 instanceof ClStrength)) {
      this.parent(a1, a2, a3);
    } else if (((a1 instanceof ClLinearExpression) || (a1 instanceof ClAbstractVariable)) &&
               ((a2 instanceof ClLinearExpression) || (a2 instanceof ClAbstractVariable))) {
      if (a1 instanceof ClLinearExpression) {
        a1 = a1.clone();
      } else {
        a1 = new ClLinearExpression(a1);
      }
      this.parent(a1, a3, a4);
      print("this._expression's type = " + typeof(this._expression));
      CL.Assert(this._expression instanceof ClLinearExpression);
      if (a2 instanceof ClAbstractVariable) {
        this._expression.addVariable(a2, -1);
      } else {
        this._expression.addExpression(a2, -1);
      }
    }
  },

  toString: function() {
    return this.parent() + " = 0 )";
  },
});
