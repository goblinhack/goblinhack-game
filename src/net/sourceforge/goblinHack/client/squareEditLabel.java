package net.sourceforge.goblinHack.client;
import com.google.gwt.user.client.ui.*;



public class squareEditLabel extends Label {
	public squareEditLabel() {
		this.setStyleName("squareEditLabel");
			
		this.addClickListener( new ClickListener() {

			public void onClick(Widget sender) {
				// TODO Auto-generated method stub
				setText(String.valueOf(globalDS.curKey));
			}
		});
		
		this.addMouseListener( new MouseListener() {

			public void onMouseDown(Widget sender, int x, int y) {
				// TODO Auto-generated method stub
				
			}

			public void onMouseEnter(Widget sender) {
				// TODO Auto-generated method stub
				addStyleName("squareEditLabel_hover");
			}

			public void onMouseLeave(Widget sender) {
				// TODO Auto-generated method stub
				removeStyleName("squareEditLabel_hover");
			}

			public void onMouseMove(Widget sender, int x, int y) {
				// TODO Auto-generated method stub
				
			}

			public void onMouseUp(Widget sender, int x, int y) {
				// TODO Auto-generated method stub
				
			}
			
		});
	}
}
