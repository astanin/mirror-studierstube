package org.studierstube.opentracker.tests;

import javax.vecmath.Vector3f;

import org.studierstube.opentracker.OTEvent;

public class OTEventChannelSourceObject extends OTPushSupp {

	private static OTEventChannelSourceObject source;

	public OTEventChannelSourceObject(String[] args, String eventChannel) {
		super(args, eventChannel);
	}

	public OTEventChannelSourceObject(String[] args) {
		super(args, "EventChannel");
	}

	public OTEventChannelSourceObject(String eventChannel)
	{
		super(new String[0], eventChannel);
	}


	public static void main(String[] args) {
//		System.err.println(args.length);
		source = new OTEventChannelSourceObject("EventChannel");
		float[] pos_array = {(float)1.1, (float)2.2, (float)3.3};
		source.pushPositionEventOntoEventChannel(pos_array);
	}

	public void pushPositionEventOntoEventChannel(float[] pos_array)
	{
		OTEvent ev = new OTEvent(getORB());
		Vector3f pos = new Vector3f();
		pos.set(pos_array);
		ev.setPosition(pos);
		System.err.println("Pushing position: (" + pos_array[0] + ", " + pos_array[1] + ", " + pos_array[2] + ")");
		pushDataOntoEventChannel(ev.asAny());
	}

	public void disconnect_push_supplier() {
		// TODO Auto-generated method stub

	}
}
