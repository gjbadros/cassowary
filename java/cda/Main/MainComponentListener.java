/*
 * Component listener to pass resize events from Main to MainPanel
 *
 * $Id$
 */

import java.awt.event.ComponentAdapter;
import java.awt.event.ComponentEvent;

public class MainComponentListener extends ComponentAdapter {

  // Reference to main panel
  MainPanel mainPanel;

  public MainComponentListener(MainPanel mainPanel) {
    this.mainPanel = mainPanel;
  }

  public void componentResized(ComponentEvent e) {
/*
    System.out.println("Main.cR invoked, str= " + e.paramString() + "'");
*/
    mainPanel.dispatchEvent(e);
  }
}

/*
 * $Log$
 * Revision 1.1  1998/05/09 00:11:18  gjb
 * Added
 *
 * Revision 1.2  1998/04/28 20:08:26  Michael
 * Removed debugging code
 *
 * Revision 1.1  1998/02/17 10:42:13  Michael
 * Split files with multiple classes
 *
 *
 */
