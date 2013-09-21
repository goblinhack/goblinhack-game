package net.sourceforge.goblinHack.client;

import com.google.gwt.core.client.EntryPoint;
import com.google.gwt.user.client.ui.*;
import com.google.gwt.user.client.DOM;

/**
 * Entry point classes define <code>onModuleLoad()</code>.
 */
public class goblinHack implements EntryPoint {

  /**
   * This is the entry point method.
   */
  public void onModuleLoad() {
    final Button button = new Button("Click me");
    final Label label = new Label();
    
    resizeableTable rs = new resizeableTable();

    button.addClickListener(new ClickListener() {
      public void onClick(Widget sender) {
        if (label.getText().equals(""))
          label.setText("Hello World!");
        else
          label.setText("");
      }
    });

    globalDS.gh_cols.put(" ", "#000000");
    globalDS.gh_cols.put("w", "#F6F6F6");
    globalDS.gh_cols.put("r", "#FF0000");
    globalDS.gh_cols.put("g", "#00FF00");
    globalDS.gh_cols.put("y", "#FFFF00");
    globalDS.gh_cols.put("b", "#0000FF");
    globalDS.gh_cols.put("p", "#FF9999");
    globalDS.gh_cols.put("c", "#00FFFF");
    globalDS.gh_cols.put("_", "#999999");
    globalDS.gh_cols.put("W", "#FFFFFF");
    globalDS.gh_cols.put("o", "#FFcc99");
    globalDS.gh_cols.put("G", "#99FF99");
    globalDS.gh_cols.put("Y", "#FFFF99");
    globalDS.gh_cols.put("B", "#9999FF");
    globalDS.gh_cols.put("P", "#FF00FF");
    globalDS.gh_cols.put("C", "#99FFFF");
    globalDS.gh_cols.put("O", "#FFddaa");
    globalDS.gh_cols.put("0", "#141414");
    globalDS.gh_cols.put("1", "#282828");
    globalDS.gh_cols.put("2", "#3C3C3C");
    globalDS.gh_cols.put("3", "#505050");
    globalDS.gh_cols.put("4", "#646464");
    globalDS.gh_cols.put("5", "#787878");
    globalDS.gh_cols.put("6", "#8b8b8b");
    globalDS.gh_cols.put("7", "#9f9f9f");
    globalDS.gh_cols.put("8", "#C0C0C0");
    globalDS.gh_cols.put("9", "#E0E0E0");
    globalDS.gh_cols.put("R", "#000000");
    
    int numCols = globalDS.gh_cols.size();
    int i,j;
    
    String[] colarr = new String[] {" ", "w", "r", "g", "y", "b", "p", "c", "_", "W", "o", "G", "Y", "B", "P", "C", "O", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "R"};
    
    FlexTable cols = new FlexTable();
    cols.insertRow(0);
    for (i = 0; i< numCols;i++) {
    	cols.addCell(0);
    	Label tl = new Label();
    	tl.setStyleName("squareEditLabel");
//    	cols.setText(0,i,"");
    	cols.setWidget(0,i,tl);
    	
    	
    	DOM.setStyleAttribute(cols.getCellFormatter().getElement(0,i),"backgroundColor",(String)globalDS.gh_cols.get(colarr[i])); 
    	
    }
    
    
    rs.resize(12,8);
    rs.setBorderWidth(0);
    rs.setStyleName("resizeableTable");
    
    
    for (i=0; i <rs.getRowCount(); i++) {
    	for (j=0; j <rs.getColumnCount(); j++) {
    		squareEditLabel sel = new squareEditLabel();
    		rs.setWidget(i,j, sel);
    		sel.addStyleName("squareEditLabel_default");
    	}
    }
    
    squareEditLabel cw = new squareEditLabel();
    globalDS.curKeyWidget = cw;
  
 
   
    
//    rs.getCellFormatter().setStyleName(4,4,"resizeableTable_br");
    //    rs.setHTML(3,3,"hi");
    
    
    FocusPanel fp = new FocusPanel();

    fp.add(rs);
    fp.addKeyboardListener( new KeyboardListener() {

		public void onKeyDown(Widget sender, char keyCode, int modifiers) {
			// TODO Auto-generated method stub
			
		}

		public void onKeyPress(Widget sender, char keyCode, int modifiers) {
			// TODO Auto-generated method stub
			globalDS.curKey = keyCode;
			globalDS.curKeyWidget.setText(String.valueOf(keyCode));
		}

		public void onKeyUp(Widget sender, char keyCode, int modifiers) {
			// TODO Auto-generated method stub
			
		}
    	
    });

    RootPanel.get("slot1").add(cols);
    RootPanel.get("slot1").add(fp);
    RootPanel.get("slot1").add(button);
    RootPanel.get("slot2").add(label);
    RootPanel.get("slot2").add(cw);
  }
}
