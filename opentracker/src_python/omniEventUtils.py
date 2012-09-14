def getConsumerAdmin(channel):
  while(1):
    try:
        consumer_admin = channel.for_consumers ()
        if consumer_admin is None:
          sys.stderr.write("Event Channel returned nil Consumer Admin!\n")
          sys.exit(1)
        break
    except CORBA.COMM_FAILURE, ex:
      sys.stderr.write("Caught COMM_FAILURE Exception. "+ \
        "obtaining Consumer Admin! Retrying...\n")
      time.sleep(1)
  print "Obtained ConsumerAdmin."
  return consumer_admin

def getProxySupplier(consumer_admin):
    #
    # Get proxy supplier - retrying on Comms Failure.
    while (1):
      try:
          proxy_supplier = consumer_admin.obtain_push_supplier ()
          if proxy_supplier is None:
            sys.stderr.write("Consumer Admin return nil proxy_supplier!\n")
            sys.exit(1)
          break
      except CORBA.COMM_FAILURE, ex:
        sys.stderr.write("Caught COMM_FAILURE Exception. "+ \
          "obtaining Proxy Push Supplier! Retrying...\n")
        time.sleep(1)
    print "Obtained ProxyPushSupplier."
    return proxy_supplier

def connectPushConsumer(proxy_supplier, consumer):
    #
    # Connect Push Consumer - retrying on Comms Failure.
    while (1):
      try:
          proxy_supplier.connect_push_consumer(consumer._this())
          break
      except CORBA.BAD_PARAM, ex:
        sys.stderr.write( \
          'Caught BAD_PARAM Exception connecting Push Consumer!\n')
        sys.exit(1)
      except CosEventChannelAdmin.AlreadyConnected, ex:
        sys.stderr.write('Proxy Push Supplier already connected!\n')
        sys.exit(1)
      except CORBA.COMM_FAILURE, ex:
        sys.stderr.write("Caught COMM_FAILURE Exception " +\
          "connecting Push Consumer! Retrying...\n")
        time.sleep(1)
    print "Connected Push Consumer."

def disconnectPushSupplier():
    #
    # Disconnect - retrying on Comms Failure.
    while (1):
	try:
	    proxy_supplier.disconnect_push_supplier()
	    break
	except CORBA.COMM_FAILURE, ex:
	    sys.stderr.write("Caught COMM_FAILURE Exception " +\
			     "disconnecting Push Consumer! Retrying...\n")
	    time.sleep(1)
    print "Disconnected Push Consumer."

def getSupplierAdmin(channel):
    # 
    # Get Supplier Admin interface - retrying on Comms Failure.
    while(1):
	try:
	    supplier_admin=channel.for_suppliers()
	    if supplier_admin is None:
		sys.stderr.write("Event Channel returned nil Supplier Admin!\n")
		sys.exit(1)
	    break
	except CORBA.COMM_FAILURE, ex:
	    sys.stderr.write("Caught COMM_FAILURE Exception. "+ \
			     "obtaining Supplier Admin! Retrying...\n")
	    time.sleep(1)
    print "Obtained SupplierAdmin."
    return supplier_admin

def getProxyConsumer(supplier_admin):
    #
    # Get proxy consumer - retrying on Comms Failure.
    while (1):
	try:
	    proxy_consumer=supplier_admin.obtain_push_consumer()
	    if proxy_consumer is None:
		sys.stderr.write("Supplier Admin returned nil proxy_consumer!\n")
		sys.exit(1)
	    break
	except CORBA.COMM_FAILURE, ex:
	    sys.stderr.write("Caught COMM_FAILURE Exception "+ \
			     "obtaining Proxy Push Consumer! Retrying...\n")
	    time.sleep(1)
    print "Obtained ProxyPushConsumer."
    return proxy_consumer


def connectPushSupplier(proxy_consumer, sptr):
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
    return proxy_consumer

def disconnectPushConsumer(proxy_consumer):
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

    
