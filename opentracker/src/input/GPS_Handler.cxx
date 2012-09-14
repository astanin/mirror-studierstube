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
/** source file for GPS_Handler
 *
 * @author Gerhard Reitmayr
 * 
 * $Id: GPS_Handler.cxx 1977 2007-08-27 11:48:30Z bornik $
 *
 * @file                                                                   */
/* ======================================================================= */

#ifdef WIN32
#pragma warning(disable:4244)
#endif

#include <cstdlib>
#include <OpenTracker/tool/FixWinCE.h>
#include <OpenTracker/input/GPS_Handler.h>
#include <OpenTracker/input/GPSDriver.h>

#ifndef OT_NO_GPS_SUPPORT


namespace ot {


    GPS_Handler::GPS_Handler(GPSDriver * parent_) :
        parent( parent_ ),
        nmeaind( 0 )
    {
	
    }

    GPS_Handler::~GPS_Handler()
    {
	   printf("GPSHANDLER DELETED BY SOMEONE\n");
    }

    int GPS_Handler::open( void * factory )
    {
#ifdef WIN32
	return reactor()->register_handler(this);
#else
	return reactor()->register_handler(this, ACE_Event_Handler::READ_MASK);
#endif
    }

    int	GPS_Handler::handle_input(ACE_HANDLE fd)
    {
        return handle_signal(0, NULL, NULL);
    }

    int GPS_Handler::handle_signal( int, siginfo_t *, ucontext_t * )
    {
        int ncnt, rd;
        char * line;

        ncnt = peer().recv( nmeabuf + nmeaind, NMEABUFSZ - nmeaind);
        if (ncnt > 0) {
            nmeaind += ncnt;
            while ((line = nmeaRead(nmeabuf, nmeaind, rd)) !=  NULL) {
    		parent->new_line( line );
                nmeaind -= rd;
                memmove(nmeabuf, nmeabuf + rd, nmeaind);
            }
	}
#ifdef WIN32
        else // this is a hack to avoid high resource consumption on windows !
            ACE_OS::sleep(ACE_Time_Value(0,20000));
#endif
	return 0;
    }

    char * GPS_Handler::nmeaRead( char * nmeabuf, int incnt, int & outnt )
    {
        char *cp, *crp, *onmeabuf;
        int oincnt;
    
        onmeabuf = nmeabuf;
        oincnt = incnt;
    
        if ((cp = (char *)memchr(nmeabuf, '$', incnt)) == NULL) {
            /* no nmea delims - junk it all */
            outnt = incnt;
            return NULL;
        }
        /* else align buff and adjust count */
        incnt -= cp - nmeabuf;
        nmeabuf = cp;
    
        if ((crp = (char *)memchr(nmeabuf, '\n', incnt)) == NULL) {
            /*
             * No cr - return till more data trickles in.
             * Let 'em know how much of buff we looked at and discarded.
             */
            outnt = nmeabuf - onmeabuf;
            return NULL;
        }
        *crp = '\0';		/* turn current buff fragment into a string */
        nmeabuf = crp;
        outnt = nmeabuf - onmeabuf;
        return cp;
    }


} // namespace ot


#else
#pragma message(">>> OT_NO_GPS_SUPPORT")
#endif // OT_NO_GPS_SUPPORT

/* 
 * ------------------------------------------------------------
 *   End of GPS_Handlerx.cxx
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
