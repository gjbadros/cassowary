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
// CL.java
// The enumerations from ClLinearInequality,
// and `global' functions that we want easy to access

class CL {
  public static final byte GEQ = 1;
  public static final byte LEQ = 2;

  public static ClLinearExpression Plus(ClLinearExpression e1, ClLinearExpression e2)
    { return e1.plus(e2); }

  public static ClLinearExpression Plus(ClLinearExpression e1, double e2)
    { return e1.plus(new ClLinearExpression(e2)); }

  public static ClLinearExpression Plus(double e1, ClLinearExpression e2)
    { return (new ClLinearExpression(e1)).plus(e2); }

  public static ClLinearExpression Plus(ClVariable e1, ClLinearExpression e2)
    { return (new ClLinearExpression(e1)).plus(e2); }

  public static ClLinearExpression Plus(ClLinearExpression e1, ClVariable e2)
    { return e1.plus(new ClLinearExpression(e2)); }

  public static ClLinearExpression Plus(ClVariable e1, double e2)
    { return (new ClLinearExpression(e1)).plus(new ClLinearExpression(e2)); }

  public static ClLinearExpression Plus(double e1, ClVariable e2)
    { return (new ClLinearExpression(e1)).plus(new ClLinearExpression(e2)); }

  
  public static ClLinearExpression Minus(ClLinearExpression e1, ClLinearExpression e2)
    { return e1.minus(e2); }

  public static ClLinearExpression Minus(double e1, ClLinearExpression e2)
    { return (new ClLinearExpression(e1)).minus(e2); }

  public static ClLinearExpression Minus(ClLinearExpression e1, double e2)
    { return e1.minus(new ClLinearExpression(e2)); }

  public static ClLinearExpression Times(ClLinearExpression e1, ClLinearExpression e2) 
    throws ExCLNonlinearExpression
    { return e1.times(e2); }

  public static ClLinearExpression Times(ClLinearExpression e1, ClVariable e2) 
    throws ExCLNonlinearExpression
    { return e1.times(new ClLinearExpression(e2)); }

  public static ClLinearExpression Times(ClVariable e1, ClLinearExpression e2) 
    throws ExCLNonlinearExpression
    { return (new ClLinearExpression(e1)).times(e2); }

  public static ClLinearExpression Times(ClLinearExpression e1, double e2) 
    throws ExCLNonlinearExpression
    { return e1.times(new ClLinearExpression(e2)); }

  public static ClLinearExpression Times(double e1, ClLinearExpression e2) 
    throws ExCLNonlinearExpression
    { return (new ClLinearExpression(e1)).times(e2); }

  public static ClLinearExpression Times(ClVariable e1, double e2) 
    throws ExCLNonlinearExpression
    { return (new ClLinearExpression(e1)).times(new ClLinearExpression(e2)); }

  public static ClLinearExpression Times(double e1, ClVariable e2) 
    throws ExCLNonlinearExpression
    { return (new ClLinearExpression(e1)).times(e2); }

  public static ClLinearExpression Divide(ClLinearExpression e1, ClLinearExpression e2)
    throws ExCLNonlinearExpression
    { return e1.divide(e2); }


}
