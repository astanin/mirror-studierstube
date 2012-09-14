/**
 * 
 */
package org.studierstube.opentracker;

import java.util.Set;
import java.util.Iterator;

import org._3pq.jgrapht.DirectedGraph;
//import org._3pq.jgrapht.Graph;
import org._3pq.jgrapht.graph.DefaultListenableGraph;
import org._3pq.jgrapht.graph.DirectedMultigraph;

import org.studierstube.opentracker.node.AbstractOpenTrackerNode;

/**
 * @author Michael Kalkusch
 *
 */
public class OpenTrackerListenableDirectedMultigraph extends DefaultListenableGraph
		implements DirectedGraph {

	private static final long serialVersionUID = 2L;
	
	/**
	 * @param g
	 */
	public OpenTrackerListenableDirectedMultigraph() {
		super( new DirectedMultigraph() );
		
	}
	
	public String listEdgesAndNodes() {
		
		Set edgeSet = this.edgeSet();		
		Set vertexSet = this.vertexSet();
		
		Iterator iterEdge = edgeSet.iterator();
		
		String XML_OpenTracker_Header = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"+
		"<!DOCTYPE OpenTracker SYSTEM \"opentracker.dtd\">\n"+
		"<OpenTracker> \n"+
		"  <configuration> \n"+
		"    <ConsoleConfig headerline=\"Sample Output\" interval=\"10\"/> \n" +
		"  </configuration>\n\n";
		
		String XML_OpenTracker_Footer ="</OpenTracker>";
		
		String result = XML_OpenTracker_Header;
		
		System.out.println(" EDGES:"); 
		
		while ( iterEdge.hasNext() ) {
			Object buffer = iterEdge.next();
			
			System.out.println(" " + buffer.getClass().toString() + ":  " + buffer.toString() );
		}	
		
		System.out.println(" VERTICES:\n"); 
		
		Iterator iterVertex = vertexSet.iterator();
		
		System.out.print( XML_OpenTracker_Header );
		
		/**
		 * Reset state od each node...
		 */
		while ( iterVertex.hasNext() ) {
			((AbstractOpenTrackerNode) iterVertex.next()).resetTreePreXML();
		}
		
		iterVertex = vertexSet.iterator();
		
		/**
		 * Create output...
		 */
		while ( iterVertex.hasNext() ) {
			AbstractOpenTrackerNode buffer = 
				(AbstractOpenTrackerNode) iterVertex.next();
					
			if ( buffer.isSink() ) {
				result += ((AbstractOpenTrackerNode) buffer).toXML(1);
			}
		}
		result += XML_OpenTracker_Footer;
		
		System.out.print( result );
		
		return result;
	}

}
