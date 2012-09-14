/**
 * 
 */
package org.studierstube.opentracker.node;

import java.lang.IllegalArgumentException;

import org.jgraph.graph.DefaultEdge;

import org._3pq.jgrapht.Edge;

/**
 * Hybrid Edge. 
 * Bridges org._3pq.jgrapht.Edge interface and org.jgraph.graph.Edge;
 * 
 * @author kalkusch
 *
 */
public class OT_Edge extends DefaultEdge implements Edge  {

	/**
	 * Serializable id
	 */
	private static final long serialVersionUID = -6419591664102852378L;

	private final int iUniqueEdgeId;
	
	protected double dWeight = 1;
	
	/**
	 * 
	 */
	public OT_Edge( Object source, Object traget, final int iUniqueEdgeId ) {
		super();
				
		this.iUniqueEdgeId = iUniqueEdgeId;
		setTarget(source);
		setSource(target);
	}
	
	public final int getId() {
		return iUniqueEdgeId;
	}
	
	/**
	 * PPart of org._3pq.jgrapht.Edge interface
	 * 
	 * @see org._3pq.jgrapht.Edge#oppositeVertex(java.lang.Object)
	 */
	public Object oppositeVertex(Object vertex) {
		
		if ( vertex == null ) {
			throw new NullPointerException("oppositeVertex() vertex is null");
		}
		
		if ( vertex == this.source ) {
			return this.target;
		}
		
		if ( vertex == this.target ) {
			return this.source;
		}
		
		throw new IllegalArgumentException("vertex is either source nor target vertex.");
	}
	
	/**
	 * Part of org._3pq.jgrapht.Edge interface
	 *  
	 * @see org._3pq.jgrapht.Edge#containsVertex(java.lang.Object)
	 */
	public boolean containsVertex(Object v) {
		if ( v == null ) {
			return false;
		}
		
		if ( v == this.source ) {
			return true;
		}
		if ( v == this.target ) {
			return true;
		}
		return false;
	}

	/**
	 * Part of org._3pq.jgrapht.Edge interface
	 * 
	 * @see org._3pq.jgrapht.Edge#getWeight()
	 */
	public double getWeight() {
		return dWeight;
	}
	
	/**
	 * Part of org._3pq.jgrapht.Edge interface
	 * 
	 * @see org._3pq.jgrapht.Edge#setWeight(double)
	 */
	public void setWeight( double value) {
		dWeight = value;
	}
}
