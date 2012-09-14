package org.studierstube.opentracker.tests;

import org.studierstube.opentracker.OT_CORBA.*;
import org.studierstube.opentracker.OTEvent;

import javax.vecmath.*;

public class OTSinkExample extends CorbaApp {

    class Sink_i extends org.studierstube.opentracker.OT_CORBA.OTEntityPOA {
   
	    public void setEvent(EventAttribute[] atts) {
	    	OTEvent ev = new OTEvent(getORB(), atts);
	    	Vector3f pos = ev.getPosition();
	    	Vector4f ori = ev.getOrientation();
	    	//Float timestamp = new Float(ev.getTimestamp());

	    	System.out.println("position = (" + pos.x + ", " + pos.y + ", " + pos.z + ")");
	    	System.out.println("orientation = [(" + ori.x + ", " + ori.y + ", " + ori.z + "), " + ori.w + "]");
	    	//System.out.println("time = " + timestamp.toString());
		}
	}
	
	protected Sink_i sink;
	
	public OTSinkExample(String[] args) throws ManagerActivationFailure {
		super(args);
		sink = new Sink_i();
	}

	public void activateSink(String name) {
		try {
			activateObject(sink);
			bindObectToName(sink._this(), name);
		} catch (ObjectActivationFailure e1) {
			System.err.println("Unable to activate sink. Exiting...");
			System.exit(-1);
		} catch (NamingServiceUnavailable e) {
			System.err.println("Unable to contact naming service. Exiting...");
			System.exit(-1);
		}
	}
	
	public static void main(String[] args) {
		System.out.println("Starting sink.");
		try {
			OTSinkExample _sink = new OTSinkExample(args);
			_sink.activateSink("CORBA.Sink01"); 
			getORB().run();
		} catch (ManagerActivationFailure ex) {
			System.err.println("Manager could not be activated");
		}
	}

}
