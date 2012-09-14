/* ========================================================================
 * Copyright (c) 2006,
 * Institute for Computer Graphics and Vision
 * Graz University of Technology
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * Neither the name of the Graz University of Technology nor the names of
 * its contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ========================================================================
 * PROJECT: OpenTracker
 * ======================================================================== */
/** source file for CORBAUtils class
 *
 * @author Joseph Newman
 *
 * $Id: CORBAUtils.cxx 1151 2006-06-13 12:31:43Z jfn $
 * @file                                                                   */
/* ======================================================================= */

#include <ace/OS.h>
#include <OpenTracker/network/CORBAUtils.h>
#include <iostream>
//#include <OpenTracker/tool/OT_ACE_Log.h>
#include <OpenTracker/core/OtLogger.h>
#ifdef USE_OMNIEVENTS
#include <COS/CosEventComm.hh>
#include <COS/CosEventChannelAdmin.hh>
#include <OpenTracker/skeletons/OT_EventChannel.hh>
#endif //ENABLE_OMNIEVENTS

namespace ot {
#ifdef USE_OMNIEVENTS
    // Disconnect PushConsumer - retrying on Comms Failure.
    void CORBAUtils::disconnectPushConsumer(const CosEventChannelAdmin::ProxyPushSupplier_var& proxy_supplier) {
	while (1)
	{
	    try {
		proxy_supplier->disconnect_push_supplier();
		break;
	    }
	    catch (CORBA::COMM_FAILURE& ex) {
		logPrintE("Caught COMM_FAILURE Exception disconnecting Push Consumer! Retrying...\n");
		continue;
	    }
	}
	logPrintI("Disconnected Push Consumer.\n");
    }

    //
    // Connect Push Consumer - retrying on Comms Failure.
    void CORBAUtils::connectPushConsumer(const CosEventChannelAdmin::ProxyPushSupplier_var&  proxy_supplier, const CosEventComm::PushConsumer_var& consumer_ref) {
	while (1)
	{
	    try {
		//proxy_supplier->connect_push_consumer(consumer->_this());
		proxy_supplier->connect_push_consumer(consumer_ref);
		break;
	    }
	    catch (CORBA::BAD_PARAM& ex) {
		logPrintE("Caught BAD_PARAM Exception connecting Push Consumer!\n");
	    }
	    catch (CosEventChannelAdmin::AlreadyConnected& ex) {
		logPrintE("Proxy Push Supplier already connected!\n");
           break;
	    }
	    catch (CORBA::COMM_FAILURE& ex) {
		logPrintE("Caught COMM_FAILURE exception connecting Push Consumer! Retrying...\n");
		continue;
	    }
	}
	logPrintI("Connected Push Consumer.\n");
    }

    //
    // Get Consumer admin interface - retrying on Comms Failure.
    CosEventChannelAdmin::ConsumerAdmin_var CORBAUtils::getConsumerAdmin(const CosEventChannelAdmin::EventChannel_var& channel) {
	CosEventChannelAdmin::ConsumerAdmin_var consumer_admin;
	while (1)
	{
	    try {
		consumer_admin = channel->for_consumers ();
		if (CORBA::is_nil (consumer_admin))
		{
		    logPrintE("Event Channel returned nil Consumer Admin!\n");
		    exit(1);
		}
		break;
	    }
	    catch (CORBA::COMM_FAILURE& ex) {
		logPrintE("Caught COMM_FAILURE exception obtaining Consumer Admin! Retrying...\n");
		continue;
	    }
	}
	logPrintI("Obtained ConsumerAdmin.\n");
	return consumer_admin;
    }
    
