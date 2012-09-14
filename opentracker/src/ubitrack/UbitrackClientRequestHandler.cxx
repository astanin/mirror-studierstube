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

#include <OpenTracker/OpenTracker.h>
#include <OpenTracker/ubitrack/UbitrackClientRequestHandler.h>
#include <OpenTracker/ubitrack/UbitrackClientRequestConnector.h>
#include <OpenTracker/ubitrack/UbitrackClient.h>
#include <OpenTracker/ubitrack/UbitrackMessage.h>

#include <ace/Reactor.h>
#include <ace/Signal.h>

#include <iostream>
#include <sstream>

#if !defined (FLUSH_TIMEOUT)
#define FLUSH_TIMEOUT 30 /* 120 seconds == 2 minutes. */
#endif /* FLUSH_TIMEOUT */

namespace ot {
    UbitrackClientRequestHandler::
    UbitrackClientRequestHandler(UbitrackClient *utcl)
        : 
        ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_MT_SYNCH>(),
        daemon_(utcl)
    {
    }

    int UbitrackClientRequestHandler::open(void * connector)
    {
        logPrintI("UbitrackClientRequestHandler::open\n");
        connector_ =
            static_cast<UbitrackClientRequestConnector *> (connector);
        int bufsiz = ACE_DEFAULT_MAX_SOCKET_BUFSIZ;
        peer ().set_option (SOL_SOCKET, SO_SNDBUF,
                            &bufsiz, sizeof bufsiz);
        if (reactor ()->register_handler
            (this, ACE_Event_Handler::READ_MASK) == -1)
            return -1;

        if (msg_queue ()->activate () == ACE_Message_Queue_Base::ACTIVATED) 
        {
            msg_queue ()->high_water_mark (QUEUE_MAX);
            return activate (THR_SCOPE_SYSTEM);
        } 
        else
        {
            return 0;
        }
    }


    int UbitrackClientRequestHandler::put(ACE_Message_Block *mb, 
                                          ACE_Time_Value *timeout)
    {    
    
        int result;
        while ((result = putq(mb, timeout)) == -1)
        {
            if (msg_queue()->state() != ACE_Message_Queue_Base::PULSED)
                break;
        }        
        //logPrintI("UbitrackClientRequestHandler::put %d\n", result);

        return result;
    }


    /// Called when a new client connected to the acceptor

    void
    UbitrackClientRequestHandler::sendMessage(const UbitrackMessage& nMessage)
    {
        using namespace std;
        logPrintI("UbitrackClientRequestHandler::sendMessage (%d bytes)\n",nMessage.length());   
        ACE_Message_Block *mb;
        ACE_NEW(mb,  ACE_Message_Block(nMessage.length()+8));
        // setup fill length string with length
        ostringstream osslen;
        osslen.width(8);
        osslen.fill('0');
        osslen << hex <<  nMessage.length();        
        //cerr << osslen.str().c_str() << endl;
        mb->wr_ptr((size_t)ACE_OS::sprintf(mb->wr_ptr(), osslen.str().c_str()));
        
        //cerr << "b: " << mb->length() << " " << mb->wr_ptr() << endl;
        mb->wr_ptr((size_t)ACE_OS::sprintf(mb->wr_ptr(), nMessage.data()));
        mb->wr_ptr(mb->wr_ptr()+1);
        //cerr << "c: " << mb->length() << " " << mb->wr_ptr() << endl;
        
        //cout << "Len: " << (*((unsigned long*)(mb->base())))
        //     << endl; 
        //cout << "Content: " << endl;
        //cout << ((char*)(mb->base())) << endl;       

        put(mb);
    }


