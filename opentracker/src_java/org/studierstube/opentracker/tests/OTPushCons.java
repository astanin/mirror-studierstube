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

abstract public class OTPushCons extends PushConsumerPOA
{
	//
	// class members
	protected int _disconnect =0;
	protected int _count =0;
	protected int discnum =0;
	protected int sleepInterval =0;
	protected boolean _disconnectFlag =false;
	protected ConsumerAdmin consumer_admin;
	protected EventChannel channel =null;
	
	static protected ORB orb;
	static protected POA rootPoa;
	static protected POAManager pman;

	public OTPushCons(String[] args)
	{
		// Must strip out ORB arguments manually in Java,
		// since ORB.init() doesn't do it for us.
		String other_args[] = stripOrbArgs(args);

		// Process Options
		String channelName ="EventChannel";
		
		Getopt g =new Getopt("eventc", other_args,"ud:s:n:");
		int c;
		while ((c = g.getopt()) != -1)
		{
			switch (c)
			{
			case 'd': Integer discnumObj =new Integer(g.getOptarg());
			discnum=discnumObj.intValue();
			break;

			case 's': Integer sleepIntervalObj =new Integer(g.getOptarg());
			sleepInterval=sleepIntervalObj.intValue();
			break;

			case 'n': channelName=g.getOptarg();
			break;

			case 'u':
			default : usage();
			System.exit(-1);
			break;
			}
		}
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
		consumer_admin = getConsumerAdmin(channel);
	}

	/** Used to pass a signal from the CORBA message processing thread
	 *  (in push() method) to the main thread. In C++ & Python we use a
	 * condition variable. Java doesn't have such luxury, so we use a
	 * flag (_disconnectFlag) instead.
	 */
	protected synchronized void setDisconnectFlag(boolean value)
	{
		_disconnectFlag=value;
		notifyAll(); // Wakes up threads stuck at wait()
	}

	/** Wait for a signal to disconnect. */
	protected synchronized void waitDisconnect()
	{
		while(_disconnectFlag==false)
		{
			try {
				wait(); // Sleep until notifyAll() is called.
			} catch(InterruptedException ex) { }
		}
	}

	/** The one and only PushCons object */
	//static protected OTPushCons consumer =null;

	//static ConsumerAdmin consumer_admin =null;
	


	static public ConsumerAdmin getConsumerAdmin(EventChannel channel) {
		//
		// Get Consumer admin interface - retrying on Comms Failure.
		ConsumerAdmin consumer_admin;
		while(true)
		{
			try {
				consumer_admin=channel.for_consumers ();
				if	(consumer_admin==null)
				{	
					System.err.println(
					"Event Channel returned nil Consumer Admin!");
					System.exit (1);
				}
				break;
			}
			catch (COMM_FAILURE ex) {
				System.err.println("Caught COMM_FAILURE exception "
						+"obtaining Consumer Admin! Retrying...");
				continue;
			}
		}
		System.out.println("Obtained ConsumerAdmin.");
		return consumer_admin;

	}

	static public void connectPushConsumer(ProxyPushSupplier proxy_supplier, PushConsumerPOA consumer) {
		//
		// Connect Push Consumer - retrying on Comms Failure.
		while(true)
		{
			try {
				proxy_supplier.connect_push_consumer(consumer._this());
				break;
			}
			catch(BAD_PARAM ex) {
				System.err.println(
				"Caught BAD_PARAM exception connecting Push Consumer!");
				System.exit(1);
			}
			catch(AlreadyConnected ex) {
				System.err.println("Proxy Push Supplier already connected!");
				break;
			}
			catch(TypeError ex) {
				System.err.println(
				"Caught TypeError connecting Push Consumer!");
				break;
			}
			catch (COMM_FAILURE ex) {
				System.err.println("Caught COMM_FAILURE exception "
						+"connecting Push Consumer! Retrying...");
				continue;
			}
		}
		System.out.println("Connected Push Consumer.");
	}

