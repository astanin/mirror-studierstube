import sys
from time import sleep

from omniORB import CORBA, any

import OTGraph
from OTGraph import *

from itertools import izip

# Import the stubs for the Naming service
import CosNaming

def initialiseORB():
    return CORBA.ORB_init(sys.argv, CORBA.ORB_ID)

def getGraphReference(orb):
    # Obtain a reference to the root naming context
    obj         = orb.resolve_initial_references("NameService")
    rootContext = obj._narrow(CosNaming.NamingContext)

    if rootContext is None:
        print "Failed to narrow the root naming context"
        sys.exit(1)

    name = [CosNaming.NameComponent("Foo", "Test")]

    obj = rootContext.resolve(name)
    graph_ref = obj._narrow(OTGraph.DataFlowGraph)
    return graph_ref

def init():
    orb = initialiseORB()
    return orb, getGraphReference(orb)

def gotoSleep(t):
    whole_seconds = int(t)
    frac_second   = t % 1
    for i in range(0, whole_seconds):
	sleep(1)
	print ".", 
	sys.stdout.flush()
    sleep(frac_second)
    print "."

if __name__ == '__main__':
    # Initialise the ORB
    orb, graph = init()
    sources = []
    sinks = []
    for i in range(0,1):
        source = graph.create_node("TestSource", [KeyValuePair("frequency", "10"), KeyValuePair("noise", "0.1"), KeyValuePair("frequency", "10"), KeyValuePair("DEF", "Node"+str(i))])
	sources += [source]
	sinks += [graph.create_node("CORBASink",[KeyValuePair("name","CORBA.Sink01")])]

    for source, sink in izip(sources, sinks):
	graph.connect_nodes(source, sink)
    print "sleeping for five seconds"
    gotoSleep(5)
    print "woken up"
    for source, sink in izip(sources, sinks):
        print source.get_type(), source.get_id()
        print sink.get_type(), sink.get_id()
	graph.disconnect_nodes(source, sink)
	graph.remove_node(source)
#        print source.get_type(), source.get_id()
	graph.remove_node(sink)
    

