// $Id$
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// (C) 1998, All Rights Reserved.
//
// (c) 1998 Greg J. Badros and Alan Borning.
// See LICENSE file for terms of use.
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
    .AddPointStay(db[0].CenterPt(),weight *= 2)
    .AddPointStay(db[1].CenterPt(),weight *= 2)
    .AddPointStay(db[2].CenterPt(),weight *= 2)
    .AddPointStay(db[3].CenterPt(),weight *= 2)
    ;
    
  // keep midpoints at midpoints
  solver
    .AddConstraint(new ClLinearEquation(mp[0].X(), (db[0].X() + db[1].X())/2))
    .AddConstraint(new ClLinearEquation(mp[0].Y(), (db[0].Y() + db[1].Y())/2))
    .AddConstraint(new ClLinearEquation(mp[1].X(), (db[1].X() + db[2].X())/2))
    .AddConstraint(new ClLinearEquation(mp[1].Y(), (db[1].Y() + db[2].Y())/2))
    .AddConstraint(new ClLinearEquation(mp[2].X(), (db[2].X() + db[3].X())/2))
    .AddConstraint(new ClLinearEquation(mp[2].Y(), (db[2].Y() + db[3].Y())/2))
    .AddConstraint(new ClLinearEquation(mp[3].X(), (db[3].X() + db[0].X())/2))
    .AddConstraint(new ClLinearEquation(mp[3].Y(), (db[3].Y() + db[0].Y())/2))
    ;


  // keep from turning inside out
  solver
    .AddConstraint(new ClLinearInequality(db[0].X() + 10, cnLEQ, db[2].X()))
    .AddConstraint(new ClLinearInequality(db[0].X() + 10, cnLEQ, db[3].X()))
    .AddConstraint(new ClLinearInequality(db[1].X() + 10, cnLEQ, db[2].X()))
    .AddConstraint(new ClLinearInequality(db[1].X() + 10, cnLEQ, db[3].X()))

    .AddConstraint(new ClLinearInequality(db[0].Y() + 10, cnLEQ, db[1].Y()))
    .AddConstraint(new ClLinearInequality(db[0].Y() + 10, cnLEQ, db[2].Y()))
    .AddConstraint(new ClLinearInequality(db[3].Y() + 10, cnLEQ, db[1].Y()))
    .AddConstraint(new ClLinearInequality(db[3].Y() + 10, cnLEQ, db[2].Y()))
    ;

  int MaxX = 450;
  int MaxY = 450;
  solver
    .AddConstraint(new ClLinearInequality(db[0].X(), cnGEQ, 0))
    .AddConstraint(new ClLinearInequality(db[1].X(), cnGEQ, 0))
    .AddConstraint(new ClLinearInequality(db[2].X(), cnGEQ, 0))
    .AddConstraint(new ClLinearInequality(db[3].X(), cnGEQ, 0))
    .AddConstraint(new ClLinearInequality(db[0].Y(), cnGEQ, 0))
    .AddConstraint(new ClLinearInequality(db[1].Y(), cnGEQ, 0))
    .AddConstraint(new ClLinearInequality(db[2].Y(), cnGEQ, 0))
    .AddConstraint(new ClLinearInequality(db[3].Y(), cnGEQ, 0))
    .AddConstraint(new ClLinearInequality(db[0].X(), cnLEQ, MaxX))
    .AddConstraint(new ClLinearInequality(db[1].X(), cnLEQ, MaxX))
    .AddConstraint(new ClLinearInequality(db[2].X(), cnLEQ, MaxX))
    .AddConstraint(new ClLinearInequality(db[3].X(), cnLEQ, MaxX))
    .AddConstraint(new ClLinearInequality(db[0].Y(), cnLEQ, MaxY))
    .AddConstraint(new ClLinearInequality(db[1].Y(), cnLEQ, MaxY))
    .AddConstraint(new ClLinearInequality(db[2].Y(), cnLEQ, MaxY))
    .AddConstraint(new ClLinearInequality(db[3].Y(), cnLEQ, MaxY));
    
}

QuadDemoWindow::~QuadDemoWindow()
{
  solver.RemoveAllEditVars();
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
      .AddEditVar(db[idbDragging].X())
      .AddEditVar(db[idbDragging].Y())
      .BeginEdit();
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
          .EndEdit();
        repaint();
        }
#ifdef QUAD_DEMO_SHOW_INTERNAL_INFO
      solver.PrintInternalInfo(cerr);
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
      .SuggestValue(db[idbDragging].X(),e->x())
      .SuggestValue(db[idbDragging].Y(),e->y())
      .Resolve();

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
