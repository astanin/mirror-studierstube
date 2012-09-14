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
public class OT_ConsoleNode extends AbstractOpenTrackerNode {
	
	/**
	 * @param graph
	 * @param iUniqueId
	 */
	public OT_ConsoleNode(OpenTrackerListenableDirectedMultigraph graph,
			int iUniqueId) {
		super(graph, iUniqueId, OpenTrackerNodeType.CONSOLE_SINK);
		
		keyValuePair.addKeyValue( "comment" , "Test Source "+ getIdAsString() );
		keyValuePair.addKeyValue( "joes_stuff" , "any data" );
	}

	public String toXMLbody( final String intendXML, final int iCurrentLevel ) {
		return this.toXMLbodyDefault(intendXML,iCurrentLevel);
	}

}
