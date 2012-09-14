//Package   : OpenTracker
//CorbaApp.java
//Created   : 2006/11/1
//Author    : Joseph Newman

//Copyright (C) 2006 Joseph Newman

//This file is part of the OpenTracker framework.

package org.studierstube.opentracker.tests;

import java.util.ArrayList;
import org.omg.CORBA.*;
import org.omg.CORBA.Object;
import org.omg.CORBA.ORBPackage.InvalidName;
import org.omg.CosNaming.NameComponent;
import org.omg.CosNaming.NamingContextExtHelper;
import org.omg.CosNaming.NamingContextExtOperations;
import org.omg.CosNaming.NamingContextPackage.AlreadyBound;
import org.omg.CosNaming.NamingContextPackage.CannotProceed;
import org.omg.CosNaming.NamingContextPackage.NotFound;
import org.omg.PortableServer.*;
import org.omg.PortableServer.POAManagerPackage.AdapterInactive;
import org.omg.PortableServer.POAPackage.ServantAlreadyActive;
import org.omg.PortableServer.POAPackage.WrongPolicy;

abstract public class CorbaApp
{
	//
	// class members
	static protected ORB orb;
	static protected POA rootPoa;
	static protected POAManager pman;
	static protected NamingContextExtOperations rootContext = null;

	static class NamingServiceFailure extends Exception {
		private static final long serialVersionUID = 4759765287644763254L;
	}
	static class NamingServiceUnavailable extends NamingServiceFailure {
		private static final long serialVersionUID = 6431140587106965650L;
	};
	static class ManagerActivationFailure extends Exception {
		private static final long serialVersionUID = 6431140587106965651L;
	};
	static class ObjectActivationFailure extends Exception {
		private static final long serialVersionUID = 6431140587106965652L;
	};

	public CorbaApp(String[] args) throws ManagerActivationFailure
	{
		// Must strip out ORB arguments manually in Java,
		// since ORB.init() doesn't do it for us.
		orb = getORB(args);

		String action=""; // Use this variable to help report errors.
		try {
			activateManager();
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

	static public POAManager getPOAManager() throws InvalidName {
		if (pman == null) {
			pman = getPOA().the_POAManager();
			return pman;
		}
		return pman;
	}

	static public NamingContextExtOperations getRootContext() throws NamingServiceUnavailable {
		if (rootContext == null) {
			try {
				org.omg.CORBA.Object obj = getORB().resolve_initial_references("NameService");
				rootContext = NamingContextExtHelper.narrow(obj);
				if (rootContext == null)
					throw new OBJECT_NOT_EXIST();
			} catch (InvalidName e) {
				System.err.println("Unable to contact NameService.");
				throw new NamingServiceUnavailable();
			} catch (COMM_FAILURE e) {
				System.err.println("Caught COMM_FAILURE");
				throw new NamingServiceUnavailable();
			}
		}
		return rootContext;
	}

	static public Object getObjectReference(String name) throws NamingServiceFailure {
		NameComponent[] nc = {};
		try {
			nc = getRootContext().to_name(name);
			return getRootContext().resolve(nc);
		} catch (org.omg.CosNaming.NamingContextPackage.InvalidName e) {
			throw new NamingServiceFailure();
		} catch (NotFound e) {
			throw new NamingServiceFailure();
		} catch (CannotProceed e) {
			throw new NamingServiceFailure();
		}
	}

	static public void bindObectToName(org.omg.CORBA.Object obj, String name) throws NamingServiceUnavailable {
		NameComponent[] nc = {};
		try {
			NamingContextExtOperations context = getRootContext();
			nc = context.to_name(name);
			try {
				context.bind(nc, obj);
			} catch (AlreadyBound e) {
				context.rebind(nc, obj);
			}
		} catch (org.omg.CosNaming.NamingContextPackage.InvalidName e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (NotFound e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (CannotProceed e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}


	static public void activateManager() throws ManagerActivationFailure {
		try {
			getPOAManager().activate();
		} catch (AdapterInactive e) {
			throw new ManagerActivationFailure();
		} catch (InvalidName e) {
			throw new ManagerActivationFailure();
		}
	}

	static public void activateObject(Servant servant) throws ObjectActivationFailure {
		try {
			CorbaApp.getPOA().activate_object(servant);
		} catch (ServantAlreadyActive e) {
			throw new ObjectActivationFailure();
		} catch (WrongPolicy e) {
			throw new ObjectActivationFailure();
		} catch (InvalidName e) {
			throw new ObjectActivationFailure();
		}
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
		//String other_args[] = stripOrbArgs(args);

		// Process Options
		String hostName    ="localhost";

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



} // end class CorbaApp
