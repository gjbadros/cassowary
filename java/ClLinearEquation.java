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
// ClLinearEquation
//

class ClLinearEquation extends ClLinearConstraint
{
  public ClLinearEquation(ClLinearExpression cle,
			  ClStrength strength,
			  double weight)
  { super(cle, strength, weight); }

  public ClLinearEquation(ClLinearExpression cle,
			  ClStrength strength)
  { super(cle, strength); }

  public ClLinearEquation(ClLinearExpression cle)
  { super(cle); }


  public ClLinearEquation(ClAbstractVariable clv,
			  ClLinearExpression cle,
			  ClStrength strength,
			  double weight)
  { 
    super(cle, strength, weight); 
    my_expression.addVariable(clv,-1.0);
  }

  public ClLinearEquation(ClAbstractVariable clv,
			  ClLinearExpression cle,
			  ClStrength strength)
  { 
    super(cle, strength); 
    my_expression.addVariable(clv,-1.0);
  }

  public ClLinearEquation(ClAbstractVariable clv,
			  ClLinearExpression cle)
  { 
    super(cle);
    my_expression.addVariable(clv,-1.0);
  }


  public ClLinearEquation(ClLinearExpression cle,
			  ClAbstractVariable clv,
			  ClStrength strength,
			  double weight)
  { 
    super(cle, strength, weight); 
    my_expression.addVariable(clv,-1.0);
  }

  public ClLinearEquation(ClLinearExpression cle,
			  ClAbstractVariable clv,
			  ClStrength strength)
  { 
    super(cle, strength); 
    my_expression.addVariable(clv,-1.0);
  }


  public ClLinearEquation(ClLinearExpression cle,
			  ClAbstractVariable clv)
  { 
    super(cle); 
    my_expression.addVariable(clv,-1.0);
  }

  public ClLinearEquation(ClLinearExpression cle1,
			  ClLinearExpression cle2,
			  ClStrength strength,
			  double weight)
  { 
    super(cle1, strength, weight); 
    my_expression.addExpression(cle2,-1.0);
  }

  public ClLinearEquation(ClLinearExpression cle1,
			  ClLinearExpression cle2,
			  ClStrength strength)
  { 
    super(cle1, strength); 
    my_expression.addExpression(cle2,-1.0);
  }

  public ClLinearEquation(ClLinearExpression cle1,
			  ClLinearExpression cle2)
  { 
    super(cle1); 
    my_expression.addExpression(cle2,-1.0);
  }
  
  public String toString()
  { return super.toString() + " = 0 )"; }
}
