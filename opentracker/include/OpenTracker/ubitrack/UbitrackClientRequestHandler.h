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
/** ubitrack client request handler
 *
 * @author Alexander Bornik
 *
 * $Id$
 * @file                                                                   */
/* ======================================================================= */

#ifndef UBITRACKCLIENTREQUESTHANDLER_H
#define UBITRACKCLIENTREQUESTHANDLER_H

#include "../dllinclude.h"

#include <ace/Task.h>
#include "ace/Svc_Handler.h"
#include "ace/SOCK_Stream.h"
#include "ace/Log_Record.h"

#include "UbitrackMessage.h"

namespace ot 
{
    class UbitrackClientRequestConnector;
    class UbitrackClient;


    /// Handles incomming connections
    /**
     *  The client handler is instantiated by the client connector
     *  object. It waits for incomming messages, processes
     *  them and sends replies.
     */

    class  OPENTRACKER_API UbitrackClientRequestHandler : 
        public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_MT_SYNCH>
    {
    public:
        enum { QUEUE_MAX = sizeof (ACE_Log_Record) * ACE_IOV_MAX };

        //UbitrackClientRequestHandler(UbitrackClientRequestConnector *nConnector);
        UbitrackClientRequestHandler(UbitrackClient *utcl = NULL);

        //virtual ~UbitrackClientRequestHandler();

        virtual int open(void * connector);

        /// Puts a message block into the send list.
        virtual int put(ACE_Message_Block *mb, ACE_Time_Value * = 0);

        /// Method to send messages (non-blocking)
        void sendMessage(const UbitrackMessage &nMessage);        

    protected:
        UbitrackClientRequestConnector *connector_;
        UbitrackClient *daemon_;
        
        // Handle disconnects from the logging server.
        virtual int handle_input (ACE_HANDLE handle);

        // Hook method forwards log records to server logging daemon.
        virtual int svc ();

        // Send the buffered log records using a gather-write operation.
        virtual int send (ACE_Message_Block *smblk);

        bool handleReceivedMessage(UbitrackMessage& nMsg);        
    
        

    };
} // namespace ot

#endif //UBITRACKCLIENTREQUESTHANDLER_H

/* 
 * ------------------------------------------------------------
 *   End of UbitrackClientRequestHandler.h
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
