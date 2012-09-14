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
/** header file for the raw input source node.
 *
 * @author Mathis Csisinko
 *
 * $Id: RawInputSource.h 1507 2006-10-11 08:22:20Z bornik $
 * @file                                                                   */
/* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section rawinputsource RawInputSource
 * The RawInputSource node is a simple EventGenerator that inserts events 
 * by making use of the Windows raw input API. It has the following elements :
 * @li @c deviceName the Windows device name of the input device which should
 * be tracked
 *
 * An example element looks like this :
 * @verbatim
 <RawInputSource deviceName="\??\HID#Vid_046d&amp;Pid_c016#6&amp;3473f8a6&amp;0&amp;0000#{378de44c-56ef-11d1-bc8c-00a0c91405dd}" />@endverbatim
*/

#ifndef _RAWINPUTSOURCE_H
#define _RAWINPUTSOURCE_H

#include "../OpenTracker.h"

#ifdef USE_RAWINPUT
#ifdef WIN32

#include <windows.h>


namespace ot {

    /** A RawInputSource represents a single input device that is tracked by
     * the Windows raw input API.
     * @ingroup input
     */
    class OPENTRACKER_API RawInputSource: public Node
    {
        friend class RawInputModule;

        // methods
    protected:
        enum MoveMode
        {
            XYZAbsolute,
            XYAbsolute,
            XYRelative,
            ZRelative
        };

        /** constructor method
         * @param hDevice handle to the input device
         */
        RawInputSource(HANDLE hDevice);

        /** tests for EventGenerator interface being present. Is overriden to
         * return 1 always.
         * @return always 1 */
        virtual int isEventGenerator() { return 1; }
	
        const HANDLE getDevice() const { return hDevice; }

        void setButtonEvent(unsigned short,unsigned short);

        void setMoveEvent(MoveMode,float,float,float);

        bool calcEvent();

        Event & getEvent() { return event; }

        // members
    private:
        bool modified;
        Event event;
        HANDLE hDevice;
    };

} // namespace ot

#endif
#endif  // USE_RAWINPUT

#endif

/* ===========================================================================
   End of RawInputSource.h
   ===========================================================================
   Automatic Emacs configuration follows.
   Local Variables:
   mode:c++
   c-basic-offset: 4
   eval: (c-set-offset 'subeventment-open 0)
   eval: (c-set-offset 'case-label '+)
   eval: (c-set-offset 'eventment 'c-lineup-runin-eventments)
   eval: (setq indent-tabs-mode nil)
   End:
   =========================================================================== */
