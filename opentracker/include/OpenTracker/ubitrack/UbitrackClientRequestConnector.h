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
/** ubitrack client request connector
 *
 * @author Alexander Bornik
 *
 * $Id$
 * @file                                                                   */
/* ======================================================================= */

#ifndef UBITRACKCLIENTREQUESTCONNECTOR_H
#define UBITRACKCLIENTREQUESTCONNECTOR_H

#include "../dllinclude.h"

#include "UbitrackClientRequestHandler.h"
#include "ace/SOCK_Connector.h"
#include "ace/Connector.h"
#include "ace/SOCK_Stream.h"

namespace ot {

    class UbitrackClient;

    class OPENTRACKER_API UbitrackClientRequestConnector : 
        public ACE_Connector<UbitrackClientRequestHandler, ACE_SOCK_CONNECTOR>
    {
    public:
        typedef ACE_Connector<UbitrackClientRequestHandler, 
            ACE_SOCK_CONNECTOR> PARENT;
        UbitrackClientRequestConnector(UbitrackClientRequestHandler *handler = 0);
        virtual ~UbitrackClientRequestConnector() { };

        // Initialize the Connector.
        virtual int open (ACE_Reactor *r = ACE_Reactor::instance (),
                          int flags = 0);
    
        // Re-establish a connection to the logging server.
        int reconnect ();

        UbitrackClientRequestHandler *getHandler() const {return handler_; };

    protected:
        // Connection establishment and authentication hook method.
        virtual int connect_svc_handler
            (UbitrackClientRequestHandler *&svc_handler,
             const ACE_SOCK_Connector::PEER_ADDR &remote_addr,
             ACE_Time_Value *timeout,
             const ACE_SOCK_Connector::PEER_ADDR &local_addr,
             int reuse_addr, int flags, int perms);

        virtual int connect_svc_handler
            (UbitrackClientRequestHandler *&svc_handler,
             UbitrackClientRequestHandler *&sh_copy,
             const ACE_SOCK_Connector::PEER_ADDR &remote_addr,
             ACE_Time_Value *timeout,
             const ACE_SOCK_Connector::PEER_ADDR &local_addr,
             int reuse_addr, int flags, int perms);

        UbitrackClientRequestHandler *handler_;

        // Address at which logging server listens for connections.
        ACE_INET_Addr remote_addr_;
    };

} // namespace ot

#endif //UBITRACKCLIENTREQUESTCONNECTOR_H

/* 
 * ------------------------------------------------------------
 *   End of UbitrackClientRequestConnector.h
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
