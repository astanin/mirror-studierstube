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
/** header file for GPSInfoSource Node.
 *
 * @author Gerhard Reitmayr
 *
 * $Id: GPSInfoSource.h 712 2004-07-14 12:44:50Z tamer $
 * @file                                                                   */
/* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section gpsinfosource GPSInfoSource
 * The GPSInfoSource node is a simple EventGenerator that outputs additional 
 * status data on the GPS signal. It encodes the type of fix in the first entry 
 * of the position, the number of satellites in the second and the hdop value in 
 * the third. It will always fire even if no there is no position fix. The fix type is 
 * 0 for no fix, 1 for uncorrected and 2 for corrected position.
 *
 * An example element looks like this :
 * @verbatim
 <GPSInfoSource/>@endverbatim
*/

#ifndef _GPSINFOSOURCE_H
#define _GPSINFOSOURCE_H

#include <assert.h>

#include "../OpenTracker.h"
#include "GPSDriver.h"


#ifndef OT_NO_GPS_SUPPORT


namespace ot {


    /**
     * A very simple EventGenerator node for outputing GPS position data.
     * @author Gerhard Reitmayr
     * @ingroup input
     */
    class OPENTRACKER_API GPSInfoSource : public Node, public GPSListener  
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
	GPSInfoSource() {};

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

    inline void GPSInfoSource::newData( const GPResult * res, const char * line, void * userData )
    {
        assert( userData != NULL );
        if( res->type == GPResult::GPGGA){
            GPGGA * point = (GPGGA *) res;
            GPSModule * module = (GPSModule *)userData;
            module->lockLoop();
            buffer.timeStamp();
            buffer.getPosition()[0] = (float)point->fix;
            buffer.getPosition()[1] = (float)point->numsats;
            buffer.getPosition()[2] = (float)point->hdop;
            module->unlockLoop();
        }
    }


}  // namespace ot


#endif // OT_NO_GPS_SUPPORT


#endif // !defined(_GPSINFOSOURCE_H)

/* 
 * ------------------------------------------------------------
 *   End of GPSInfoSource.h
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
