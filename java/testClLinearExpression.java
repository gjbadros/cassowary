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
// testClLinearExpression.java

public class testClLinearExpression {
  public static void main(String[] args) {
    ClVariable a = new ClVariable("a");
    ClLinearExpression cle = new ClLinearExpression(a);
    System.out.println(cle.toString());
    ClVariable b = new ClVariable("b");
    cle.addVariable(b,2);
    System.out.println(cle.toString()); //ASKCSK
    System.out.println(cle.times(2).toString());
    System.out.println((cle.times(2).addVariable(new ClVariable("c"),3)).times(-1).toString());
    cle = ClLinearExpression.Plus(cle,new ClLinearExpression(8));
    System.out.println(cle.toString());
    cle.changeSubject(a,b);
    System.out.println(cle.toString());
    ClLinearExpression cle2 = (ClLinearExpression) cle.clone();
    cle.addExpression(cle,-1);
    System.out.println(cle.toString());
    System.out.println(b.toString());

//   ClSimplexSolver solver;
//   cout << cle2 << endl;
//   ClVariable c("c");
//   ClLinearExpression cleNew = cle2 * 2 + 9;
//   cout << cleNew << endl;
//   cle2.substituteOut(a,cleNew,c,solver);
//   cout << cle2 << endl;

  }
}
