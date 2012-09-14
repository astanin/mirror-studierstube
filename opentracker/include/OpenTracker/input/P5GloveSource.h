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
/** header file for P5GloveSource Node.
 *
 * @author Hannes Kaufmann, Istvan Barakonyi
 *
 * $Id: P5GloveSource.h 1547 2006-10-25 10:33:10Z veas $
 * @file                                                                   */
/* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section p5glovesource P5GloveSource
 * The P5GloveSource node is an EventGenerator that outputs the
 * current position of the glove and bending information for each finger. It is driven by
 * the @ref p5glovemodule. Note that at this time it only outputs whether the index finger
 * is bent more the 50 degrees. This is the same for all fingers !
 * 
 * It has the following attributes
 * @li @c finger the number of the finger to track, in the range 0 - 4. The mapping is: 0 = thumb, 1 = index, 2 = middle, 3 = ring, 4 = pinky
 *
 * An example element looks like this :
 * @verbatim
 <P5GloveSource finger="1"/>@endverbatim
 *
 */

#ifndef _P5GLOVESOURCE_H
#define _P5GLOVESOURCE_H

#include "../OpenTracker.h"

#ifdef USE_P5GLOVE

namespace ot {

    /**
     * This class implements a simple source that sets its valid flag in
     * regular intervals and updates any EventObservers. 
     * @author Hannes Kaufmann, Istvan Barakonyi
     * @ingroup input
     */
    class OPENTRACKER_API P5GloveSource : public Node
    {
        // Members
    public: 
        /// the event that is posted to the EventObservers
        Event event;
	/// finger to be tracked
	int finger;

        // Methods
    protected:
        /** simple constructor, sets members to initial values */
        P5GloveSource( int finger_) : Node(), finger(finger_)
        {}

    public:
    
        
        /** tests for EventGenerator interface being present. Is overriden to
         * return 1 always.
         * @return always 1 */
        virtual int isEventGenerator()
        {
            return 1;
        }

        friend class P5GloveModule;
    };

}  // namespace ot

#endif

#endif

/* 
 * ------------------------------------------------------------
 *   End of P5GloveSource.h
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
