// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998, All Rights Reserved.
//
// (c) 1998 Alan Borning and Greg Badros.  This code is provided for use by
// students for course projects in the course CSE 595 in the Department of
// Computer Science and Engineering, University of Washington, during winter
// quarter 1998.  Any other use requires written permission from the copyright
// holders.
//
// testClStrength.java

class testClStrength {
  public static void main(String[] args) {
    ClSymbolicWeight clsw = new ClSymbolicWeight(0.0,1.0,0.0);
    ClSymbolicWeight clsw2 = new ClSymbolicWeight(2.0,0.5,0.5);
    ClSymbolicWeight clsw3 = new ClSymbolicWeight(2.0,0.5,0.5);
    ClSymbolicWeight clsw4 = new ClSymbolicWeight(2.0,0.4,0.5);

    System.out.println(ClStrength.required);
    System.out.println(clsw2.asDouble());

    ClStrength cls = new ClStrength("cls",clsw);
    System.out.println(cls);
    
    ClStrength cls2 = new ClStrength("cls2", clsw.times(2));
    System.out.println(cls2);
  }
}
