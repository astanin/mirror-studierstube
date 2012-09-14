package org.studierstube.opentracker.viewer;

import java.awt.Dimension;
import java.io.IOException;
import java.io.StringReader;
import java.io.StringWriter;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLConnection;

import att.grappa.Graph;
import att.grappa.GrappaPanel;
import att.grappa.GrappaSupport;
import att.grappa.Parser;
import att.grappa.Subgraph;

import javax.swing.JScrollPane;
import javax.swing.SwingUtilities;

public class DotPane extends JScrollPane implements Runnable {
	private static final long serialVersionUID = 2191488579017007629L;
	private String dot_string;
	private StringReader sr;
	private Parser parser;

	public class DotPanel extends GrappaPanel {
		private static final long serialVersionUID = 6726440978562249417L;

		public DotPanel(Subgraph subgraph) {
			super(subgraph);
			setScaleToFit(false);
		}
	}

	public DotPane() {
		dot_string = "digraph G {}";
		sr = new StringReader(dot_string);
		parser = new Parser(sr);
		try {
			parser.parse();
			Graph graph = parser.getGraph(); // this graph hasn't yet used the dot engine for layouting purposes
			setViewportView(new DotPanel(graph));
			setHorizontalScrollBarPolicy(HORIZONTAL_SCROLLBAR_AS_NEEDED);
			setVerticalScrollBarPolicy(VERTICAL_SCROLLBAR_AS_NEEDED);
		} catch (Exception e) {
			System.out.println("Caught exception parsing dot");
			e.printStackTrace();
		}
	}
	
	public Dimension getPreferredSize() {
	    return new Dimension(640, 480);
	}
	
    public Dimension getMinimumSize() {
	    return new Dimension(480, 360);
    }
    
    public void setDot(String dotstring) {
    	dot_string = dotstring;
    	SwingUtilities.invokeLater(this);
    }

	public void run() {
		System.out.println("about to parse dot_string");
		StringReader sr = new StringReader(dot_string);
        Parser parser = new Parser(sr);
        try {
            parser.parse();
        } catch (Exception e) {
            System.err.println("caught exception parsing graph");
        }
        Graph graph = parser.getGraph(); // this graph hasn't yet used the dot engine for layouting purposes
        URL url;
		try {
			url = new URL("http://www.research.att.com/~john/cgi-bin/format-graph");
	        URLConnection urlConn = url.openConnection();
	        urlConn.setDoInput(true);
	        urlConn.setDoOutput(true);
	        urlConn.setUseCaches(false);
	        urlConn.setRequestProperty("Content-Type","application/x-www-form-urlencoded");
	        if (!GrappaSupport.filterGraph(graph,urlConn)) {
	            System.err.println("ERROR: somewhere in filterGraph");
	        }
	        sr.close();
	        StringWriter sw = new StringWriter();
	        graph.printGraph(sw);
	        System.out.println(sw.toString());
	        setViewportView(new DotPanel(graph));
		} catch (MalformedURLException e) {
			System.err.println("Caught malformed URL exception");
		} catch (IOException e) {
			System.err.println("Caught IOException");
		}
	}
}

