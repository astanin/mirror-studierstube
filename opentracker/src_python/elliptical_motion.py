import os,sys
import time
import threading
import signal
import getopt
import CORBA
import CosNaming
import CosEventComm, CosEventComm__POA
import CosEventChannelAdmin
import OT_CORBA
from naming import *
from math import cos, sin, pi, atan2
from omniORB import any

class Supplier_i(CosEventComm__POA.PushSupplier):
  def disconnect_push_supplier (self):
      print "Push Supplier: disconnected."

class EllipticalMotion:
  def __init__(self, a, b, omega):
    self.a = a
    self.b = b
    self.omega  = omega;

  def getEvent(self):
    t = time.time()
    x = self.a * cos(self.omega * t)
    y = 1.7
    z = self.b * sin(self.omega * t)
    xdot = -self.a * self.omega * sin(self.omega * t)
    zdot = self.b  * self.omega * cos(self.omega * t)
    theta = atan2 (xdot, zdot)
    alpha = theta
    qx, qy, qz, qw = 0.0, sin(alpha*0.5), 0.0, cos(alpha*0.5)
    return [OT_CORBA.EventAttribute("position", CORBA.Any(CORBA.TypeCode("IDL:OT_CORBA/FloatVector:1.0"), [x,y,z])),
            OT_CORBA.EventAttribute("orientation", CORBA.Any(CORBA.TypeCode("IDL:OT_CORBA/FloatVector:1.0"), [qx,qy,qz,qw])),
            OT_CORBA.EventAttribute("timestamp", any.to_any(t))]#OT_CORBA.Event([x, y, z], [qx, qy, qz, qw], t, 0, 0.5)

class CircularMotion:
  def __init__(self, radius, speed):
    self.radius = radius
    self.omega  = speed / radius;

  def getEvent(self):
    t = time.time()
    x = self.radius * cos(self.omega * t)
    y = self.radius * sin(self.omega * t)
    z = 0.0
    alpha = self.omega * t
    qx, qy, qz, qw = 0.0, 0.0, sin(alpha*0.5), cos(alpha*0.5)
    return [OT_CORBA.EventAttribute("position", CORBA.Any(CORBA.TypeCode("IDL:OT_CORBA/FloatVector:1.0"), [x,y,z])),
            OT_CORBA.EventAttribute("orientation", CORBA.Any(CORBA.TypeCode("IDL:OT_CORBA/FloatVector:1.0"), [qx,qy,qz,qw])),
            OT_CORBA.EventAttribute("timestamp", any.to_any(t))]

def main():
  orb = CORBA.ORB_init(sys.argv, CORBA.ORB_ID)

  testloop    = TestLoop()
  channelName = "EventChannel"

  # Process Options
  try:
    opts,args=getopt.getopt(sys.argv[1:],"d:rs:n:h")
  except getopt.error:
    # print help information and exit:
    usage()
    sys.exit(-1)
  for option, optarg in opts:
    if option=='-d':
      testloop.discnum = int(optarg)
    elif option=='-r':
      testloop.refnil = 1
    elif option=='-s':
      testloop.sleepInterval = int(optarg)
    elif option=='-n':
      channelName = optarg
    elif option=='-h':
      usage()
      sys.exit(0)
    else:
      usage()
      sys.exit(-1)

  # Ignore broken pipes
  if signal.__dict__.has_key('SIGPIPE'):
    signal.signal(signal.SIGPIPE, signal.SIG_IGN)

  action="start" # Use this variable to help report errors.
  try:

    # A Push Supplier can be implemented as a pure client or as a mixed
    # client-server process, depending on whether it requires and is
    # prepared to service disconnect requests from the channel.
    # If it is then create the servant object (later) and activate the POA.
    if not testloop.refnil:
      action="resolve initial reference 'RootPOA'"
      poa=orb.resolve_initial_references("RootPOA")

      action="activate the RootPOA's POAManager"
      poaManager=poa._get_the_POAManager()
      poaManager.activate()

    #
    # Obtain reference to the Event Channel.
    # (from command-line argument or from the Naming Service).
    if len(args):
      action="convert URI from command line into object reference"
      obj=orb.string_to_object(args[0])
    else:
      #
      # Get Name Service root context.
      action="resolve initial reference 'NameService'"
      obj=orb.resolve_initial_references("NameService")
      rootContext=obj._narrow(CosNaming.NamingContext)
      if rootContext is None:
        raise CORBA.OBJECT_NOT_EXIST(0,CORBA.COMPLETED_NO)

      #
      # Obtain reference to the Event Channel.
      action="find Event Channel in naming service"
      obj=rootContext.resolve(str2name(channelName))

    action="narrow object reference to event channel"
    channel=obj._narrow(CosEventChannelAdmin.EventChannel)
    if channel is None:
      raise CORBA.OBJECT_NOT_EXIST(0,CORBA.COMPLETED_NO)

  except CORBA.ORB.InvalidName, ex: # resolve_initial_references()
     sys.stderr.write("Failed to %s. ORB::InvalidName\n"%action)
     sys.exit(1)
  except CosNaming.NamingContext.InvalidName, ex: # resolve()
     sys.stderr.write("Failed to %s. NamingContext::InvalidName\n"%action)
     sys.exit(1)
  except CosNaming.NamingContext.NotFound, ex: # resolve()
     sys.stderr.write("Failed to %s. NamingContext::NotFound\n"%action)
     sys.exit(1)
  except CosNaming.NamingContext.CannotProceed, ex: # resolve()
     sys.stderr.write("Failed to %s. NamingContext::CannotProceed\n"%action)
     sys.exit(1)
  except CORBA.TRANSIENT, ex:
     sys.stderr.write("Failed to %s. TRANSIENT\n"%action)
     sys.exit(1)
  except CORBA.OBJECT_NOT_EXIST, ex:
     sys.stderr.write("Failed to %s. OBJECT_NOT_EXIST\n"%action)
     sys.exit(1)
  except CORBA.COMM_FAILURE, ex:
     sys.stderr.write("Failed to %s. COMM_FAILURE\n"%action)
     sys.exit(1)
  except CORBA.SystemException, ex:
     sys.stderr.write("System exception, unable to %s.\n"%action)
     sys.exit(1)
  except CORBA.Exception, ex:
     sys.stderr.write("CORBA exception, unable to %s.\n"%action)
     sys.exit(1)

  #
  # Get Supplier Admin interface - retrying on Comms Failure.
  while(1):
    try:
        testloop.supplier_admin=channel.for_suppliers()
        if testloop.supplier_admin is None:
          sys.stderr.write("Event Channel returned nil Supplier Admin!\n")
          sys.exit(1)
        break
    except CORBA.COMM_FAILURE, ex:
      sys.stderr.write("Caught COMM_FAILURE Exception. "+ \
        "obtaining Supplier Admin! Retrying...\n")
      time.sleep(1)
  print "Obtained SupplierAdmin."

  # Perform connection tests in a new thread.
  # Do this so that the main thread can respond to keyboard interrupts.
  testloop.start()

  # Make sure that the main thread gets some time every 200ms, so that it
  # can respond to keyboard interrupts.
  try:
    while(1):
      time.sleep(1)
  except:
    os._exit(0) # Kills all known threads, dead!
