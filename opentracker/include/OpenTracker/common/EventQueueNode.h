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
/** header file for class EventQueueNode.
 *
 * @author Gerhard Reitmayr
 *
 * $Id: EventQueueNode.h 1807 2007-04-12 10:35:50Z bornik $
 * @file                                                                   */
/* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section EventQueue EventQueue
 * The EventQueueNode stores a queue of events of fixed size. The size is
 * set by the attribute length. It stores the last events it received from
 * its EventGenerator child and also passes on events via the EventGenerator
 * interface. It has the following attributes :
 * @li @c length 1 the maximal number of last events to store
 *
 * An example element looks like this :
 * @verbatim
 <EventQueue length="10">
 <Any EventGenerator element type>
 </EventQueue>@endverbatim
*/

#ifndef _EVENTQUEUENODE_H
#define _EVENTQUEUENODE_H

#include "../OpenTracker.h"


#ifndef OT_NO_EVENTQUEUE_SUPPORT


#include "../core/EventQueueImplementation.h"

/**
 * EventQueueNode stores a queue of size length of the last received events.
 * It implements the EventGenerator as well as the EventQueue interface,
 * because it also notifies parents of events it receives.
 * @author Gerhard Reitmayr
 * @ingroup common
 */

namespace ot {

    class OPENTRACKER_API EventQueueNode : public Node, protected EventQueueImplementation
    {
        // Members
    protected:
        /// maximal length of the queue
        int length;

        // Methods
    protected:
        /** constructor method.
         * @param maximum length of the queue */
        EventQueueNode(int length_) : Node(), EventQueueImplementation()
	{
            length = length_;
	}
    public:    
        /** tests for EventGenerator interface being present. Overriden in this
         * node to return this.
         * @return always this */
        virtual int isEventGenerator()
	{
            return 1;
	};
        /** tests for EventQueue interface being present. Overriden in this
         * node to return this.
         * @return always this */
        virtual int isEventQueue()
	{
            return 1;
	};

        /** this method is called by the EventGenerator to update it's observers
         * @param event new event
         * @param generator the calling EventGenerator */
        virtual void onEventGenerated( Event& event, Node& generator);

        /** returns the event number n back in time starting with the
         * newest event for n = 0.
         * @param number the number of the event to be retrieved
         * @return reference to the Event */
        virtual Event& getEvent(unsigned int number = 0)
	{
            return EventQueueImplementation::getEvent( number );
	}
 
        /** returns the event closes to the given point in time
         * @param time point in the the event should be closest to
         * @return reference to the found event */
        virtual Event& getEventNearTime(double time)
	{
            return EventQueueImplementation::getEventNearTime( time );
	}

        /** returns the size of the queue */
        virtual unsigned int getSize()
	{
            return EventQueueImplementation::getSize();
	}

        void pushEvent();
        void pullEvent();

        friend class CommonNodeFactory;
    };

} // namespace ot


#endif //OT_NO_EVENTQUEUE_SUPPORT


#endif


/* 
 * ------------------------------------------------------------
 *   End of EventQueueNode.h
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
