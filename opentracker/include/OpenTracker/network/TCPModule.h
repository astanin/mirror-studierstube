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
/** header file for TCPModule module for the virtual show case viewer.
 *
 * @author Gerhard Reitmayr
 *
 * $Id: TCPModule.h 2103 2007-12-19 16:39:50Z bornik $
 * @file                                                                   */
/* ======================================================================= */

/**
 * @page module_ref Module Reference
 * @section tcpmodule TCPModule
 * The TCPModule is a simple network transmit module that implements a TCP/IP
 * server for data streams of several @ref tcpsink nodes. It is configured with a port
 * to listen to for new connections. All clients will get the same stream of
 * data that consists of a station number for the next station and position and
 * orientation information. Clients can connect and disconnect at will. The configuration 
 * element is @c TCPConfig and has the following attributes :
 * @li @c port port number to listen for new connections
 *
 * An example configuration element looks like this :
 * @verbatim
 <TCPConfig port="12345"/>@endverbatim
 * 
 * This module was implemented specifically for the virtual show case viewer of
 * Oliver Bimber <obimber@crcg.edu>. 
 */

#ifndef _TCPMODULE_H
#define _TCPMODULE_H

#include "TCPSink.h"
#include "../OpenTracker.h"

// forward declaration of the ACE stream socket class
class ACE_SOCK_Stream;


#ifndef OT_NO_NETWORK_SUPPORT


namespace ot {

    /** TCPModule implements a multithreaded module to serve a list of clients
     * with a stream of data captured by TCPSink nodes via TCP. The thread is 
     * to establish new connections, in the main thread the sinks are checked for
     * new data and their data is send over the connections. There is no 
     * guaranteed order in which the connection data is send. 
     *
     * The protocol in detail is :
     * @li int - station number
     * @li unsigned char - protocol header
     * @li 3 floats - position in x, y, z 
     * @li 4 floats - orientation as x, y, z, w
     * @li short unsigned - button events
     * @li double - timestamp
     * All data is send in network byte order.
     *
     * @author Gerhard Reitmayr
     * @ingroup network
     */
    class OPENTRACKER_API TCPModule  : public ThreadModule, public NodeFactory {


    protected:
        /// list of sink nodes 
	NodeVector sinks;
        /// list of sockets that connect to clients
        std::vector<ACE_SOCK_Stream *> connections;
        /// port number to listen to
	int port;
        /// flag to stop the listening thread 
        int running;

        void run();

    public:

        TCPModule() : running(0)
	{};

	~TCPModule();

        Node * createNode( const std::string& name,  const StringTable& attributes);
 
	void pullEvent();
	
	void close();

	void init(StringTable& attributes,  ConfigNode * localTree);
    
	void start();
    };

	OT_MODULE(TCPModule);
} // namespace ot


#endif // OT_NO_NETWORK_SUPPORT


#endif

/* 
 * ------------------------------------------------------------
 *   End of TCPModule.h
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
