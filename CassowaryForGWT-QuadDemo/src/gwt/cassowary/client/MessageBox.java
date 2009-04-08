package gwt.cassowary.client;

import com.google.gwt.user.client.ui.Button;
import com.google.gwt.user.client.ui.ClickListener;
import com.google.gwt.user.client.ui.DialogBox;
import com.google.gwt.user.client.ui.TextArea;
import com.google.gwt.user.client.ui.VerticalPanel;
import com.google.gwt.user.client.ui.Widget;

public class MessageBox extends DialogBox {
   public MessageBox(String title, String text) {
      this.setText(title);

      Button ok = new Button("OK");
      ok.addClickListener(new ClickListener() {
         public void onClick(Widget sender) {
            hide();
         }
      });
      VerticalPanel v = new VerticalPanel();
      TextArea a = new TextArea();
      a.setSize("400px", "200px");
      a.setText(text);
      a.setReadOnly(true);
      v.add(a);
      v.add(ok);
      setWidget(v);
   }

   static void displayMessage(String title, String text) {
      new MessageBox(title, text).show();
   }

   static void displayWarning(String text) {
      displayMessage("Warning!", text);
   }
}
