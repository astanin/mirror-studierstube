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
/** header file for GPSSource Node.
 *
 * @author Gerhard Reitmayr
 *
 * $Id: GPSSource.h 1807 2007-04-12 10:35:50Z bornik $
 * @file                                                                   */
/* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section gpssource GPSSource
 * The GPSSource node is a simple EventGenerator that outputs GPS position data encoded
 * in the position part of the event. This data is in Latitue, Longitute, Height 
 * in the WGS84 coordinate system. The confidence value is the inverse of the PDOP value. 
 * It is managed by the @ref gpsmodule, see there for more information on how to configure GPS support.
 * Use a filter after that to transform the data into your local coordinate system. The node
 * has no further attributes besides the ID attribute.
 *
 * An example element looks like this :
 * @verbatim
 <GPSSource/>@endverbatim
*/

#ifndef _GPSSOURCE_H
#define _GPSSOURCE_H

#include <assert.h>

#include "../OpenTracker.h"
#include "GPSDriver.h"

/**
 * A very simple EventGenerator node for outputing GPS position data.
 * @author Gerhard Reitmayr
 * @ingroup input
 */


#ifndef OT_NO_GPS_SUPPORT


namespace ot {

    class OPENTRACKER_API GPSSource : public Node, public GPSListener  
    {
    public:

        /// the event that is posted to the EventObservers
        Event event;
        /// the buffer event for data from the GPS receiver
        Event buffer;
    
	/** tests for EventGenerator interface being present. Is overriden to
         * return 1 always.
         * @return always 1 */
        virtual int isEventGenerator()
        {
            return 1;
        }

        virtual void newData( const GPResult * point, const char * line, void * userData );

    protected:
	/// protected constructor so it is only accessible by the module
	GPSSource() {};

        void pushEvent()
        {
            lock();
            if(event.time < buffer.time )
            {
                event = buffer;
                unlock();
                updateObservers( event );
            }
            else
            {
                unlock();
            }
        }

        void pullEvent() {};

	friend class GPSModule;
    };

    inline void GPSSource::newData( const GPResult * res, const char * line, void * userData )
    {
        
        ACE_ASSERT( userData != NULL );
        if( res->type == GPResult::GPGGA){
            GPGGA * point = (GPGGA *) res;
            if( point->fix == 0)
                return;

            GPSModule * module = (GPSModule *)userData;
            module->lockLoop();
            buffer.timeStamp();

          
            
            
            
           if (!ACE_OS::strcmp(module->position_mode.c_str(), ACE_TEXT_CHAR_TO_TCHAR("ecef")))
            {
            buffer.getPosition()[0] = (float)(point->xECEF);
            buffer.getPosition()[1] = (float)(point->yECEF);
            buffer.getPosition()[2] = (float)(point->zECEF);
            } else
	    if (!ACE_OS::strcmp(module->position_mode.c_str(), ACE_TEXT_CHAR_TO_TCHAR("utm_33n")))
            {
		
            buffer.getPosition()[0] = (float)(point->lon_utm_33n);
            buffer.getPosition()[1] = (float)(point->lat_utm_33n);
            buffer.getPosition()[2] = (float)(1.0f);

            } else
	    if (!ACE_OS::strcmp(module->position_mode.c_str(), ACE_TEXT_CHAR_TO_TCHAR("gk_m34")))
            {
                
            buffer.getPosition()[0] = (float)(point->lon_gk_m34);
            buffer.getPosition()[1] = (float)(point->lat_gk_m34);
            buffer.getPosition()[2] = (float)(1.0f);                 
                

            } else
	    if (!ACE_OS::strcmp(module->position_mode.c_str(), ACE_TEXT_CHAR_TO_TCHAR("bmn_34")))
            {
              
            buffer.getPosition()[0] = (float)(point->lon_bmn_34);
            buffer.getPosition()[1] = (float)(point->lat_bmn_34);
            buffer.getPosition()[2] = (float)(1.0f);    
                          

            } 

            
            
            
            
     
            
            
            buffer.getConfidence() = (float)(1 / point->hdop);

            buffer.setAttribute<double>("lat", point->lat); 
            buffer.addAttribute<double>("lon", point->lon); 
            buffer.addAttribute<double>("alt", point->altitude); 
            buffer.addAttribute<short>("numsats", point->numsats); 

            module->unlockLoop();
            
//             std::cout << "Position = " << buffer.getPosition()[0] << buffer.getPosition()[1] << buffer.getPosition()[2] << std::endl;
            
        }
    }
    
} // namespace ot

#endif // !defined(_GPSSOURCE_H)


#endif //OT_NO_GPS_SUPPORT

/* 
 * ------------------------------------------------------------
 *   End of GPSSource.h
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
