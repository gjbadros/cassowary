// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This Java Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998 Alan Borning and Greg Badros
// See ../COPYRIGHT for legal details regarding this software
//
// ClEditOrStayConstraint
//

abstract class ClEditOrStayConstraint extends ClConstraint
{

  public ClEditOrStayConstraint(ClVariable var,
				ClStrength strength,
				double weight)
  {
    super(strength, weight);
    my_variable = var;
    my_expression = new ClLinearExpression(my_variable, -1.0, my_variable.value());
  }

  public ClEditOrStayConstraint(ClVariable var,
				ClStrength strength)
  {
    this(var,strength,1.0);
  }

  public ClEditOrStayConstraint(ClVariable var)
  {
    this(var,ClStrength.required,1.0);
    my_variable = var;
  }
  
  public ClVariable variable()
    { return  my_variable; }

  public ClLinearExpression expression()
    { return my_expression; }

  private void setVariable(ClVariable v)
    { my_variable = v; }

  protected ClVariable  my_variable;
  // cache the expresion
  private ClLinearExpression my_expression;

}
