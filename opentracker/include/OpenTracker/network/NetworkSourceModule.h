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
/** header file for NetworkSourceModule module.
 *
 * @author Gerhard Reitmayr, Mathis Csisinko
 *
 * $Id: NetworkSourceModule.h 2103 2007-12-19 16:39:50Z bornik $
 * @file                                                                    */
/* ======================================================================== */

/**
 * @page module_ref Module Reference
 * @section networksourcemodule NetworksourceModule
 * The NetworkSourceModule listens for data from the Network sent to
 * multicast groups or by unicast datagrams and pushes the event updates into
 * the tracker tree via @ref networksource nodes. It has no attributes and need
 * not be present in the configuration section to operate.
 * In unicast mode and in absence of received data polling datagrams are
 * regularily sent to indicate interest in tracking data and a single leave
 * datagram on termination.
 */

#ifndef _NETWORKSOURCEMODULE_H
#define _NETWORKSOURCEMODULE_H

#include "../OpenTracker.h"
#include "Network.h"
#include "NetworkSource.h"


#ifndef OT_NO_NETWORK_SUPPORT


namespace ot {

    struct NetworkReceiver;
    struct MulticastReceiver;

    typedef std::vector<MulticastReceiver *> MulticastReceiverVector;
    typedef std::vector<UnicastReceiver *> UnicastReceiverVector;

    /**
     * The module and factory to drive the reception of network event updates.
     * It builds NetworkSource nodes that insert data from the network into
     * the tracker tree. It uses a thread per multicast group or unicast port to
     * receive and send (if necessary) data.
     *
     * @author Gerhard Reitmayr, Mathis Csisinko, Jochen von Spiczak
     * @ingroup network
     */
    class OPENTRACKER_API NetworkSourceModule : public Module, public NodeFactory
    {
        // members
    protected:

        /// ACE Thread manager
        //ACE_Thread_Manager * manager;
        /// list of multicast groups to listen for
        MulticastReceiverVector multicasts;
        /// list of unicast receivers to listen for
        UnicastReceiverVector unicasts;

        // methods
    protected:
        /** Converts num floats from network byte order.
         * @param floats pointer to source data
         * @param result pointer to result array
         * @param num number of floats to convert
         */
        static void convertFloatsNToHl(float* floats, float* result, int num);
        /**
         * Runs the multicast receiver thread. The function reads from the network
         * and parses network packages.
         * @param data the multicast receiver
         */
        static void runMulticastReceiver( void * data );
        /**
         * Runs the unicast transceiver thread. The function reads from and writes
         * to the network and parses network packages.
         * @param data the unicast receiver
         */
        static void runUnicastTransceiver( void * data );
        /**
         * Processes the received data buffer and writes the decoded data the event
         * of the station defined by the station number.
         * @param receiver the network receiver
         * @return whether the record could be successfully processed
         */
        static bool processRecord( NetworkReceiver * receiver );

    public:
        /** basic constructor */
        NetworkSourceModule();

        /** destructor */
        virtual ~NetworkSourceModule();

        /** This method is called to construct a new Node. It compares
         * name to the NetworkSource element name, and if it matches
         * creates a new NetworkSource node.
         * @param name reference to string containing element name
         * @attributes refenrence to StringTable containing attribute values
         * @return pointer to new Node or NULL. The new Node must be
         *         allocated with new ! */
        virtual Node * createNode( const std::string& name,  const StringTable& attributes);
        /**
         * opens the sockets needed for communication and
         * starts the receive thread. It is called after initialisation is done.*/
        virtual void start();
        /**
         * closes the module and closes any communication sockets and stops thread.*/
        virtual void close();
        /**
         * pushes event information into the tree. It checks whether there is new
         * data for any NetworkSource node, copies it into the nodes and calls
         * push on them.
         */
        virtual void pushEvent();
    };
	
	OT_MODULE(NetworkSourceModule);

}  // namespace ot


#endif // OT_NO_NETWORK_SUPPORT


#endif

/* 
 * ------------------------------------------------------------
 *   End of NetworkSourceModule.h
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
