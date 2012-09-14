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
/** source file for DGPSIP_Handler
 *
 * @author Gerhard Reitmayr
 * 
 * $Id: DGPSIP_Handler.cxx 1977 2007-08-27 11:48:30Z bornik $
 *
 * @file                                                                   */
/* ======================================================================= */

#define ACE_NLOGGING

#ifdef WIN32
#pragma warning(disable:4244)
#pragma warning(disable:4786)
#endif

// required for WinCE to make sure time_t is 32-bits !!!
// crappy WinCE SDK defines 32-bit version AND 64-bit version,
// while ACE expects the 32-bits version...
#include <cstdlib>

#include <OpenTracker/tool/FixWinCE.h>
#include <OpenTracker/input/DGPSIP_Handler.h>


#ifndef OT_NO_GPS_SUPPORT


namespace ot {

    DGPSIP_Handler::DGPSIP_Handler( GPSDriver * parent_ ) :
        isReconnecting( false ),
        parent( parent_ ),
        counter( 0 )
    {
    
    }

    DGPSIP_Handler::~DGPSIP_Handler()
    {
        parent->removeListener(this);
    }

    int DGPSIP_Handler::open( void * factory )
    {
        int result = ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>::open( factory );
        if( result == 0)
        {
            // send initialization string here 
            char hn[1024];
            char buf[4*1024];
            ACE_OS::hostname( hn, sizeof(hn));
            // the last "\r\nR" part tells the server to send real time updates 
            // alternatively use "\r\nB" for bulk service
            ACE_OS::snprintf(buf, sizeof(buf), "HELO %s %s %s%s\r\n", hn, "GPSDriver", "0.1", "\r\nR");
            result = peer().send_n( buf, ACE_OS::strlen(buf));
            peer().get_remote_addr( remoteAddr  );
            parent->addListener( this );
            ACE_DEBUG((LM_DEBUG, "DGPSIP_Handler::open succeeded\n"));
        }
        return result;
    }

    int DGPSIP_Handler::handle_input(ACE_HANDLE fd)
    {
        // handle rtcm input and send to receiver
        char buf[4*1024];
        int cnt;
        if ((cnt = peer().recv( buf, sizeof(buf))) > 0) {
            ACE_DEBUG((LM_DEBUG, "DGPSIP_Handler::handle_input received %i bytes\n", cnt));
            ACE_DEBUG((LM_DEBUG, "DGPSIP_Handler::handle_input send to rtcm\n"));
            // send data to GPS receiver
            parent->send_rtcm( buf, cnt );
        }
        ACE_DEBUG((LM_DEBUG, "DGPSIP_Handler::handle_input received %i bytes and error %i != %i\n", cnt, errno, EWOULDBLOCK));
        if( cnt <= 0 )  // try a reconnect !
        {
            isReconnecting = true;
            ACE_DEBUG((LM_DEBUG, "DGPSIP_Handler::handle_input trying to reconnect...\n"));
            // remove ourselves from the reactor
            reactor()->remove_handler(fd, ACE_Event_Handler::READ_MASK | ACE_Event_Handler::DONT_CALL );
            peer().close();
            // try to reconnect
            const size_t MAX_RETRIES = 5;
            ACE_Time_Value timeout(1);
            size_t i;
            DGPSIP_Handler * This = (DGPSIP_Handler *) this;
            for( i = 0; i < MAX_RETRIES; ++i )
            {
                ACE_Synch_Options options (ACE_Synch_Options::USE_TIMEOUT, timeout);
                DGPSIP_Connector ipconnect( reactor() );
                if( i > 0 ) 
                    ACE_OS::sleep(timeout);
                ACE_DEBUG((LM_DEBUG, "%i...\n",i));
                if( ipconnect.connect(This, remoteAddr, options ) == 0 )
                    break;
                timeout *= 2;                
            }
            isReconnecting = false;
            ACE_DEBUG((LM_DEBUG, "done %i\n",i));
            return i == MAX_RETRIES ? -1 : 0;
        }
        ACE_DEBUG((LM_DEBUG, "DGPSIP_Handler::handle_input returned\n"));
        return 0;
    }

    void DGPSIP_Handler::newData( const GPResult * res, const char *, void * userData )
    {
        char rptbuf[1024];
    
        if( res->type == GPResult::GPGGA )
        {
            ++counter;
            if( counter % 10 == 0)
            {
                ACE_DEBUG((LM_DEBUG, "DGPSIP_Handler::newData send data to server\n"));
                GPGGA * point = (GPGGA *) res;
                // only send 2 digit on the wire to anonymize our position 
                ACE_OS::snprintf(rptbuf, 1024,
                                 "R %0.2f %0.2f %0.2f -- %d %0.2e %0.2e %0.2e %0.1f %0.1f\n",
                                 point->lat, point->lon, point->height, (double)1.0, (double)1.0, 
                                 (double)1.0, (double)1.0,(double)point->hdop,(double)point->numsats );
                peer().send_n( rptbuf, ACE_OS::strnlen( rptbuf, 1024 ));
                counter = 0;
            }	
        }
    }

    int DGPSIP_Handler::handle_close( ACE_HANDLE fd, ACE_Reactor_Mask mask )
    {
        ACE_DEBUG((LM_DEBUG, "DGPSIP_Handler::handle_close with isReconnecting %i\n", isReconnecting));
        if( !isReconnecting )
            return ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>::handle_close( fd, mask);
        return 0;
    }


}  // namespace ot


#else
#pragma message(">>> OT_NO_GPS_SUPPORT")
#endif // OT_NO_GPS_SUPPORT

/* 
 * ------------------------------------------------------------
 *   End of DGPSIP_Handler.cxx
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
