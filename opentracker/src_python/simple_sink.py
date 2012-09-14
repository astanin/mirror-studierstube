#!/usr/bin/env python
import sys
sys.path.append('../idl/skeletons')
from copy import deepcopy
from omniORB import CORBA, PortableServer, PortableServer__POA
from omniORB import any
from omniORBUtils import *

# Import the stubs for the Naming service
import CosNaming

# Import the stubs and skeletons for the Example module
import OT_CORBA, OT_CORBA__POA

# Define an implementation of the Echo interface
class SimpleSink_i (OT_CORBA__POA.OTEntity):
    def setEvent(self, event):
	print "**************************"
	print event
	for att in event:
	    print "%s:\t%s" % (att.name, str(any.from_any(att.value)))

if __name__ == '__main__':
    # Initialise the ORB
    string_name = sys.argv[1]
    if len(sys.argv) > 2:
	endpoint_port = sys.argv[2]
	sys.argv.extend(["-ORBendPoint", "giop:tcp:localhost:" + endpoint_port])
    orb = CORBA.ORB_init(sys.argv, CORBA.ORB_ID)

    # Find the root POA
    poa = orb.resolve_initial_references("RootPOA")
    poaManager = poa._get_the_POAManager()
    
    # Create a child POA with the right policies 
    ps = [poa.create_id_assignment_policy(PortableServer.USER_ID),
	  poa.create_lifespan_policy(PortableServer.PERSISTENT)]
    persistentPOA = poa.create_POA("childPOA", poaManager, ps)
    childManager = persistentPOA._get_the_POAManager()

    # Create an instance of SimpleSink_i
    servant = SimpleSink_i()

    # don't implicitly activate the object
    #sinkref = servant._this()
    #oid = PortableServer.string_to_ObjectId("my object");
    #persistentPOA.activate_object(servant)
    persistentPOA.activate_object_with_id("test", servant);
    sinkref = servant._this()
    
    print orb.object_to_string(sinkref)
    
    # Obtain a reference to the root naming context
    obj         = orb.resolve_initial_references("NameService")
    extContext  = obj._narrow(CosNaming.NamingContextExt)

    if extContext is None:
	print "Failed to narrow the root naming context"
	sys.exit(1)

    name = extContext.to_name(string_name)
    context_name = name[:-1]
    sink_name = name[-1:]
    sink_context = getContext(extContext, context_name)

    # Bind the SimpleSink `sink_name' object to the appropriate context
    try:
	sink_context.bind(sink_name, sinkref)
	print "New Sink object bound"
    except CosNaming.NamingContext.AlreadyBound:
	sink_context.rebind(sink_name, sinkref)
	print "Sink binding already existed -- rebound"

	# Note that is should be sufficient to just call rebind() without
	# calling bind() first. Some Naming service implementations
	# incorrectly raise NotFound if rebind() is called for an unknown
	# name, so we use the two-stage approach above

    # Activate the POA
    childManager.activate()
    poaManager.activate()

    # Everything is running now, but if this thread drops out of the end
    # of the file, the process will exit. orb.run() just blocks until the
    # ORB is shut down
    orb.run()
