package gwt.cassowary.client;

import com.google.gwt.user.client.Event;

import gwt.canvas.client.*;

import gwt.cassowary.client.EDU.Washington.grad.gjb.*;

public class QuadDemoCanvas extends Canvas {
   DraggableBox    db[];      // line endpoints
   DraggableBox    mp[];      // midpoints
   int             dbDragging; // the db is being, -1 for none
   ClSimplexSolver solver;

   public QuadDemoCanvas() {
      super();
      init();
   }

   public QuadDemoCanvas(int width, int height) {
      super(width, height);
      init();
   }

   public void init() {

      solver = new ClSimplexSolver();
      dbDragging = -1;

      db = new DraggableBox[8];
      mp = new DraggableBox[4];

      int a;

      for (a = 0; a < 8; a++)
         db[a] = new DraggableBox(a);

      for (a = 0; a < 4; a++)
         mp[a] = db[a + 4];

      db[0].SetCenter(5, 5);
      db[1].SetCenter(5, 200);
      db[2].SetCenter(200, 200);
      db[3].SetCenter(200, 5);

      // Add constraints
      try {
         // Add stay constraints on line endpoints
         solver.addPointStay(db[0].CenterPt(), 1.0);
         solver.addPointStay(db[1].CenterPt(), 2.0);
         solver.addPointStay(db[2].CenterPt(), 4.0);
         solver.addPointStay(db[3].CenterPt(), 8.0);

         ClLinearExpression cle;
         ClLinearEquation cleq;

         // Add constraints to keep midpoints at line midpoints
         cle = new ClLinearExpression(db[0].X());
         cle = (cle.plus(db[1].X())).divide(2);
         cleq = new ClLinearEquation(mp[0].X(), cle);
         // System.out.println("Adding " + cleq);
         solver.addConstraint(cleq);
         cle = new ClLinearExpression(db[0].Y());
         cle = (cle.plus(db[1].Y())).divide(2);
         cleq = new ClLinearEquation(mp[0].Y(), cle);
         // System.out.println("Adding " + cleq);
         solver.addConstraint(cleq);

         cle = new ClLinearExpression(db[1].X());
         cle = (cle.plus(db[2].X())).divide(2);
         cleq = new ClLinearEquation(mp[1].X(), cle);
         // System.out.println("Adding " + cleq);
         solver.addConstraint(cleq);
         cle = new ClLinearExpression(db[1].Y());
         cle = (cle.plus(db[2].Y())).divide(2);
         cleq = new ClLinearEquation(mp[1].Y(), cle);
         // System.out.println("Adding " + cleq);
         solver.addConstraint(cleq);

         cle = new ClLinearExpression(db[2].X());
         cle = (cle.plus(db[3].X())).divide(2);
         cleq = new ClLinearEquation(mp[2].X(), cle);
         // System.out.println("Adding " + cleq);
         solver.addConstraint(cleq);
         cle = new ClLinearExpression(db[2].Y());
         cle = (cle.plus(db[3].Y())).divide(2);
         cleq = new ClLinearEquation(mp[2].Y(), cle);
         // System.out.println("Adding " + cleq);
         solver.addConstraint(cleq);

         cle = new ClLinearExpression(db[3].X());
         cle = (cle.plus(db[0].X())).divide(2);
         cleq = new ClLinearEquation(mp[3].X(), cle);
         // System.out.println("Adding " + cleq);
         solver.addConstraint(cleq);
         cle = new ClLinearExpression(db[3].Y());
         cle = (cle.plus(db[0].Y())).divide(2);
         cleq = new ClLinearEquation(mp[3].Y(), cle);
         // System.out.println("Adding " + cleq);
         solver.addConstraint(cleq);

         cle = CL.Plus(db[0].X(), 10);
         solver.addConstraint(new ClLinearInequality(cle, CL.Op.LEQ, db[2].X())).addConstraint(
               new ClLinearInequality(cle, CL.Op.LEQ, db[3].X()));

         cle = CL.Plus(db[1].X(), 10);
         solver.addConstraint(new ClLinearInequality(cle, CL.Op.LEQ, db[2].X())).addConstraint(
               new ClLinearInequality(cle, CL.Op.LEQ, db[3].X()));

         cle = CL.Plus(db[0].Y(), 10);
         solver.addConstraint(new ClLinearInequality(cle, CL.Op.LEQ, db[1].Y())).addConstraint(
               new ClLinearInequality(cle, CL.Op.LEQ, db[2].Y()));

         cle = CL.Plus(db[3].Y(), 10);
         solver.addConstraint(new ClLinearInequality(cle, CL.Op.LEQ, db[1].Y())).addConstraint(
               new ClLinearInequality(cle, CL.Op.LEQ, db[2].Y()));

         int width = this.getWidth();
         int height = this.getHeight();

         // Add constraints to keep points inside window
         solver.addConstraint(new ClLinearInequality(db[0].X(), CL.Op.GEQ, 0.0));
         solver.addConstraint(new ClLinearInequality(db[0].Y(), CL.Op.GEQ, 0.0));
         solver.addConstraint(new ClLinearInequality(db[1].X(), CL.Op.GEQ, 0.0));
         solver.addConstraint(new ClLinearInequality(db[1].Y(), CL.Op.GEQ, 0.0));
         solver.addConstraint(new ClLinearInequality(db[2].X(), CL.Op.GEQ, 0.0));
         solver.addConstraint(new ClLinearInequality(db[2].Y(), CL.Op.GEQ, 0.0));
         solver.addConstraint(new ClLinearInequality(db[3].X(), CL.Op.GEQ, 0.0));
         solver.addConstraint(new ClLinearInequality(db[3].Y(), CL.Op.GEQ, 0.0));

         solver.addConstraint(new ClLinearInequality(db[0].X(), CL.Op.LEQ, width));
         solver.addConstraint(new ClLinearInequality(db[0].Y(), CL.Op.LEQ, height));
         solver.addConstraint(new ClLinearInequality(db[1].X(), CL.Op.LEQ, width));
         solver.addConstraint(new ClLinearInequality(db[1].Y(), CL.Op.LEQ, height));
         solver.addConstraint(new ClLinearInequality(db[2].X(), CL.Op.LEQ, width));
         solver.addConstraint(new ClLinearInequality(db[2].Y(), CL.Op.LEQ, height));
         solver.addConstraint(new ClLinearInequality(db[3].X(), CL.Op.LEQ, width));
         solver.addConstraint(new ClLinearInequality(db[3].Y(), CL.Op.LEQ, height));

      }
      catch (ExCLInternalError e) {
         MessageBox.displayWarning("constructor: CLInternalError!");
      }
      catch (ExCLRequiredFailure e) {
         MessageBox.displayWarning("constructor: CLRequiredFailure!");
      }
      catch (ExCLNonlinearExpression e) {
         MessageBox.displayWarning("constructor: CLNonlinearExpression!");
      }

      paint();
      sinkEvents(Event.MOUSEEVENTS); // start listening to mouse events
   }

