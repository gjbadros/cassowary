// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This Java Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998 Alan Borning and Greg Badros
// See ../COPYRIGHT for legal details regarding this software
//
// ClObjectiveVariable
//

class ClObjectiveVariable extends ClAbstractVariable
{
  public ClObjectiveVariable(String name)
    {
      super(name);
    }
  public ClObjectiveVariable(long number, String prefix)
    {
      super(number,prefix);
    }
  
  public String toString()
       //    { return "[" + name() + ":obj:" + hashCode() + "]"; }
    { return "[" + name() + ":obj]"; }

  public boolean isExternal()
    { return false; }

  public boolean isPivotable()
    { return false; }

  public boolean isRestricted()
    { return false; }

}