#end main()


class TestLoop(threading.Thread):
  def __init__(self):
      threading.Thread.__init__(self)
      self.discnum=0
      self.refnil=0
      self.sleepInterval=0
      #self.motion = CircularMotion( 4.0, 3.0 )
      self.motion = EllipticalMotion( 3.0, 8.0,  0.5)
      self.supplier_admin=None
  
  def run(self):
      if self.refnil:
        sptr=None
      else:
        supplier=Supplier_i()
        sptr=supplier._this() # SIDE EFFECT: Activates object in POA
      
      l=0

      while (1):
        #
        # Get proxy consumer - retrying on Comms Failure.
        while (1):
          try:
            proxy_consumer=self.supplier_admin.obtain_push_consumer()
            if proxy_consumer is None:
              sys.stderr.write("Supplier Admin returned nil proxy_consumer!\n")
              sys.exit(1)
            break
          except CORBA.COMM_FAILURE, ex:
            sys.stderr.write("Caught COMM_FAILURE Exception "+ \
              "obtaining Proxy Push Consumer! Retrying...\n")
            time.sleep(1)
        print "Obtained ProxyPushConsumer."

        #
        # Connect Push Supplier - retrying on Comms Failure.
        while (1):
          try:
              proxy_consumer.connect_push_supplier(sptr)
              break
          except CORBA.BAD_PARAM, ex:
            sys.stderr.write( \
              'Caught BAD_PARAM Exception connecting Push Supplier!')
            sys.exit(1)
          except CosEventChannelAdmin.AlreadyConnected, ex:
            sys.stderr.write('Proxy Push Consumer already connected!')
            sys.exit(1)
          except CORBA.COMM_FAILURE, ex:
            sys.stderr.write("Caught COMM_FAILURE Exception " +\
              "connecting Push Supplier! Retrying...")
            time.sleep(1)
        print "Connected Push Supplier."

        # Push data.
        i=0
        while((self.discnum == 0) or (i < self.discnum)):
          try:
              sys.stdout.write("Push Supplier: push() called. ")
              any_event = any.to_any(self.motion.getEvent())
              proxy_consumer.push(any_event)
              print "Data :",l
          except CosEventComm.Disconnected, ex: 
              print "Failed. Caught Disconnected Exception!"
          except CORBA.COMM_FAILURE, ex: 
              print "Failed. Caught COMM_FAILURE Exception!"
          i=i+1
          l=l+1
          #time.sleep(1.0)
          time.sleep(0.05)
          #time.sleep(0.001)

        # Disconnect - retrying on Comms Failure.
        while(1):
          try:
              proxy_consumer.disconnect_push_consumer()
              break
          except CORBA.COMM_FAILURE, ex:
              sys.stderr.write("Caught COMM_FAILURE Exception " + \
                "disconnecting Push Supplier! Retrying...\n")
              time.sleep(1)
        print "ProxyPushConsumer disconnected."

        # Yawn.
        print "Sleeping",self.sleepInterval," Seconds."
        time.sleep(self.sleepInterval)
#end class TestLoop


def usage():
  print """
Create a PushSupplier to send events to a channel.
syntax: python pushsupp.py OPTIONS [CHANNEL_URI]

CHANNEL_URI: The event channel may be specified as a URI.
 This may be an IOR, or a corbaloc::: or corbaname::: URI.

OPTIONS:                                         DEFAULT:
 -d NUM   disconnect after sending NUM events     [0 - never disconnect]
 -r       connect using a nil reference
 -s SECS  sleep SECS seconds after disconnecting  [0]
 -n NAME  channel name (if URI is not specified)  ["EventChannel"]
 -h       display this help text
"""

################################################################################
# If this file is executed directly, then we start here.
if(__name__=="__main__"):
  main()
