/**
 * 
 */
package org.studierstube.opentracker;

/**
 * @author Michael Kalkusch
 *
 */
public enum OpenTrackerNodeType {

	
	TRANSFORM_NODE("TransformNode","TRANS","TRAN"),
	CONSOLE_SINK("ConsolSink","CONSOL","CON"),
	NETWORK_SINK("NetworkSink","NETWORK","NET"),
	MERGE_NODE("Merge","MERGE","MER"),
	NOT_DEFINED("UndefinedSink","UNDEFINED","---");
	
	private static final int iMaximumIndex = 3;
	
	private String sNodeText;
	
	private String sNodeButtonText;
	
	private String sNodePrefix;
	
	private OpenTrackerNodeType( String sNodeText, String sNodeButtonText, String sNodePrefix) {
		this.sNodeText = sNodeText;
		this.sNodeButtonText = sNodeButtonText;
		this.sNodePrefix = sNodePrefix;
	}
	
	public String getNodeText() {
		return sNodeText;
	}
	
	public String getNodeButtonText() {
		return sNodeButtonText;
	}
	
	public String getNodePrefix() {
		return sNodePrefix;
	}
	
	public static OpenTrackerNodeType getTypeByIndex( final int iIndex ) {
		switch ( iIndex ) {
		
		case 0:
			return OpenTrackerNodeType.CONSOLE_SINK;
		case 1:
			return OpenTrackerNodeType.NETWORK_SINK;
		case 2:
			return OpenTrackerNodeType.TRANSFORM_NODE;			
		case 3:
			return OpenTrackerNodeType.MERGE_NODE;
			
		default:
			return OpenTrackerNodeType.NOT_DEFINED;
		}
	}
	
	public static int getMaxiumumIndex() {
		return iMaximumIndex;
	}
}
