package org.studierstube.opentracker.viewer;

import java.awt.FlowLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.TimerTask;

import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.Timer;

public class GraphStatusBar extends JPanel {
	private static final long serialVersionUID = -8604223670383149579L;

	private class TransientLabel extends JLabel implements ActionListener {
		private static final long serialVersionUID = -3376269722819008359L;
		
		private Timer timer=null;
		private int delay;
		
		public TransientLabel(String text, int _delay) {
			delay = _delay;
			timer = new Timer(_delay, this);
			timer.setRepeats(false);
			setText(text);
		}
		
		public TransientLabel() {
			this("Ready", 3000);
		}

		public void actionPerformed(ActionEvent e) {
			super.setText(" ");
		}
		
		public void setText(String text) {
			super.setText(text);
			if (timer != null) {
				timer.restart();
			}
		}
	}	

	private TransientLabel label;
	
	public GraphStatusBar() {
		super(new FlowLayout(FlowLayout.LEFT));
		label = new TransientLabel();
		add(label);
	}
	
	public JLabel getMessageLabel() {
		return label;
	}

}
