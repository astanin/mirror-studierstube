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
public class OT_MergeNode extends AbstractOpenTrackerNode {
	
	/**
	 * @param graph
	 * @param iUniqueId
	 */
	public OT_MergeNode(OpenTrackerListenableDirectedMultigraph graph,
			int iUniqueId) {
		super(graph, iUniqueId, OpenTrackerNodeType.MERGE_NODE );
		
		keyValuePair.addKeyValue( "date" , "values" );
	}

	public String toXMLbody( final String intendXML, final int iCurrentLevel ) {
		return this.toXMLbodyDefault(intendXML,iCurrentLevel);
	}

}
