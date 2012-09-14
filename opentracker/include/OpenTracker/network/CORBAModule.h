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
/** header file for CORBAModule module.
  *
  * @author Joseph Newman
  *
  * $Id: CORBAModule.h 760 2004-11-14 18:20:34Z daniel $
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page module_ref Module Reference
 * @section testmodule TestModule
 * The TestModule provides and drives @ref testsource nodes that generate standard
 * events in certain intervals. It doesn't have a configuration element
 * but reserves the name 'TestConfig' for it. It does something useful as soon as
 * at least one @ref testsource node is created.
 */

#ifndef _CORBAMODULE_H
#define _CORBAMODULE_H

#include "../OpenTracker.h"

#ifdef USE_CORBA

#include "../dllinclude.h"

#ifndef SWIG
#include <vector>
#endif

#ifndef SWIG
#include <OpenTracker/skeletons/OT_CORBA.hh>
#endif

#include <OpenTracker/core/Module.h>
#include <OpenTracker/core/NodeFactory.h>
#include <OpenTracker/network/CORBASink.h>
#include <OpenTracker/network/CORBASource.h>
#ifdef USE_OMNIEVENTS
#include <OpenTracker/network/PushSupp.h>
#include <OpenTracker/network/PushCons.h>
#ifdef USE_SHARED
#include <OpenTracker/network/SharedEngineNode.h>
#endif //USE_SHARED
#endif //USE_OMNIEVENTS

namespace ot {

typedef std::vector<CORBASink*> CORBASinkVector;
//typedef std::map<CORBASource*, OT_CORBA::OTSource_var> SourceNodeMap;
typedef std::vector<CORBASource*> CORBASourceVector;
#ifdef USE_OMNIEVENTS
#ifdef USE_SHARED
typedef std::vector<SharedEngineNode*> SharedEngineNodeVector;
#endif //USE_SHARED
typedef std::vector<PushSupp*> PushSuppVector;
typedef std::vector<PushCons*> PushConsVector;
typedef std::map<PushCons*, CosEventChannelAdmin::ProxyPushSupplier_var> ProxyPushSupplierMap;
#endif //USE_OMNIEVENTS

/**
 * The module and factory to drive the test source nodes. It constructs
 * OTSource and OTSink nodes via the NodeFactory interface and pushes
 * events into the tracker tree according to the nodes configuration.
 * @author Joseph Newman
 * @ingroup network
 */

class OPENTRACKER_API CORBAModule : public Module, public NodeFactory
{
// Members
protected:
    /// list of CORBASource nodes in the tree
    CORBASinkVector sinks;
    CORBASourceVector sources;
#ifdef USE_OMNIEVENTS
    PushSuppVector pushsupps;
    PushConsVector pushconsumers;
    ProxyPushSupplierMap proxy_pushsupplier_map;
#ifdef USE_SHARED
    SharedEngineNodeVector sharedengines;
#endif
#endif
    static bool persistent;

// Methods
public:
    /** constructor method. */
 CORBAModule() : Module(), NodeFactory()
    {
      // Empty constructor
    };
    /** Destructor method, clears nodes member. */
    virtual ~CORBAModule();

    virtual void init(StringTable& attributes,  ConfigNode * localTree);
    virtual void setContext(Context * c);
    /** initializes CORBA orb */
    static void initializeORB(int& argc, char**& argv);

    /** block waiting for orb to be destroyed */
#ifndef SWIG
    void runORB();
#endif
    /** initializes CORBA orb */
    void destroyORB();

    static PortableServer::POAManager_var pman;
    static PortableServer::POA_var getPOA();
    static PortableServer::POA_var getRootPOA();
    static PortableServer::POA_var getINSPOA();
    static PortableServer::POAManager_var getPOAManager();
    static PortableServer::POAManager_var getINSPOAManager();
    static CORBA::ORB_var getORB();
    
    /** Clears nodes */
    virtual void clear();
    virtual void removeNode(Node *);
    /** This method is called to construct a new Node. It compares
     * name to the element name, and if it matches
     * creates a new node.
     * @param name reference to string containing element name
     * @attributes refenrence to StringMap containing attribute values
     * @return pointer to new Node or NULL. The new Node must be
     *         allocated with new ! */
    virtual Node * createNode( const std::string& name, const StringTable& attributes);
    
private:
    static CORBA::ORB_var orb;
    static PortableServer::POA_var poa;
    static PortableServer::POA_var root_poa;
    static PortableServer::POA_var ins_poa;
    static bool initialised;


#ifdef USE_LIVE
        friend class LiveContext;
        friend class Context;
#endif


};

OT_MODULE(CORBAModule);

} // namespace ot

#endif //USE_CORBA

#endif
