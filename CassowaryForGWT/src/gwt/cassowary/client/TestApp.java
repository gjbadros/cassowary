package gwt.cassowary.client;

import gwt.cassowary.client.EDU.Washington.grad.gjb.ExCLConstraintNotFound;
import gwt.cassowary.client.EDU.Washington.grad.gjb.ExCLError;
import gwt.cassowary.client.EDU.Washington.grad.gjb.ExCLInternalError;
import gwt.cassowary.client.EDU.Washington.grad.gjb.ExCLNonlinearExpression;
import gwt.cassowary.client.EDU.Washington.grad.gjb.ExCLRequiredFailure;

import com.google.gwt.core.client.EntryPoint;
import com.google.gwt.user.client.ui.Button;
import com.google.gwt.user.client.ui.ClickListener; //import com.google.gwt.user.client.ui.DialogBox;
//import com.google.gwt.user.client.ui.Image;
import com.google.gwt.user.client.ui.*;

/**
 * Entry point classes define <code>onModuleLoad()</code>.
 */
public class TestApp implements EntryPoint {
   /**
    * This is the entry point method.
    */
   static private TextArea textArea;

   static public void println(String nextLine) {
      textArea.setText(textArea.getText() + nextLine + "\n");
   }

   static public void clearTextArea() {
      textArea.setText("");
   }

   public void onModuleLoad() {
      // Image img = new
      // Image("http://code.google.com/webtoolkit/logo-185x175.png");
      Button button = new Button("Run Cassowary Test");

      // We can add style names
      button.addStyleName("pc-template-btn");
      // or we can set an id on a specific element for styling
      // img.getElement().setId("pc-template-img");

      VerticalPanel vPanel = new VerticalPanel();
      vPanel.setWidth("100%");
      vPanel.setHorizontalAlignment(VerticalPanel.ALIGN_CENTER);
      // vPanel.add(img);
      vPanel.add(button);

      textArea = new TextArea();
      textArea.setCharacterWidth(80);
      textArea.setVisibleLines(30);

      vPanel.add(textArea);
      // ScrollPanel scrollPanel = new ScrollPanel(textArea);
      // scrollPanel.setTitle("Test Results");
      // vPanel.add(scrollPanel);
      // Add image and button to the RootPanel
      RootPanel.get().add(vPanel);

      button.addClickListener(new ClickListener() {
         public void onClick(Widget sender) {
            clearTextArea();
            try {
               ClTests.gwtTest(new String[] { "400", "400", "1000" });
            }
            catch (ExCLInternalError e) {
               // TODO Auto-generated catch block
               e.printStackTrace();
            }
            catch (ExCLNonlinearExpression e) {
               // TODO Auto-generated catch block
               e.printStackTrace();
            }
            catch (ExCLRequiredFailure e) {
               // TODO Auto-generated catch block
               e.printStackTrace();
            }
            catch (ExCLConstraintNotFound e) {
               // TODO Auto-generated catch block
               e.printStackTrace();
            }
            catch (ExCLError e) {
               // TODO Auto-generated catch block
               e.printStackTrace();
            }
         }
      });
   }
}
