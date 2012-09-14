import sys
from omniORB import CORBA
import CosNaming
from naming import str2name
from copy import deepcopy

def getContextFromName(root_context, context_name):
    try:
	obj = root_context.resolve(context_name)
    except CosNaming.NamingContext.NotFound, ex:
	print "Name not found"
	return None
    sink_context = obj._narrow(obj)
    if sink_context is None:
	print "Object reference isn't a Context"
	return None
    return sink_context
    
def getContext(root_context, context_name):
    name = deepcopy(context_name)
    context_already_bound = None
    uncontextualised_names = []
    # terminate loop when `name' is non-existent or a bound context is found
    while name and not(context_already_bound):
	context_already_bound = getContextFromName(root_context, name)
	if context_already_bound is None:
	    uncontextualised_names.append(name.pop())
    uncontextualised_names.reverse() # reverse order of names
    print "name = ", name
    if not(name):
	context_already_bound = root_context
    previous_context = context_already_bound
    for n in uncontextualised_names:
	print "binding context with name ", n, " to context ", previous_context
	next_context = previous_context.bind_new_context([n])
	previous_context = next_context
    return previous_context

def initialiseORB():
    return CORBA.ORB_init(sys.argv, CORBA.ORB_ID)

def initialiseORBandPOA():
    orb = initialiseORB()
    poa = getRootPOA(orb)
    getActivePOAManager(poa)
    return orb, poa

def getRootContext(orb):
    # Obtain a reference to the root naming context
    obj         = orb.resolve_initial_references("NameService")
    rootContext = obj._narrow(CosNaming.NamingContext)

    if rootContext is None:
        print "Failed to narrow the root naming context"
        sys.exit(1)
    return rootContext

def getObjectReference(orb, name):
    rootContext = getRootContext(orb)
    obj = rootContext.resolve(name)
    return obj

def bindObjectReferenceToName(orb, obj, name):
    cos_name = str2name(name)
    root_context = getRootContext(orb)
    context = getContext(root_context, cos_name[:-1])
    print cos_name[-1], context
    try:
	root_context.bind(cos_name, obj)
	"Object bound"
    except CosNaming.NamingContext.AlreadyBound:
	print cos_name[-1]
	root_context.rebind(cos_name, obj)
	"Object rebound"

def getNarrowedObjectReference(orb, name, objectClass):
    obj = getObjectReference(orb, name)
    narrowed_ref = obj._narrow(objectClass)
    return narrowed_ref

def getRootPOA(orb):
    poa=orb.resolve_initial_references("RootPOA")
    poaManager=poa._get_the_POAManager()
    return poa

def getActivePOAManager(poa):
    poaManager=poa._get_the_POAManager()
    poaManager.activate()
    return poaManager

