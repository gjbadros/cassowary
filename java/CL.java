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
  protected static boolean fDebugOn = false;
  protected static boolean fTraceOn = false;

  protected void debugprint(String s)
  { if (fDebugOn) System.err.println(s); }

  protected static void traceprint(String s)
  { if (fTraceOn) System.err.println(s); }

  protected static void fnenterprint(String s)
  { if (fTraceOn) System.err.println("* " + s); }

  protected static void fnexitprint(String s)
  { if (fTraceOn) System.err.println("- " + s); }

  protected void assert(boolean f) throws ExCLInternalError
  { 
    if (!f) { 
      System.err.println("Assertion failed!"); throw new ExCLInternalError(); 
    }
  }

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

  public static boolean approx(double a, double b)
    {
      double epsilon = 1.0e-8;
      if (a == 0.0) {
	return (Math.abs(b) < epsilon);
      } else if (b == 0.0) {
	return (Math.abs(a) < epsilon);
      } else {
	return (Math.abs(a-b) < Math.abs(a) * epsilon);
      }
    }
  
  public static boolean approx(ClVariable clv, double b)
    {
      return approx(clv.value(),b);
    }
  
  static boolean approx(double a, ClVariable clv)
    {
      return approx(a,clv.value());
    }
}
