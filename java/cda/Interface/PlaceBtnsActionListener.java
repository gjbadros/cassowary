/*
 * Mouse event handler for PlaceBtns panel
 *
 * $Id$
 *
 */

import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;

public final class PlaceBtnsActionListener implements ActionListener {

  // Reference to DrawPanel
  DrawPanel drawPanel;

  public void actionPerformed(ActionEvent e) {

    if ( e.getActionCommand().equals("P1") ) {
      drawPanel.createNewCC("LineCC");
      return;
    }
    if ( e.getActionCommand().equals("P2") ) {
      drawPanel.createNewCC("MidpointLineCC");
      return;
    }
    if ( e.getActionCommand().equals("P3") ) {
      drawPanel.createNewCC("RectangleCC");
      return;
    }
    if ( e.getActionCommand().equals("P4") ) {
      drawPanel.createNewCC("CircleCC");
      return;
    }
  }

  public PlaceBtnsActionListener(DrawPanel drawPanel) {
    this.drawPanel = drawPanel;
  }
}

/*
 * $Log$
 * Revision 1.4  1998/04/08 05:39:55  Michael
 * Removed printing msg
 *
 * Revision 1.3  1998/04/05 03:12:21  Michael
 * Added circle CC
 *
 * Revision 1.2  1998/03/19 09:18:50  Michael
 * Added hooks for rectangle CC
 *
 * Revision 1.1  1998/02/17 10:42:01  Michael
 * Split files with multiple classes
 *
 *
 */
