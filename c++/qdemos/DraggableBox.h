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
  DraggableBox(Number x = 0.0, Number y = 0.0, int width_ = 6, int height_ = 6) :
    center(x,y),
    height(height_),
    width(width_)
    { }

  void DrawMe(QPainter *qpainter) const
    {
    qpainter->drawRect(QRectangle());
    }

  void SetCenter(Number x, Number y)
    {
    center.X() = x;
    center.Y() = y;
    }

  void SetSize(int width_, int height_)
    {
    width = width_;
    height = height_;
    }

  QRect QRectangle() const 
    {
    QRect qr =  QRect(CenterX()-width/2, CenterY()-height/2, width, height);
    return qr;
    }

  Number CenterX() const
    { return center.Xvalue(); }

  Number CenterY() const
    { return center.Yvalue(); }

  ClVariable &X() 
    { return center.X(); }

  ClVariable &Y() 
    { return center.Y(); }

  const ClVariable &X() const
    { return center.X(); }

  const ClVariable &Y() const
    { return center.Y(); }

  const ClPoint &CenterPt() const
    { return center; }

  ClPoint &CenterPt()
    { return center; }

  QPoint QCenterPt() const
    { return QPoint(CenterX(),CenterY()); }

  bool FContains(int x, int y) const 
    { return QRectangle().contains(QPoint(x,y)); }

  friend ostream &operator<<(ostream &xo, const DraggableBox &db);

 protected:
  ClPoint center;
  int height, width;
  
};

inline ostream &
operator<<(ostream &xo, const DraggableBox &db)
{
  xo << db.center << "[" << db.width << ", " << db.height << "]";
  return xo;
}

#endif