    CosEventChannelAdmin::ProxyPushSupplier_var CORBAUtils::getProxyPushSupplier(const CosEventChannelAdmin::ConsumerAdmin_var& consumer_admin)
    {
	CosEventChannelAdmin::ProxyPushSupplier_var proxy_supplier;
	//
	// Get proxy supplier - retrying on Comms Failure.
	while (1)
	{
	    try {
		proxy_supplier = consumer_admin->obtain_push_supplier ();
		if (CORBA::is_nil (proxy_supplier))
		{
		    logPrintE("Consumer Admin returned nil proxy_supplier!\n");
		    exit (1);
		}
		break;
	    }
	    catch (CORBA::COMM_FAILURE& ex) {
		logPrintE("Caught COMM_FAILURE Exception obtaining Push Supplier! Retrying...\n");
		continue;
	    }
	}
	logPrintI("Obtained ProxyPushSupplier.\n");
	return proxy_supplier;
    }

    CosEventChannelAdmin::ProxyPushConsumer_var CORBAUtils::getProxyPushConsumer(const CosEventChannelAdmin::SupplierAdmin_var& supplier_admin) {
	while (1)
	{
	    //
	    // Get proxy consumer - retrying on Comms Failure.
	    CosEventChannelAdmin::ProxyPushConsumer_var proxy_consumer;
	    while (1)
	    {
		try {
		    proxy_consumer = supplier_admin->obtain_push_consumer ();
		    if (CORBA::is_nil(proxy_consumer))
		    {
			logPrintE("Supplier Admin returned nil proxy_consumer!\n");
			exit(1);
		    }
		    break;
		}
		catch (CORBA::COMM_FAILURE& ex) {
		    logPrintE("Caught COMM_FAILURE Exception obtaining Proxy Push Consumer! Retrying...\n");
		    continue;
		}
	    }
	    logPrintI("Obtained ProxyPushConsumer.\n");
	    return proxy_consumer;
	}
    }

    //
    // Get Supplier Admin interface - retrying on Comms Failure.
    CosEventChannelAdmin::SupplierAdmin_var CORBAUtils::getSupplierAdmin(const CosEventChannelAdmin::EventChannel_var& channel) 
    {
	CosEventChannelAdmin::SupplierAdmin_var supplier_admin;
	while (1)
	{
	    try {
		supplier_admin = channel->for_suppliers ();
		if (CORBA::is_nil(supplier_admin))
		{
		    logPrintE("Event Channel returned nil Supplier Admin!\n");
		    exit(1);
		}
		break;
	    }
	    catch (CORBA::COMM_FAILURE& ex) {
		logPrintE("Caught COMM_FAILURE Exception obtaining Supplier Admin! Retrying...\n");
		continue;
	    }
	}
	logPrintI("Obtained SupplierAdmin.\n");
	return supplier_admin;
    }

    void CORBAUtils::connectPushSupplier(const CosEventChannelAdmin::ProxyPushConsumer_var& proxy_consumer, const CosEventComm::PushSupplier_var& sptr) 
    {
	while (1)
	{
	    try {
		proxy_consumer->connect_push_supplier(sptr.in());
		break;
	    }
	    catch (CORBA::BAD_PARAM& ex) {
		logPrintE("Caught BAD_PARAM Exception connecting Push Supplier!\n");
		exit (1);
	    }
	    catch (CosEventChannelAdmin::AlreadyConnected& ex) {
		logPrintE("Proxy Push Consumer already connected!\n");
		break;
	    }
	    catch (CORBA::COMM_FAILURE& ex) {
		logPrintE("Caught COMM_FAILURE Exception connecting Push Supplier! Retrying...\n");
		continue;
	    }
	}
	logPrintI("Connected Push Supplier.\n");

    }

    void CORBAUtils::disconnectPushSupplier(const CosEventChannelAdmin::ProxyPushConsumer_var& proxy_consumer)
    {
	// Disconnect - retrying on Comms Failure.
	while (1)
	{
	    try {
		proxy_consumer->disconnect_push_consumer();
		break;
	    }
	    catch (CORBA::COMM_FAILURE& ex) {
		logPrintE("Caught COMM_FAILURE Exception disconnecting Push Supplier! Retrying...\n");
		continue;
	    }
	}
	logPrintI("ProxyPushConsumer disconnected.\n");
	
    }
#endif //ENABLE_OMNIEVENTS    

    
    // This method returns an object id based on the id of the last NameComponent of a Name sequence
    PortableServer::ObjectId_var CORBAUtils::getObjectId(const CORBA::ORB_var& orb, const CosNaming::NamingContextExt::StringName_var& string_name) {
        CosNaming::NamingContextExt_var extContext = getRootContext(orb);
        CosNaming::Name_var name;
        try {
            // Create a name object from the string_name
            name = extContext->to_name(string_name);
        } catch(CosNaming::NamingContext::InvalidName) {
            // This exception is thrown if the name is not valid
            logPrintE("Name is not valid\n Exiting....");
            exit(-1);
        }
        CORBA::String_var oid_string = CORBA::string_dup(name[name->length()-1].id);
        return PortableServer::string_to_ObjectId(oid_string);
    }
    
