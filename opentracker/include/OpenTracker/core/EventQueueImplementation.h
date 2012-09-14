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
/** header file for class EventQueueImplementation
 *
 * @author Gerhard Reitmayr
 *
 * $Id: EventQueueImplementation.h 1971 2007-08-21 09:48:51Z bornik $
 * @file                                                                   */
/* ======================================================================= */

#ifndef _EVENTQUEUEIMPLEMENTATION_H
#define _EVENTQUEUEIMPLEMENTATION_H

#include "../dllinclude.h"

#ifndef SWIG
#include <deque>
#endif

#include "Event.h"

namespace ot {

    /** A front and back insert Vector like container, for simple and
     * fast queueing of timestamped events. */
    typedef std::deque<Event> EventQueue;

    /**
     * Helper class that implements the EventQueue interface using a STL deque
     * container. Some classes do not need the implementation of the EventQeue
     * interfaces while others depend on it. The later should derive from this
     * class.
     * @author Gerhard Reitmayr
     * @ingroup core
     */

    class OPENTRACKER_API EventQueueImplementation
    {
        // Members
    protected:
        /// queue of events
        EventQueue queue;
        // Methods
    protected:
        /** inserts an event so that it is in order in time with the other events.
         * @param event timestamped event that is inserted */
        void insertAtTime(Event& event);

    public:
        /// Destructor method clears queue
        virtual ~EventQueueImplementation();
        /** returns the event number n back in time starting with the
         * newest event for n = 0.
         * @param number the number of the event to be retrieved
         * @return reference to the Event */
        virtual Event& getEvent(unsigned int number = 0) ;
        /** returns the event closes to the given point in time
         * @param time point in the the event should be closest to
         * @return reference to the found event */
        virtual Event& getEventNearTime(double time) ;
        /** returns the size of the queue */
        virtual unsigned int getSize() ;
    };

} // namespace ot

#endif

/* 
 * ------------------------------------------------------------
 *   End of EventQueueImplementation.h
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
