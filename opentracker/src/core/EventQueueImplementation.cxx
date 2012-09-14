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
/** source file for class EventQueueImplementation
 *
 * @author Gerhard Reitmayr
 *
 * $Id: EventQueueImplementation.cxx 1547 2006-10-25 10:33:10Z veas $
 * @file                                                                   */
/* ======================================================================= */

#include <OpenTracker/core/EventQueueImplementation.h>

#include <algorithm>

namespace ot {

    /** simple functor to compare events with a fixed time */
    struct SameOrAfter {
        double time;

        SameOrAfter( double time_ ) :
            time( time_ )
        {}

        bool operator()( const Event & event )
        {
            return (event.time >= time);
        }
    };

    // Destructor method.

    EventQueueImplementation::~EventQueueImplementation()
    {
        queue.clear();
    }

    // returns the event number n back in time starting with the newest for n = 0

    Event& EventQueueImplementation::getEvent(unsigned int number)
    {
        if( number < queue.size())
        {
            return queue[number];
        }
        return Event::null;
    }

    // returns the event closes to the given point in time

    Event& EventQueueImplementation::getEventNearTime(double time)
    {
        EventQueue::iterator index = std::find_if(queue.begin(), queue.end(), SameOrAfter( time ));
        if( index != queue.end())
        {
            if( index == queue.begin() )
            {
                return (*index);
            }
            EventQueue::iterator pre = index - 1;
            if( (*pre).time - time < time - (*index).time )
            {
                return (*pre);
            } else
            {
                return (*index);
            }
        }
        return queue[queue.size()-1];
    }

    // returns the size of the queue

    unsigned int EventQueueImplementation::getSize()
    {
        return queue.size();
    }

    // inserts an event so that it is in order in time with the other events

    void EventQueueImplementation::insertAtTime(Event& event)
    {
        EventQueue::iterator index = std::find_if(queue.begin(), queue.end(), SameOrAfter( event.time ));
        if( index != queue.end())
        {
            if( (*index).time == event.time )
            {
                (*index) = event;
            }
            else
            {
                queue.insert( index, event );
            }
        }
        else
        {
            queue.push_back( event );
        }
    }

} // namespace ot

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
