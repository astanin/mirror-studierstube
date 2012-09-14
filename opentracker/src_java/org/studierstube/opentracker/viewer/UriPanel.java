package org.studierstube.opentracker.viewer;
import org.studierstube.opentracker.OTGraph.*;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.ItemEvent;
import java.awt.event.ItemListener;
import java.util.ListIterator;
import java.util.Vector;

import javax.swing.BoxLayout;
import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JPanel;
import javax.swing.JLabel;
import javax.swing.JTextField;
import javax.swing.Timer;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;

import org.omg.CORBA.*;

public class UriPanel extends JPanel implements DocumentListener, ActionListener {
	public class PollingCheckBox extends JCheckBox implements ActionListener, ItemListener {
		private static final long serialVersionUID = 2125820433489239305L;
		private DataFlowGraph graph_ref=null;
		private DotPane pane;
		private int delay;
		private Timer timer;
		public PollingCheckBox(DotPane _pane, int _delay) {
			super("Polling");
			delay = _delay;
			pane = _pane;
			timer = new Timer(delay, this);
			addItemListener(this);
		}
		
		public PollingCheckBox(DotPane _pane) {
			this(_pane, 1000);
		}
		
		public void actionPerformed(ActionEvent e) {
			if (graph_ref != null) {
				String dotstring = graph_ref.getDot();
				if (dotpane != null) {
					dotpane.setDot(dotstring);
				}
			}
		}

		public void itemStateChanged(ItemEvent e) {
			if (e.getStateChange() == ItemEvent.SELECTED) {
				timer.restart();
			} else if (e.getStateChange() == ItemEvent.DESELECTED) {
				timer.stop();
			}
		}
		
		public void setGraphRef(DataFlowGraph _graph) {
			graph_ref = _graph;
		}
	}
	
	private static final long serialVersionUID = -5995608106416029725L;
	private DotPane dotpane=null;
	private JButton ref_button, refresh_button;
	private PollingCheckBox polling_checkbox;
	private JTextField text_field;
	private Vector<JLabel> labels;
	private DataFlowGraph graph_ref=null;
	private ORB orb;
	
	public UriPanel() {
		String[] argv = {};
	    orb = ORB.init(argv, System.getProperties());
	    labels = new Vector<JLabel>();
		new BoxLayout(this, BoxLayout.X_AXIS);
        ref_button = new JButton("Obtain Reference");
        refresh_button = new JButton("Refresh");
        polling_checkbox = new PollingCheckBox(dotpane);
        disableRefresh();        
        text_field = new JTextField("corbaloc::localhost:12090/DFG", 30);
        add(text_field);
        add(ref_button);
        add(refresh_button);
        add(polling_checkbox);
        addListeners();
	}

	private void addListeners() {
		text_field.getDocument().addDocumentListener(this);
	    text_field.addActionListener(this);
	    ref_button.addActionListener(this);
	    refresh_button.addActionListener(this);
	}
	
	public void addLabel(JLabel l) {
		labels.add(l);
	}

	public void removeLabel(JLabel l) {
		labels.removeElement(l);
	}
	
	public void sendMessage(String text) {
		ListIterator<JLabel> iter = labels.listIterator();
		while (iter.hasNext()) {
		    iter.next().setText(text);
		}	
	}
	
    private void refreshAction() {
        sendMessage("Rendering latest version of graph");
        try {
            String dotstring = graph_ref.getDot();
            if (dotpane != null) {
            	dotpane.setDot(dotstring);
            }
        } catch (COMM_FAILURE e) {
        	sendMessage("COMM_FAILURE: Graph uncontactable");
            disableRefresh();
        }
    }
	
	private void disableRefresh() {
		refresh_button.setEnabled(false);
	}

	private void enableRefresh() {
		refresh_button.setEnabled(true);
	}

	public void changedUpdate(DocumentEvent e) {
		// TODO Auto-generated method stub
		
	}

	public void insertUpdate(DocumentEvent e) {
		// TODO Auto-generated method stub
		
	}

	public void removeUpdate(DocumentEvent e) {
		// TODO Auto-generated method stub
		
	}

	public void actionPerformed(ActionEvent event) {
		if (event.getSource() == text_field) {
            System.out.println("got TextField Action");
            refAction();
		} else if (event.getSource() == ref_button) {
			System.out.println("got bind Action");
            refAction();
		} else if (event.getSource() == refresh_button) {
            System.out.println("got refresh Action");
            refreshAction();
		}
	}

	private void refAction() {
		String uri_string = text_field.getText();
        System.out.println("uri_string = " + uri_string);
        try {
        	String IOR = new String(uri_string);
        	org.omg.CORBA.Object obj = orb.string_to_object(IOR);
        	graph_ref = DataFlowGraphHelper.narrow(obj);
        	if (graph_ref != null) {
        	//String message = "Obtaining reference for object with uri ";
        		ref_button.setText("Reobtain Reference");
        		sendMessage("Narrowed object to Graph");
        		enableRefresh();
        		polling_checkbox.setGraphRef(graph_ref);
        	} else {
        		sendMessage("Unable to obtain reference to Graph");
        	}
        } catch (OBJECT_NOT_EXIST e) {
        	sendMessage("Graph object does not exist");
        } catch (Exception e) {
        	//e.printStackTrace();
        	sendMessage("Caught exception obtaining reference to DataFlowGraph");
        }
	}
	
	public void registerDotPane(DotPane pane) {
		dotpane = pane;
	}

}
