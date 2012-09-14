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
/** header file for RangeFilter Node.
 *
 * @author Gerhard Reitmayr
 * 
 * $Id: RangeFilterNode.h 1807 2007-04-12 10:35:50Z bornik $
 * @file                                                                   */
/* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section RangeFilterNode RangeFilter
 * The RangeFilter node filters updates according to the length of the position
 * value. It is configured with a @c min and @c max attribute that define
 * the range of lengths of events to be passed through. Other events are dropped
 *
 * The element has the following attributes :
 * @li @c min 0, minimal length in meters
 * @li @c max large value, maximal length in meters
 *
 * An example element looks like this :
 * @verbatim
 <RangeFilter min="0.1" max="2.5">
 <Any EventGenerator element type>
 </RangeFilter>@endverbatim
*/

#ifndef _RANGEFILTERNODE_H
#define _RANGEFILTERNODE_H

#include "../OpenTracker.h"

/**
 * The RangeFilterNode implements a filter that allows only events the position
 * value of which has a length within a given range. It is a simple EventGenerator
 * node.
 * @author Gerhard Reitmayr
 * @ingroup common
 */


#ifndef OT_NO_RANGEFILTER_SUPPORT


namespace ot {

    class OPENTRACKER_API RangeFilterNode
        : public Node
    {
        // Members
    protected:

        /// minimum length, stores as the square of the length not the actual length !
        float min;
        /// maximum length stored as the square of the length not the actual length !
        float max;

        // Methods
    protected:

        /** constructor method
         */
        RangeFilterNode( float min, float max );

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


#endif //OT_NO_RANGEFILTER_SUPPORT


#endif


/* 
 * ------------------------------------------------------------
 *   End of RangeFilterNode.h
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
