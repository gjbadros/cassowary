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
// Implementation of the QuadDemoWindow widget.
//
// QuadDemoWindow.cc

#include "QuadDemoWindow.h"
#include "Cl.h"
#include <qpushbt.h>
#include <qpainter.h>
#include <qpixmap.h>
#include <qbitmap.h>

/*
  Constructs a QuadDemoWindow widget.
*/

QuadDemoWindow::QuadDemoWindow( QWidget *parent, const char *name )
  : QWidget(parent,name),
    db1(5,5), db2(5,200), db3(200,200), db4(200,5),
    peditX(NULL), peditY(NULL),
    idbDragging(-1)
{
  resize( 450, 450 );
  double weight = 1;
  s
    .addPointStay(db1.CenterPt(),weight)
    .addPointStay(db2.CenterPt(),weight *= 2)
    .addPointStay(db3.CenterPt(),weight *= 2)
    .addPointStay(db4.CenterPt(),weight *= 2)
    ;
    
  s
    .addConstraint(ClLinearEquation(mp1.Xvar(),(db1.Xvar() + db2.Xvar())/2))
    .addConstraint(ClLinearEquation(mp1.Yvar(),(db1.Yvar() + db2.Yvar())/2))
    .addConstraint(ClLinearEquation(mp2.Xvar(),(db2.Xvar() + db3.Xvar())/2))
    .addConstraint(ClLinearEquation(mp2.Yvar(),(db2.Yvar() + db3.Yvar())/2))
    .addConstraint(ClLinearEquation(mp3.Xvar(),(db3.Xvar() + db4.Xvar())/2))
    .addConstraint(ClLinearEquation(mp3.Yvar(),(db3.Yvar() + db4.Yvar())/2))
    .addConstraint(ClLinearEquation(mp4.Xvar(),(db4.Xvar() + db1.Xvar())/2))
    .addConstraint(ClLinearEquation(mp4.Yvar(),(db4.Yvar() + db1.Yvar())/2))
    ;

  setBackgroundMode(NoBackground);
}

QuadDemoWindow::~QuadDemoWindow()
{
  if (peditX)
    delete peditX;
  if (peditY)
    delete peditY;
}

void QuadDemoWindow::mousePressEvent( QMouseEvent *e )
{
  int x = e->x();
  int y = e->y();
  ClVariable *pedit_x = NULL;
  ClVariable *pedit_y = NULL;
  if (db1.FContains(x,y))
    {
    idbDragging = 1;
    pedit_x = &db1.Xvar();
    pedit_y = &db1.Yvar();
    }
  else if (db2.FContains(x,y))
    {
    idbDragging = 2;
    pedit_x = &db2.Xvar();
    pedit_y = &db2.Yvar();
    }
  else if (db3.FContains(x,y))
    {
    idbDragging = 3;
    pedit_x = &db3.Xvar();
    pedit_y = &db3.Yvar();
    }
  else if (db4.FContains(x,y))
    {
    idbDragging = 4;
    pedit_x = &db4.Xvar();
    pedit_y = &db4.Yvar();
    }
  if (idbDragging != -1)
    {
    peditX = new ClEditConstraint(*pedit_x,clsStrong());
    peditY = new ClEditConstraint(*pedit_y,clsStrong());
    s.addConstraint(*peditX);
    s.addConstraint(*peditY);
    }
}

void QuadDemoWindow::mouseReleaseEvent( QMouseEvent * )
{
  if (idbDragging != -1)
    {
    try
      {
      assert(peditX != NULL);
      assert(peditY != NULL);
      idbDragging = -1;
      s.removeConstraint(*peditX);
      s.removeConstraint(*peditY);
      delete peditX;
      delete peditY;
      peditX = peditY = NULL;
      repaint();
      }
    catch (ExCLError &error) 
      {
      cerr << "Exception! " << error.description() << endl;
      return;
      }
    catch (...) 
      {
      cerr << "Unknown exception" << endl;
      return;
      }
    }
}


void QuadDemoWindow::mouseMoveEvent( QMouseEvent *e )
{
  if (idbDragging != -1)
    {
    s.resolve(e->x(),e->y());
    repaint();
    }
}



/*
  Handles paint events for the QuadDemoWindow widget.

  Flicker-free update. The figure is first drawn in the pixmap and the
  pixmap is then blt'ed to the screen.
*/

void QuadDemoWindow::paintEvent( QPaintEvent * )
{
  // Create the pixmap and fill it with the widget's background
#ifdef USING_PIXMAP_NOT_BITMAP
  QPixmap pm( size() );
  pm.fill( backgroundColor() );
#else
  QBitmap pm( size() );
#endif
  
  // Paint the pixmap
  QPainter p;
  p.begin( &pm );
  // paint the outside rectangle
  p.drawLine(db1.QCenterPt(),db2.QCenterPt());
  p.drawLine(db2.QCenterPt(),db3.QCenterPt());
  p.drawLine(db3.QCenterPt(),db4.QCenterPt());
  p.drawLine(db4.QCenterPt(),db1.QCenterPt());

  // paint the inscribed parallelogram
  p.drawLine(QPFromClP(mp1),QPFromClP(mp2));
  p.drawLine(QPFromClP(mp2),QPFromClP(mp3));
  p.drawLine(QPFromClP(mp3),QPFromClP(mp4));
  p.drawLine(QPFromClP(mp4),QPFromClP(mp1));

  // paint the control points
  db1.DrawMe(&p);
  db2.DrawMe(&p);
  db3.DrawMe(&p);
  db4.DrawMe(&p);
  
  p.end();

  // Copy the pixmap to the QuadDemoWindow widget
#ifdef USING_PIXMAP_NOT_BITMAP
  QPainter painter;
  painter.begin(this);
  painter.drawPixmap(0,0,pm);
  painter.end();
#else
  bitBlt( this, 0, 0, &pm );
#endif
}
