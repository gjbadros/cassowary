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
// ClAbstractVariable

import java.lang.*;

abstract class ClAbstractVariable
{
  public ClAbstractVariable(String name)
    {
      my_name = name;
      hash_code = iVariableNumber++;
    }

  public ClAbstractVariable()
    {
      my_name = "v" + iVariableNumber;
      hash_code = iVariableNumber++;
    }

  public ClAbstractVariable(long varnumber, String prefix)
    {
      my_name = prefix + varnumber;
      hash_code = iVariableNumber++;
    }

  public String name()
    { return my_name; }
  
  public void setName(String name)
    { my_name = name; }
  
  public boolean isDummy()
    { return false; }
  
  public abstract boolean isExternal();

  public abstract boolean isPivotable();

  public abstract boolean isRestricted();

  public abstract String toString();

  public final int hashCode() { return hash_code; }

  private String my_name;

  // FIXGJB for debugging
  private int hash_code;

  private static int iVariableNumber;

}
