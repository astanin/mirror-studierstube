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
/** source file for GPSDriver
 *
 * @author Gerhard Reitmayr
 * 
 * $Id: GPSDriver.cxx 1621 2006-11-13 12:48:35Z schall $
 *
 * @file                                                                   */
/* ======================================================================= */


// required for WinCE to make sure time_t is 32-bits !!!
// crappy WinCE SDK defines 32-bit version AND 64-bit version,
// while ACE expects the 32-bits version...
#include <cstdlib>

// this will remove the warning 4786
#include <OpenTracker/tool/disable4786.h>
#include <cstdlib>

#include <OpenTracker/OpenTracker.h>
#include <OpenTracker/tool/FixWinCE.h>
#include <ace/Reactor.h>
#include <iostream>
#include <algorithm>
#include <memory>

#include <OpenTracker/input/GPSDriver.h>
#include <OpenTracker/input/GPS_Handler.h>
#include <OpenTracker/input/DGPSIP_Handler.h>
#include <OpenTracker/input/DGPSMirror_Handler.h>

#include <ace/Log_Msg.h>
#include <OpenTracker/tool/OT_ACE_Log.h>


#ifndef OT_NO_GPS_SUPPORT


namespace ot {

    GPSDriver::GPSDriver(ACE_Reactor * reactor_) :
        reactor( reactor_ ),
        receiver( NULL ),
        server( NULL ),
        acceptor( NULL ),
        rtcmdev( NULL ),
        fix( false )
    {
	if( NULL == reactor )
	{       printf("\n reactor is null");
            reactor = ACE_Reactor::instance();
	}
    }

    GPSDriver::~GPSDriver()
    {
	if( receiver != NULL )
            close();
	listeners.clear();
        std::vector<DGPSMirror_Handler *>::reverse_iterator it = clients.rbegin();
        while( it != clients.rend())
        {
            (*it)->destroy();
            it++;
        }
        clients.clear();
    }

    int GPSDriver::open( const std::string & device, int baud, const std::string & serveraddr, int port, int dgpsmirror, const std::string & rtcmdev )
    {
        if( getDebug())
            ACE_DEBUG((LM_INFO, ACE_TEXT("ot:GPSDriver open\n")));

	int result;
	// open the serial port the the GPS receiver
	receiver = new GPS_Handler( this );
	GPS_Connector gpsconnect( reactor );
	result = gpsconnect.connect( receiver, ACE_DEV_Addr(ACE_TEXT_CHAR_TO_TCHAR(device.c_str())));
	if( result == 0)
	{
            // set the appropriate parameters
            ACE_TTY_IO::Serial_Params params;		
            result = receiver->peer().control( ACE_TTY_IO::GETPARAMS, &params);
            params.baudrate = baud;
            params.databits = 8;
            params.stopbits = 1;
            params.parityenb = 0;
            params.ctsenb = 0;
            params.rcvenb = 1;
            params.rtsenb = 1;
            params.dsrenb = 0;
            result = receiver->peer().control(ACE_TTY_IO::SETPARAMS, &params );
            if( result != 0 )
            {
                receiver = NULL;
                //std::cerr << "GPSDriver could not open serial port " << device << " !\n";
                ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:GPSDriver could not open serial port %s\n"), device.c_str()));
            }
	}
        if( getDebug())
            ACE_DEBUG((LM_INFO, ACE_TEXT("ot:GPSDriver opened serial port %s\n"), device.c_str()));
	
	// open the tcp connection to the server, if required
	if( result == 0 && serveraddr.compare("") != 0 )
	{
            server = new DGPSIP_Handler( this );
            DGPSIP_Connector ipconnect( reactor );
            if( ipconnect.connect( server, ACE_INET_Addr( port, serveraddr.c_str() )) != 0 )
            {
                server = NULL;
                logPrintE("GPSDriver could not open connection to DGPS server %s port %d\n", serveraddr.c_str(), port);
            }
            if( getDebug())
            {
                logPrintI("GPSDriver opened connection to %s\n", serveraddr.c_str());
            }
	}

        // open a mirror if we have a DGPS handler
        if( server != NULL && dgpsmirror != -1)
        {
            acceptor = new DGPSMirror_Acceptor( this );
            if( acceptor->open( ACE_INET_Addr( dgpsmirror) , reactor ) != 0 )
            {
                delete acceptor;
                acceptor = NULL;
                ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:GPSDriver could not open DGPS mirror server on port %d\n"), dgpsmirror));
            }
            if( getDebug())
                ACE_DEBUG((LM_INFO, ACE_TEXT("ot:GPSDriver opened mirror listener.\n")));
        }

        // open a dedicated serial port for RTCM output, if we have one
        // it assumes the same baud rate as the serial port
        if( rtcmdev.compare("") != 0)
        {
            this->rtcmdev = new ACE_TTY_IO;
            ACE_DEV_Connector connector( *this->rtcmdev, ACE_DEV_Addr(ACE_TEXT_CHAR_TO_TCHAR(rtcmdev.c_str())));
            // set the appropriate parameters
            ACE_TTY_IO::Serial_Params params;		
            result = this->rtcmdev->control( ACE_TTY_IO::GETPARAMS, &params);
            params.baudrate = baud;
            params.databits = 8;
            params.stopbits = 1;
            params.parityenb = 0;
            params.ctsenb = 0;
            params.rcvenb = 1;
            params.rtsenb = 1;
            params.dsrenb = 0;
            result = this->rtcmdev->control(ACE_TTY_IO::SETPARAMS, &params );			
            if( result != 0 )
            {
                delete this->rtcmdev;
                this->rtcmdev = NULL;
                logPrintE("GPSDriver could not open serial port %s.\n", rtcmdev.c_str());
            }
            if( getDebug())
            {
                logPrintS("GPSDriver opened serial port %s\n", rtcmdev.c_str());
            }
        }    
	return result;
    }

