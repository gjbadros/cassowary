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
// ClPoint
//

class ClPoint
{
  public ClPoint(double x, double y)
    {
      my_clv_x = new ClVariable(x);
      my_clv_y = new ClVariable(y);
    }

  public ClPoint(ClVariable clv_x, ClVariable clv_y)
    { my_clv_x = clv_x; my_clv_y = clv_y; }

  public ClVariable X()
    { return my_clv_x; }

  public ClVariable Y()
    { return my_clv_y; }

  public void SetXY(double x, double y)
    { my_clv_x = new ClVariable(x); my_clv_y = new ClVariable(y); }

  public void SetXY(ClVariable clv_x, ClVariable clv_y)
    { my_clv_x = clv_x; my_clv_y = clv_y; }

  public double Xvalue()
    { return X().value(); }

  public double Yvalue()
    { return Y().value(); }

  public String toString()
    {
      return "(" + my_clv_x.toString() + ", " + my_clv_y.toString() + ")";
    }

  private ClVariable my_clv_x;

  private ClVariable my_clv_y;

}
