/* ======================================================================== 
 * Copyright (C) 2006  Graz University of Technology
 *
 * This framework is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This framework is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this framework; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * For further information please contact 
 * Dieter Schmalstieg
 * <schmalstieg@icg.tu-graz.ac.at>
 * Graz University of Technology, 
 * Institut for Computer Graphics and Vision,
 * Inffeldgasse 16a, 8010 Graz, Austria.
 * ========================================================================
 * PROJECT: OpenTracker
 * ======================================================================== */
/** source file for CORBAModule module.
  *
  * @author Joseph Newman
  *
  * $Id: CORBAModule.cxx 897 2005-12-03 20:23:38Z jfn $
  * @file                                                                   */
 /* ======================================================================= */

#include <OpenTracker/network/CORBAModule.h>

#ifdef USE_CORBA

// this will remove the warning 4786
#include <OpenTracker/tool/disable4786.h>
#include <OpenTracker/network/CORBASource.h>
#include <OpenTracker/network/CORBASink.h>
#include <OpenTracker/network/CORBATransform.h>

#include <OpenTracker/tool/OT_ACE_Log.h>
#include <OpenTracker/skeletons/OT_CORBA.hh>

#ifdef USE_OMNIEVENTS
#include <COS/CosEventComm.hh>
#include <COS/CosEventChannelAdmin.hh>
#include <OpenTracker/skeletons/OT_EventChannel.hh>
#include <OpenTracker/network/PushCons.h>
#include <OpenTracker/network/PushSupp.h>
#ifdef USE_SHARED
#include <OpenTracker/network/SharedEngineNode.h>
#endif //USE_SHARED
#endif //ENABLE_OMNIEVENTS

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <algorithm>

#include <ace/Log_Msg.h>

//using namespace std;

// Destructor method

namespace ot {

	OT_MODULE_REGISTER_FUNC(CORBAModule){
		OT_MODULE_REGISTRATION_DEFAULT(CORBAModule, "CORBAConfig" );
	}	


 PortableServer::POAManager_var CORBAModule::pman;
  CORBA::ORB_var CORBAModule::orb;
 PortableServer::POA_var CORBAModule::poa;
 PortableServer::POA_var CORBAModule::root_poa;
 PortableServer::POA_var CORBAModule::ins_poa;
 bool CORBAModule::persistent;
 bool CORBAModule::initialised;




void CORBAModule::destroyORB()
{
  try {
    //delete myimpl;
    orb->destroy();
    CORBA::release(orb);
  }
  catch(CORBA::SystemException&) {
    logPrintE("Caught CORBA::SystemException when trying to destroy ORB.\n");
  }
  catch(CORBA::Exception&) {
    logPrintE("Caught CORBA::Exception when trying to destroy ORB");
  }
  catch(omniORB::fatalException& fe) {
    logPrintE("Caught omniORB::fatalException when trying to destroy ORB:");
    logPrintE("  file: %s", fe.file());
    logPrintE("  line: %s", fe.line());
    logPrintE("  mesg: %s", fe.errmsg());
  }
  catch(...) {
    logPrintE("Caught unknown exception while trying to destroy ORB.");
  }
}

  CORBA::ORB_var CORBAModule::getORB() { 
    CORBA::ORB_var _orb = orb;
    return CORBA::ORB::_duplicate(_orb);
  }

  PortableServer::POA_var CORBAModule::getPOA() { 
    return PortableServer::POA::_duplicate(CORBAModule::poa); 
  }

  PortableServer::POA_var CORBAModule::getRootPOA() { 
    return PortableServer::POA::_duplicate(CORBAModule::root_poa); 
  }

  PortableServer::POA_var CORBAModule::getINSPOA() {
    CORBA::Object_var obj = orb->resolve_initial_references("omniINSPOA");
    return PortableServer::POA::_narrow(obj);
  }

