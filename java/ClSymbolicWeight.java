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
      my_values = new double[cLevels];
      // FIXGJB: ok to assume these get initialized to 0?
//       for (int i = 0; i < cLevels; i++) {
// 	my_values[i] = 0;
//       }
    }

  public ClSymbolicWeight(double w1, double w2, double w3)
    { 
      my_values = new double[3];
      my_values[0] = w1;
      my_values[1] = w2;
      my_values[2] = w3;
    }

  public ClSymbolicWeight(double[] weights)
    { 
      final int cLevels = weights.length;
      my_values = new double[cLevels];
      for (int i = 0; i < cLevels; i++) {
	my_values[i] = weights[i];
      }
    }

  public static final ClSymbolicWeight clsZero = new ClSymbolicWeight(0.0, 0.0, 0.0);

  public Object clone()
    {
      return new ClSymbolicWeight(my_values);
    }

  public ClSymbolicWeight times(double n)
    {
      ClSymbolicWeight clsw = (ClSymbolicWeight) clone();
      for (int i = 0; i < my_values.length; i++) {
        clsw.my_values[i] *= n;
      }
      return clsw;
    }

  public ClSymbolicWeight divideBy(double n)
    {
      // assert(n != 0);
      ClSymbolicWeight clsw = (ClSymbolicWeight) clone();
      for (int i = 0; i < my_values.length; i++) {
        clsw.my_values[i] /= n;
      }
      return clsw;
    }

  public ClSymbolicWeight add(ClSymbolicWeight cl)
    {
      // assert(cl.cLevels() == cLevels());
      
      ClSymbolicWeight clsw = (ClSymbolicWeight) clone();
      for (int i = 0; i < my_values.length; i++) {
        clsw.my_values[i] += cl.my_values[i];
      }
      return clsw;
    }

  public ClSymbolicWeight subtract(ClSymbolicWeight cl)
    {
      // assert(cl.cLevels() == cLevels());

      ClSymbolicWeight clsw = (ClSymbolicWeight) clone();
      for (int i = 0; i < my_values.length; i++) {
        clsw.my_values[i] -= cl.my_values[i];
      }
      return clsw;
    }

  public boolean lessThan(ClSymbolicWeight cl)
    {
      // assert cl.cLevels() == cLevels()
      for (int i = 0; i < my_values.length; i++) {
	if (my_values[i] < cl.my_values[i])
	  return true;
	else if (my_values[i] > cl.my_values[i])
	  return false;
      }
      return false; // they are equal
    }
  
  public boolean lessThanOrEqual(ClSymbolicWeight cl)
    {
      // assert cl.cLevels() == cLevels()
      for (int i = 0; i < my_values.length; i++) {
	if (my_values[i] < cl.my_values[i])
	  return true;
	else if (my_values[i] > cl.my_values[i])
	  return false;
      }
      return true; // they are equal
    }
  
  public
    boolean equal(ClSymbolicWeight cl)
    {
      for (int i = 0; i < my_values.length; i++) {
	if (my_values[i] != cl.my_values[i])
	  return false;
      }
      return true; // they are equal
    }

  public boolean greaterThan(ClSymbolicWeight cl)
    {
      return !this.lessThanOrEqual(cl);
    }

  public boolean greaterThanOrEqual(ClSymbolicWeight cl)
    {
      return !this.lessThan(cl);
    }
  
  public boolean isNegative()
    {
      return this.lessThan(clsZero);
    }

  public double asDouble()
    {
      ClSymbolicWeight clsw = (ClSymbolicWeight) clone();
      double sum  =  0;
      double factor = 1;
      double multiplier = 1000;
      for (int i = my_values.length - 1; i >= 0; i--) 
        {
	sum += my_values[i] * factor;
	factor *= multiplier;
	}
      return sum;
    }
  
  public String toString()
    { 
      StringBuffer bstr = new StringBuffer("[");
      for (int i = 0; i < my_values.length-1; i++) {
	bstr.append(my_values[i]);
	bstr.append(",");
      }
      bstr.append(my_values[my_values.length-1]);
      bstr.append("]");
      return bstr.toString();
    }

  public int cLevels()
    { return my_values.length; }

  private double[] my_values;

}
