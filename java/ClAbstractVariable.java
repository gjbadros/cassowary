// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This Java Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998 Alan Borning and Greg Badros
// See ../COPYRIGHT for legal details regarding this software
//
// ClAbstractVariable

import java.lang.*;

abstract class ClAbstractVariable
{
  public ClAbstractVariable(String name)
    {
      //hash_code = iVariableNumber;
      my_name = name;
      iVariableNumber++;
    }

  public ClAbstractVariable()
    {
      //hash_code = iVariableNumber;
      my_name = "v" + iVariableNumber;
      iVariableNumber++;
    }

  public ClAbstractVariable(long varnumber, String prefix)
    {
      //hash_code = iVariableNumber;
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

  public static int numCreated() 
    { return iVariableNumber; }

  // for debugging
  //  public final int hashCode() { return hash_code; }

  private String my_name;

  // for debugging
  // private int hash_code;

  private static int iVariableNumber;

}
