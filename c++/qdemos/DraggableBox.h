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
// DraggableBox.h

#ifndef DraggableBox_H
#define DraggableBox_H

#include <qpainter.h>
#include <qrect.h>

#include "ClPoint.h"

class DraggableBox {
 public:
  DraggableBox(Number x = 0.0, Number y = 0.0, int width = 6, int height = 6) :
    _center(x,y),
    _height(height),
    _width(width)
    { }

  void DrawMe(QPainter *qpainter) const
    {
    qpainter->drawRect(QRectangle());
    }

  void SetCenter(Number x, Number y)
    {
    _center.X() = x;
    _center.Y() = y;
    }

  void SetSize(int width, int height)
    {
    _width = width;
    _height = height;
    }

  QRect QRectangle() const 
    {
    QRect qr =  QRect(CenterX()-_width/2, CenterY()-_height/2, _width, _height);
    return qr;
    }

  Number CenterX() const
    { return _center.Xvalue(); }

  Number CenterY() const
    { return _center.Yvalue(); }

  ClVariable &X() 
    { return _center.X(); }

  ClVariable &Y() 
    { return _center.Y(); }

  const ClVariable &X() const
    { return _center.X(); }

  const ClVariable &Y() const
    { return _center.Y(); }

  const ClPoint &CenterPt() const
    { return _center; }

  ClPoint &CenterPt()
    { return _center; }

  QPoint QCenterPt() const
    { return QPoint(CenterX(),CenterY()); }

  bool FContains(int x, int y) const 
    { return QRectangle().contains(QPoint(x,y)); }

  friend ostream &operator<<(ostream &xo, const DraggableBox &db);

 protected:
  ClPoint _center;
  int _height, _width;
  
};

inline ostream &
operator<<(ostream &xo, const DraggableBox &db)
{
  xo << db._center << "[" << db._width << ", " << db._height << "]";
  return xo;
}

#endif