    void GPSDriver::close()
    {
		if ( reactor != NULL)
		{
			//reactor->remove_handler(receiver);
			reactor->close();
		}
	if( server != NULL )
	{
            server->destroy();
            server = NULL;
	}
	if( receiver != NULL )
	{
		
		receiver->shutdown();
		//receiver->destroy();
		//receiver = NULL;
	}
        if( acceptor != NULL )
        {
            delete acceptor;
            acceptor = NULL;
        }
        if( rtcmdev != NULL )
        {
            delete rtcmdev;
            rtcmdev = NULL;
        }    
    }

    void GPSDriver::addListener( GPSListener * listener, void * userData )
    {
	listeners[listener] = userData;
    }

    void GPSDriver::removeListener( GPSListener * listener, void * userData )
    {
	std::map<GPSListener *, void *>::iterator it = listeners.find( listener );
	if( it != listeners.end())
            listeners.erase( listener );
    }

    void GPSDriver::addClient( DGPSMirror_Handler * client )
    {
        if( std::find( clients.begin(), clients.end(), client) == clients.end())
            clients.push_back( client );
    }

    void GPSDriver::removeClient( DGPSMirror_Handler * client )
    {
        std::vector<DGPSMirror_Handler *>::iterator it = std::find( clients.begin(), clients.end(), client);
        if( it != clients.end())
            clients.erase( it );
    }

    void GPSDriver::new_line( const char * line )
    {
	std::auto_ptr<GPResult> result((GPResult *)GPSParser::parse(line));
        // for some messages we use some data on the side 
        if( result->type == GPResult::GPGGA )
        {
            fix = (((GPGGA *)result.get())->fix > 0);
            hdop = ((GPGGA *)result.get())->hdop;
            numsat = ((GPGGA *)result.get())->numsats;
        }
        std::map<GPSListener *, void *>::iterator it;
        for( it = listeners.begin(); it != listeners.end(); it++ )
            (*it).first->newData( result.get(), line, (*it).second );
    }

    void GPSDriver::send_rtcm( const char * buffer, const int len )
    {
        if( NULL != this->rtcmdev )
        {
            ACE_DEBUG((LM_DEBUG, "GPSDriver::send_rtcm send to device\n"));
            rtcmdev->send_n( buffer, len );
        }
	else if( NULL != receiver )
        {
            ACE_DEBUG((LM_DEBUG, "GPSDriver::send_rtcm send to receiver\n"));
            receiver->peer().send_n( buffer, len );
        }
        std::vector<DGPSMirror_Handler *>::iterator it;
        for( it = clients.begin(); it != clients.end(); it ++ )
        {
            ACE_DEBUG((LM_DEBUG, "GPSDriver::send_rtcm send to peer\n"));
            if( (*it)->peer().send_n( buffer, len ) < 0 )
            {
                (*it)->destroy();
                it--;
            }
        }
    }

    void GPSDriver::setDebug( bool debug )
    {
	debugOn = debug;
    }

} // namespace ot


#else
#pragma message(">>> OT_NO_GPS_SUPPORT")
#endif // OT_NO_GPS_SUPPORT

/* 
 * ------------------------------------------------------------
 *   End of GPSDriver.cxx
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
