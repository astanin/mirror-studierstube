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
/** source file for PhantomMiddleware module.
  *
  * @author Joseph Newman
  *
  * $Id: PhantomMiddleware.cxx 897 2005-12-03 20:23:38Z jfn $
  * @file                                                                   */
 /* ======================================================================= */

#ifdef USE_PHANTOMMIDDLEWARE

// this will remove the warning 4786
#include <OpenTracker/tool/disable4786.h>
#include <OpenTracker/network/PhantomMiddlewareModule.h>
#include <OpenTracker/network/PhantomMiddlewareSource.h>
#include <phantom/EventIdQuery.hh>
#include <phantom/Exceptions.hh>

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>

#include <ace/Log_Msg.h>
#include <ace/Thread.h>

//using namespace std;

// Destructor method

namespace ot {

  OT_MODULE_REGISTER_FUNC(PhantomMiddlewareModule) {
    OT_MODULE_REGISTRATION_DEFAULT(PhantomMiddlewareModule, "PhantomMiddlewareConfig" );
  }	
  
  // destructor cleans up any allocated memory
  PhantomMiddlewareModule::~PhantomMiddlewareModule()
  {
    logPrintI("PhantomMiddlewareModule destructor\n");
  }

  void PhantomMiddlewareModule::init(StringTable& attributes, ConfigNode * localTree)
  {    
    logPrintI("PhantomMiddlewareModule::init\n");
  } // init

  Node * PhantomMiddlewareModule::createNode( const std::string & name, StringTable & attributes )
  {
    if( name.compare("PhantomLocationSink") == 0 ) 
      {
	int frequency;
	int num = sscanf(attributes.get("frequency").c_str(), " %i", &frequency );
	if( num == 0 || frequency == 0){
	  frequency = 1;
	} 
	const char* group = attributes.get("group").c_str();
	int pid;
	num = sscanf(attributes.get("pid").c_str(), " %i", &pid );
	short eid  =  (short) atoi(attributes.get("eid").c_str());
	std::string source = attributes.get("source");//.c_str();
	PhantomMiddlewareSink* sink;
	if (source == "") {
	  sink = new PhantomMiddlewareSink( group, frequency, pid, eid );
	} else {
	  sink = new PhantomMiddlewareSink( group, frequency, pid, eid, source );
	}
	num = sscanf(attributes.get("eid").c_str(), " %h", &eid );
	sinks.push_back( sink );
	return sink;
      } else if ( name.compare("PhantomLocationSource") == 0 ) {
	std::string group = attributes.get("group");
	int pid;
	sscanf(attributes.get("pid").c_str(), " %i", &pid );
	short eid  =  (short) atoi(attributes.get("eid").c_str());
	std::string source_description = attributes.get("source");
	PhantomLocationSource* source;
	if  (source_description == "") {
	  source = new PhantomLocationSource( group.c_str(), pid, eid );
	} else {
	  source = new PhantomLocationSource( group.c_str(), pid, eid, source_description);
	}
	//GroupMapping::iterator g = groups.find(group);
	GroupLocationListenerMap::iterator l = location_listeners.find(group);
	if (l == location_listeners.end()) {
	  PhantomLocationListener* listener = new PhantomLocationListener(group);
	  listener->addNode(source);
	  listener->Start();
	  listener->activate();
	  location_listeners[group] = listener;
	} else {
	  l->second->addNode(source);
	}
	return source;
    } else if ( name.compare("PhantomZoneSource") == 0 ) {
      std::string group = attributes.get("group");
      int zid, oid;
      short enter_id = (short) atoi(attributes.get("enterid").c_str());
      short exit_id  = (short) atoi(attributes.get("exitid").c_str());
      sscanf(attributes.get("zid").c_str(),     " %i", &zid);
      sscanf(attributes.get("oid").c_str(),     " %i", &oid);
      GroupZoneListenerMap::iterator l = zone_listeners.find(group);
      PhantomZoneSource* source = new PhantomZoneSource(group.c_str(), enter_id, exit_id, zid, oid);
      if (l == zone_listeners.end()) {
	PhantomZoneListener* listener = new PhantomZoneListener(group);
	listener->addNode(source);
	listener->Start();
	listener->activate();
	zone_listeners[group] = listener;
      } else {
	l->second->addNode(source);
      }
      return source;
      } 
    return NULL;
  };

  void PhantomMiddlewareModule::removeNode(Node* node) {
    if (node->getType().compare("PhantomLocationSink") == 0) {
      PhantomMiddlewareSinkVector::iterator result = std::find( sinks.begin(), sinks.end(), node );
        if( result != sinks.end())
        {
	  sinks.erase( result );
	  delete node;
        } else {
	  logPrintE("Node with ID %s not in sinks PhantomMiddlewareModule sinks vector\n", node->get("ID").c_str());
	}
    } else if (node->getType().compare("PhantomLocationSource") == 0) {
      std::string group = node->get("group");
      GroupLocationListenerMap::iterator result = location_listeners.find(group);
      if (result != location_listeners.end()) {
	result->second->removeNode((PhantomMiddlewareSource *) node);
      } else {
	logPrintE("Node not present in SourceNodeMap");
      }
    }
  }

  void PhantomMiddlewareModule::start()
  {

  }
  
  void PhantomMiddlewareModule::close()
  {
    logPrintI("PhantomMiddlewareModule::close()\n");
  }

  void PhantomMiddlewareModule::clear()
  {
    for( PhantomMiddlewareSinkVector::iterator it = sinks.begin(); it != sinks.end(); it++ )
      {
	delete *it;
      }
    sinks.clear();
    // TODO!!!!!!!! Clean up code for sources
  }
  

  void PhantomMiddlewareModule::pushEvent()
  {        
    for (GroupLocationListenerMap::const_iterator listener_it = location_listeners.begin(); listener_it != location_listeners.end(); ++listener_it) {
      listener_it->second->pushEvent();
    }
    for (GroupZoneListenerMap::const_iterator listener_it = zone_listeners.begin(); listener_it != zone_listeners.end(); ++listener_it) {
      listener_it->second->pushEvent();
    }
  }  
} //namespace ot
#endif //USE_PHANTOMMIDDLEWARE
