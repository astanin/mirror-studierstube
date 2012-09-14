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

#include <OpenTracker/OpenTracker.h>
#include <OpenTracker/ubitrack/UbitrackClientRequestConnector.h>
#include <OpenTracker/ubitrack/UbitrackClient.h>

namespace ot 
{
    UbitrackClientRequestConnector::UbitrackClientRequestConnector(UbitrackClientRequestHandler *handler) :
        handler_(handler)
    {  
        logPrintI("UbitrackClientRequestConnector: created\n");
    }

    int UbitrackClientRequestConnector::open (ACE_Reactor *r, int flags) 
    {
        logPrintI("UbitrackClientRequestConnector: open\n");
    
        if (PARENT::open (r, flags) != 0) 
        {
            logPrintI("UbitrackClientRequestConnector: open -> could not open\n");

            return -1;
        }

        return 0;
    }

    int  UbitrackClientRequestConnector::connect_svc_handler
    (UbitrackClientRequestHandler *&svc_handler,
     const ACE_SOCK_Connector::PEER_ADDR &remote_addr,
     ACE_Time_Value *timeout,
     const ACE_SOCK_Connector::PEER_ADDR &local_addr,
     int reuse_addr, int flags, int perms) 
    {

        if (PARENT::connect_svc_handler
            (svc_handler, remote_addr, timeout,
             local_addr, reuse_addr, flags, perms) == -1) 
        {
            return -1;
        }
    
        remote_addr_ = remote_addr;

        return 0;
    }

    int UbitrackClientRequestConnector::connect_svc_handler
    (UbitrackClientRequestHandler *&svc_handler,
     UbitrackClientRequestHandler *&sh_copy,
     const ACE_SOCK_Connector::PEER_ADDR &remote_addr,
     ACE_Time_Value *timeout,
     const ACE_SOCK_Connector::PEER_ADDR &local_addr,
     int reuse_addr, int flags, int perms) 
    {
        sh_copy = svc_handler;
        return this->connect_svc_handler (svc_handler, remote_addr, timeout,
                                          local_addr, reuse_addr, flags, perms);
    }

    int UbitrackClientRequestConnector::reconnect () 
    {
        logPrintI("UbitrackClientRequestConnector: reconnect\n");

        // Maximum number of times to retry connect.
        const size_t MAX_RETRIES = 5;
        ACE_Time_Value timeout (1);
        size_t i;
        for (i = 0; i < MAX_RETRIES; ++i) {
            ACE_Synch_Options options (ACE_Synch_Options::USE_TIMEOUT,
                                       timeout);
            if (i > 0) ACE_OS::sleep (timeout);
            if (connect (handler_, remote_addr_, options) == 0)
                break;
            timeout *= 2; // Exponential backoff.
        }
        return i == MAX_RETRIES ? -1 : 0;
    }

} // namespace ot

/* 
 * ------------------------------------------------------------
 *   End of UbitrackClientRequestConnector.cxx
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
