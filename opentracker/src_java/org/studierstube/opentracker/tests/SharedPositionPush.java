package org.studierstube.opentracker.tests;

import javax.vecmath.Vector3f;

import org.omg.CORBA.Any;
import org.omg.CosEventComm.PushSupplier;
import org.omg.CosEventComm.PushSupplierHelper;
//import org.studierstube.opentracker.Shared;
//import org.studierstube.opentracker.Shared;

public class SharedPositionPush extends OTPushSupp {

	private static SharedPositionPush source;

	public SharedPositionPush(String[] args, String eventChannel) {
		super(args, eventChannel);
	}

	public SharedPositionPush(String[] args) {
		super(args, "EventChannel");
	}

	public SharedPositionPush(String eventChannel)
	{
		super(new String[0], eventChannel);
	}


	public static void main(String[] args) {
//		System.err.println(args.length);
		source = new SharedPositionPush("TestChannel");
		float[] pos_array = {(float)10.1, (float)20.2, (float)-3.3};
		source.pushPositionEventOntoEventChannel(pos_array);
	}

	public void pushPositionEventOntoEventChannel(float[] pos_array)
	{
		    PushSupplier ref = PushSupplierHelper.narrow(this._this());
			float theta = 0;
			while (true) {
				String[] pos = new String[1];
				pos_array[0] = (float) (0.1*Math.cos(theta));
				pos_array[1] = (float) (0.1*Math.sin(theta));
				pos_array[2] = (float) 0.0;
				pos[0] = pos_array[0] + " " + pos_array[1] + " " + pos_array[2];
				//pos[0] = new String("1.023 0.1234234 0.252342");
		//String pos = Float.toString(pos_array[0]) + " " 
				//FieldUpdate fu = new FieldUpdate(ref, pos);
				org.studierstube.opentracker.Shared.FieldUpdate fu = new org.studierstube.opentracker.Shared.FieldUpdate();
				fu.supplier = ref;
				fu.value = pos;
				Any ret = orb.create_any();
				org.studierstube.opentracker.Shared.FieldUpdateHelper.insert(ret, fu);
				System.out.println(ret.type());
				pushDataOntoEventChannel(ret);
				theta += 0.1;
				try {
					Thread.sleep(50);
				} catch (InterruptedException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
	}

	public void disconnect_push_supplier() {
		// TODO Auto-generated method stub

	}
}
