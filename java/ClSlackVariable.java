// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This Java Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998 Alan Borning and Greg Badros
// See ../COPYRIGHT for legal details regarding this software
//
// ClSlackVariable
//

class ClSlackVariable extends ClAbstractVariable
{
  // friend ClTableau;
  // friend ClSimplexSolver;

  public ClSlackVariable(String name)
    {  super(name); }

  public ClSlackVariable()
    {  }
  
  public ClSlackVariable(long number, String prefix)
    { super(number,prefix); }
  
  public String toString()
    { return "[" + name() + ":slack]"; }

  public boolean isExternal()
    { return false; }

  public boolean isPivotable()
    { return true; }

  public boolean isRestricted()
    { return true; }
  
}
