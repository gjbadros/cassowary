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
// ClVariable

class ClVariable extends ClAbstractVariable
{

  public ClVariable(String name, double value)
    {
      super(name);
      my_value = value;
    }

  public ClVariable(String name)
    {
      super(name);
      my_value = 0.0;
    }

  public ClVariable(double value)
    {
      super("");
      my_value = value;
    }

  public ClVariable()
    {
      super("");
      my_value = 0.0;
    }

 
  public ClVariable(long number, String prefix, double value )
    {
      super(number,prefix);
      my_value = value;
    }

  public ClVariable(long number, String prefix )
    {
      super(number,prefix);
      my_value = 0.0;
    }

  public boolean isDummy()
    { return false; }

  public boolean isExternal()
    { return true; }

  public boolean isPivotable()
    { return false; }

  public boolean isRestricted()
    { return false; }

  public String toString()
    {  
      return "[" + name() + ":" + my_value + "]";
    }

  public double value()
    { return my_value; }

  public void set_value(double value)
    { my_value = value; }

  private double my_value;


  public static boolean clApprox(double a, double b)
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
  
  public static boolean clApprox(ClVariable clv, double b)
    {
      return clApprox(clv.value(),b);
    }
  
  static boolean clApprox(double a, ClVariable clv)
    {
      return clApprox(a,clv.value());
    }
}
