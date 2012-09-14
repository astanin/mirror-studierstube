/* MutableGraph.cs
 * 
 */

using System;
using System.Runtime.Remoting.Channels;
using System.Runtime.Remoting;
using Ch.Elca.Iiop;
using Ch.Elca.Iiop.Services;
using omg.org.CosNaming;
using omg.org.CosNaming.NamingContext_package;
using omg.org.CORBA;
using OTGraph;

namespace opentracker.examples.MutableGraph.mono {

    public class MutableGraphClient {

        [STAThread]
        public static void Main(string[] args) {
	  try {
                // register the channel
                IiopClientChannel channel = new IiopClientChannel();
                ChannelServices.RegisterChannel(channel);

		Object obj = new Object();
		try {
		// access COS naming Service
		NamingContext nameService = (NamingContext)RemotingServices.Connect(typeof(NamingContext), "corbaloc::localhost:2809/NameService");
		Console.WriteLine("Got reference to nameService");

                // get the reference to the Graph
 		NameComponent[] name = new NameComponent[] { new NameComponent("Foo", "Test") };
		Console.WriteLine("About to resolve Foo.Test");
 		obj = (DataFlowGraph) nameService.resolve(name);
		Console.WriteLine("Got reference to graph");
		} catch (NotFound e) {
		  Console.WriteLine("Could not find graph in NameService");
		  return;
		} catch (TRANSIENT e) {
		  Console.WriteLine("Could not contact NameService");
		  return;
		}
		
		if (obj == null) {
		  Console.WriteLine("Reference to graph is null...");
		  return;
		}

		DataFlowGraph graph = (DataFlowGraph) obj;
 		KeyValuePair[] sink_attributes = new KeyValuePair[] { new KeyValuePair("name", "CORBA.Sink01") };
 		Console.WriteLine("About to create node");
		Node sink = (Node) graph.create_node("CORBASink", sink_attributes);
 		Console.WriteLine("Created CORBA sink node");
 		Node[] nodes = graph.get_nodes();
 		foreach (Node n in nodes) {
		  Console.WriteLine(n.get_attribute("ID"));
 		}
	  } catch (NotFound e) {
	    Console.WriteLine("Could not find graph in NameService");
	  } catch (TRANSIENT e) {
	    Console.WriteLine("Could not contact graph");
	  } catch (Exception e) {
	    Console.WriteLine("exception: " + e);
	  } 

        }

    }
}