   // Event handlers

   private boolean draggingMouse = false;

   @Override
   public void onBrowserEvent(Event event) {
      super.onBrowserEvent(event);

      int x = event.getClientX() - getAbsoluteLeft();
      int y = event.getClientY() - getAbsoluteTop();

      switch (event.getTypeInt()) {
         case Event.ONMOUSEDOWN:
            mouseDown(event, x, y);
            draggingMouse = true;
            break;

         case Event.ONMOUSEMOVE:
            if (draggingMouse)
               mouseDrag(event, x, y);
            break;

         case Event.ONMOUSEUP:
            draggingMouse = false;
            mouseUp(event, x, y);
            break;
      }
   }

   // Event handlers
   public void mouseDown(Event e, int x, int y) {
      for (int a = 0; a < db.length; a++) {
         if (db[a].Contains(x, y)) {
            dbDragging = a;
            break;
         }
      }
      paint();
      if (dbDragging != -1) {
         try {
            solver.addEditVar(db[dbDragging].X()).addEditVar(db[dbDragging].Y()).beginEdit();
         }
         catch (ExCLInternalError ex) {
            MessageBox.displayWarning("mouseDown: CLInternalError!\n" + ex.description());
         }
      }
   }

   public void mouseUp(Event e, int x, int y) {
      if (dbDragging != -1) {
         try {
            dbDragging = -1;
            solver.endEdit();
            paint();
         }
         catch (ExCLInternalError ex) {
            MessageBox.displayWarning("mouseUp: CLInternalError!\n" + ex.description());
         }
      }
   }

   public void mouseDrag(Event e, int x, int y) {
      if (dbDragging != -1) {
         try {
            solver.suggestValue(db[dbDragging].X(), x).suggestValue(db[dbDragging].Y(), y).resolve();
            paint();
         }
         catch (ExCLInternalError ex) {
            MessageBox.displayWarning("mouseDrag: CLInternalError!");
         }
         catch (ExCLError ex) {
            MessageBox.displayWarning("mouseDrag: CLError!");
         }
      }
   }

   // Paint the display
   public void drawLine(int x1, int y1, int x2, int y2) {
      beginPath();
      moveTo(x1, y1);
      lineTo(x2, y2);
      closePath();
      this.stroke();
   }

   public void paint() {
      this.setLineWidth(1);
      this.clear();
      drawLine((int) db[0].CenterX(), (int) db[0].CenterY(), (int) db[1].CenterX(), (int) db[1].CenterY());
      drawLine((int) db[1].CenterX(), (int) db[1].CenterY(), (int) db[2].CenterX(), (int) db[2].CenterY());
      drawLine((int) db[2].CenterX(), (int) db[2].CenterY(), (int) db[3].CenterX(), (int) db[3].CenterY());
      drawLine((int) db[3].CenterX(), (int) db[3].CenterY(), (int) db[0].CenterX(), (int) db[0].CenterY());
      // Connecting lines
      drawLine((int) mp[0].CenterX(), (int) mp[0].CenterY(), (int) mp[1].CenterX(), (int) mp[1].CenterY());
      drawLine((int) mp[1].CenterX(), (int) mp[1].CenterY(), (int) mp[2].CenterX(), (int) mp[2].CenterY());
      drawLine((int) mp[2].CenterX(), (int) mp[2].CenterY(), (int) mp[3].CenterX(), (int) mp[3].CenterY());
      drawLine((int) mp[3].CenterX(), (int) mp[3].CenterY(), (int) mp[0].CenterX(), (int) mp[0].CenterY());

      // Control points
      for (int a = 0; a < 8; a++)
         db[a].draw(this);
      if (dbDragging != -1) {
         save();
         this.setStrokeStyle("#ff0000");
         db[dbDragging].draw(this);
         restore();
      }
   }
}
