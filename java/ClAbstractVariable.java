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
      
      iVariableNumber++;
      if (name.length() == 0)
      {
	my_name = "v" + iVariableNumber;
      }
    }

  public ClAbstractVariable(long varnumber, String prefix)
    {
      my_name = prefix + varnumber;
      iVariableNumber++;
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

  private String my_name;

  private static long iVariableNumber;

}
