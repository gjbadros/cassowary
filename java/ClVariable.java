// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This Java Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998 Alan Borning and Greg Badros
// See ../COPYRIGHT for legal details regarding this software
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
      my_value = value;
    }

  public ClVariable()
    {
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
}