    int UbitrackClientRequestHandler::handle_input (ACE_HANDLE h) 
    {
        using namespace std;
        logPrintI("UbitrackClientRequestHandler::handle_input\n");
        ACE_Message_Block *mblk = NULL;
        ACE_Message_Block *payload = NULL;
        ACE_NEW_NORETURN(mblk, ACE_Message_Block(9));
        if (peer().recv_n (mblk->wr_ptr (), 8) == 8) 
        {
            mblk->wr_ptr (8);               // Reflect addition of 8 bytes
            std::istringstream iss(mblk->base());
            ACE_UINT64 length;
            // read length from input
            iss >> hex >> length;
            
            // allocate payload message block
            ACE_NEW_NORETURN(payload, ACE_Message_Block(length));

            size_t ilen;
            
            if ((ilen = peer().recv_n(payload->wr_ptr(), length)) > 0)
            {
                payload-> wr_ptr(ilen);
                mblk->cont(payload);
                cerr << "received message with  " << ilen
                     << " bytes from Ubitrack Server" << endl;

                cerr << "refcount: " << mblk->reference_count() << endl;
                UbitrackMessage msg;
                msg.setXMLString(payload->base());
                mblk->release();
                
                handleReceivedMessage(msg);
                
                return 0;
            }
            cerr << "pl refcount: " << payload->reference_count() << endl;
            payload->release();
            payload = 0;
        }

        // Error cases end up here, so we need to release the memory to
        // prevent a leak.
        cerr << "mb refcount: " << mblk->reference_count() << endl;
        mblk->release();
        mblk = 0;

        return -1;
    }

    int UbitrackClientRequestHandler::svc () 
    {
        using namespace std;
        logPrintI("UbitrackClientRequestHandler: svc\n");

        size_t message_index = 0;
        ACE_Time_Value time_of_last_send (ACE_OS::gettimeofday ());
        ACE_Time_Value timeout;
        ACE_Sig_Action no_sigpipe ((ACE_SignalHandler) SIG_IGN);
        ACE_Sig_Action original_action;
        no_sigpipe.register_action (SIGPIPE, &original_action);
    
        for (;;) 
        {
            logPrintI("UbitrackClientRequestHandler: svc: loop ...\n");
        
            timeout = ACE_OS::gettimeofday ();
            timeout += FLUSH_TIMEOUT;
 

            ACE_Message_Block *mblk = 0;
            if (getq (mblk, &timeout) == -1) 
            {
                logPrintI("UbitrackClientRequestHandler: svc: received or timeout ...\n");

                if (errno == ESHUTDOWN) {
                    if (connector_->reconnect () == -1) break;
                    continue;
                } 
                else if (errno != EWOULDBLOCK) break;
                else if (message_index == 0) continue;
            } 
            else 
            {
                if (mblk->size () == 0
                    && mblk->msg_type () == ACE_Message_Block::MB_STOP)
                { 
                    mblk->release (); 
                    break; 
                }
            }

            if (send (mblk) == -1) 
            {
                break;
            }
            time_of_last_send = ACE_OS::gettimeofday ();
    
        }

        logPrintI("UbitrackClientRequestHandler: svc: loop exit\n");

        no_sigpipe.restore_action (SIGPIPE, original_action);
        return 0;
    }

    int UbitrackClientRequestHandler::send (ACE_Message_Block *smblk) 
    {
        logPrintI("UbitrackClientRequestHandler: send\n");

        int result = 0;

    
        while (peer ().send_n (smblk) == -1)
        {
            if (connector_->reconnect () == -1) 
            {
                result = -1;
                break;
            }

            logPrintI("UbitrackClientRequestHandler: send %d\n", result);
        }
        smblk->release();

        return result;
    }


    // This method takes an incomming message and tries to
    // apply the included operation and data onto the XML
    // database. after that it sends a reply to the sender
    // using the same TCP connection.
    bool
    UbitrackClientRequestHandler::handleReceivedMessage(UbitrackMessage& nInMsg)
    {
        using namespace std;
        logPrintI("UbitrackClientRequestHandler::handleReceivedMessage\n");
        cerr << "message content: " << endl;
        cerr << nInMsg.XMLString() << endl;

        if (daemon_)
        {
            return daemon_->handleMessage(nInMsg);
        }
        else
        {
            logPrintW("UbitrackClientRequestHandler::handleReceivedMessage: UbitrackClient is NULL\n");            
            return false;
        }
    }

} // namespace ot

/* 
 * ------------------------------------------------------------
 *   End of UbitrackClientRequestHandler.cpp
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
