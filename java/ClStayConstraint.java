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
// ClStayConstraint
// 

class ClStayConstraint extends ClEditOrStayConstraint
{

  public ClStayConstraint(ClVariable var, ClStrength strength, double weight)
    { super(var,strength,weight); }

  public ClStayConstraint(ClVariable var, ClStrength strength)
    { super(var,strength,1.0); }

  public ClStayConstraint(ClVariable var)
    { super(var,ClStrength.clsWeak,1.0); }

  public boolean isStayConstraint()
    { return true; }

  public String toString()
    { return "stay" + variable().toString(); }

}
