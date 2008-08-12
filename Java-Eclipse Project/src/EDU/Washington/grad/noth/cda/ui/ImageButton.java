/*
 * ImageButton class.  Class for a button that contains an image.
 *
 * $Id$
 */

package EDU.Washington.grad.noth.cda.ui;

import java.awt.*;
import java.awt.image.ImageProducer;
import java.applet.Applet; // import java.net.URL;
import java.io.IOException;
import java.net.URL;

public class ImageButton extends Button {
    private static final long serialVersionUID = 1L;
    // Image that appears on button
    Image img;
    // Image name for button icon
    String imgName;
    // Parent applet of button, used for some info lookup
    Component parentApp;
    // Observer that monitors image load state
    ImageButtonImageObserver ibio;
    // Background color of icon (used for transparency)
    Color bgColor;

    // Constructor that specifies a label only
    public ImageButton(String label) {
        super(label);
        setLabel(null);
        setActionCommand(label);
        img = null;
        imgName = null;
        parentApp = null;
    }

    // Constructor that specifies a label and name of image; image is
    // actually loaded later
    public ImageButton(String label, String imgName) {
        super(label);
        setLabel(null);
        setActionCommand(label);
        img = null;
        this.imgName = imgName;
        parentApp = null;
    }

    // Helper function to ensure that image is loaded and ready to go
    // Assume that the button is sitting inside an Applet of some sort,
    // from which the document base can be retrieved. Image name is
    // treated as a name relative the base.
    public void setImg() {
        parentApp = this.getParent();
        while (!(parentApp instanceof Applet)) {
            parentApp = parentApp.getParent();
        }
        // Retrieve the image
        String fullImgName = new String(
                "/EDU/Washington/grad/noth/cda/ui/images/" + imgName + ".gif");
        System.out.println("Loading image '" + fullImgName + "'");

        URL url = ((Applet) parentApp).getClass().getResource(fullImgName);
        try {
            img = ((Applet) parentApp).createImage((ImageProducer) url
                    .getContent());
        } catch (IOException e) {
            e.printStackTrace();
        }

        ibio = new ImageButtonImageObserver(this);

        // Set background color to light grey
        bgColor = new Color(192, 192, 192);
    }

    // Update function, invoked by repaint()
    public void update(Graphics g) {
        if (img == null) {
            setImg();
        }
        paint(g);

        /*
         * // boolean dIResult = g.drawImage(img, 1, 1, bgColor, ibio); boolean
         * dIResult = g.drawImage(img, 1, 1, ibio);
         */
    }

    // Paint function. Just draw the image.
    public void paint(Graphics g) {
        /*
         * System.out.println("IB.paint: Drawing image " + img);
         */
        if (img == null) {
            setImg();
        }
        g.drawImage(img, 1, 1, bgColor, ibio);
    }

    public Dimension getPreferredSize() {
        return new Dimension(36, 36);
    }
}

/*
 * $Log$
 * Revision 1.1  2008/08/12 21:31:38  larrymelia
 * Updated Java sources for Java 1.5+ (Larry Melia)
 * Revision 1.3 1999/12/16 02:10:02 gjb
 * java/cda/Makefile.am: Put Constraint/, Main/ files into the distribution and
 * build with them. java/demos/.java: Move everything into the
 * EDU.Washington.grad.gjb.cassowary_demos package. java/cda/classes/run.html,
 * java/demos/quaddemo.htm: Fix nl convention, name class explicitly w/ package
 * in front, w/o trailing .class. java/cda/: Move everything into the
 * EDU.Washington.grad.noth.cda package.
 * 
 * Revision 1.2 1998/05/09 00:30:34 gjb Remove cr-s
 * 
 * Revision 1.1 1998/05/09 00:11:14 gjb Added
 * 
 * Revision 1.9 1998/04/23 09:01:07 Michael Removed debugging msg
 * 
 * Revision 1.8 1998/04/08 05:39:01 Michael Removed printing msgs
 * 
 * Revision 1.7 1998/03/27 06:23:57 Michael Set label of image buttons to none
 * to prevent annoying flicker
 * 
 * Revision 1.6 1998/02/22 05:00:01 Michael Removed debugging print statements
 * 
 * Revision 1.5 1998/02/17 10:43:29 Michael Removed extracted code
 * 
 * Revision 1.4 1998/02/17 10:42:00 Michael Split files with multiple classes
 * 
 * Revision 1.3 1998/02/17 08:25:31 Michael Moved button icon over 1 pixel
 * 
 * Revision 1.2 1998/02/16 10:32:42 Michael Commented out some debugging stmts
 * 
 * Revision 1.1 1998/02/12 07:55:02 Michael ImageButton helper class, first
 * check-in
 */
