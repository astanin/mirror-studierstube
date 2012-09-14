import sys

from omniORB import CORBA, any

import OTGraph
from OTGraph import *

# Import the stubs for the Naming service
import CosNaming

from omniORBUtils import *

def getGraphReference(orb):
    return getNarrowedObjectReference(orb, 
				      [CosNaming.NameComponent("Foo", "Test")],
				      OTGraph.DataFlowGraph)
def init():
    orb = initialiseORB()
    return orb, getGraphReference(orb)

if __name__ == '__main__':
    # Initialise the ORB
    orb, graph = init()
    n1=graph.create_node("CORBASink",[KeyValuePair("name","CORBA.Sink01")])
    n2=graph.create_node("TestSource", [KeyValuePair("frequency", "100"), KeyValuePair("noise", "0.1")])
    graph.connect_nodes(n2, n1)
