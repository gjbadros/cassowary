// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This Java Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998 Alan Borning and Greg Badros
// See ../LICENSE for legal details regarding this software
//
// ClStayConstraint
// 

package EDU.Washington.grad.gjb.cassowary;

public class ClStayConstraint extends ClEditOrStayConstraint
{

  public ClStayConstraint(ClVariable var, ClStrength strength, double weight)
    { super(var,strength,weight); }

  public ClStayConstraint(ClVariable var, ClStrength strength)
    { super(var,strength,1.0); }

  public ClStayConstraint(ClVariable var)
    { super(var,ClStrength.weak,1.0); }

  public boolean isStayConstraint()
    { return true; }

  public String toString()
    { return "stay " + super.toString(); }

}
