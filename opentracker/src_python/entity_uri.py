#!/usr/bin/env python
import sys, time, math
sys.path.append('../idl/skeletons')

from omniORB import CORBA, any

# Import the stubs for the Naming service
import CosNaming

# Import the stubs and skeletons for the Example module
import OT_CORBA, OT_CORBA__POA

if __name__ == '__main__':
    # Initialise the ORB
    orb = CORBA.ORB_init(sys.argv, CORBA.ORB_ID)
    if (len(sys.argv) !=2) and (len(sys.argv) !=3):
	print "Usage: source_uri.py uri <rate>"
	print "e.g.: source_uri.py corbaloc:iiop:localhost:5555/MyObjectId 15"
	sys.exit(-1)

    obj = orb.string_to_object(sys.argv[1])
    source_ref = obj._narrow(OT_CORBA.OTEntity)
    if source_ref is None:
        print "Object found in NamingService is not of type OTSource"
        sys.exit(-1)

    try:
        rate = float(sys.argv[2])
    except IndexError:
        rate = 2.0
    previous_time = time.time()
    period = 1.0/rate;
    omega = 1;
    r = 40
    while True:
        t = time.time()
	ev = [OT_CORBA.EventAttribute("position", CORBA.Any(CORBA.TypeCode("IDL:OT_CORBA/FloatVector:1.0"), [r*math.cos(omega*t), r*math.sin(omega*t),0.5])),
              OT_CORBA.EventAttribute("orientation", CORBA.Any(CORBA.TypeCode("IDL:OT_CORBA/FloatVector:1.0"), [math.cos(omega*t), 0.0, 0.0, math.sin(omega*t)])),
              OT_CORBA.EventAttribute("foo", any.to_any("bar")),
              OT_CORBA.EventAttribute("confidence", CORBA.Any(CORBA.TC_float, 0.053)),
              OT_CORBA.EventAttribute("timestamp", CORBA.Any(CORBA.TC_float, 1001.323)),
              OT_CORBA.EventAttribute("button", CORBA.Any(CORBA.TC_ushort, 6))]#,[math.cos(math.pi/4), 0.0, math.sin(math.pi/4), 0.0], time.time()*1000.0, 0, 0.2)
	try:
	    source_ref.setEvent(ev)
	except CORBA.COMM_FAILURE:
	    print "caught COMM_FAILURE"
	except CORBA.TRANSIENT:
	    print "caught CORBA.TRANSIENT"
        except CORBA.OBJECT_NOT_EXIST:
            print "caught CORBA.OBJECT_NOT_EXIST"
	current_time = time.time()
	sleep_time = period - (current_time - previous_time)
	if sleep_time > 0.0:
	    time.sleep(sleep_time)
	previous_time = current_time
