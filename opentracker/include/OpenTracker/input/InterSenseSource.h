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
/** header file for InterSenseSource Node.
 *
 * @author Ivan Viola, Matej Mlejnek, Gerhard Reitmayr
 *
 * $Id: InterSenseSource.h 1807 2007-04-12 10:35:50Z bornik $
 * @file                                                                   */
/* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section intersensesource InterSenseSource
 * The InterSenseSource node is a simple EventGenerator that generates events
 * for an InterSense tracker. It only outputs data that is available from 
 * the tracker, for example an InterTrax device will not generate position 
 * data. It is associated with a tracker configuration element from the 
 * @ref intersensemodule via the @c id attribute. See the @ref intersensemodule for
 * details on the reported position and orientation data. The element has the following
 * attributes :
 * @li @c id the id of the tracker as set in the configuration element
 * @li @c station the number of the station of the tracker to output. This 
 *        attribute is ignored for InterTrax devices. Between 0 and 7 
 *        inclusive.
 *
 * An example element looks like this :
 * @verbatim
 <InterSenseSource id="IS600" station="1"/>@endverbatim
*/

#ifndef _INTERSENCESOURCE_H
#define _INTERSENCESOURCE_H

#include "../OpenTracker.h"

// #ifdef USE_INTERSENSE

/**
 * This class implements a simple EventGenerator. It is updated by the
 * InterSenseModule.
 * @author Ivan Viola, Matej Mlejnek, Gerhard Reitmayr
 * @ingroup input
 */

namespace ot {

    class OPENTRACKER_API InterSenseSource : public Node
    {
        // Members
    public: 
        /// the event that is posted to the EventObservers
        Event event;
        /// the station of the tracker
        unsigned station;

        // Methods
    protected:
        /** simple constructor, sets members to initial values */
        InterSenseSource(unsigned station_) : Node(), station(station_)
        {}

    public:            
        /** tests for EventGenerator interface being present. Is overriden to
         * return 1 always.
         * @return always 1 */
        virtual int isEventGenerator()
        {
            return 1;
        }
        void pushEvent() {};
        void pullEvent() {};

        friend class InterSenseModule;
    };

} // namespace ot

#endif

// #endif

/* 
 * ------------------------------------------------------------
 *   End of InterSenseSource.h
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
