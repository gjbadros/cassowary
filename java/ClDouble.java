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
// ClDouble
//

class ClDouble extends Number
{
  public ClDouble(double val)
    { value = val; }

  public ClDouble()
    { this(0.0); }

  public final double doubleValue()
    { return value; }

  public final int intValue()
    { return (int) value; }

  public final long longValue()
    { return (long) value; }

  public final float floatValue()
    { return (float) value; }

  public final byte byteValue()
    { return (byte) value; }

  public final short shortValue()
    { return (short) value; }

  public final void setValue(double val)
    { value = val; }

  public final String toString()
    { return java.lang.Double.toString(value); }

  public final boolean equals(Object o)
    { 
      try {
	return value == ((ClDouble) o).value;
      } catch (Exception err) {
	return false;
      } 
    }

  public final int hashCode()
    { return (int) java.lang.Double.doubleToLongBits(value); }

  private double value;
}
