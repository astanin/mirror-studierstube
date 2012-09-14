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
/** header file for DGPSIP_Handler
 *
 * @author Gerhard Reitmayr
 * 
 * $Id: DGPSIP_Handler.h 1547 2006-10-25 10:33:10Z veas $
 *
 * @file                                                                   */
/* ======================================================================= */

#ifndef _DGPSIP_HANDLER_H
#define _DGPSIP_HANDLER_H

#include <ace/Svc_Handler.h>
#include <ace/Connector.h>
#include <ace/SOCK_Stream.h>
#include <ace/SOCK_Connector.h>
#include "GPSDriver.h"

/**
 * This class retrieves RTCM correction data from a DGPSIP server and sends
 * the correction data to a connected GPS receiver. For debugging purposes, it 
 * can parse and output the RTCM data.
 * @ingroup input
 * @author Gerhard Reitmayr
 */


#ifndef OT_NO_GPS_SUPPORT


namespace ot {

    class DGPSIP_Handler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>, GPSListener
    {
    public:
	/// default constructor for ace framework. Do not use !
	DGPSIP_Handler(){};

	DGPSIP_Handler( GPSDriver * parent_ );
	virtual ~DGPSIP_Handler();

	virtual int open( void * factory );
	virtual int handle_input(ACE_HANDLE fd);

	virtual void newData( const GPResult * point, const char * line, void * userData );

        virtual int handle_close( ACE_HANDLE fd = ACE_INVALID_HANDLE, ACE_Reactor_Mask mask = ACE_Event_Handler::ALL_EVENTS_MASK);

    protected:
        bool isReconnecting;
        GPSDriver * parent;
        int counter;
        ACE_INET_Addr remoteAddr;
    };

    typedef ACE_Connector<DGPSIP_Handler, ACE_SOCK_CONNECTOR> DGPSIP_Connector;

} // namespace ot


#endif // OT_NO_GPS_SUPPORT


#endif // !defined(_DGPSIP_HANDLER_H)

/* 
 * ------------------------------------------------------------
 *   End of DGPSIP_Handler.h
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
