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

package EDU.Washington.grad.gjb.cassowary;

public class ClVariable extends ClAbstractVariable
{

  public ClVariable(String name, double value)
    {
      super(name);
      _value = value;
    }

  public ClVariable(String name)
    {
      super(name);
      _value = 0.0;
    }

  public ClVariable(double value)
    {
      _value = value;
    }

  public ClVariable()
    {
      _value = 0.0;
    }

 
  public ClVariable(long number, String prefix, double value )
    {
      super(number,prefix);
      _value = value;
    }

  public ClVariable(long number, String prefix )
    {
      super(number,prefix);
      _value = 0.0;
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
      return "[" + name() + ":" + _value + "]";
    }

  public double value()
    { return _value; }

  public void set_value(double value)
    { _value = value; }

  private double _value;
}
