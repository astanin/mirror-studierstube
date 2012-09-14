package org.studierstube.opentracker.tests;

import org.omg.CORBA.Any;
import org.omg.CosEventComm.Disconnected;

import org.studierstube.opentracker.OTEvent;
import org.studierstube.opentracker.OT_CORBA.*;

public class OpenTrackerEventConsumer extends OTPushCons {

	public OpenTrackerEventConsumer(String[] args) {
		super(args);
	}

	/** CORBA method implementation. */
	public void disconnect_push_consumer()
	{
		System.out.println("Push Consumer: disconnected.");
	}
	
	synchronized public void push(Any data) throws Disconnected {
		OTEvent event = new OTEvent(getORB(), EventHelper.extract(data));
		
		System.out.println("Push Consumer: push() called");
		System.out.println("Position = [" + event.getPosition().x + ", " + event.getPosition().y + ", " + event.getPosition().z + "]");
		System.out.println("Orientation = [" + event.getOrientation().x + ", " + event.getOrientation().y + 
				", " + event.getOrientation().z +  event.getOrientation().w + "]\n");
	}
	
	static public void main(String args[]) {
		System.out.println("OpenTrackerEventConsumer.main");
		OpenTrackerEventConsumer consumer = new OpenTrackerEventConsumer(args);
		consumer.mainLoop();
	}
}
