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
// ClStrength

class ClStrength
{
  public ClStrength(String name, ClSymbolicWeight symbolicWeight)
    {  my_name = name;	my_symbolicWeight = symbolicWeight; }

  public ClStrength(String name, double w1, double w2, double w3)
    {
      my_name = name;
      my_symbolicWeight = ClSymbolicWeight(w1,w2,w3);
    }

  public boolean isRequired()
    { return (my_symbolicWeight == clsRequired.symbolicWeight()); }

  public String printString()
    { return name () + (!isRequired()? (":" + symbolicWeight()) : ""); }

  public ClSymbolicWeight symbolicWeight()
    { return my_symbolicWeight; }

  public String name()
    { return my_name; }

  public void set_name(String name)
    { my_name = name; }

  public void set_symbolicWeight(ClSymbolicWeight symbolicWeight)
    { my_symbolicWeight = symbolicWeight; }

  public static final ClStrength clsRequired = new ClStrength("<Required>", 1000, 1000, 1000);

  public static final ClStrength clsStrong = new ClStrength("strong", 1.0, 0.0, 0.0);

  public static final ClStrength clsMedium = new ClStrength("medium", 0.0, 1.0, 0.0);

  public static final ClStrength clsWeak = new ClStrength("weak", 0.0, 0.0, 1.0);

  private String my_name;

  private ClSymbolicWeight my_symbolicWeight;
  
}
