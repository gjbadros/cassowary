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
    { my_strength = strength; my_weight = weight; }

  public ClConstraint(ClStrength strength)
    { my_strength = strength; my_weight = 1.0; }

  public ClConstraint()
    { my_strength = ClStrength.required; my_weight = 1.0; }
  
  public abstract ClLinearExpression expression();

  public boolean isEditConstraint()
    { return false; }

  public boolean isInequality()
    { return false; }

  public boolean isRequired()
    { return my_strength.isRequired(); }

  public boolean isStayConstraint()
    { return false; }

  public ClStrength strength()
    { return my_strength; }

  public double weight()
    { return my_weight; }

  public String toString()
    { return my_strength.toString() +
	" {" + weight() + "} (" + expression(); }

  private void setStrength(ClStrength strength)
    { my_strength = strength; }

  private void setWeight(double weight)
    { my_weight = weight; }

  private ClStrength my_strength;
  private double my_weight;

}
