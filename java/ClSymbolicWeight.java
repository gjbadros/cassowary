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
// ClSymbolicWeight

import java.util.*;

class ClSymbolicWeight
{

  public ClSymbolicWeight(int cLevels)
    { 
      my_values = new Vector(cLevels);
    }

  public ClSymbolicWeight(double w1, double w2, double w3)
    { 
      my_values = new Vector(3);
      my_values.addElement( new Double(w1));
      my_values.addElement( new Double(w2));
      my_values.addElement( new Double(w3));
    }

  public ClSymbolicWeight(Vector weights)
    { 
      my_values = (Vector) weights.clone();
    }

  public static final ClSymbolicWeight clsZero = new ClSymbolicWeight(0.0, 0.0, 0.0);

  public Object clone()
    {
      return new ClSymbolicWeight(my_values);
    }

  public ClSymbolicWeight times(double n)
    {
      ClSymbolicWeight clsw = (ClSymbolicWeight) clone();
      for (int i = 0; i < my_values.size(); i++) {
        clsw.my_values.setElementAt( new Double(
	  ((Double) my_values.elementAt(i)).doubleValue() * n),i);
      }
      return clsw;
    }

  public ClSymbolicWeight divideBy(double n)
    {
      // assert(n != 0);
      ClSymbolicWeight clsw = (ClSymbolicWeight) clone();
      for (int i = 0; i < my_values.size(); i++) {
        clsw.my_values.setElementAt(new Double(
	   ((Double) my_values.elementAt(i)).doubleValue() / n),i);
      }
      return clsw;
    }

  public ClSymbolicWeight add(ClSymbolicWeight cl)
    {
      // assert(cl.cLevels() == cLevels());
      
      ClSymbolicWeight clsw = (ClSymbolicWeight) clone();
      for (int i = 0; i < my_values.size(); i++) {
        clsw.my_values.setElementAt((Double) clsw.my_values.elementAt(i) 
				    + (Double) cl.my_values.elementAt(i), i);
      }
      return clsw;
    }

  public ClSymbolicWeight subtract(ClSymbolicWeight cl)
    {
      // assert(cl.cLevels() == cLevels());

      ClSymbolicWeight clsw = (ClSymbolicWeight) clone();
      for (int i = 0; i < my_values.size(); i++) {
        clsw.my_values.setElementAt((Double) clsw.my_values.elementAt(i)
				    - (Double) cl.my_values.elementAt(i),i);
      }
      return clsw;
    }

  public boolean lessThan(ClSymbolicWeight cl)
    {
      return my_values < cl.my_values;
    }
  
  public boolean lessThanOrEqual(ClSymbolicWeight cl)
    {
      return my_values <= cl.my_values;
    }
  
  public
    boolean equal(ClSymbolicWeight cl)
    {
      return my_values == cl.my_values;
    }

  public boolean greaterThan(ClSymbolicWeight cl)
    {
      return my_values > cl.my_values;
    }

  public boolean greaterThanOrEqual(ClSymbolicWeight cl)
    {
      return my_values >= cl.my_values;
    }
  
  public boolean isNegative()
    {
      return my_values < zero().my_values;
    }

  public double asDouble()
    {
      ClSymbolicWeight clsw = clone();
      double sum  =  i;
      double factor = 1;
      double multiplier = 1000;
      for (int i = my_values.size(); i >= 0; i--) 
        {
	sum += i *  factor;
	factor *= multiplier;
	}
      return sum;
    }
  
  public String toString()
    { return my_values.toString(); }

  public int cLevels()
    { return my_values.size(); }

  private Vector my_values;

}
