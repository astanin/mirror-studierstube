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
/** header file for NetworkSinkModule module.
 *
 * @author Gerhard Reitmayr, Mathis Csisinko
 *
 * $Id: NetworkSinkModule.h 2116 2008-02-05 11:29:45Z sareika $
 * @file                                                                    */
/* ======================================================================== */

/**
 * @page module_ref Module Reference
 * @section networksinkmodule NetworkSinkModule
 * The NetworkSinkModule graps events from the tracker tree and sends them
 * to other hosts either via multicast groups or by unicast datagrams. The
 * receiving groups addresses are set in the @ref networksink nodes.
 * It's configuration element has the following attributes :
 * @li @c name a string idenfifying the server
 *
 * An example configuration element looks like this :
 * @verbatim
 <NetworkSinkConfig name="TrackSpatz Server"/>@endverbatim
*/

#ifndef _NETWORKSINKMODULE_H
#define _NETWORKSINKMODULE_H

#include "../OpenTracker.h"
#include "Network.h"
#include "NetworkSink.h"


#ifndef OT_NO_NETWORK_SUPPORT


namespace ot {

    struct MulticastSender;
    struct UnicastSender;

    /** a list of MulticastSender records */
    typedef std::vector<MulticastSender *> MulticastSenderVector;
    /** a list of UnicastSender records */
    typedef std::vector<UnicastSender *> UnicastSenderVector;

    /** a list of NetworkSink nodes */
    typedef std::vector<NetworkSink *> SinkVector;

    /**
     * The module and factory to drive the transmission of tracker events via
     * the network. It grabs the events from various NetworkSink nodes and
     * distributes them to specified multicast groups or unicast datagrams.
     *
     * @author Gerhard Reitmayr, Mathis Csisinko, Jochen von Spiczak
     * @ingroup network
     */
	
    class OPENTRACKER_API NetworkSinkModule : public Module, public NodeFactory
    {
		// members
    protected:
        /// list of NetworkSink nodes
        SinkVector nodes;
        /// list of MulticastSender objects
        MulticastSenderVector multicasts;
        /// list of UnicastSender objects
        UnicastSenderVector unicasts;
        /// server name
        std::string serverName;

        bool blockMulticast;

        //methods
    protected:
        /** converts floats to network byte order.
         * @param floats vector of floats to convert
         * @param result pointer where to store the result ( can be floats )
         * @param num number of floats to convert */
        void convertFloatsHToNl(std::vector<float>& floats, float* result, int num);
        /**
         * Runs the unicast transceiver thread. The function reads from and writes
         * to the network and parses network packages.
         * @param data the unicast receiver
         */
    
        static void runUnicastTransceiver( void * data );

    public:
        /** basic constructor */
        NetworkSinkModule();

        /** destructor */
        virtual ~NetworkSinkModule();

        /**
         * initializes the tracker module.
         * @param attributes StringMap of elements attribute values. Should be
         *        possibly , but is not for convenience.
         * @param localTree pointer to root of configuration nodes tree
         */
        virtual void init(StringTable& attributes, ConfigNode * localTree);

        /** This method is called to construct a new Node. It compares
         * name to the NetworkSink element name, and if it matches
         * creates a new NetworkSink node.
         * @param name reference to string containing element name
         * @attributes refenrence to StringMap containing attribute values
         * @return pointer to new Node or NULL. The new Node must be
         *         allocated with new ! */
        virtual Node * createNode( const std::string& name,  const StringTable& attributes);
        /**
         * This method is called after initialisation is finished and before the
         * main loop is started. It opens the sockets needed for communication. */
        virtual void start();
        /**
         * closes the module and closes any communication sockets. */
        virtual void close();
        /**
         * pulls event information out of the tracker tree. It checks the
         * modified member on each NetworkSink node and if a new value is stored
         * it inserts it into the apropriate network data buffer. Then it sends
         * any non-empty data buffers to the network.
         */
        virtual void pullEvent();

    };
   OT_MODULE(NetworkSinkModule);

} // namespace ot


#endif // OT_NO_NETWORK_SUPPORT


#endif

/* 
 * ------------------------------------------------------------
 *   End of NetworkSinkModule.h
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
