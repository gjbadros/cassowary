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
// ClLinearConstraint
//

class ClLinearConstraint extends ClConstraint
{

  public ClLinearConstraint(ClLinearExpression cle,
			    ClStrength strength,
			    double weight)
  { 
    super(strength,weight);
    my_expression = cle;
  }

  public ClLinearConstraint(ClLinearExpression cle,
			    ClStrength strength)
  { 
    super(strength,1.0);
    my_expression = cle;
  }

  public ClLinearConstraint(ClLinearExpression cle)
  { 
    super(ClStrength.required,1.0);
    my_expression = cle;
  }

  public ClLinearExpression expression()
  { return my_expression; }

  protected void setExpression(ClLinearExpression expr)
  { my_expression = expr; }

  protected ClLinearExpression my_expression;
}
