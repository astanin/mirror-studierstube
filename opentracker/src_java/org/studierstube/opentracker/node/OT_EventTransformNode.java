/**
 * 
 */
package org.studierstube.opentracker.node;

import org.studierstube.opentracker.OpenTrackerListenableDirectedMultigraph;
import org.studierstube.opentracker.OpenTrackerNodeType;

/**
 * @author kalkusch
 *
 */
public class OT_EventTransformNode extends AbstractOpenTrackerNode {
	
	/**
	 * @param graph
	 * @param iUniqueId
	 */
	public OT_EventTransformNode(OpenTrackerListenableDirectedMultigraph graph,
			int iUniqueId) {
		super(graph, iUniqueId, OpenTrackerNodeType.TRANSFORM_NODE);		
		
		keyValuePair.addKeyValue( "translation" , "1 1 1" );
	}
	
	public String toXMLbody( final String intendXML, final int iCurrentLevel ) {
		return this.toXMLbodyDefault(intendXML,iCurrentLevel);
	}

}
