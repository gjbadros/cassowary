//
// File: hello.h
//
// Definition of the Hello widget.
//

#ifndef HELLO_H
#define HELLO_H

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
  DraggableBox db1, db2, db3, db4;
  ClPoint mp1, mp2, mp3, mp4;
  ClSimplexSolver s;
  ClEditConstraint *peditX;
  ClEditConstraint *peditY;
  int idbDragging;
};

inline QPoint QPFromClP(const ClPoint &clp)
{ return QPoint(clp.X(), clp.Y()); }

#endif
