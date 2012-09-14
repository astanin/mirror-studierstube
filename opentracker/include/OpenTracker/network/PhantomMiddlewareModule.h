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
/** header file for PhantomMiddleware module.
  *
  * @author Gerhard Reitmayr
  *
  * $Id: PhantomMiddlewareModule.h 760 2004-11-14 18:20:34Z daniel $
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page module_ref Module Reference
 * @section testmodule PhantomMiddlewareModule
 * The PhantomMiddlewareModule handles receiving and sending
 * Phantom Events
 */

#ifndef _PHANTOMMIDDLEWAREMODULE_H
#define _PHANTOMMIDDLEWAREMODULE_H

#ifdef USE_PHANTOMMIDDLEWARE

#include "../dllinclude.h"

#ifndef SWIG
#include <vector>
#endif

#ifndef SWIG
#include <phantom/Exceptions.hh>
#endif

#include <OpenTracker/OpenTracker.h>
#include <OpenTracker/network/PhantomMiddlewareSink.h>
#include <OpenTracker/network/PhantomMiddlewareSource.h>
#include <OpenTracker/network/PhantomLocationSource.h>
#include <OpenTracker/network/PhantomZoneSource.h>
#include <OpenTracker/network/PhantomListener.h>
#include <OpenTracker/network/PhantomLocationListener.h>
#include <OpenTracker/network/PhantomZoneListener.h>


namespace ot {

  typedef std::vector<PhantomMiddlewareSink*> PhantomMiddlewareSinkVector;

//typedef std::map<std::string, PhantomZoneListener*>     GroupZoneListenerMap;

/**
 * The module and factory to drive the test source nodes. It constructs
 * PhantomMiddlewareSource and PhantomMiddlewareSink nodes via the 
 * NodeFactory interface and pushes events into the tracker tree according 
 * to the nodes configuration.
 * @author Joseph Newman
 * @ingroup network
 */
class OPENTRACKER_API PhantomMiddlewareModule : public Module, public NodeFactory
{
// Members
protected:
    /// list of PhantomMiddlewareModule nodes in the tree
    PhantomMiddlewareSinkVector sinks;
    GroupLocationListenerMap    location_listeners;
    GroupZoneListenerMap        zone_listeners;
// Methods

  public:
    /** constructor method. */
 PhantomMiddlewareModule() : Module(), NodeFactory()
    {
      // Empty constructor
    };
    /** Destructor method, clears nodes member. */
    virtual ~PhantomMiddlewareModule();

    virtual void init(StringTable& attributes,  ConfigNode * localTree);

    /** Clears nodes */
    //virtual void clear();
    /** This method is called to construct a new Node. It compares
     * name to the MessageReceiver and MessageSender element name, 
     * and if it matches
     * creates a new TestSource node.
     * @param name reference to string containing element name
     * @attributes refenrence to StringMap containing attribute values
     * @return pointer to new Node or NULL. The new Node must be
     *         allocated with new ! */
    virtual Node * createNode( const std::string& name,  StringTable& attributes);

    virtual void clear();
      /**
       * closes the module and closes any communication and stops thread.
       */
    virtual void close();
    
    /**
     * starts the receiving threads. It is called after initialisation is done.*/
    virtual void start();

    /**
     * pushes events into the tracker tree. Checks all TestSources and
     * pushes new events, if a TestSource fires.
     */
    virtual void pushEvent();

    virtual void removeNode(Node *);
 private:
};

OT_MODULE(PhantomMiddlewareModule);

} // namespace ot

#endif //USE_PHANTOMMIDDLEWARE

#endif
