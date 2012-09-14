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
/** header file for XSensSource Node.
 *
 * @author Gerhard Reitmayr
 *
 * $Id$
 * @file                                                                   */
/* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section XSensSource XSensSource
 * The XSensSource node is a simple EventGenerator that outputs orientation data
 * from a single XSens MT9-B orientation tracker. See the @ref XSensModule for more information.
 * The configuration element has the following attributes :
 * @li @c comport the number of the serial port the tracker is connected to, starting with 1
 * @li @c amd (true|false) false whether to use the automatic adjustment to magnetic disturbances.
 *
 * An example element looks like this :
 * @verbatim
 <XSensSource comport="1"/>@endverbatim
 *
 * The coordinate system of the orientation data is as follows. X and Y form the horizontal plane and
 * Z is upwards. The measurement cube itself has a similar body system and reports orientations from the
 * body system into the global system. To convert the readings into an Open Inventor like system use the
 * following configuration. Then X and Z form the horizontal plane and Y is upwards for both global and body system.
 * @verbatim
 <EventTransform rotationtype="matrix" rotation="1 0 0 0 0 1 0 -1 0">
 <EventVirtualTransform rotationtype="matrix" rotation="1 0 0 0 0 -1 0 1 0">
 <Ref USE="xsens"/>
 </EventVirtualTransform>
 </EventTransform>@endverbatim
 *
 */

#ifndef _XSENSSOURCE_H
#define _XSENSSOURCE_H

#include "../OpenTracker.h"

/// XSens driver's interface object
struct IMotionTracker;


#ifndef OT_NO_XSENS_SUPPORT


namespace ot {


    /**
     * The XSensSource reads and propagates data from an XSens MT9-B sensor.
     * @author Gerhard Reitmayr
     * @ingroup input
     */
    class OPENTRACKER_API XSensSource : public Node 
    {
    public:

        /// the event that is posted to the EventObservers
        Event event;
    
	/** tests for EventGenerator interface being present. Is overriden to
         * return 1 always.
         * @return always 1 */
        virtual int isEventGenerator()
        {
            return 1;
        }
    
        /// serial comport numbered from 0
        int comport;

        /// whether to use automatic adjustment to magnetic disturbances or not
        bool amd;

        virtual ~XSensSource();

    protected:
	/// protected constructor so it is only accessible by the module
	XSensSource(int _comport = 1, bool _amd = false);

        void pushEvent();
        void pullEvent();
        

        IMotionTracker * pMT;

        friend class XSensModule;
    };


}  // namespace ot


#endif // OT_NO_XSENS_SUPPORT


#endif // !defined(_XSensSource_H)

/* 
 * ------------------------------------------------------------
 *   End of XSensSource.h
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