    // This method binds an object reference to a Name specified using a string

    void CORBAUtils::bindObjectReferenceToName(const CORBA::ORB_var& orb, const CORBA::Object_var& obj, const CosNaming::NamingContextExt::StringName_var& string_name)
    {
      CosNaming::NamingContextExt_var extContext = CosNaming::NamingContextExt::_duplicate(getRootContext(orb));
        if (CORBA::is_nil(extContext)) {
            logPrintI("Could not obtain a reference to the root Context\nExiting....\n");
            exit(-1);
        }
        {
	  CosNaming::Name_var name;
	  try {
            // Create a name object from the string_name
            name = extContext->to_name(string_name);
	  } catch(CosNaming::NamingContext::InvalidName) {
            // This exception is thrown if the name is not valid
            logPrintE("Name is not valid\n Exiting....");
            exit(-1);
	  }
	  CosNaming::Name object_name;
	  object_name.length(1);
	  CORBA::ULong length = name->length();
	  object_name[0].id = name[length-1].id;
	  object_name[0].kind = name[length-1].kind;
	  name->length(length-1); // shorten name so it only refers to the contexts
	  {
	      CosNaming::NamingContext_var desired_context = CosNaming::NamingContext::_narrow(getContext(CosNaming::NamingContext::_narrow(extContext), name));
	    if (CORBA::is_nil(desired_context)) {
	      logPrintE("Was unable to obtain the appropriate context\nExiting...");
	      exit(-1);
	    }
	    try {
	      desired_context->bind(object_name, obj);
	    } catch(CosNaming::NamingContext::AlreadyBound) {
	      logPrintE("The context %s.%s is already bound. Rebinding...", (const char*) object_name[0].id, (const char*) object_name[0].kind);
	      desired_context->rebind(object_name, obj);
	    }
	  } // close scope that includes desired_context
	} // close scope that includes name
    } // close scope that includes extContext
  
  CosNaming::NamingContext_var CORBAUtils::getContextFromName(const CosNaming::NamingContext_var& root_context, const CosNaming::Name_var& name)
  {
    CORBA::Object_var obj;
    try {
      obj = root_context->resolve(name);
    } catch (CosNaming::NamingContext::NotFound) {
            logPrintE("Name not found\n");
            return CosNaming::NamingContext::_nil();
        } 
        //CosNaming::NamingContext_var sink_context = CosNaming::NamingContextExt::_narrow(obj);
	CosNaming::NamingContext_var sink_context = CosNaming::NamingContext::_narrow(obj);
        if  (CORBA::is_nil(sink_context)) {
            logPrintE("Object reference isn't a Context");
        }
        return sink_context;
    }

