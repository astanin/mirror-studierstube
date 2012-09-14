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
/** source file for GPSModule
 *
 * @author Gerhard Reitmayr
 * 
 * $Id: GPSModule.cxx 2103 2007-12-19 16:39:50Z bornik $
 *
 * @file                                                                   */
/* ======================================================================= */

#ifdef WIN32
#pragma warning(disable:4244)
#endif

#include <cstdlib>
#include <OpenTracker/tool/FixWinCE.h>
#include <ace/OS.h>
#include <ace/Reactor.h>
#include <ace/Thread.h>
#include <ace/FILE_Connector.h>
#include <ace/FILE_IO.h>

#include <OpenTracker/input/GPSModule.h>
#include <OpenTracker/input/GPSSource.h>
#include <OpenTracker/input/GPSDirectionSource.h>
#include <OpenTracker/input/GPSGarminCompass.h>
#include <OpenTracker/input/GPSGarminAltitude.h>
#include <OpenTracker/input/GPSInfoSource.h>

#include <iostream>

#include <ace/Log_Msg.h>
#include <OpenTracker/tool/OT_ACE_Log.h>


#ifndef OT_NO_GPS_SUPPORT


namespace ot {

	OT_MODULE_REGISTER_FUNC(GPSModule){
        OT_MODULE_REGISTRATION_DEFAULT(GPSModule, "GPSConfig");
	}

//ACE_Reactor gps_reactor;

    GPSModule::GPSModule() :
        position_mode( "default" ),
        source( NULL ),
        dirSource( NULL ),
        compassSource( NULL ),
        altitudeSource( NULL ),
        infoSource( NULL ),
        driver( NULL ),
        logFile( NULL )
    {
		gps_reactor = (void *)new ACE_Reactor;
    }

    GPSModule::~GPSModule()
    {
        if( source != NULL )
            delete source;
        if( dirSource != NULL )
            delete dirSource;
        if( compassSource != NULL )
            delete compassSource;
        if( altitudeSource != NULL )
            delete altitudeSource;
        if( infoSource != NULL )
            delete infoSource;
        if( logFile != NULL )
            delete logFile;
		if ( gps_reactor != NULL)
			delete (ACE_Reactor *)gps_reactor;
    }

    void GPSModule::init(StringTable& attributes,  ConfigNode * localTree)
    {
	device = attributes.get("dev");
	dgpsServer = attributes.get("DGPSserver");
        rtcmdev = attributes.get("rtcmdev");
        position_mode = attributes.get("position_mode");
	if( attributes.get( "baudrate", &baudRate ) != 1 )
	{
            baudRate = 9600;		// default GPS receiver baud rate
	}
	if( attributes.get( "DGPSport", &dgpsPort ) != 1 )
	{
            dgpsPort = 2101;		// default DGPSIP port
	}
	if( attributes.get("debug").compare("on") == 0 )
	{
            debug = true;
	}
	else
	{
            debug = false;
	}
        if( attributes.get("DGPSmirror", &dgpsmirror) != 1 )
        {
            dgpsmirror = -1;
        }
        if( attributes.containsKey("logfile") )
        {
            logFile = new ACE_FILE_IO;
            ACE_FILE_Connector connector; 
            if( connector.connect( *logFile, ACE_FILE_Addr(ACE_TEXT_CHAR_TO_TCHAR(attributes.get("logfile").c_str())), 0, ACE_Addr::sap_any, 0, O_WRONLY|O_CREAT|O_APPEND))
            {
                delete logFile;
                logFile = NULL;
                logPrintE("DGPSModule error opening log file %s\n", attributes.get("logfile").c_str());
            }
        }
        Module::init( attributes, localTree );
	logPrintI("GPSModule initialized for port %s and server %s\n", device.c_str(), dgpsServer.c_str());
    }

