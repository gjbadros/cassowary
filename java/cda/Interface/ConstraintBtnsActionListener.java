/*
 * Mouse event handler for ConstraintBtns panel
 *
 * $Id$
 *
 */

import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;

public final class ConstraintBtnsActionListener implements ActionListener {

  // Reference to DrawPanel
  DrawPanel drawPanel;

  public void actionPerformed(ActionEvent e) {

    if ( e.getActionCommand().equals("C1") ) {
      // Create a new anchor constraint
      drawPanel.createNewConstraint("Anchor");
      return;
    }

    if ( e.getActionCommand().equals("C2") ) {
      // Create a new colocation constraint
      drawPanel.createNewConstraint("Colocation");
      return;
    }
    if ( e.getActionCommand().equals("C3") ) {
      // Create a new Above constraint
      drawPanel.createNewConstraint("Above");
      return;
    }
    if ( e.getActionCommand().equals("C4") ) {
      // Create a new Below constraint
      drawPanel.createNewConstraint("Below");
      return;
    }
    if ( e.getActionCommand().equals("C5") ) {
      // Create a new LeftOf constraint
      drawPanel.createNewConstraint("LeftOf");
      return;
    }
    if ( e.getActionCommand().equals("C6") ) {
      // Create a new RightOf constraint
      drawPanel.createNewConstraint("RightOf");
      return;
    }
    if ( e.getActionCommand().equals("C7") ) {
      // Create a new LeftAlign constraint
      drawPanel.createNewConstraint("LeftAlign");
      return;
    }
    if ( e.getActionCommand().equals("C8") ) {
      // Create a new TopAlign constraint
      drawPanel.createNewConstraint("TopAlign");
      return;
    }
    if ( e.getActionCommand().equals("C9") ) {
      // Create a new RightAlign constraint
      drawPanel.createNewConstraint("RightAlign");
      return;
    }
    if ( e.getActionCommand().equals("C10") ) {
      // Create a new BottomAlign constraint
      drawPanel.createNewConstraint("BottomAlign");
      return;
    }
  }

  public ConstraintBtnsActionListener(DrawPanel drawPanel) {
    this.drawPanel = drawPanel;
  }
}

/*
 * $Log$
 * Revision 1.5  1998/04/23 09:00:55  Michael
 * Added alignment constraint buttons
 *
 * Revision 1.4  1998/04/08 05:40:14  Michael
 * Removed printing msg
 *
 * Revision 1.3  1998/04/02 06:59:56  Michael
 * Added relational constraints
 *
 * Revision 1.2  1998/03/27 06:23:34  Michael
 * Added new buttons for relational constraints
 *
 * Revision 1.1  1998/02/17 10:42:00  Michael
 * Split files with multiple classes
 *
 *
 */
