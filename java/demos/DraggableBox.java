// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998, All Rights Reserved.
//
// (c) 1998 Alan Borning, Michael Noth, and Greg Badros.  
// This code is provided for use by students for course projects in
// the course CSE 595 in the Department of Computer Science and
// Engineering, University of Washington, during winter quarter 1998.
// Any other use requires written permission from the copyright
// holders.
//
// Implementation of the DraggableBox class
// By Michael Noth, 9 Feb 1998
//
// DraggableBox.java

import java.awt.Graphics;

public class DraggableBox {
  ClPoint center; 
  int    sx, sy;        // Screen x, y of center
  int    width, height; // Dims of box

  public DraggableBox(double x, double y, int width, int height) {
    center = new ClPoint(x, y);
    this.width = width;
    this.height = height;
    cvt();
  }

  // this version for debugging
  // creates variables with names xa, ya
  public DraggableBox(int a) {
    center = new ClPoint(0, 0, a);
    this.width = 6;
    this.height = 6;
    cvt();
  }

  public DraggableBox() {
    center = new ClPoint(0, 0);
    this.width = 6;
    this.height = 6;
    cvt();
  }
    

  // Helper function to convert center to screen coords
  void cvt() {
    sx = (int) center.X().value();
    sy = (int) center.Y().value();
  }

  // Draw the box
  public void draw(Graphics g) {
    cvt();
    g.drawRect(sx - (width / 2), sy - (height / 2), width, height);
  }

  public void SetCenter(double x, double y) {
    center.SetXY(x, y);
    cvt();
  }

  public void SetSize(int width, int height) {
    this.width = width;
    this.height = height;
  }

  public double CenterX() { return center.Xvalue(); }
  public double CenterY() { return center.Yvalue(); }
  public ClVariable X() { return center.X(); }
  public ClVariable Y() { return center.Y(); }

  public ClPoint CenterPt() { return center; }

  public boolean Contains(int x, int y) {
    if ( (x >= sx - width/2) && (x <= sx + width/2) &&
         (y >= sy - height/2) && (y <= sy + height/2) )
      return true;
    else
      return false;
  }
  
}
