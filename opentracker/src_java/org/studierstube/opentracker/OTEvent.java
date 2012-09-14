package org.studierstube.opentracker;

import org.studierstube.opentracker.OT_CORBA.EventAttribute;
import org.studierstube.opentracker.OT_CORBA.EventHelper;
import org.studierstube.opentracker.OT_CORBA.FloatVectorHelper;

import java.util.HashMap;
import java.util.HashSet;
import java.util.Set;

import javax.vecmath.Vector3f;
import javax.vecmath.Vector4f;

import org.omg.CORBA.ORB;
import org.omg.CORBA.Any;

/**
 * Multimodal OpenTracker Event.
 * 
 * @author Michael Kalkusch
 * @author jfn
 */
public class OTEvent extends HashMap<String, Any> {
	/**
	 * 
	 */
	private static final long serialVersionUID = -6849795742997397981L;
	private ORB orb;
	
	public OTEvent(ORB _orb) {
		orb = _orb;
	}
	
	public OTEvent(ORB _orb, EventAttribute[] atts) {
		this(_orb);
		for (int i = 0; i < atts.length; i++) {
			System.out.println("name of event attribute is " + atts[i].name);
			put(atts[i].name, atts[i].value);
		}
	}
	
	public Vector3f getPosition() {
		return new Vector3f(FloatVectorHelper.extract(get("position")));
	}
	
	public void setPosition(Vector3f pos) {
		Any value = orb.create_any();
		float fv[] = {pos.x, pos.y, pos.y};
		FloatVectorHelper.insert(value, fv);
		put("position", value);
	}

	public Vector4f getOrientation() {
		return new Vector4f(FloatVectorHelper.extract(get("orientation")));
	}
	
	public void setOrientation(Vector4f ori) {
		Any value = orb.create_any();
		float fv[] = {ori.x, ori.y, ori.z, ori.w};
		FloatVectorHelper.insert(value, fv);
		put("orientation", value);
	}
	
	public float getTimestamp() {
		return get("timestamp").extract_float();
	}
	
	public void setTimestamp(float timestamp) {
		Any value = orb.create_any();
		value.insert_float(timestamp);
		put("timestamp", value);
	}
	
	public void setButton(short button) {
		Any value = orb.create_any();
		value.insert_short(button);
		put("button", value);
	}
	
	public short getButton() {
		return get("button").extract_short();
	}
	
	public EventAttribute[] getEventAttributes() {
		Set<EventAttribute> atts = new HashSet<EventAttribute>();
		Set<String> keys = keySet();
		for (String key : keys) {
			EventAttribute att = new EventAttribute();
			att.name  = key;
			att.value = get(key);
			atts.add(att);
		}
		return (EventAttribute[]) atts.toArray(new EventAttribute[atts.size()]);
	}
	
	public Any asAny() {
		Any ret = orb.create_any();
		EventHelper.insert(ret, getEventAttributes());
		return ret;
	}
}
 