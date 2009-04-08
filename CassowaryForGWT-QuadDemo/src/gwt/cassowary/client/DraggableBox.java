package gwt.cassowary.client;

import gwt.cassowary.client.EDU.Washington.grad.gjb.*;
import gwt.canvas.client.*;

public class DraggableBox {
   ClPoint center;
   int     sx, sy; // Screen x, y of center
   int     width, height; // Dims of box

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
   public void draw(Canvas g) {
      cvt();
      g.strokeRect(sx - (width / 2), sy - (height / 2), width, height);
   }

   public void SetCenter(double x, double y) {
      center.SetXY(x, y);
      cvt();
   }

   public void SetSize(int width, int height) {
      this.width = width;
      this.height = height;
   }

   public double CenterX() {
      return center.Xvalue();
   }

   public double CenterY() {
      return center.Yvalue();
   }

   public ClVariable X() {
      return center.X();
   }

   public ClVariable Y() {
      return center.Y();
   }

   public ClPoint CenterPt() {
      return center;
   }

   public boolean Contains(int x, int y) {
      if ((x >= sx - width / 2) && (x <= sx + width / 2) && (y >= sy - height / 2) && (y <= sy + height / 2))
         return true;
      else
         return false;
   }

}