  PortableServer::POAManager_var CORBAModule::getPOAManager() { 
    return PortableServer::POAManager::_duplicate(pman); 
  }

  PortableServer::POAManager_var CORBAModule::getINSPOAManager() { 
    return getINSPOA()->the_POAManager();
  }

// This method initialises the ORB and poa


void CORBAModule::initializeORB(int& argc, char**& argv)
{
  try {
    // initialize the ORB
    //orb = CORBA::ORB_init(argc, argv);
    CORBA::ORB_var _orb = CORBA::ORB_init(argc, argv);
    orb = CORBA::ORB::_duplicate(_orb);
    if (CORBA::is_nil(orb)) {
      logPrintE("Orb is nil. Exiting...\n");
    } else {
      logPrintI("Orb successfully initialised\n");
    }
    CORBAModule::initialised = true;
    
    // Initialise the POA.
    CORBA::Object_var objref = orb->resolve_initial_references("RootPOA");
    CORBAModule::root_poa = PortableServer::POA::_narrow(objref);
    pman = root_poa->the_POAManager();
    pman->activate();
    
    if (CORBAModule::persistent) {
      // Create a new POA with the persistent lifespan policy.
      CORBA::PolicyList pl;
      pl.length(2);
      pl[0] = CORBAModule::root_poa->create_lifespan_policy(PortableServer::PERSISTENT);
      pl[1] = CORBAModule::root_poa->create_id_assignment_policy(PortableServer::USER_ID);
      poa = CORBAModule::root_poa->create_POA("persistent poa", pman, pl);
      logPrintI("Created POA with id assignment policy and persistence policy\n");
    } else {
      // Create a new POA with the id assignment policy.
      CORBA::PolicyList pl;
      pl.length(1);
      pl[0] = CORBAModule::root_poa->create_id_assignment_policy(PortableServer::SYSTEM_ID);
      poa = CORBAModule::root_poa->create_POA("transient poa", pman, pl);
      logPrintI("Created POA with id assignment policy\n");
    }
    logPrintI("Concluding ORB and POA initialisation\n");
  }
  catch(CORBA::SystemException&) {
    logPrintE("Caught CORBA::SystemException.");
  }
  catch(CORBA::Exception&) {
    logPrintE("Caught CORBA::Exception.");
  } 
  catch(omniORB::fatalException& fe) {
    logPrintE("Caught omniORB::fatalException:");
    logPrintE("  file: %s", fe.file());
    logPrintE("  line: %s", fe.line());
    logPrintE("  mesg: ", fe.errmsg());
  }
  catch(...) {
    logPrintE("Caught unknown exception.");
  }
}

// This method destroys the ORB

  CORBAModule::~CORBAModule()
  {
    
    //sinks.clear();
    clear();
    //if (initialised) {
    //  destroyORB();
    //}
  }
  
