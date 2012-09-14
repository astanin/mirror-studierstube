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
public class OT_NetworkSinkNode extends AbstractOpenTrackerNode {

	/**
	 * @param graph
	 * @param iUniqueId
	 */
	public OT_NetworkSinkNode(OpenTrackerListenableDirectedMultigraph graph,
			int iUniqueId) {
		super(graph, iUniqueId, OpenTrackerNodeType.NETWORK_SINK);
	}
	
	public String toXMLbody( final String intendXML, final int iCurrentLevel ) {
		return this.toXMLbodyDefault(intendXML,iCurrentLevel);
	}

}
