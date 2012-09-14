/**
 * 
 */
package org.studierstube.opentracker.event;

import org.jgraph.event.GraphModelEvent;
import org.jgraph.event.GraphModelEvent.GraphModelChange;
import org.jgraph.event.GraphModelListener;

import org.jgraph.graph.DefaultEdge;
import org.studierstube.opentracker.OpenTrackerListenableDirectedMultigraph;

import org._3pq.jgrapht.ext.JGraphModelAdapter;

/**
 * @author kalkusch
 *
 */
public class OT_GraphModelListener implements GraphModelListener {
	
	private OpenTrackerListenableDirectedMultigraph graph;

	/**
	 * 
	 */
	public OT_GraphModelListener( final OpenTrackerListenableDirectedMultigraph graph ) {
		this.graph = graph;
	}

	/* (non-Javadoc)
	 * @see org.jgraph.event.GraphModelListener#graphChanged(org.jgraph.event.GraphModelEvent)
	 */
	public void graphChanged(GraphModelEvent e) {
						
		GraphModelChange buffer = e.getChange();
		
		Object[] array = buffer.getChanged();
		
//		System.err.println("update!" + buffer.toString() + 
//				" changed= " + buffer.getSource().toString() +
//				"\n info=" + buffer.getChanged() .getClass().toString()) ;
		
		for ( int i=0; i<array.length; i++ ) {
//			System.err.println("   I: " + array[i].toString() + "  class=" +
//					array[i].getClass().toString() ); 
			
			if ( array[i].getClass() == org.jgraph.graph.DefaultEdge.class ) {
				DefaultEdge edge = (DefaultEdge) array[i];
				
				if (( edge.getSource() == null ) && ( edge.getTarget() == null )) {
//					System.err.println("  remove this edge!");
					
					Object[] removeBuffer = new Object[1];					
					removeBuffer[0] = edge;
					
					JGraphModelAdapter adapter = (JGraphModelAdapter) buffer.getSource();
					adapter.remove( removeBuffer );
				}
				
				if ( edge.getSource() != null ) {
					
				}
			}
			else if ( array[i].getClass() == org.jgraph.graph.DefaultGraphCell.class ) {
				
			}
		}
	}

	public final OpenTrackerListenableDirectedMultigraph getGraph() {
		return graph;
	}

}