  void CORBAModule::init(StringTable& attributes,  ConfigNode * localTree)
  {
    logPrintI("CORBAModule is being initialised\n");
    Module::init(  attributes, localTree );
    if( attributes.containsKey("persistent")) {
      std::string is_persistent = attributes.get("persistent");
      if (is_persistent.compare("true") == 0) {
	CORBAModule::persistent = true;
      } else {
	CORBAModule::persistent = false;
      }
    } else {
      CORBAModule::persistent = true;
    }
    std::vector<std::string> args;
    args.push_back("opentracker");
    if( attributes.containsKey("InitRef")) {
      std::string InitRef = attributes.get("InitRef"); //e.g. "NameService=corbaname::herceg.cl.cam.ac.uk"
      args.push_back("-ORBInitRef");
      args.push_back(InitRef);
    }
    if( attributes.containsKey("endPoint")) {
      CORBAModule::persistent = true;
      std::string endpoint = attributes.get("endPoint"); //"giop:tcp:scumble.lce.cl.cam.ac.uk:9999";
      args.push_back("-ORBendPoint");
      args.push_back(endpoint);
      args.push_back("-ORBendPoint");

      int lastpos = endpoint.rfind(':');
      if (lastpos != std::string::npos)
      {
        std::string epport = endpoint.substr(lastpos+1);
        std::ostringstream oss;
        oss << "giop:tcp::" << epport;
        logPrintI("Creating second CORBA endpoint: %s\n", oss.str().c_str());
        args.push_back(oss.str());
      }
    }
    args.push_back("-ORBabortOnInternalError");
    args.push_back("1");
    int argc = args.size();
    char** argv = new char* [argc+1];
    //char** argv = (char **) malloc(argc + 1); // note +1
    for (int i=0; i<argc; i++) {
      argv[i] = new char [args[i].length() + 1];
      //argv[i] = (char *) malloc(args[i].length() + 1);
      strcpy(argv[i], args[i].c_str());
    }
    //argv[argc] = (char *) malloc(1);
    argv[argc] = new char [1];
    argv[argc] = '\0';
    initializeORB(argc, argv);
    // Now free the memory
    //#ifdef fish
    for (int i=0; i<=argc; i++) {
      //if (argv[i] != NULL)
      // delete [] argv[i];
    }
    if (argv)
      {
	//delete [] argv;
      }
    //#endif
  }
    
