//Package   : OpenTracker
//pushcons.cc              Created   : 2006/11/1
//Author    : Joseph Newman

//Copyright (C) 2006 Joseph Newman

//This file is part of the OpenTracker framework.


//Description:
//Push Model consumer implementation borrowed from Alex Tingle


package org.studierstube.opentracker.tests;

import java.util.ArrayList;
import gnu.getopt.Getopt;
import org.omg.CORBA.*;
import org.omg.CORBA.ORBPackage.InvalidName;
import org.omg.PortableServer.*;
import org.omg.PortableServer.POAManagerPackage.AdapterInactive;
import org.omg.PortableServer.POAPackage.ServantAlreadyActive;
import org.omg.PortableServer.POAPackage.WrongPolicy;
import org.omg.CosNaming.*;
import org.omg.CosNaming.NamingContextPackage.*;
import org.omg.CosEventComm.*;
import org.omg.CosEventChannelAdmin.*;

abstract public class OTPushSupp extends PushSupplierPOA
{
	//
	// class members
	protected int _disconnect =0;
	protected int _count =0;
	protected int sleepInterval =0;
	protected int l=0;
	protected boolean _disconnectFlag =false;
	protected SupplierAdmin supplier_admin;
	protected ProxyPushConsumer proxy_consumer;
	protected EventChannel channel =null;

	static protected ORB orb;
	static protected POA rootPoa;
	static protected POAManager pman;

	public OTPushSupp(String[] args, String channelName)
	{
		// Must strip out ORB arguments manually in Java,
		// since ORB.init() doesn't do it for us.
		@SuppressWarnings("unused")
		String other_args[] = stripOrbArgs(args);

		//orb =ORB.init(orb_args, null);
		orb = getORB(args);

		String action=""; // Use this variable to help report errors.
		try {
			this.activate();
			activateManager();
			channel = getEventChannel(channelName);

		}
		catch(org.omg.CORBA.ORBPackage.InvalidName ex) { // resolve_initial_references
			System.err.println("Failed to "+action+". ORB::InvalidName");
			System.exit(1);
		}
		catch(TRANSIENT ex) { // _narrow()
			System.err.println("Failed to "+action+". TRANSIENT");
			System.exit(1);
		}
		catch(OBJECT_NOT_EXIST ex) { // _narrow()
			System.err.println("Failed to "+action+". OBJECT_NOT_EXIST");
			System.exit(1);
		}
		catch (SystemException ex) {
			System.err.println("System exception, unable to "+action);
			System.exit(1);
		}
		catch (UserException ex) {
			System.err.println("CORBA exception, unable to "+action);
			System.exit(1);
		}
		supplier_admin = getSupplierAdmin(channel);
		proxy_consumer = getProxyPushConsumer(supplier_admin);
		connectPushSupplier((PushSupplier) this._this(), proxy_consumer);

	}

	public void pushDataOntoEventChannel(Any any) {
	      try {
	        System.out.print("Push Supplier: push() called. ");
	        proxy_consumer.push(any);
	        System.out.println("Data : "+(l-1));
	      }
	      catch(java.lang.Exception ex) {
	        System.out.println("Failed!");
	      }
	}


	public static void connectPushSupplier(PushSupplier push_supplier, ProxyPushConsumer proxy_consumer) {
	      //
	      // Connect Push Supplier - retrying on Comms Failure.

	      while(true)
	      {
	        try {
	          proxy_consumer.connect_push_supplier(push_supplier);
	          break;
	        }
	        catch(BAD_PARAM ex) {
	          System.err.println(
	            "Caught BAD_PARAM exception connecting Push Supplier!");
	          System.exit(1);
	        }
	        catch(AlreadyConnected ex) {
	          System.err.println("Proxy Push Consumer already connected!");
	          break;
	        }
	        catch(COMM_FAILURE ex) {
	          System.err.println("Caught COMM_FAILURE exception "
	            +"connecting Push Supplier! Retrying...");
	          continue;
	        }
	      }
	      System.out.println("Connected Push Supplier.");
	}

	public static SupplierAdmin getSupplierAdmin(EventChannel channel) {
		SupplierAdmin supplier_admin;
	    while(true)
	    {
	      try {
	        supplier_admin=channel.for_suppliers();
	        if(supplier_admin==null)
	        {
	          System.err.println("Event Channel returned nil Supplier Admin!");
	          System.exit(1);
	        }
	        break;
	      }
	      catch (COMM_FAILURE ex) {
	         System.err.println("Caught COMM_FAILURE exception "
	           +"obtaining Supplier Admin! Retrying...");
	         continue;
	      }
	    }
	    System.out.println("Obtained SupplierAdmin.");
	    return supplier_admin;
	}



	public static ProxyPushConsumer getProxyPushConsumer(SupplierAdmin supplier_admin) {
		ProxyPushConsumer proxy_consumer;
		//
		// Get proxy consumer - retrying on Comms Failure.
		while(true)
		{
			try {
				proxy_consumer=supplier_admin.obtain_push_consumer();
				if(proxy_consumer==null)
				{
					System.err.println("Supplier Admin returned nil proxy_consumer!");
					System.exit(1);
				}
				break;
			}
			catch(COMM_FAILURE ex) {
				System.err.println("Caught COMM_FAILURE exception "
						+"obtaining Push Consumer! Retrying...");
				continue;
			}
		}
		System.out.println("Obtained ProxyPushConsumer.");
		return proxy_consumer;
	}

