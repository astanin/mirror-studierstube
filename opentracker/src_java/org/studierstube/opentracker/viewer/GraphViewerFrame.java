package org.studierstube.opentracker.viewer;

import java.awt.Container;
import java.awt.Dimension;
import java.awt.GraphicsConfiguration;
import java.awt.HeadlessException;
import javax.swing.BoxLayout;
import javax.swing.JFrame;
import javax.swing.JPanel;

public class GraphViewerFrame extends JFrame {

	/**
	 * 
	 */
	private static final long serialVersionUID = -2259117237300505213L;
	private UriPanel uri_panel;
	private DotPane graph_panel;
	private GraphStatusBar status_bar;
	
	public GraphViewerFrame() throws HeadlessException {
		// TODO Auto-generated constructor stub
	}

	public GraphViewerFrame(GraphicsConfiguration gc) {
		super(gc);
		// TODO Auto-generated constructor stub
	}
	
	public GraphViewerFrame(String title) throws HeadlessException {
		super(title);
		JPanel pane = new JPanel();
		setMinimumSize(new Dimension(640, 480));
		pane.setLayout(new BoxLayout(pane, BoxLayout.Y_AXIS));
		
		Container contentPane = getContentPane();
		
		uri_panel = new UriPanel();
		graph_panel = new DotPane();
		status_bar = new GraphStatusBar();
		uri_panel.setVisible(true);
		graph_panel.setVisible(true);
		pane.add(uri_panel);
		pane.add(graph_panel);
		pane.add(status_bar);
		uri_panel.addLabel(status_bar.getMessageLabel());
		contentPane.add(pane);
		uri_panel.registerDotPane(graph_panel);
//        self.name_panel.registerMessageListener(self.status_bar.getMessageLabel())
//        self.name_panel.registerDotPane(self.graph_panel)
//        pane.add(self.status_bar)
	}

	public UriPanel getUriPanel() {
		return uri_panel;
	}

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		GraphViewerFrame f = new GraphViewerFrame("Graph Viewer");
		f.setVisible(true);
		f.pack();
	}

	public Dimension getMinimumSize() {
		return new Dimension(640, 480);
	}
}
