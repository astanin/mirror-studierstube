#                            Package   : omniEvents
# pushsupp.py                Created   : 16/11/2003
#                            Author    : Alex Tingle
#
#    Copyright (C) 2003-2004 Alex Tingle.
#
#    This file is part of the omniEvents application.
#
#    omniEvents is free software; you can redistribute it and/or
#    modify it under the terms of the GNU Lesser General Public
#    License as published by the Free Software Foundation; either
#    version 2.1 of the License, or (at your option) any later version.
#
#    omniEvents is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#    Lesser General Public License for more details.
#
#    You should have received a copy of the GNU Lesser General Public
#    License along with this library; if not, write to the Free Software
#    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#

"""Push Model consumer implementation."""

import os,sys
import time
import threading
import signal
import getopt
import CORBA
import CosNaming
import CosEventComm__POA
import CosEventChannelAdmin
from naming import *
from omniORB import any

from omniEventUtils import *

class Consumer_i(CosEventComm__POA.PushConsumer):
  def __init__(self,disconnect=0):
      self.event=threading.Event()
      self.disconnect=disconnect
      self.i=0
  
  def push(self, data):
    self.i=self.i+1
    ev = any.from_any(data)
    print "Push Consumer: push() called. Data :"
    for d in ev:
      print d['name'], '\t\t', d['value']
    #  print d['name'], any.from_any(d['value'])
    # Exercise Disconnect
    if (self.i == self.disconnect):
      self.i = 0
      # NOTE : The proxy_supplier object is disposed at the server
      #        during the disconnect_push_supplier call. Do NOT
      #        use the proxy_supplier reference after disconnecting.

      # Signal main thread to disconnect and re-connect.
      self.event.set()

  def disconnect_push_consumer(self):
      print "Push Consumer: disconnected."
#end class Consumer_i


def main():
  orb = CORBA.ORB_init(sys.argv, CORBA.ORB_ID)

  discnum = 0
  sleepInterval = 0
  channelName = "EventChannel"

  # Process Options
  try:
    opts,args=getopt.getopt(sys.argv[1:],"d:s:n:h")
  except getopt.error:
    # print help information and exit:
    usage()
    sys.exit(-1)
  for option, optarg in opts:
    if option=='-d':
      discnum = int(optarg)
    elif option=='-s':
      sleepInterval = int(optarg)
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

  consumer_admin = getConsumerAdmin(channel)
  
  #
  # Make a Push Consumer.        
  consumer = Consumer_i(discnum)

  while(1):
    proxy_supplier = getProxySupplier(consumer_admin)
    connectPushConsumer(proxy_supplier, consumer)
    #
    # Wait for indication to disconnect before re-connecting.
    #  Make sure that the main thread gets some time every 200ms, so that it
    #  can respond to keyboard interrupts.
    while(not consumer.event.isSet()):
      try:
        consumer.event.wait(0.2)
      except:
        os._exit(0) # Kills all known threads, dead!
    consumer.event.clear()

    disconnectPushSupplier()
    # Yawn
    print "Sleeping ",sleepInterval," seconds."
    time.sleep(sleepInterval)
  #end while
#end main()


def usage():
  print """
Create a PushConsumer to receive events from a channel.
syntax: python pushcons.py OPTIONS [CHANNEL_URI]

CHANNEL_URI: The event channel may be specified as a URI.
 This may be an IOR, or a corbaloc::: or corbaname::: URI.

OPTIONS:                                         DEFAULT:
 -d NUM   disconnect after receiving NUM events   [0 - never disconnect]
 -s SECS  sleep SECS seconds after disconnecting  [0]
 -n NAME  channel name (if URI is not specified)  ["EventChannel"]
 -h       display this help text
"""


################################################################################
# If this file is executed directly, then we start here.
if(__name__=="__main__"):
  main()
