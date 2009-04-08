package gwt.cassowary.client;

import com.google.gwt.core.client.EntryPoint;
import com.google.gwt.user.client.ui.RootPanel;

/**
 * Entry point classes define <code>onModuleLoad()</code>.
 */
public class QuadDemo implements EntryPoint {
   QuadDemoCanvas canvas;

   /**
    * This is the entry point method.
    */

   public void onModuleLoad() {
      canvas = new QuadDemoCanvas(400, 400); // Make a new canvas 400x400 pixels
      RootPanel.get().add(canvas);
   }
}
