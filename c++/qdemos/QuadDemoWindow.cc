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
    mp(db+4), // mid-point draggable boxes are aliased with db[4..7]
    idbDragging(-1)
{
  resize( 450, 450 );
  setBackgroundMode(NoBackground);

  db[0].SetCenter(5,5);
  db[1].SetCenter(5,200);
  db[2].SetCenter(200,200);
  db[3].SetCenter(200,5);
       
 double weight = .5 ;
  solver
    .addPointStay(db[0].CenterPt(),weight *= 2)
    .addPointStay(db[1].CenterPt(),weight *= 2)
    .addPointStay(db[2].CenterPt(),weight *= 2)
    .addPointStay(db[3].CenterPt(),weight *= 2)
    ;
    
  // keep midpoints at midpoints
  solver
    .addConstraint(new ClLinearEquation(mp[0].X(), (db[0].X() + db[1].X())/2))
    .addConstraint(new ClLinearEquation(mp[0].Y(), (db[0].Y() + db[1].Y())/2))
    .addConstraint(new ClLinearEquation(mp[1].X(), (db[1].X() + db[2].X())/2))
    .addConstraint(new ClLinearEquation(mp[1].Y(), (db[1].Y() + db[2].Y())/2))
    .addConstraint(new ClLinearEquation(mp[2].X(), (db[2].X() + db[3].X())/2))
    .addConstraint(new ClLinearEquation(mp[2].Y(), (db[2].Y() + db[3].Y())/2))
    .addConstraint(new ClLinearEquation(mp[3].X(), (db[3].X() + db[0].X())/2))
    .addConstraint(new ClLinearEquation(mp[3].Y(), (db[3].Y() + db[0].Y())/2))
    ;


  // keep from turning inside out
  solver
    .addConstraint(new ClLinearInequality(db[0].X() + 10, cnLEQ, db[2].X()))
    .addConstraint(new ClLinearInequality(db[0].X() + 10, cnLEQ, db[3].X()))
    .addConstraint(new ClLinearInequality(db[1].X() + 10, cnLEQ, db[2].X()))
    .addConstraint(new ClLinearInequality(db[1].X() + 10, cnLEQ, db[3].X()))

    .addConstraint(new ClLinearInequality(db[0].Y() + 10, cnLEQ, db[1].Y()))
    .addConstraint(new ClLinearInequality(db[0].Y() + 10, cnLEQ, db[2].Y()))
    .addConstraint(new ClLinearInequality(db[3].Y() + 10, cnLEQ, db[1].Y()))
    .addConstraint(new ClLinearInequality(db[3].Y() + 10, cnLEQ, db[2].Y()))
    ;

  int MaxX = 450;
  int MaxY = 450;
  solver
    .addConstraint(new ClLinearInequality(db[0].X(), cnGEQ, 0))
    .addConstraint(new ClLinearInequality(db[1].X(), cnGEQ, 0))
    .addConstraint(new ClLinearInequality(db[2].X(), cnGEQ, 0))
    .addConstraint(new ClLinearInequality(db[3].X(), cnGEQ, 0))
    .addConstraint(new ClLinearInequality(db[0].Y(), cnGEQ, 0))
    .addConstraint(new ClLinearInequality(db[1].Y(), cnGEQ, 0))
    .addConstraint(new ClLinearInequality(db[2].Y(), cnGEQ, 0))
    .addConstraint(new ClLinearInequality(db[3].Y(), cnGEQ, 0))
    .addConstraint(new ClLinearInequality(db[0].X(), cnLEQ, MaxX))
    .addConstraint(new ClLinearInequality(db[1].X(), cnLEQ, MaxX))
    .addConstraint(new ClLinearInequality(db[2].X(), cnLEQ, MaxX))
    .addConstraint(new ClLinearInequality(db[3].X(), cnLEQ, MaxX))
    .addConstraint(new ClLinearInequality(db[0].Y(), cnLEQ, MaxY))
    .addConstraint(new ClLinearInequality(db[1].Y(), cnLEQ, MaxY))
    .addConstraint(new ClLinearInequality(db[2].Y(), cnLEQ, MaxY))
    .addConstraint(new ClLinearInequality(db[3].Y(), cnLEQ, MaxY));
    
}

QuadDemoWindow::~QuadDemoWindow()
{
  solver.removeAllEditVars();
}

void QuadDemoWindow::mousePressEvent( QMouseEvent *e )
{
  int x = e->x();
  int y = e->y();

  // Already got edit constraints?
  if (solver.numEditVars() > 0)
    return;
  
  for (int i = 0; i < cdb; i++)
    {
    if (db[i].FContains(x,y))
      {
      idbDragging = i;
      break;
      }
    }
  if (idbDragging != -1)
    {
    imbUsed = e->button();
    solver
      .addEditVar(db[idbDragging].X())
      .addEditVar(db[idbDragging].Y())
      .beginEdit();
    }
}

void QuadDemoWindow::mouseReleaseEvent( QMouseEvent *e )
{
  if (idbDragging != -1)
    {
    try
      {
      if (e->button() == imbUsed)
        {
        // Only remove the constraints if the button
        // that we initiated the drag with is released
        idbDragging = -1;
        solver
          .endEdit();
        repaint();
        }
#ifdef QUAD_DEMO_SHOW_INTERNAL_INFO
      solver.printInternalInfo(cerr);
      cerr << endl;
#endif
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
    solver
      .suggestValue(db[idbDragging].X(),e->x())
      .suggestValue(db[idbDragging].Y(),e->y())
      .resolve();

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
  pm.fill( backgroundColor() );
#endif
  
  // Paint the pixmap
  QPainter p;
  p.begin( &pm );
  // paint the outside rectangle
  p.drawLine(db[0].QCenterPt(),db[1].QCenterPt());
  p.drawLine(db[1].QCenterPt(),db[2].QCenterPt());
  p.drawLine(db[2].QCenterPt(),db[3].QCenterPt());
  p.drawLine(db[3].QCenterPt(),db[0].QCenterPt());

  // paint the inscribed parallelogram
  p.drawLine(mp[0].QCenterPt(),mp[1].QCenterPt());
  p.drawLine(mp[1].QCenterPt(),mp[2].QCenterPt());
  p.drawLine(mp[2].QCenterPt(),mp[3].QCenterPt());
  p.drawLine(mp[3].QCenterPt(),mp[0].QCenterPt());

  // paint the control points
  for (int i=0; i < cdb; i++)
    db[i].DrawMe(&p);
  
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
