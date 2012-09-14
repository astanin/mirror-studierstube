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
/** header file for VRPN module.
 *
 * @author Gerhard Reitmayr
 *
 * $Id: VRPNModule.h 1547 2006-10-25 10:33:10Z veas $
 * @file                                                                   */
/* ======================================================================= */

/**
 * @page module_ref Module Reference
 * @section vrpnmodule VRPNModule
 * The VRPN module provides the @ref vrpnsource and @ref vrpnsink nodes. It 
 * provides a bi-directional link between VRPN (http://www.vrpn.org/) and 
 * OpenTracker because it can act as both a server and a client. This also
 * enables the use of OpenTracker as a configurable transformation server 
 * within a VRPN network.
 *
 * @note Under Windows, the default server port is already occupied. Therefore
 * use the configuration element to set the port number differently, for example
 * to 3883 the official IANA number for VRPN!
 *
 * The configuration element is @c VRPNConfig and has the following attributes :
 * @li @c port port number to listen for new connections
 * @li @c interface ip address of network interface to use for incoming connections
 *
 * An example configuration element looks like this :
 * @verbatim<VRPNConfig port="3883" interface="localhost"/>@endverbatim
 */

#ifndef _VRPNMODULE_H
#define _VRPNMODULE_H

#include "../OpenTracker.h"

#ifdef USE_VRPN

class vrpn_Connection;

namespace ot {

    class VRPNSource;
    class VRPNSink;

    typedef std::vector<VRPNSource *> VRPNSourceVector;
    typedef std::vector<VRPNSink *> VRPNSinkVector;

    /**
     * The module and factory to drive the VRPN nodes. It constructs
     * VRPNNode nodes via the NodeFactory interface and registers
     * VRPNs with them after parsing
     * @author Gerhard Reitmayr
     * @ingroup network
     */
    class OPENTRACKER_API VRPNModule : public Module, public NodeFactory
    {
        // Members
    protected:
        /// list of sink nodes
        VRPNSinkVector sinks;
        /// list of source nodes
        VRPNSourceVector sources;
        /// interface for VRPNSink servers to use
        std::string ip;
        /// port for VRPNSink servers to use
        int port;
        /// Connection object for VRPNSink servers
        vrpn_Connection * connection;

        // Methods
    public:
        /** constructor method. */
        VRPNModule();

        /** Destructor method, clears nodes member. */
        virtual ~VRPNModule();    

        virtual void init(StringTable& attributes,  ConfigNode * localTree);

        /** This method is called to construct a new Node. It compares
         * name to the VRPNSink and VRPNSource element name, and if it matches
         * creates a new VRPNSink or VRPNSource node.
         * @param name reference to string containing element name
         * @attributes refenrence to StringMap containing attribute values
         * @return pointer to new Node or NULL. The new Node must be
         *         allocated with new ! */
        virtual Node * createNode( const std::string& name,  StringTable& attributes);

        virtual void start();

        virtual void pullEvent();

        virtual void pushEvent();

    };

	OT_MODULE(VRPNModule);
} // namespace ot

#endif

#endif

/* 
 * ------------------------------------------------------------
 *   End of VRPNModule.h
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
