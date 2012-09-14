/**
 * 
 */
package org.studierstube.opentracker.node;

import java.util.List;
import java.util.Iterator;

import org._3pq.jgrapht.edge.DirectedEdge;

import org.studierstube.opentracker.OpenTrackerListenableDirectedMultigraph;
import org.studierstube.opentracker.OpenTrackerNodeType;
import org.studierstube.opentracker.node.OT_KeyValuePair;

/**
 * @author kalkusch
 *
 */
public abstract class AbstractOpenTrackerNode {

	private final OpenTrackerListenableDirectedMultigraph graph;
	
	private int iUniqueId;
	
	private String sXMLNodeName = "ConsoleSink";
	
	
	private String sXML_RefName = "";
	
	private boolean bXMLisSink = true;
	
	private boolean bXMLstatusIsVisited = false;
	
	private boolean bXMLstatusUseRef = false;
	
	private OpenTrackerNodeType nodeType = OpenTrackerNodeType.NOT_DEFINED;
	
	protected String sOpenTrackerType = "souceNode";

	protected OT_KeyValuePair keyValuePair;
	
	/**
	 * 
	 */
	protected AbstractOpenTrackerNode( final OpenTrackerListenableDirectedMultigraph graph, 
			final int iUniqueId,
			final OpenTrackerNodeType nodeType) {
		
		assert graph != null : "Can not create node without reference to graph";
		
		this.keyValuePair = new OT_KeyValuePair();		
		this.graph = graph; 
		
		graph.addVertex( this ); 		
		setId( iUniqueId ); 	
		setNodeType(nodeType);
		
		resetTreePreXML();
	}

	/**
	 * Get access to the key value data of the node.
	 * 
	 * @return key-value data object
	 */
	public final OT_KeyValuePair getKeyValueData() {
		return this.keyValuePair;
	}
	
	/**
	 * Get type of node.
	 * 
	 * @return
	 */
	public final OpenTrackerNodeType getNodeType() {
		return nodeType;
	}
	
	/**
	 * 
	 * @param sIntendText indents header
	 * @return text for header of node
	 */
	protected final String toXMLHeader( final String sIntendText ) {
		String result =	sIntendText+ "<" + sXMLNodeName;
		
		if ( ! bXMLstatusUseRef ) {						
			sXML_RefName = "OTnode_" + Integer.toString( iUniqueId );
			result += " DEF=\"" + sXML_RefName + "\" "; 
		}
		
		return result;
	}
	
	/**
	 * Footer created from comonent name.
	 * 
	 * @param sIntendText indents header
	 * @return text for footer of node
	 */
	protected final String toXMLFooter( final String sIntendText ) {
		String result =	sIntendText;
		
		if ( bXMLisSink ) {
			result += "<!-- sink -->\n" + sIntendText;
		}
		
		result += "<" + sXMLNodeName + "\\>\n";
		
		return result;
	}
	

	/**
	 * Creates uniform indention for hole XML file.
	 * 
	 * @param iLevel level of recusion [0..]
	 * @return spaces for indention
	 */
	protected final String intendByLevel( final int iLevel ) {
		String result = "  ";
		
		for ( int i=0; i < iLevel; i++ ) {
			result += "  ";
		}
		
		return result;
	}
	
	protected abstract String toXMLbody( final String intendXML, final int iCurrentLevel );
	
	protected void setNodeType( OpenTrackerNodeType nodeType) {
		this.nodeType = nodeType;
		this.sXMLNodeName = this.nodeType.getNodeText();
	}
	
	/**
	 * Put all content for the node here. 
	 * Overlaod this methode.
	 * 
	 * @param intendXML indention for each line
	 * @param iCurrentLevel current level of recursion, usedd for idention of text
	 * @return String containing the XML text
	 */
	protected String toXMLbodyDefault( final String intendXML, final int iCurrentLevel ) {

		String result = toXMLHeader(intendXML);
		
		bXMLstatusUseRef = true;	
		
		/**
		 * add unique name..
		 */
		result += "id=\"" + Integer.toString( iUniqueId ) + "\" ";

		/**
		 * add key value pairs..
		 */
		result += this.keyValuePair.toXML() + ">\n";
		
		
		List listEdges = graph.outgoingEdgesOf( this );
		
		if ( listEdges != null ) {
			if ( ! listEdges.isEmpty() ) {
			Iterator iter = listEdges.iterator();
			
				while (iter.hasNext()) {
					
					DirectedEdge buffer = (DirectedEdge) iter.next();
					
					AbstractOpenTrackerNode bufferOT = (AbstractOpenTrackerNode) buffer.getTarget();
					
					if ( bufferOT != null ) {
						result += bufferOT.toXML( iCurrentLevel+1 );
						
					}
					
				}
			}
		}		
		
		result += toXMLFooter(intendXML);
		
		return  result;
	}
	
	/**
	 * The methode creates teh XML output as a String. 
	 * It recursivly queries linked nodes.
	 * 
	 * @param iLevel level from root node, only used to indent text
	 * @return String containing the XML output
	 */
	public final String toXML( final int iLevel ) {
		
		bXMLstatusIsVisited = true;		
		String intendXML = intendByLevel(iLevel);
		
		if ( bXMLstatusUseRef ) {
			return intendXML + "<Ref USE=\"" + sXML_RefName + "\">\n";
		} 
		
		return toXMLbody( intendXML, iLevel );
	}

	/**
	 * Called before writing graph to XML. 
	 * Must be called for each node before writing to XML. 
	 */
	public void resetTreePreXML() {
		bXMLstatusIsVisited = false;
		bXMLstatusUseRef = false;
		isSink();
	}
	
	/**
	 * Intended to detect cycles.
	 * Note: for each sink node query this setting.
	 * 
	 * @return TRUE if is was aleadry visited and is a sink node
	 */
	public final boolean isVisitedAsSinkNode() {
		if (( this.bXMLisSink )&&(this.bXMLstatusIsVisited)) {
			return true;
		}
		return false;
	}
	
	/**
	 * Tests is no incoming edges exist on the fly.
	 * 
	 * @return if it is a sink node with no incoming edges.
	 */
	public final boolean isSink() {
		
		List listEdges = graph.incomingEdgesOf( this );		
		bXMLisSink = listEdges.isEmpty(); 
		
		return bXMLisSink;
	}
	
	/**
	 * Get unique Id of the node.
	 * 
	 * @see org.studierstube.opentracker.node.AbstractOpenTrackerNode#setId(int)
	 * 
	 * @return unique Id
	 */
	public final int getId() {
		return iUniqueId;
	}
	
	/**
	 * Get unique Id of the node as a String.
	 * 
	 * @see org.studierstube.opentracker.node.AbstractOpenTrackerNode#getId()
	 * @see org.studierstube.opentracker.node.AbstractOpenTrackerNode#setId(int)
	 * 
	 * @return unique Id as String
	 */
	public final String getIdAsString() {
		return Integer.toString(iUniqueId);
	}
	
	/**
	 * Set the unique Id. 
	 * Note: No test is performed if this if is unique!
	 * 
	 * @see org.studierstube.opentracker.node.AbstractOpenTrackerNode#getId()
	 * 
	 * @param iUniqueId system wide unique id. 
	 */
	public final void setId( final int iUniqueId ) {
		this.iUniqueId = iUniqueId;
	}
	
	
	public String toString() {
		
		String result; 
			
//		result = toXMLHeader();
		
		result =  nodeType.getNodePrefix() + Integer.toString( iUniqueId );
		
//		result += toXMLFooter();
		
		return  result;
	}
	
	
}
