package org.studierstube.opentracker.tests;

//                            Package   : omniEvents
//   PushSupp.cc              Created on: 2004/03/12
//                            Author    : Alex Tingle
//
//    Copyright (C) 2004 Alex Tingle
//
//    This file is part of the omnievents application.
//
//    omniEvents is free software; you can redistribute it and/or
//    modify it under the terms of the GNU Lesser General Public
//    License as published by the Free Software Foundation; either
//    version 2.1 of the License, or (at your option) any later version.
//
//    omniEvents is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//    Lesser General Public License for more details.
//
//    You should have received a copy of the GNU Lesser General Public
//    License along with this library; if not, write to the Free Software
//    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// Description:
//    Push Model supplier implementation.
//

import gnu.getopt.Getopt;
import org.omg.CORBA.*;
import org.omg.PortableServer.*;
import org.omg.CosNaming.*;
import org.omg.CosNaming.NamingContextPackage.*;
import org.omg.CosEventComm.*;
import org.omg.CosEventChannelAdmin.*;
import net.sourceforge.omniorb.EventChannelAdmin.*;

public class PushSupp extends PushSupplierPOA
{
  /** CORBA method implementation. */
  public void
  disconnect_push_supplier()
  {
    System.out.println("Push Supplier: disconnected.");
  }

  public static void main(String args[])
  {
    int l =0;
    //
    // Start orb.
    ORB orb =ORB.init(args, null);
    // Must strip out ORB arguments manually in Java,
    // since ORB.init() doesn't do it for us.
    args=stripOrbArgs(args);

    // Process Options
    int discnum =0;
    boolean refnil =false;
    int sleepInterval =0;
    String channelName ="EventChannel";
    String channelKind ="EventChannel";

    Getopt g =new Getopt("eventc",args,"d:rs:n:k:h");
    int c;
    while ((c = g.getopt()) != -1)
    {
      switch (c)
      {
          case 'd': Integer discnumObj =new Integer(g.getOptarg());
                    discnum=discnumObj.intValue();
                    break;

          case 'r': refnil=true;
                    break;

          case 's': Integer sleepIntervalObj =new Integer(g.getOptarg());
                    sleepInterval=sleepIntervalObj.intValue();
                    break;

          case 'n': channelName=g.getOptarg();
                    break;

          case 'k': channelKind=g.getOptarg();
                    break;

          case 'h':
          default : usage();
                    System.exit(-1);
                    break;
       }
    }

    PushSupp supplier =null;
    EventChannel channel =null;

    String action=""; // Use this variable to help report errors.
    try {
      org.omg.CORBA.Object obj;

      // A Push Supplier can be implemented as a pure client or as a mixed
      // client-server process, depending on whether it requires and is
      // prepared to service disconnect requests from the channel.
      // If its is then create the servant object and activate the POA.
      if(!refnil)
      {
        action="resolve initial reference 'RootPOA'";
        obj=orb.resolve_initial_references("RootPOA");
        POA rootPoa =POAHelper.narrow(obj);
        if(rootPoa==null)
            throw new OBJECT_NOT_EXIST();

        action="activate the PushSupplier";
        supplier=new PushSupp();
        rootPoa.activate_object(supplier);

        action="activate the RootPOA's POAManager";
        POAManager pman =rootPoa.the_POAManager();
        pman.activate();
      }

      //
      // Obtain object reference to EventChannel
      // (from command-line argument or from the Naming Service).
      if(g.getOptind()<args.length)
      {
        action="convert URI from command line into object reference";
        obj=orb.string_to_object(args[g.getOptind()]);
      }
      else
      {
        action="resolve initial reference 'NameService'";
        obj=orb.resolve_initial_references("NameService");
        NamingContext rootContext=NamingContextHelper.narrow(obj);
        if(rootContext==null)
            throw new OBJECT_NOT_EXIST();

        NameComponent name[] ={ new NameComponent(channelName,channelKind) };

        action="find EventChannel in NameService";
        System.out.println(action);
        obj=rootContext.resolve(name);
      }

      action="narrow object reference to event channel";
      channel=EventChannelHelper.narrow(obj);
      if(channel==null)
      {
        System.err.println("Failed to narrow Event Channel reference.");
        System.exit(1);
      }

    }
    catch(org.omg.CORBA.ORBPackage.InvalidName ex) { // resolve_initial_references
      System.err.println("Failed to "+action+". ORB::InvalidName");
      System.exit(1);
    }
    catch(org.omg.CosNaming.NamingContextPackage.InvalidName ex) { // resolve
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

    //
    // Get Supplier Admin interface - retrying on Comms Failure.
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

    while(true)
    {
      //
      // Get proxy consumer - retrying on Comms Failure.
      ProxyPushConsumer proxy_consumer;
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

      //
      // Connect Push Supplier - retrying on Comms Failure.
      PushSupplier sptr =null;
      if(!refnil) {
        sptr=supplier._this();
      }

      while(true)
      {
        try {
          proxy_consumer.connect_push_supplier(sptr);
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

      // Push data.
      for(int i=0; (discnum == 0) || (i < discnum); i++)
      {
        Any any =orb.create_any();
        any.insert_ulong(l++);
        try {
          System.out.print("Push Supplier: push() called. ");
          proxy_consumer.push(any);
          System.out.println("Data : "+(l-1));
        }
        catch(java.lang.Exception ex) {
          System.out.println("Failed!");
        }
      }

      // Disconnect - retrying on Comms Failure.
      while(true)
      {
        try {
          proxy_consumer.disconnect_push_consumer();
          break;
        }
        catch(COMM_FAILURE ex) {
          System.err.println("Caught COMM_FAILURE exception "
            +"disconnecting Push Supplier! Retrying...");
          continue;
        }
      }
      System.out.println("ProxyPushConsumer disconnected.");

      // Yawn.
      System.out.println("Sleeping "+sleepInterval+" seconds.");
      try {
        java.lang.Thread.sleep(sleepInterval*1000);
      }
      catch(java.lang.InterruptedException ex) {
        System.err.println("Sleep interrupted.");
        // just carry on...
      }

    } // end loop while(true)
  } // end main()


  static String[]
  stripOrbArgs(String[] args)
  {
    int len =0;
    for(int i=0; i<args.length; ++i)
    {
      if(args[i].startsWith("-ORB"))
          ++i; // Skip this arg AND the next one.
      else
          args[len++]=args[i]; // keep this arg.
    }
    String[] result =new String[len];
    System.arraycopy(args,0,result,0,len);
    return result;
  }


  static void
  usage()
  {
    System.err.println(
 "\nCreate a PushSupplier to send events to a channel.\n"
+"syntax: java PushSupp OPTIONS [CHANNEL_URI]\n"
+"\n"
+"CHANNEL_URI: The event channel may be specified as a URI.\n"
+" This may be an IOR, or a corbaloc::: or corbaname::: URI.\n"
+"\n"
+"OPTIONS:                                         DEFAULT:\n"
+" -d NUM   disconnect after sending NUM events     [0 - never disconnect]\n"
+" -r       connect using a nil reference\n"
+" -s SECS  sleep SECS seconds after disconnecting  [0]\n"
+" -n NAME  channel name (if URI is not specified)  [\"EventChannel\"]\n"
+" -k KIND  channel kind (if URI is not specified)  [\"EventChannel\"]\n"
+" -h       display this help text\n");
  }
} // end class PushSupp