  void CORBAModule::removeNode(Node * node) {
    logPrintI("CORBAModule::removeNode\n");
    if ((node->getType().compare("CORBASink") == 0) || (node->getType().compare("CORBATransform") == 0)) {
      CORBASinkVector::iterator result = std::find( sinks.begin(), sinks.end(), node );
        if( result != sinks.end())
        {
          //delete *result;
	  sinks.erase( result );
	  return;
        } else {
	  logPrintE("Node with ID %s not in sinks CORBAModule sinks vector\n", node->get("DEF").c_str());
	}
    } else if (node->getType().compare("CORBASource") == 0) {
      CORBASourceVector::iterator result = std::find( sources.begin(), sources.end(), node);
      if (result != sources.end()) {
	logPrintE("removeNode is unimplemented for CORBASource nodes\n");
	//OT_CORBA::OTSource_var corba_source_ref = result->second;
	// deactivate object
	//PortableServer::ObjectId_var corba_source_id = getPOA()->reference_to_id(corba_source_ref);
	
	//delete result->first;
	sources.erase( result );
      } else {
	logPrintE("Node not present in CORBASourceVector");
      }
      return;
    } 
#ifdef USE_OMNIEVENTS
    else if (node->getType().compare("PushSupp") == 0) {
      // TODO: handle removal of PushSupp nodes...
      PushSuppVector::iterator result =  std::find( pushsupps.begin(), pushsupps.end(), node );//pushsupps.find((PushSupp*) node);
      if (result != pushsupps.end()) {
	pushsupps.erase( result );
	//delete *result;
	return;
      }
    }
#ifdef USE_SHARED
    else if (node->getType().compare("SharedEngineNode") == 0) {
      // TODO: handle removal of PushSupp nodes...
      SharedEngineNodeVector::iterator result =  std::find( sharedengines.begin(), sharedengines.end(), node );//pushsupps.find((PushSupp*) node);
      if (result != sharedengines.end()) {
	sharedengines.erase( result );
	//delete *result;
	return;
      }
    } 
#endif
    else if (node->getType().compare("PushCons") == 0) {
      // TODO: handle removal of PushCons nodes...
      PushConsVector::iterator result = std::find( pushconsumers.begin(), pushconsumers.end(), node );//pushconsumers.find((PushCons*) node);
      if (result != pushconsumers.end()) {
	// disconnect the PushCons node
	CORBAUtils::disconnectPushConsumer(proxy_pushsupplier_map[*result]);
	std::cerr << "disconnecting push consumer!" << std::endl;
	proxy_pushsupplier_map.erase(*result);
	pushconsumers.erase(result);
      	//delete *result;
      }
    }
#endif //USE_OMNIEVENTS 
  }

// This method is called to remove all nodes

void CORBAModule::clear()
{
  // for( CORBASinkVector::iterator it = sinks.begin(); it != sinks.end(); it++ )
//     {
//       delete *it;
//     }
  sinks.clear();
  // for (SourceNodeMap::iterator source_it=sources.begin(); source_it!=sources.end(); ++source_it) 
//     {
//       OT_CORBA::OTSource_var corba_source_ref = source_it->second;
//       // deactivate object
//       PortableServer::ObjectId_var corba_source_id = getPOA()->reference_to_id(corba_source_ref);
      
      
//       delete source_it->first;
//       //have to sort out reference counting - it may be sufficient to delete the servant
//       //delete source_it->second; //have to sort out reference counting
//     }
  sources.clear();
#ifdef USE_OMNIEVENTS
//   for( PushSuppVector::iterator it = pushsupps.begin(); it != pushsupps.end(); it++ )
//     {
//       delete *it;
//     }
  pushsupps.clear();
  for( PushConsVector::iterator it = pushconsumers.begin(); it != pushconsumers.end(); it++ )
    {
      // disconnect the PushCons node
      CORBAUtils::disconnectPushConsumer(proxy_pushsupplier_map[*it]);
      // have to sort out the reference counting wrt to the proxy_pushsupplier_map
      //delete *it;
    }
  pushconsumers.clear();
  proxy_pushsupplier_map.clear();
  
#endif //USE_OMNIEVENTS
}

// This method is called to construct a new Node.
  Node * CORBAModule::createNode( const std::string& name, const StringTable& attributes) {
    StringTable xattributes(attributes);
    
    CORBA::Object_var obj = CORBA::Object::_nil();
    if (xattributes.containsKey("name")) {
      CosNaming::NamingContextExt::StringName_var string_name = CORBA::string_dup((const char*) xattributes.get("name").c_str());
      obj = CORBAUtils::getObjectReference(orb, string_name);
    }
    if (xattributes.containsKey("uri")) {
      obj = orb->string_to_object(xattributes.get("uri").c_str());
    }
    if( name.compare("CORBASink") == 0 )     {
      int frequency;
      int num = sscanf(xattributes.get("frequency").c_str(), " %i", &frequency );
      if( num <= 0 ) {
	frequency = 1;
      } else {
	frequency = num;
      }
      CORBASink * sink;
      if (CORBA::is_nil(obj)) {
	std::cerr << "no name or uri yielded a suitable reference" << std::endl;
	sink = new CORBASink(frequency);
      } else {
	OT_CORBA::OTEntity_ptr sink_ref = OT_CORBA::OTEntity::_narrow(obj);
	if (CORBA::is_nil(sink_ref)) {
	  std::cerr << "name or uri could not be narrowed to OTEntity" << std::endl;
	  CORBA::release(sink_ref);
	  sink = new CORBASink( frequency );

	} else {
	  std::cerr << "instantiating CORBASink with OTEntity reference" << std::endl;
	  sink = new CORBASink( sink_ref , frequency );
	}
      }
      sinks.push_back( sink );
      ACE_DEBUG((LM_DEBUG, ACE_TEXT("ot:Build CORBASink node\n")));
      return sink;
    } 
    else if( name.compare("CORBATransform") == 0 ) 
      {
	CosNaming::NamingContextExt::StringName_var string_name = CORBA::string_dup((const char*) xattributes.get("name").c_str());
	CORBA::Object_var obj = CORBAUtils::getObjectReference(orb, string_name);
	if (CORBA::is_nil(obj)) {
	  logPrintI("Could not obtain a reference to object supposedly bound to %s.\nExiting....\n", (const char*) string_name);
	exit(-1);
      }
      OT_CORBA::TransformNode_var sink_ref = OT_CORBA::TransformNode::_narrow(obj);
      
      CORBATransform * transform = new CORBATransform( sink_ref );
      sinks.push_back( transform );
      ACE_DEBUG((LM_DEBUG, ACE_TEXT("ot:Build CORBATransform node\n")));
      return transform;
    } else if (name.compare("CORBASource") == 0 ) {
      CosNaming::NamingContextExt::StringName_var name = CORBA::string_dup((const char*) xattributes.get("name").c_str());
      CORBASource * source_impl = new CORBASource( );    
      source_impl->setContext(context);
      sources.push_back(source_impl);
      ACE_DEBUG((LM_DEBUG, ACE_TEXT("ot:Build CORBASource node\n")));
      return source_impl;
    }
#ifdef USE_OMNIEVENTS
  else if (name.compare("PushCons") == 0 ) 
    {
      logPrintI("creating PushCons node\n");
      CosNaming::NamingContextExt::StringName_var string_name = CORBA::string_dup((const char*) xattributes.get("name").c_str());
      CORBA::Object_var obj = CORBAUtils::getObjectReference(orb, string_name);
      if (CORBA::is_nil(obj)) {
	logPrintI("Could not obtain a reference to event channel supposedly bound to %s.\nExiting....\n", (const char*) string_name);
	exit(-1);
      }
      CosEventChannelAdmin::EventChannel_var channel;
      channel = CosEventChannelAdmin::EventChannel::_narrow(obj);

      PushCons * pushcons_impl = new PushCons( );
      if (pushcons_impl == NULL) {
	logPrintE("pushcons_impl is NULL exiting...\n");
	exit(-1);
      } else {
	logPrintE("pushcons_impl is not NULL. Carrying on...\n");
      }
      // Now activate object
      if (persistent) {
	if (!xattributes.containsKey("DEF")) {
	  // need to generate a unique ID
	  xattributes.put("DEF", CORBAUtils::generateUniqueId());
	}
	context->activateNode(pushcons_impl, xattributes.get("DEF").c_str());
      } else {
	context->activateNode(pushcons_impl);
      }
      OTGraph::Node_var node_ref = context->getNode(pushcons_impl);
      OT_EventChannel::PushConsNode_var pushcons_ref = OT_EventChannel::PushConsNode::_narrow(node_ref);

      // get Consumer Admin
      CosEventChannelAdmin::ConsumerAdmin_var consumer_admin = CORBAUtils::getConsumerAdmin(channel);

      // get Proxy Supplier
      CosEventChannelAdmin::ProxyPushSupplier_var proxy_supplier = CORBAUtils::getProxyPushSupplier(consumer_admin);

      //      CORBA::String_var string_id = CORBA::string_dup( xattributes.get("name").c_str() );
      //      logPrintI("got string id\n");
      //      PortableServer::ObjectId_var corba_id = PortableServer::string_to_ObjectId(string_id);
      //      logPrintI("got object id");
      //      poa->activate_object_with_id(corba_id, pushcons_source);
      //PortableServer::ObjectId_var corba_source_id = 
      //	poa->activate_object(pushcons_source);
      //      OT_EventChannel::PushConsNode_var pushcons_ref = 
      //	pushcons_source->_this();
      CosEventComm::PushConsumer_var consumer_ref = 
	CosEventComm::PushConsumer::_narrow(pushcons_ref);
      if (CORBA::is_nil(consumer_ref)) {
	logPrintE("Could not narrow down to CosEventComm::PushConsumer\n");
	exit(-1);
      }
      CORBAUtils::connectPushConsumer(proxy_supplier, consumer_ref);
      //CORBAUtils::connectPushConsumer(proxy_supplier, pushcons_ref);
      proxy_pushsupplier_map[pushcons_impl] = proxy_supplier;
      pushconsumers.push_back( pushcons_impl );
      return pushcons_impl;
    }
  else if (name.compare("PushSupp") == 0 ) 
    {
      CosNaming::NamingContextExt::StringName_var string_name = CORBA::string_dup((const char*) xattributes.get("name").c_str());
      CORBA::Object_var obj = CORBAUtils::getObjectReference(orb, string_name);
      if (CORBA::is_nil(obj)) {
	logPrintI("Could not obtain a reference to event channel supposedly bound to %s.\nExiting....\n", (const char*) string_name);
	exit(-1);
      }
      CosEventChannelAdmin::EventChannel_var channel;
      channel = CosEventChannelAdmin::EventChannel::_narrow(obj);      
      PushSupp * pushsupp = new PushSupp( channel );
      pushsupps.push_back( pushsupp );
      ACE_DEBUG((LM_DEBUG, ACE_TEXT("ot:Build PushSupp node\n")));
      return pushsupp;
    }   
#ifdef USE_SHARED
  else if (name.compare("SharedEngineNode") == 0 ) 
    {
      CosNaming::NamingContextExt::StringName_var string_name = CORBA::string_dup((const char*) xattributes.get("name").c_str());
      CORBA::Object_var obj = CORBAUtils::getObjectReference(orb, string_name);
      if (CORBA::is_nil(obj)) {
	logPrintI("Could not obtain a reference to event channel supposedly bound to %s.\nExiting....\n", (const char*) string_name);
	exit(-1);
      }
      CosEventChannelAdmin::EventChannel_var channel;
      channel = CosEventChannelAdmin::EventChannel::_narrow(obj);      
      SharedEngineNode * shared_engine = new SharedEngineNode( channel );
      sharedengines.push_back( shared_engine );
      ACE_DEBUG((LM_DEBUG, ACE_TEXT("ot:Build SharedEngineNode node\n")));
      return shared_engine;
    }
#endif //USE_SHARED
#endif
  return NULL;
}
// pushes events into the tracker tree.