    Node * GPSModule::createNode( const std::string & name, const StringTable & attributes )
    {
	if( name.compare("GPSSource") == 0 )
	{
            if( source != NULL )
            {
                ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Only one GPSSource can be build !\n")));
                return NULL;
            }
            if( !isInitialized() )
            {
                ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:GPSModule is not initialized, cannot build GPSSource !\n")));
                return NULL;
            }
            source = new GPSSource;
            logPrintI("Built GPSSource node.\n");
            return source;
	}
        if( name.compare("GPSDirectionSource") == 0 )
        {
            if( dirSource != NULL )
            {
                ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Only one GPSDirectionSource can be build !\n")));
                return NULL;
            }
            if( !isInitialized() )
            {
                ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:GPSModule is not initialized, cannot build GPSDirectionSource !\n")));
                return NULL;
            }
            dirSource = new GPSDirectionSource;
            logPrintI("Built GPSDirectionSource node.\n");
            return dirSource;
        }
        if( name.compare("GPSGarminCompass") == 0 )
        {
            if( compassSource != NULL )
            {
                ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Only one GPSGarminCompass can be build !\n")));
                return NULL;
            }
            if( !isInitialized() )
            {
                ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:GPSModule is not initialized, cannot build GPSGarminCompass !\n")));
                return NULL;
            }
            compassSource = new GPSGarminCompass;
            logPrintI("Built GPSGarminCompass node.\n");
            return compassSource;
        }
        if( name.compare("GPSGarminAltitude") == 0 )
        {
            if( altitudeSource != NULL )
            {
                ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Only one GPSGarminAltitude can be build !\n")));
                return NULL;
            }
            if( !isInitialized() )
            {
                ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:GPSModule is not initialized, cannot build GPSGarminAltitude !\n")));
                return NULL;
            }
            altitudeSource = new GPSGarminAltitude;
            logPrintI("Built GPSGarminAltitude node.\n");
            return altitudeSource;
        }
        if( name.compare("GPSInfoSource") == 0 )
        {
            if( infoSource != NULL )
            {
                ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Only one GPSInfoSource can be build !\n")));
                return NULL;
            }
            if( !isInitialized() )
            {
                ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:GPSModule is not initialized, cannot build GPSInfoSource !\n")));
                return NULL;
            }
            infoSource = new GPSInfoSource;
            logPrintI("Built GPSInfoSource node.\n");
            return infoSource;
        }
	return NULL;	
    }

    void GPSModule::pushEvent()
    {
        // here for some template magic:
        // updateSource is a template member function but the compiler appearantly
        // creates different variants for each type passed in :)

        /*
          not necessary for per-node traversal
        updateSource(source);
        updateSource(dirSource);
        updateSource(compassSource);
        updateSource(altitudeSource);
        updateSource(infoSource);
        */
    }

    void GPSModule::start()
    {
	if( isInitialized() && (source != NULL || dirSource != NULL || compassSource != NULL || altitudeSource != NULL || infoSource != NULL ))
	{
            ThreadModule::start();
	}
    }

    void GPSModule::close()
    {
	if( driver != NULL )
	{
            if( driver->getReactor() != NULL )
            {
                driver->getReactor()->end_reactor_event_loop();
            }
            ThreadModule::close();
	}
        if( logFile != NULL )
            logFile->close();
    }

    void GPSModule::run()
    {
	driver = new GPSDriver( (ACE_Reactor *)gps_reactor );
	driver->setDebug( debug ); // only for debug purposes ...
        if( source != NULL )
            driver->addListener( source, this );
        if( dirSource != NULL )
            driver->addListener( dirSource, this );
        if( compassSource != NULL )
            driver->addListener( compassSource, this );
        if( altitudeSource != NULL )
            driver->addListener( altitudeSource, this );
        if( infoSource != NULL )
            driver->addListener( infoSource, this );
    
        if( logFile != NULL )
            driver->addListener( this );
	driver->getReactor()->owner(ACE_Thread::self());
	if( driver->open( device, baudRate, dgpsServer, dgpsPort, dgpsmirror, rtcmdev ) != 0 )
	{
            ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:GPSModule could not start GPSDriver !\n")));
            return;
	}
        if( debug )
	{
            ACE_DEBUG((LM_INFO, ACE_TEXT("ot:GPSModule started GPSDriver !\n")));
	}
	driver->getReactor()->run_reactor_event_loop();
	driver->close();
	delete driver;
    }

    void GPSModule::newData( const GPResult * res, const char * line, void * userData )
    {
        logFile->send( line, ACE_OS::strlen(line));
    }


} // namespace ot


#else
#pragma message(">>> OT_NO_GPS_SUPPORT")
#endif // OT_NO_GPS_SUPPORT

/* 
 * ------------------------------------------------------------
 *   End of GPSModule.cxx
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
