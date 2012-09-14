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
/** header file for GPS_Handler
 *
 * @author Gerhard Reitmayr
 * 
 * $Id: GPS_Handler.h 1547 2006-10-25 10:33:10Z veas $
 *
 * @file                                                                   */
/* ======================================================================= */

#ifndef _GPS_HANDLER_H
#define _GPS_HANDLER_H

#include <ace/Version.h>
#include <ace/Connector.h>
#include <ace/TTY_IO.h>
#include <ace/DEV_Connector.h>
#include <ace/Svc_Handler.h>
#include "../OpenTracker.h"


#ifndef OT_NO_GPS_SUPPORT


namespace ot {


    class GPSDriver;

#define NMEABUFSZ (8 * 1024)

    /**
     * This class retrieves data from the GPS receiver via the serial port. It uses
     * the NMEA parsing routines in nmea.cxx to parse incoming data and forward it 
     * to the GPSDriver and an optional attached DGPSIP server.
     * @ingroup input
     * @author Gerhard Reitmayr
     */

    // If you get errors in the code below, please adapt the version number in the
    // #if statement according to your ACE version.
#define ACE_VERSION_NUM(a, b, c) (((a) << 16) + ((b) << 8) + (c))
#define ACE_VERSION_CURRENT ACE_VERSION_NUM(ACE_MAJOR_VERSION, ACE_MINOR_VERSION, ACE_BETA_VERSION)

#if (ACE_VERSION_CURRENT < ACE_VERSION_NUM(5, 4, 7)) || (defined(_WIN32_WCE) && (_MSC_VER<1300))
    class GPS_Handler : public  ACE_Svc_Handler<ACE_TTY_IO, ACE_TTY_IO::PEER_ADDR, ACE_NULL_SYNCH>
#else
    class GPS_Handler : public  ACE_Svc_Handler<ACE_TTY_IO, ACE_NULL_SYNCH>
#endif
    {
    public:
	/// default constructor for ace framework. Do not use !
	GPS_Handler() {};

	GPS_Handler( GPSDriver * parent_ );
	virtual ~GPS_Handler();
	
	virtual int open( void * factory );
	virtual int handle_input(ACE_HANDLE fd=ACE_INVALID_HANDLE);
	virtual int handle_signal( int, siginfo_t *, ucontext_t * );

    protected:
        char * nmeaRead( char * nmeabuf, int incnt, int & outnt );
	GPSDriver * parent;
	int nmeaind;
	char nmeabuf[NMEABUFSZ];
    };

#undef ACE_VERSION_NUM
#undef ACE_VERSION_CURRENT

    typedef ACE_Connector<GPS_Handler, ACE_DEV_CONNECTOR> GPS_Connector;


}  // namespace ot


#endif // OT_NO_GPS_SUPPORT


#endif // !defined(_GPS_HANDLER_H)

/* 
 * ------------------------------------------------------------
 *   End of GPS_Handler.h
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