	public static void disconnectPushConsumer(ProxyPushSupplier proxy_supplier) {
		// Disconnect - retrying on Comms Failure.
		while(true)
		{
			try {
				proxy_supplier.disconnect_push_supplier();
				break;
			}
			catch(COMM_FAILURE ex) {
				System.err.println("Caught COMM_FAILURE exception "
						+"disconnecting Push Consumer! Retrying...");
				continue;
			}
		}
		System.out.println("Disconnected Push Consumer.");
	}

	public static ProxyPushSupplier getProxyPushSupplier(ConsumerAdmin consumer_admin) {
		ProxyPushSupplier proxy_supplier;
		//
		// Get proxy supplier - retrying on Comms Failure.
		while(true)
		{
			try {
				proxy_supplier=consumer_admin.obtain_push_supplier();
				if(proxy_supplier==null)
				{
					System.err.println(
					"Consumer Admin returned nil proxy_supplier!");
					System.exit(1);
				}
				break;
			}
			catch(COMM_FAILURE ex) {
				System.err.println("Caught COMM_FAILURE Exception "
						+"obtaining Push Supplier! Retrying...");
				continue;
			}
		}
		System.out.println("Obtained ProxyPushSupplier.");
		return proxy_supplier;

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
		String hostName = "localhost";
		
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
			orb_args_list.add("-ORBInitRef");
			orb_args_list.add("NameService=corbaloc::" + hostName + ":2809/NameService");
		}
		//
		// Start orb.
		orb_args = orb_args_list.toArray(new String[0]);
		return ORB.init(orb_args, null);
	}
	
	public void activate() throws ServantAlreadyActive, WrongPolicy, InvalidName {
		getPOA().activate_object(this);
	}
	
	public void mainLoop() {
		mainLoop(0);
	}
	
	public void mainLoop(int sleepInterval)
	{


		while(true)
		{
			// Connect and listen for events.
			connect(consumer_admin);
			// Disconnected... reconnect!

			// Yawn.
			System.out.println("Sleeping "+sleepInterval+" seconds.");
			try {
				java.lang.Thread.sleep(sleepInterval*1000);
			}
			catch(java.lang.InterruptedException ex) {
				System.err.println("Sleep interrupted.");
				// just carry on...
			}
		}

		// NEVER GET HERE

	} // end main()

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

	static private void connect(ConsumerAdmin consumer_admin, ProxyPushSupplier proxy_supplier, OTPushCons consumer) {
		connectPushConsumer(proxy_supplier, consumer);
	}
	
	public void connect() {
		ProxyPushSupplier proxy_supplier = getProxyPushSupplier(consumer_admin);
		connect(consumer_admin, proxy_supplier, this);
	}
	
	public void disconnect() {
		ProxyPushSupplier proxy_supplier = getProxyPushSupplier(consumer_admin);
		disconnectPushConsumer(proxy_supplier);
	}
	
	/** The Java synchronization methods we use must be called from within the
	 *  object to be synchronized. Therefore we must do the work from an
	 * instance method, rather than from main().
	 */
	private void connect(ConsumerAdmin consumer_admin)
	{
		ProxyPushSupplier proxy_supplier = getProxyPushSupplier(consumer_admin);
		setDisconnectFlag(false);

		connectPushConsumer(proxy_supplier, this);
		// Wait for indication to disconnect before re-connecting.
		waitDisconnect();

		disconnectPushConsumer(proxy_supplier);
	} // end connect()

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


	static void
	usage()
	{
		System.err.println(
				"\nCreate a PushConsumer to receive events from a channel.\n"
				+"syntax: java PushCons OPTIONS [CHANNEL_URI]\n"
				+"\n"
				+"CHANNEL_URI: The event channel may be specified as a URI.\n"
				+" This may be an IOR, or a corbaloc::: or corbaname::: URI.\n"
				+"\n"
				+"OPTIONS:                                         DEFAULT:\n"
				+" -d NUM   disconnect after receiving NUM events   [0 - never disconnect]\n"
				+" -s SECS  sleep SECS seconds after disconnecting  [0]\n"
				+" -n NAME  channel name (if URI is not specified)  [\"EventChannel\"]\n"
				+" -h NAME  host name (if ORBInitRef not specified)  [\"localhost\"]\n"				
				+" -u       display this help text\n");
	}
} // end class PushCons
