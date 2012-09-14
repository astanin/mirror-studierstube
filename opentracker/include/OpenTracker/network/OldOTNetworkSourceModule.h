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
/** header file for OldOTNetworkSourceModule module.
  *
  * @author Valerie Maquil
  *
  * $Id: OldOTNetworkSourceModule.h 900 2006-01-19 16:47:43Z spiczak $
  * @file                                                                    */
 /* ======================================================================== */

/**
 * @page module_ref Module Reference
 * @section networksourcemodule NetworksourceModule
 * The NetworkSourceModule listens for data from the Network sent to 
 * multicast groups and pushes the state updates into the tracker tree
 * via @ref networksource nodes. It has no attributes and need not be present
 * in the configuration section to operate.
 */

#ifndef _OLDOTNETWORKSOURCEMODULE_H
#define _OLDOTNETWORKSOURCEMODULE_H

#include "../OpenTracker.h"
#include "OldOTNetwork.h"
#include "OldOTNetworkSource.h"

class ACE_Thread_Manager;

#ifdef OT_OLD_NETWORK_SUPPORT

namespace ot {

class MulticastReceiver;

typedef std::vector<MulticastReceiver *> ReceiverVector;

/**
 * The module and factory to drive the reception of network state updates.
 * It builds NetworkSource nodes that insert data from the network into
 * the tracker tree. It uses the Flexible Network Protocol from the 
 * studierstube. It uses its a thread per multicast group to receive data.
 *
 * @author Gerhard Reitmayr
 * @ingroup network
 */
class OPENTRACKER_API OldOTNetworkSourceModule : public Module, public NodeFactory
{
// members
protected:    
    
    /// ACE Thread manager
    ACE_Thread_Manager * manager;
    /// list of groups to listen for
    ReceiverVector groups;
    
// methods
protected:
    /** Converts num floats from network byte order.
     * @param floats pointer to source data
     * @param result pointer to result array
     * @param num number of floats to convert
     */
    static void convertFloatsNToHl(float* floats, float* result, int num);
    /** the work method for the module thread. This is executed by the new
     * module thread. In this class it does nothing but subclasses should
     * override it to add their implementation. */
    static void run( void * data );
    
public:    
    /** basic constructor */
     OldOTNetworkSourceModule();

     /** destructor */
     virtual ~OldOTNetworkSourceModule();

    /** This method is called to construct a new Node. It compares
     * name to the NetworkSource element name, and if it matches
     * creates a new NetworkSource node.
     * @param name reference to string containing element name
     * @attributes refenrence to StringTable containing attribute values
     * @return pointer to new Node or NULL. The new Node must be
     *         allocated with new ! */
     virtual Node * createNode( const std::string& name,  StringTable& attributes);        
    /**
     * opens the sockets needed for communication and
     * starts the receive thread. It is called after initialisation is done.*/
    virtual void start();
    /**
     * closes the module and closes any communication sockets and stops thread.*/
    virtual void close();    
    /**
     * pushes state information into the tree. It checks whether there is new
     * data for any NetworkSource node, copies it into the nodes and calls
     * push on them.    
     */
    virtual void pushEvent();             
};

OT_MODULE(OldOTNetworkSourceModule);

}  // namespace ot

#endif
#endif
