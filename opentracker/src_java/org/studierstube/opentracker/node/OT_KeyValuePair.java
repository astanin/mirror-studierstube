/**
 * 
 */
package org.studierstube.opentracker.node;

import java.util.Hashtable;
import java.util.Iterator;
import java.util.List;
import java.util.LinkedList;
import java.util.Set;
import java.util.Map;

/**
 * Handle key-value pairs for OpenTracker nodes.
 * 
 * @author kalkusch
 *
 */
public class OT_KeyValuePair {

	protected Hashtable <String,String> hashKeyValuePair;
	/**
	 * 
	 */
	public OT_KeyValuePair() {		
		hashKeyValuePair = new Hashtable <String,String> (4);
	}

	public void addKeyValue( final String key, final String value ) {
		hashKeyValuePair.put( key, value );
	}
	
	public String getValueByKey( final String key ) {
		return hashKeyValuePair.get( key );
	}
	
	/**
	 * Get a list of all the keys, that contain the value passed as argument.
	 * 
	 * @param value search for this value inside all keys
	 * @return list of keys containing the value
	 */
	public List <String> getKeysBaValue( final String value ) {
		
		List <String> keyList = new LinkedList <String>();
		Set<Map.Entry<String,String>> allKeyValues = hashKeyValuePair.entrySet();				
		Iterator iter = allKeyValues.iterator();
		
		while ( iter.hasNext() ) {
			Map.Entry map = (Map.Entry) iter.next();
			if ( map.getValue() == value ) {
				keyList.add( (String) map.getKey() );
			}
		}
		
		return keyList;
	}
	
	public boolean hasKey( final String key ) {
		return hashKeyValuePair.containsKey( key );
	}
	
	public boolean hasValue( final String value ) {
		return hashKeyValuePair.containsValue( value );
	}
	
	/**
	 * Create an XML output for this key value pairs
	 * Note: this must be inserted inside an XML object, because no tags are created.
	 * 
	 * @return XML ouput for this key value pairs
	 */
	public String toXML() {
		String result ="";		
		Set<Map.Entry<String,String>> allKeyValues = hashKeyValuePair.entrySet();		
		Iterator iter = allKeyValues.iterator();
		
		while ( iter.hasNext() ) {
			Map.Entry map = (Map.Entry) iter.next();
			
			result += map.getKey() + "=\"" + map.getValue() + "\" ";				
		}
		
		return result;
	}
	
	/**
	 * @see java.lang.Object#toString()
	 */
	public String toString() {
		String result ="";		
		Set<Map.Entry<String,String>> allKeyValues = hashKeyValuePair.entrySet();		
		Iterator iter = allKeyValues.iterator();
		
		while ( iter.hasNext() ) {
			Map.Entry map = (Map.Entry) iter.next();
			
			result += map.getKey() + "=\"" + map.getValue() + "\"\n";				
		}
		
		return result;
	}
}
