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
// QuadDemoWindow.h

#ifndef QuadDemoWindow_H
#define QuadDemoWindow_H

#include <qwidget.h>
#include <qpoint.h>
#include "DraggableBox.h"
#include "ClSimplexSolver.h"

class ClEditConstraint;

class QuadDemoWindow : public QWidget
{ 
  Q_OBJECT
 public:
  QuadDemoWindow( QWidget *parent=0, const char *name=0 );
  ~QuadDemoWindow();
 protected:
  void mouseReleaseEvent( QMouseEvent * );
  void mousePressEvent( QMouseEvent * );
  void mouseMoveEvent( QMouseEvent * );

  void paintEvent( QPaintEvent * );
  private slots:
 private:
  DraggableBox db[4];
  enum {cdb = 4};
  ClPoint mp[4];
  ClSimplexSolver solver;
  ClEditConstraint *peditX;
  ClEditConstraint *peditY;
  int idbDragging;
};

inline QPoint QPFromClP(const ClPoint &clp)
{ return QPoint(clp.Xvalue(), clp.Yvalue()); }

#endif
