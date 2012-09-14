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
/** header file for Filter Node.
 *
 * @author Gerhard Reitmayr
 * 
 * $Id: FilterNode.h 1971 2007-08-21 09:48:51Z bornik $
 * @file                                                                   */
/* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section filternode Filter
 * The Filter node implements a simple linear averaging over a number of events.
 * The events are stored in an EventQueueNode that is the required child of this
 * node. Everytime it receives a new event from the EventQueueNode it computes the
 * weighted average of the events stored in the EventQueueNode. It implements an
 * Event interface.
 *
 * The position is calculated as the weighted average of the last events positions.
 * No normalization is done, so one can use this filter to calculate gain != 1. The
 * rotation is calculated in exponential space of the unit sphere of quaternions. 
 * Here the average is normalized to stay within the result space.
 * Confidence values are also averaged and not normalized.
 *
 * The element has the following attributes :
 * @li @c  weight a number of float values giving the individual weights of the
 *         events averaged. The number of events used from the queue are equal
 *         to the number of weights specified here. The first weight relates to the
 *         newest event. If less events are stored in the queue, no new event is generated.
 * @li @c  type @c all @c (all|position|orientation) a mode attribute that specifies to 
 *         which part of the event data the filter should be applied to. @c position will
 *         apply it only to the position part, @c orientation to the orientation and
 *         @all to both.
 *
 * An example element looks like this :
 * @verbatim
 <Filter weight="0.5 0.5 0.5" type="all">
 <EventQueue>...</EventQueue>
 </Filter>@endverbatim
*/

#ifndef _FILTERNODE_H
#define _FILTERNODE_H

#include "../OpenTracker.h"

#ifndef SWIG
#include <vector>
#endif

/**
 * The Filter node generates a new event event by combining several past 
 * events in a linear fashion. The weights of the summands can be adjusted.
 * It needs a child that both implements the Event and the EventQueue interface
 * to have access to the required number of events and to be triggered to
 * generate a new one.
 * @author Gerhard Reitmayr
 * @ingroup common
 */

namespace ot {

    class OPENTRACKER_API FilterNode : public Node
    {
        // Members
    protected:
        /// local event variable
        Event localEvent;

        /// array of weights
        std::vector<float> weights;

        enum Type { ALL = 0, POSITION, ORIENTATION } type;

        // Methods
    protected:

        /** constructor method
         */
        FilterNode( const std::vector<float> & weights_, const Type & type_ );

    public:

        /** tests for EventGenerator interface being present. Returns the
         * result of the childs implementation of this method.
         * @return 1 if child implements EventGenerator, 0 otherwise */
        virtual int isEventGenerator() ;

        /**
         * this method is called by the EventGenerator to update it's observers.
         * This class computes a transformed event, stores it in its local variable
         * and notifies its observers in turn, propagating the change.
         */
        virtual void onEventGenerated( Event& event, Node& generator);

        void pushEvent();
        void pullEvent();

        friend class CommonNodeFactory;
    };

} // namespace ot

#endif


/* 
 * ------------------------------------------------------------
 *   End of FilterNode.h
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
