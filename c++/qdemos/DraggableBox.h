// $Id$
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
    center.Xvar() = x;
    center.Yvar() = y;
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
    { return center.X(); }

  Number CenterY() const
    { return center.Y(); }

  ClVariable &Xvar() 
    { return center.Xvar(); }

  ClVariable &Yvar() 
    { return center.Yvar(); }

  const ClVariable &Xvar() const
    { return center.Xvar(); }

  const ClVariable &Yvar() const
    { return center.Yvar(); }

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
