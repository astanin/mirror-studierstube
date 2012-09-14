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
/** header file for GPSDriver
 *
 * @author Gerhard Reitmayr
 * 
 * $Id: GPSDriver.h 1873 2007-06-06 09:54:49Z mendez $
 *
 * @file                                                                   */
/* ======================================================================= */

#ifndef _GPSDRIVER_H
#define _GPSDRIVER_H

#include <map>
#include <string>
#include <vector>

#include "GPSParser.h"

class ACE_Reactor;
class ACE_TTY_IO;


#ifndef OT_NO_GPS_SUPPORT


namespace ot {

    class GPS_Handler;
    class DGPSIP_Handler;
    class DGPSMirror_Handler;
    class DGPSMirror_Acceptor;

    /**
     * This is an abstract interface for the clients of the GPSDriver class.
     * They have to register with the GPSDriver class and will be
     * called back, if new data arrives.
     * @ingroup input
     * @author Gerhard Reitmayr
     */
    class OPENTRACKER_API GPSListener {
    public:
        /** 
         * This is the callback method called by the GPSDriver class, whenever new data
         * arrives. Any subclasses of GPSListener must override it to receive the data.
         * @param point pointer to the GPResult object that represents the parsed data.
         * @param line the corresponding string received from the GPS receiver.
         * @param uderData a pointer to user data stored when registering the listener.
         */
	virtual void newData( const GPResult * point, const char * line, void * userData ) = 0;
	virtual ~GPSListener() {};
    };

    /**
     * The GPSDriver class encapsulates the whole GPS and ACE framework code
     * for reading data from the GPS receiver and also supplying it with
     * correction data from a DGPSIP host. It contains GPS parsing functionality via
     * the GPSParser class. The data provided to listeners is the original GPS data 
     * without any transformations such as feet to meters etc. All such operations
     * are left to the client code.
     *
     * @see GPSParser on how to extend the parsing functions.
     *
     * @ingroup input
     * @author Gerhard Reitmayr
     */
    class GPSDriver  
    {
    public:
	GPSDriver( ACE_Reactor * reactor_ = NULL );
	virtual ~GPSDriver();

	int open( const std::string & device, int baud, const std::string & serveraddr = "", int serverport = 2101, int dgpsmirror = -1, const std::string & rtcmdevice = "" );
	void close();

	void setDebug( bool debug );
	bool getDebug()
	{
            return debugOn;
	};

	ACE_Reactor * getReactor()
	{
            return reactor;
	};

	void addListener( GPSListener * listener, void * userData = NULL );
	void removeListener( GPSListener * listener, void * userData = NULL );

        void addClient( DGPSMirror_Handler * client );
        void removeClient( DGPSMirror_Handler * client );

        bool hasFix( void )
        {
            return fix;
        }

        double getHdop( void )
        {
            return hdop;
        }

        double getNumSat( void )
        {
            return numsat;
        }

    protected:

	void send_rtcm( const char * buffer, const int len );
        void new_line( const char * line );

	ACE_Reactor * reactor;
	GPS_Handler * receiver;
	DGPSIP_Handler * server;
        DGPSMirror_Acceptor * acceptor;
        ACE_TTY_IO * rtcmdev;

	bool debugOn;
        bool fix;
        double hdop;
        int numsat;

	std::map<GPSListener *, void *> listeners;

        std::vector<DGPSMirror_Handler *> clients;

	friend class GPS_Handler;
	friend class DGPSIP_Handler;
    };

} // namespace ot


#endif // OT_NO_GPS_SUPPORT


#endif // !defined(_GPSDRIVER_H)

/* 
 * ------------------------------------------------------------
 *   End of GPSDriver.h
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
