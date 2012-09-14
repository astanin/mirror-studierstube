package org.studierstube.opentracker.tests;

import javax.vecmath.Vector3f;
import javax.vecmath.Vector4f;

import org.omg.CORBA.COMM_FAILURE;
import org.studierstube.opentracker.OTEvent;
import org.studierstube.opentracker.OT_CORBA.*;

public class OTSourceExample extends CorbaApp {
	public OTSourceExample(String[] args) throws ManagerActivationFailure {
		super(args);
	}
	
	static public OTEvent getEvent() {
		OTEvent ev = new OTEvent(getORB());
		Vector3f pos = new Vector3f(1.0f, 2.0f, 3.0f);
		ev.setPosition(pos);
		ev.setOrientation(new Vector4f(0.707f, 0.0f, 0.0f, 0.707f));
		ev.setTimestamp(0.0f);
		ev.setButton((short) 3);
		return ev;
	}
	
	public static void main(String[] args) throws InterruptedException {
		try {
			org.omg.CORBA.Object obj = getObjectReference("CORBA.Source01");
			OTSource source = OTSourceHelper.narrow(obj);
			while(true) {
				try {
					System.out.println("about to set event");
					EventAttribute[] atts = getEvent().getEventAttributes(); 
					source.setEvent(atts);
				} catch (COMM_FAILURE e) {
					System.err.println("CORBA communication error. Keep trying!");
				}
				Thread.sleep(10);
			}
		} catch (NamingServiceUnavailable e) {
			System.err.println("Unable to get reference to source object");
		} catch (NamingServiceFailure e) {
			System.err.println("Unable to get reference to source object");
		}
	}
}