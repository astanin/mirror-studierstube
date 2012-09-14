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
/** header file for GPSModule
 *
 * @author Gerhard Reitmayr
 * 
 * $Id: GPSModule.h 2103 2007-12-19 16:39:50Z bornik $
 *
 * @file                                                                   */
/* ======================================================================= */
/**
 * @page module_ref Module Reference
 * @section gpsmodule GPSModule 
 *
 * This module provides and drives @ref gpssource, @ref gpsdirectionsource, @ref gpsinfosource, 
 * @ref gpsgarmincompass and @ref gpsgarminaltitude nodes that 
 * generate various GPS data events. The configuration element is called @c GPSConfig.
 * It reads GPS position data from a GPS receiver connected on the serial port 
 * and transmiting its data in NMEA format as Latitude, Longitude and Height in
 * the x, y and z components of the position. The angles are given in radiants and
 * the height in meter. The data is in WGS84 coordinate system. 
 * The hdop parameter is given as 1 / hdop in the confidence value.
 *
 * It allows to connect an optional DGPSIP server for DGPS correction data. 
 * Be sure to set your GPS receiver to NMEA output mode and RTCM input mode 
 * for this work. It also allows you to log the NMEA strings received from
 * the gps receiver into a dedicated log file for further processing.
 *
 * It has the following attributes :
 * @li @c dev serial port where the GPS receiver is attached.
 * @li @c baudrate default value is 9600 other values are standard baud rates
 * @li @c DGPSserver an optional DGPSIP server for DGPS correction data
 * @li @c DGPSport port of the DGPSIP port, default is 2101
 * @li @c debug on|off prints out RTCM and NMEA data for debugging
 * @li @c DGPSmirror a port number to run a server on that mirrors the RTCM correction
 *        data for other computers. If not specified, no server will be started.
 * @li @c logfile path and name of a file to log all received strings into (optional)
 * @li @c rtcmdev another serial port to output rtcm data, if it is not the device serial port
 *
 * An example configuration element looks like this :
 * @verbatim
 <GPSConfig dev="COM1" baudrate="9600" DGPSserver="something" DGPSport="2101" debug="off"/>@endverbatim
 *
 */

#ifndef _GPSMODULE_H
#define _GPSMODULE_H

#include "../OpenTracker.h"
#include "GPSDriver.h"

#include <string>

class ACE_FILE_IO;


#ifndef OT_NO_GPS_SUPPORT


namespace ot {


    class GPSSource;
    class GPSDirectionSource;
    class GPSGarminCompass;
    class GPSGarminAltitude;
    class GPSInfoSource;

    /**
     * This module supports a GPS receiver with optional DGPS correction data
     * from an DGPSIP server. It drives all the different GPS source nodes associated with it, but only
     * one instance per type. Currently it only supports one device.
     *
     * It uses the GPSDriver class to operate the GPS device and any DGPSIP servers. Any required
     * transformations are implemented in the individual GPS source nodes. 
     * 
     * @see GPSSource, GPSDirectionSource, GPSGarminCompass, GPSGarminAltitude for more information
     * on the individual source nodes.
     *
     * @ingroup input
     * @author Gerhard Reitmayr
     */
    class OPENTRACKER_API GPSModule : public ThreadModule , public NodeFactory, public GPSListener
    {
    public:
        GPSModule();
        virtual ~GPSModule();
    
        virtual void init(StringTable& attributes,  ConfigNode * localTree);
    
        virtual Node * createNode( const std::string & name, const StringTable & attributes );
    
        virtual void pushEvent();
    
        virtual void start();
    
        virtual void close();
    
        virtual void newData( const GPResult * point, const char * line, void * userData );
    
    protected:
    
        virtual void run();
    
        /*template<class T> void updateSource(T * source)
        {
            if( source != NULL )
            {
                source->lock();
                if( source->event.time < source->buffer.time )
                {
                    source->event = source->buffer;
                    source->unlock();
                    source->updateObservers( source->event );
                }
                else
                    source->unlock();
            }
            };
        */
    
        bool debug;
        std::string device;
        int baudRate;
        std::string dgpsServer;
        int dgpsPort;
        int dgpsmirror;
        std::string rtcmdev;
        // specifies the position mode
        // if "ecef" is specified = earth centered positioning are used, otherwise, lon/lat/1.0 is used
        std::string position_mode;
    
        GPSSource * source;
        GPSDirectionSource * dirSource;
        GPSGarminCompass * compassSource;
        GPSGarminAltitude * altitudeSource;
        GPSInfoSource * infoSource;
        GPSDriver * driver;
    
        ACE_FILE_IO * logFile;
    
		void * gps_reactor;

        friend class GPSSource;
        friend class GPSDirectionSource;
        friend class GPSGarminCompass;
        friend class GPSGarminAltitude;
        friend class GPSInfoSource;
    };

	OT_MODULE(GPSModule);

}  // namespace ot


#endif // OT_NO_GPS_SUPPORT


#endif // !defined(_GPSMODULE_H)

/* 
 * ------------------------------------------------------------
 *   End of GPSModule.h
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
