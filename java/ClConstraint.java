// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This Java Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998 Alan Borning and Greg Badros
// See ../COPYRIGHT for legal details regarding this software
//
// ClConstraint
//

import java.lang.*;


abstract class ClConstraint
{

  public ClConstraint(ClStrength strength, double weight)
    { _strength = strength; _weight = weight; }

  public ClConstraint(ClStrength strength)
    { _strength = strength; _weight = 1.0; }

  public ClConstraint()
    { _strength = ClStrength.required; _weight = 1.0; }
  
  public abstract ClLinearExpression expression();

  public boolean isEditConstraint()
    { return false; }

  public boolean isInequality()
    { return false; }

  public boolean isRequired()
    { return _strength.isRequired(); }

  public boolean isStayConstraint()
    { return false; }

  public ClStrength strength()
    { return _strength; }

  public double weight()
    { return _weight; }

  public String toString()
    { return _strength.toString() +
	" {" + weight() + "} (" + expression(); }

  private void setStrength(ClStrength strength)
    { _strength = strength; }

  private void setWeight(double weight)
    { _weight = weight; }

  private ClStrength _strength;
  private double _weight;

}
