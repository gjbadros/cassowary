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
    this(clv,cle,strength,1.0);
  }

  public ClLinearEquation(ClAbstractVariable clv,
			  ClLinearExpression cle)
  { 
    this(clv,cle,ClStrength.required,1.0);
  }

  public ClLinearEquation(ClAbstractVariable clv,
			  double val,
			  ClStrength strength,
			  double weight)
  { 
    super(new ClLinearExpression(val), strength, weight); 
    my_expression.addVariable(clv,-1.0);
  }

  public ClLinearEquation(ClAbstractVariable clv,
			  double val,
			  ClStrength strength)
  { 
    this(clv,val,strength,1.0);
  }

  public ClLinearEquation(ClAbstractVariable clv,
			  double val)
  { 
    this(clv,val,ClStrength.required,1.0);
  }

  public ClLinearEquation(ClLinearExpression cle,
			  ClAbstractVariable clv,
			  ClStrength strength,
			  double weight)
  { 
    super(((ClLinearExpression) cle.clone()), strength, weight); 
    my_expression.addVariable(clv,-1.0);
  }

  public ClLinearEquation(ClLinearExpression cle,
			  ClAbstractVariable clv,
			  ClStrength strength)
  { 
    this(cle,clv,strength,1.0);
  }


  public ClLinearEquation(ClLinearExpression cle,
			  ClAbstractVariable clv)
  { 
    this(cle,clv,ClStrength.required,1.0);
  }

  public ClLinearEquation(ClLinearExpression cle1,
			  ClLinearExpression cle2,
			  ClStrength strength,
			  double weight)
  { 
    super(((ClLinearExpression) cle1.clone()), strength, weight); 
    my_expression.addExpression(cle2,-1.0);
  }

  public ClLinearEquation(ClLinearExpression cle1,
			  ClLinearExpression cle2,
			  ClStrength strength)
  { 
    this(cle1,cle2,strength,1.0);
  }

  public ClLinearEquation(ClLinearExpression cle1,
			  ClLinearExpression cle2)
  { 
    this(cle1,cle2,ClStrength.required,1.0);
  }
  
  public String toString()
  { return super.toString() + " = 0 )"; }
}