    CosNaming::NamingContext_var CORBAUtils::getContext(const CosNaming::NamingContext_var& root_context, CosNaming::Name_var& name)
    {
      // This method obtains a context, creating it if necessary, having bound
      // it to all the other contexts specified in the 
      // name variable
      CosNaming::NamingContextExt_var context_already_bound;// = CosNaming::NamingContextExt::_nil();
      CORBA::ULong length = name->length();
      CORBA::ULong max = length;
      CosNaming::Name uncontextualised_names;
      while (CORBA::is_nil(context_already_bound) && (name->length() > 0)) {
	context_already_bound = CosNaming::NamingContextExt::_narrow(getContextFromName(root_context, name));
	if (CORBA::is_nil(context_already_bound)) {
	  logPrintE("Context not already bound\n");
	  // extend the NameComponent sequence
	  uncontextualised_names.length(max-length+1); 
	  uncontextualised_names[max-length] = name[length-1];
	  length--;
	  name->length(length); // knock-off the last CosNaming::NameComponent
	} 
      }
      if (CORBA::is_nil(context_already_bound)) {
	logPrintE("binding directly to root context\n");
	context_already_bound = CosNaming::NamingContextExt::_narrow(root_context);
      }
      CosNaming::NamingContext_var previous_context = CosNaming::NamingContextExt::_duplicate(context_already_bound);
      for (CORBA::ULong i = uncontextualised_names.length(); i > 0; --i) { 
	// Go through sequence of uncontextualised names in reverse order
	CosNaming::Name new_name;
	new_name.length(1);
	new_name[0].id = CORBA::string_dup(uncontextualised_names[i-1].id);
	new_name[0].kind = CORBA::string_dup(uncontextualised_names[i-1].kind);
	logPrintE("Binding context with name %s.%s to previous context\n", (const char*) new_name[0].id, (const char*) new_name[0].kind);
	previous_context = previous_context->bind_new_context(new_name);
      }
      return previous_context;
    } 

  CosNaming::NamingContextExt_var CORBAUtils::getRootContext(const CORBA::ORB_var& orb) 
  {
    try {
      // Obtain a reference to the root context of the Name service:
      if (CORBA::is_nil(orb)) {
	logPrintE("Reference to orb is nil. Exiting...\n");
	exit(-1);
      }
      CORBA::Object_var obj = orb->resolve_initial_references("NameService");
      
      // Narrow the reference returned.
      CosNaming::NamingContextExt_var extContext = CosNaming::NamingContextExt::_narrow(obj);
      if( CORBA::is_nil(extContext) ) {
	logPrintE("Failed to narrow the root naming context.");
	//return CORBA::Object::_nil();
	return CosNaming::NamingContextExt::_nil();
      } else {
	return extContext;
      }
    } catch(CORBA::ORB::InvalidName& ex) {
      // This should not happen!
      logPrintE("Service required is invalid [does not exist].");
      return CosNaming::NamingContextExt::_nil();
    } catch(CORBA::TRANSIENT) {
      logPrintE("Naming Service isn't running\n");
      return CosNaming::NamingContextExt::_nil();
    } catch(CORBA::INV_OBJREF) {
      logPrintE("An exception has occured that indicates that the object reference to the NameService is internally malformed.\n");
      return CosNaming::NamingContextExt::_nil();
    }
  }

  CORBA::Object_ptr CORBAUtils::getObjectReference(const CORBA::ORB_var& orb, const CosNaming::NamingContextExt::StringName_var& string_name)
    {
        
      CosNaming::NamingContextExt_var extContext = getRootContext(orb);
      if (CORBA::is_nil(extContext)) {
            logPrintI("Could not obtain a reference to the root Context\nExiting....\n");
            exit(-1);
      }

        CosNaming::Name_var name;
        try {
            // Create a name object from the string_name
            name = extContext->to_name(string_name);
        } catch(CosNaming::NamingContext::InvalidName) {
            // This exception is thrown if the name is not valid
            logPrintE("Naming context name is not valid");
            return CORBA::Object::_nil();
        }
        
        try {
            // Resolve the name to an object reference.
            return extContext->resolve(name);
        } catch(CosNaming::NamingContext::NotFound& ex) {
            // This exception is thrown if any of the components of the
            // path [contexts or the object] aren't found:
            logPrintE("Naming context not found.");
        } catch(CORBA::COMM_FAILURE& ex) {
            logPrintE("Caught system exception COMM_FAILURE -- unable to contact the naming service.");
        } catch(CORBA::SystemException&) {
            logPrintE("Caught a CORBA::SystemException while using the naming service.");
        }
        return CORBA::Object::_nil();
    }

