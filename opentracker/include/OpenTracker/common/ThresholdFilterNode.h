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
/** header file for ThresholdFilter Node.
 *
 * @author Gerhard Reitmayr
 * 
 * $Id: ThresholdFilterNode.h 1807 2007-04-12 10:35:50Z bornik $
 * @file                                                                   */
/* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section thresholdfilternode ThresholdFilter
 * The ThresholdFilter implements a filter that allows only events that differ sufficiently 
 * from the last event passed through the filter. It computes the 2-norm distance between
 * the positions of the new event and the last stored and the angle between the two quaternions
 * representing the rotations. The angle is computed in radiants. Then they are compared against
 * the two intervals given in the elements attributes. If one of them lies within the given 
 * interval, the event passes and is used as the new reference value.
 *
 * The element has the following attributes :
 * @li @c positionmin @c 0 minimal distance between the two positions
 * @li @c positionmax @c inf maximal distance between the two positions, 
 *                        @c inf means largest possible value
 * @li @c rotationmin @c 0 minimal angle between the two quaternions, a very general measure
 * @li @c rotationmax @c 3.141592654 maximal angle, default is pi the maximal possible angle.
 * Any values will be clamped to these intervals and it is ensured that max is >= min, by
 * setting max = min, if necessary.
 *
 * An example element looks like this :
 * @verbatim
 <ThresholdFilter positionmin="0.1" positionmax="3" rotationmin="0" rotationmax="3">
 <Any EventGenerator element type>
 </ThresholdFilter>@endverbatim
*/

#ifndef _THRESHOLDFILTERNODE_H
#define _THRESHOLDFILTERNODE_H

#include "../OpenTracker.h"

/**
 * The ThresholdFilterNode implements a filter that allows only events that
 * differ in a certain intervall from the last passed event. It computes the
 * 2-norm of the position difference and the dot product of the quaternions
 * representing the orientations and compares them with fixed boundary values.
 * @author Gerhard Reitmayr
 * @ingroup common
 */


#ifndef OT_NO_THRESHOLDFILTER_SUPPORT

namespace ot {

    class OPENTRACKER_API ThresholdFilterNode : public Node
    {
        // Members
    protected:
        /// last saved event variable
        Event lastEvent;
        /// min and max position values, must be in [0,inf)
        float positionMin, positionMax;
        /// min and max rotation values, must be in [0,pi]
        float rotationMin, rotationMax;

        // Methods
    protected:

        /** constructor method
         */
        ThresholdFilterNode( float posmin, float posmax, float rotmin, float rotmax );

    public:

        /** tests for EventGenerator interface being present. Returns the
         * result of the childs implementation of this method.
         * @return 1 if child implements EventGenerator, 0 otherwise */
        virtual int isEventGenerator() ;

        /**
         * this method is called by the EventGenerator to update it's observers.
         */
        virtual void onEventGenerated( Event& event, Node& generator);

        void pushEvent();
        void pullEvent();

        friend class CommonNodeFactory;
    };

} // namespace ot


#endif //OT_NO_THRESHOLDFILTER_SUPPORT

#endif


/* 
 * ------------------------------------------------------------
 *   End of ThresholdFilterNode.h
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
