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
/** source file for GPSGarminCompass Node.
 *
 * @author Gerhard Reitmayr
 *
 * $Id$
 * @file                                                                   */
/* ======================================================================= */

#include <OpenTracker/OpenTracker.h>
#include <OpenTracker/input/GPSModule.h>
#include <OpenTracker/input/GPSGarminCompass.h>

#ifndef OT_NO_GPS_SUPPORT

namespace ot {
    void GPSGarminCompass::pushEvent()  
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
  
    void GPSGarminCompass::pullEvent()
    {
        // nothing to do
    }
    
    inline void  GPSGarminCompass::newData( const GPResult * res, const char * line, void * userData )
    {
        assert( userData != NULL );
        if( res->type == GPResult::HCHDG){
            HCHDG * point = (HCHDG *) res;
            GPSModule * module = (GPSModule *)userData;
            if( !module->driver->hasFix() )
                return;
            module->lockLoop();
            buffer.timeStamp();
            float temp[4];
            temp[0] = 0;
            temp[1] = 1;
            temp[2] = 0;
            temp[3] = (float)(point->heading * MathUtils::GradToRad);
            MathUtils::axisAngleToQuaternion( copyA2V(temp, 4), buffer.getOrientation() );
            buffer.getConfidence() = (float)(1 / module->driver->getHdop());
            module->unlockLoop();

            if (module->context != NULL && module->context->doSynchronization())
            {
                module->context->dataSignal();
                module->context->consumedWait();
            }       
        }
    }

}  // namespace ot

#endif // OT_NO_GPS_SUPPORT

/* 
 * ------------------------------------------------------------
 *   End of GPSGarminCompass.cxx
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
