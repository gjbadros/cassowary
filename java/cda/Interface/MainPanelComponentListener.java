/*
 * Component listener to pass along resize events to DrawPanel from MainPanel
 *
 * $Id$
 */

import java.awt.event.ComponentAdapter;
import java.awt.event.ComponentListener;
import java.awt.event.ComponentEvent;
import java.awt.Dimension;

// Component listener to pass along resize events to the DrawPanel
public class MainPanelComponentListener extends ComponentAdapter 
    implements ComponentListener {

  // Reference to MainPanel
  MainPanel mainPanel;
 
  // Constructor
  public MainPanelComponentListener (MainPanel mainPanel) {
    this.mainPanel = mainPanel;
  }

  public void componentResized(ComponentEvent e) {
/*
    System.out.println("MPCl.cR invoked, str = '" + e.paramString() + "'");
*/
    if ( mainPanel == null ) {
      System.out.println("MPCL.compRes: mainPanel == null!");
      return;
    }
    if ( mainPanel.getParent() == null ) {
      System.out.println("MPCL.compRes: mainPanel.getParent() == null!");
      return;
    }
    Dimension d = mainPanel.getParent().getSize();

    d.width -= (mainPanel.placeBtnsPanel.getSize().width + 10);

/*
    System.out.println("MPCl.cR requesting drawPanel dims ("
      + d.width + ", " + d.height + ")");
*/
    if ( d.width < 100 )
      d.width = 100;
    if ( d.height < 100 )
      d.height = 100;

    mainPanel.drawPanel.setDesiredSize(d.width - 4, d.height - 4);
  }
}

/*
 * $Log$
 * Revision 1.1  1998/05/09 00:11:15  gjb
 * Added
 *
 * Revision 1.4  1998/04/28 20:09:11  Michael
 * Removed debugging code
 *
 * Revision 1.3  1998/04/26 06:16:12  Michael
 * Changed layout
 *
 * Revision 1.2  1998/04/08 05:39:29  Michael
 * Changed default width of DrawPanel
 *
 * Revision 1.1  1998/02/17 10:42:01  Michael
 * Split files with multiple classes
 *
 *
 */