	static public POA getPOA() throws InvalidName {
		if( rootPoa == null ) {
			org.omg.CORBA.Object obj = getORB().resolve_initial_references("RootPOA");
			rootPoa =POAHelper.narrow(obj);
			if(rootPoa==null)
				throw new OBJECT_NOT_EXIST();
		}
		return rootPoa;
	}

	static public POAManager getPOAManager() {
		if (pman == null) {
			// do nothing
			pman = rootPoa.the_POAManager();
			return pman;
		}
		return pman;
	}

	static public void activateManager() throws AdapterInactive {
		getPOAManager().activate();
	}

	static public ORB getORB() {
		if (orb == null) {
			String args[] = {};
			return initializeORB(args);
		}
		return orb;
	}

	static public ORB getORB(String args[]) {
		if (orb == null) {
			return initializeORB(args);
		} else {
			return orb;
		}
	}

	static public ORB initializeORB(String args[]) {
        // Must strip out ORB arguments manually in Java,
		// since ORB.init() doesn't do it for us.
		String orb_args[]   = filterOrbArgs(args);
		String other_args[] = stripOrbArgs(args);

		// Process Options
		String hostName = "herceg.cl.cam.ac.uk";

		Getopt g =new Getopt("eventc", other_args,"h:");
		int c;
		while ((c = g.getopt()) != -1)
		{
			switch (c)
			{
			case 'h': hostName=g.getOptarg();
			break;
			}
		}
		// If -ORBInitRef not present then make a best guess based on hostName
		ArrayList<String> orb_args_list = new ArrayList<String>(java.util.Arrays.asList(orb_args));
		if (!orb_args_list.contains("-ORBInitRef")) {
			System.err.println("Does not contain InitRef argument");
			orb_args_list.add("-ORBInitRef");
			orb_args_list.add("NameService=corbaloc::" + hostName + ":2809/NameService");
		} else {
			System.err.println("Contains InitRef argument");
		}
		//
		// Start orb.
		orb_args = orb_args_list.toArray(new String[0]);
		return ORB.init(orb_args, null);
	}

	public void activate() throws ServantAlreadyActive, WrongPolicy, InvalidName {
		getPOA().activate_object(this);
	}

	public static EventChannel getEventChannel(String channelName) {
		//
		// Obtain object reference to EventChannel
		// (from command-line argument or from the Naming Service).
		String action="";
		EventChannel _channel;
		org.omg.CORBA.Object obj = null;
		try {
				action="resolve initial reference 'NameService'";
				NamingContextExtOperations rootContext = null;
				try {
					obj=getORB().resolve_initial_references("NameService");
					rootContext=NamingContextExtHelper.narrow(obj);
				} catch (org.omg.CORBA.COMM_FAILURE ex) {
					System.err.println("Error occured whilst trying to contact the NameService. Is it running?");
					System.exit(1);
				}
				if(rootContext==null)
					throw new OBJECT_NOT_EXIST();

				NameComponent name[] = rootContext.to_name(channelName);

				action="find EventChannel in NameService";
				System.out.println(action);
				obj=rootContext.resolve(name);

			action="narrow object reference to event channel";
			_channel=EventChannelHelper.narrow(obj);
			if(_channel==null)
			{
				System.err.println("Failed to narrow Event Channel reference.");
				System.exit(1);
			}
			return _channel;
		} catch(org.omg.CosNaming.NamingContextPackage.InvalidName ex) { // resolve
			System.err.println("Failed to "+action+". NamingContext::InvalidName");
			System.exit(1);
		}
		catch(NotFound ex) { // resolve
			System.err.println("Failed to "+action+". NamingContext::NotFound");
			System.exit(1);
		}
		catch(CannotProceed ex) { // resolve
			System.err.println("Failed to "+action+". NamingContext::CannotProceed");
			System.exit(1);
		}
		catch(TRANSIENT ex) { // _narrow()
			System.err.println("Failed to "+action+". TRANSIENT");
			System.exit(1);
		}
		catch(OBJECT_NOT_EXIST ex) { // _narrow()
			System.err.println("Failed to "+action+". OBJECT_NOT_EXIST");
			System.exit(1);
		}
		catch (SystemException ex) {
			System.err.println("System exception, unable to "+action);
			System.exit(1);
		}
		catch (UserException ex) {
			System.err.println("CORBA exception, unable to "+action);
			System.exit(1);
		}

		return null;
	}



	static String[] stripOrbArgs(String[] args)
	{
		java.util.ArrayList<String> orb_arg_list = new ArrayList<String>();
		for(int i=0; i<args.length; ++i)
		{
			if(!args[i].startsWith("-ORB"))
				orb_arg_list.add(args[i]);
		}
		String orb_args[] = orb_arg_list.toArray(new String[0]);
		return orb_args;
	}

	static String[] filterOrbArgs(String[] args)
	{
		java.util.ArrayList<String> filtered_arg_list = new ArrayList<String>();
		for(int i=0; i<args.length; ++i)
		{
			if(args[i].startsWith("-ORB"))
				filtered_arg_list.add(args[i]);
		}
		String filtered_args[] = filtered_arg_list.toArray(new String[0]);
		return filtered_args;
	}

} // end class PushCons
