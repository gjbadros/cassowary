/*
 * Image observer that forces an image repaint once image is available
 *
 * $Id$
 */

import java.awt.image.ImageObserver;
import java.awt.Image;

// Image observer that handles forcing a repaint once image is available
public class ImageButtonImageObserver implements ImageObserver {
 
  // Reference to the ImageButton
  ImageButton imageButton;

  public ImageButtonImageObserver(ImageButton imageButton) {
    this.imageButton = imageButton;
  }

  public boolean imageUpdate(Image img, int infoflags, int x, int y, 
    int width, int height) {
    if ( (infoflags & ALLBITS) != 0 ) {
      // Finished retrieving the image, so draw it
      // System.out.println("IBIO.iU: ALLBITS true");
      imageButton.repaint();
      return false;
    }
    return true;
  }
}

/*
 * $Log$
 * Revision 1.1  1998/05/09 00:11:15  gjb
 * Added
 *
 * Revision 1.1  1998/02/17 10:42:00  Michael
 * Split files with multiple classes
 *
 *
 */