  std::string CORBAUtils::generateUniqueId() {
    CORBA::String_var pd_poaId;
    int pd_poaIdSize = 10;
    pd_poaId = _CORBA_String_helper::alloc(pd_poaIdSize - 1);
    ((char*) pd_poaId)[0] = '\xfe';
    CORBAUtils::generateUniqueId((_CORBA_Octet*) ((char*) pd_poaId + 1));
    ((char*) pd_poaId)[pd_poaIdSize - 1] = '\0';
    return std::string(pd_poaId);
  }
    
  void CORBAUtils::generateUniqueId(CORBA::Octet* k)
{
  OMNIORB_ASSERT(k);

  static omni_tracedmutex lock;
  omni_tracedmutex_lock sync(lock);

  static CORBA::ULong hi = 0;
  static CORBA::ULong lo = 0;

  if( !hi && !lo ) {

    CORBA::Short pid;

#ifdef HAVE_GETTIMEOFDAY
    // Use gettimeofday() to obtain the current time. Use this to
    // initialise the 32-bit field hi and med in the seed.
    // On unices, add the process id to med.
    // Initialise lo to 0.
    struct timeval v;
# ifdef GETTIMEOFDAY_TIMEZONE
    gettimeofday(&v,0);
# else
    gettimeofday(&v);
# endif
    hi = v.tv_sec;
# ifdef HAVE_GETPID
    pid = (CORBA::Short) getpid();
# else
    pid = (CORBA::Short) v.tv_usec;
    // without the process id, there is no guarantee that the keys generated
    // by this process do not clash with those generated by other processes.
# endif

#elif defined(__WIN32__)
    // Unique number on NT
    // Use _ftime() to obtain the current system time.
# ifndef __BCPLUSPLUS__
    struct _timeb v;
    _ftime(&v);
    hi = v.time;
    pid = (CORBA::Short) _getpid();
# else
    struct timeb v;
    ftime(&v);
    hi = v.time;
    pid = (CORBA::Short) getpid();
# endif

#elif defined(__VMS) && __VMS_VER < 70000000
    // VMS systems prior to 7.0
    timeb v;
    ftime(&v);
    hi = v.time;
    pid = (CORBA::Short) getpid();

#elif  defined(__vxWorks__)
    struct timespec v;
    clock_gettime(CLOCK_REALTIME, &v);
    hi = v.tv_sec;

    pid = 0; // no processes in vxWorks
#endif

    // Byte-swap the pid, so that if the counter in the lower 16
    // bits of <lo> overflows it will not change the fastest
    // moving pid bit.

    pid = ((pid & 0xff00) >> 8) | ((pid & 0x00ff) << 8);

    lo = pid << 16;
  }

  CORBA::Octet* phi = (CORBA::Octet*) &hi;
  CORBA::Octet* plo = (CORBA::Octet*) &lo;

  *k++ = *phi++;
  *k++ = *phi++;
  *k++ = *phi++;
  *k++ = *phi++;
  *k++ = *plo++;
  *k++ = *plo++;
  *k++ = *plo++;
  *k++ = *plo++;

  lo++;
}
#ifdef fish
  template <class T, class U> std::pair<PortableServer::ObjectId_var, CORBA::Object_var> CORBAUtils::activateNode(PortableServer::POA_var poa, U* node)
  {
    T<U>* tie_node = new T<U>(node);
    PortableServer::ObjectId_var corba_id = poa->activate_object(tie_node);
    CORBA::Object_var obj = CORBA::Object::_narrow(poa->id_to_reference(corba_id));
    return std::pair<PortableServer::ObjectId_var, CORBA::Object_var>(corba_id, obj);
  }
#endif  
} // namespace ot

/* 
 * ------------------------------------------------------------
 *   End of CORBAUtils.h
 * ------------------------------------------------------------
 *   Automatic Emacs configuration follows.
 *   Local Variables:
 *   mode:c++
 *   c-basic-offset: 4
 *   eval: (c-set-offset 'substatement-open 0)

 *   eval: (c-set-offset 'case-label '+)
 *   eval: (c-set-offset 'statement 'c-lineup-runin-statements)
 *   eval: (setq indent-tabs-mode nil)
 *   End:
 * ------------------------------------------------------------ 
 */
