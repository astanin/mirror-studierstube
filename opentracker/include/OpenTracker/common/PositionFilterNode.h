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
/** header file for Position Filter Node.
 *
 * @author Thomas Psik
 * 
 * $Id: PositionFilterNode.h 1807 2007-04-12 10:35:50Z bornik $
 * @file                                                                   */
/* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section PositionFilterNode PositionFilter
 * The PositionFilterNode node filters updates according to the position information. @see RangeFilterNode
 *  It is configured with a @c min and @c max attribute that define
 * the minimum and maximum of position data to be passed through (like a BoundingBox). Other events are dropped
 *
 * The element has the following attributes :
 * @li @c min -1, -1, -1, minimal position in meters
 * @li @c max 1, 1, 1, maximal position in meters
 *
 * An example element looks like this :
 * @verbatim
 <PositionFilter min="-1 -1 -1" max="1 1 1">
 <Any EventGenerator element type>
 </PositionFilter>@endverbatim
*/
#ifndef _POSITIONFILTERNODE_H
#define _POSITIONFILTERNODE_H

#include "../OpenTracker.h"


#ifndef OT_NO_POSITIONFILTER_SUPPORT


namespace ot {


    /**
     * The PositionFilterNode implements a filter that allows only events the position
     * value of which is within a given range. It is a simple EventGenerator
     * node.
     * @author Thomas Psik
     * @ingroup common
     */
    class OPENTRACKER_API PositionFilterNode
        : public Node
    {
        // Members
    protected:

        /// minimum position x,y,z
        float min[3];
        /// maximum position x,y,z
        float max[3];

        // Methods
    protected:

        /** constructor method
         */
        PositionFilterNode( const float min[3], const float max[3]);

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

}  // namespace ot


#endif //OT_NO_POSITIONFILTER_SUPPORT

#endif 

/* 
 * ------------------------------------------------------------
 *   End of PositionFilterNode.h
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