  void CORBASource::pushEvent()
  {
    //logPrintI("CORBASource::pushEvent\n");
    Event updevent;
    bool update = false;

    lock();

    update=newevent;
    if (update) 
      {
	updevent = event;
	newevent = false;
      }

    unlock();

    if (update)
      {
	//logPrintI("CORBASource::pushEvent() updateobservers\n");
	updateObservers( updevent );
      }
  }

  void CORBASource::setEvent(const OT_CORBA::Event& corba_event) 
  {

    //_lock();
    //logPrintI("CORBASource::setEvent\n");
    lock();

    event = corba_event;
    event.timeStamp();
    newevent = true;

    unlock();

    if (context != NULL)
      { 
	if (context->doSynchronization())
	  {
	    context->dataSignal();
	    context->consumedWait();
	  }
      } 

    //updateObservers( event );
    //_unlock();
  }
  void CORBAModule::setContext(Context* ctx)
  {
    context = ctx;
    CORBASourceVector::iterator it;
    for (it=sources.begin(); it!=sources.end();it++)
      {
	(*it)->setContext(context);
      }

  }

#ifdef USE_OMNIEVENTS
  void PushCons::disconnect_push_consumer () 
  {
    logPrintI("Push Consumer: disconnected.\n");
  }

  void PushCons::push(const CORBA::Any& data) 
  {
    //logPrintI("data received\n");
    const OT_CORBA::Event* new_event;
    if (data >>= new_event) {
      Event event(*new_event);
      //      OT_CORBA::Event copy_of_new_event = OT_CORBA::Event(*new_event);
      ACE_Guard<ACE_Thread_Mutex> mutexlock(*mutex);
      updateObservers( event );
      //CORBAUtils::convertFromCORBAEvent(event, copy_of_new_event);
      //      event = Event(copy_of_new_event);
    }
  }
  
#endif //USE_OMNIEVENTS

} //namespace ot
#endif //USE_CORBA

/* 
 * ------------------------------------------------------------
 *   End of CORBAModule.h
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
