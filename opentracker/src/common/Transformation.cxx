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
/** source file for Transformation Node.
 *
 * @author Gerhard Reitmayr
 *
 * $Id: Transformation.cxx 1846 2007-05-23 08:16:47Z jfn $
 * @file                                                                   */
/* ======================================================================= */

// this will remove the warning 4786
#include <OpenTracker/tool/disable4786.h>

#include <OpenTracker/common/Transformation.h>

namespace ot {

    // constructor method.

    Transformation::Transformation()
        : Node()
        , localEvent()
    {
    }

    // returns the event number n back in time starting with the newest for n = 0

    Event& Transformation::getEvent(unsigned int number)
    {
        Node * child = getChild( 0 );
        if( child != NULL )
        {
            if( child->isEventQueue() == 1 )
            {    
                return *transformEvent( & child->getEvent( number ));
            }
        }
        return Event::null;
    }

    // returns the event closes to the given point in time

    Event& Transformation::getEventNearTime(double time)
    {
        Node * child = getChild( 0 );
        if( child != NULL )
        {
            if( child->isEventQueue() == 1 )
            {    
                return *transformEvent( & child->getEventNearTime( time ));
            }
        }   
        return Event::null;
    }

    // returns the size of the queue

    unsigned int Transformation::getSize()
    {
        Node * child = getChild( 0 );
        if( child != NULL )
        {
            if( child->isEventQueue() == 1 )
            {    
                return child->getSize();
            }
        }
        return 0;
    }

    // the function evaluation method

    Event& Transformation::getEventAtTime(double time)
    {
        Node * child = getChild( 0 );
        if( child != NULL )
        {
            if( child->isTimeDependend() == 1 )
            {    
                return *transformEvent(& child->getEventAtTime( time ));
            }
        }
        return Event::null;
    }

    // tests for EventGenerator interface being present.

    int Transformation::isEventGenerator()
    {
        Node * child = getChild( 0 );
        if( child != NULL )
        {
            return child->isEventGenerator();
        }
        return 0;
    }

    // tests for EventQueue interface being present.

    int Transformation::isEventQueue()
    {
        Node * child = getChild( 0 );
        if( child != NULL )
        {
            return child->isEventQueue();
        }
        return 0;
    }

    // tests for TimeDependend interface being present.

    int Transformation::isTimeDependend()
    {
        Node * child = getChild( 0 );
        if( child != NULL )
        {
            return child->isTimeDependend();
        }
        return 0;
    }

    // this method is called by the EventGenerator to update it's observers.

    void Transformation::onEventGenerated( Event& event, Node& generator)
    {
        Event* transformed_event  = transformEvent( &event );
        updateObservers( *transformed_event );
    }

    void Transformation::pushEvent()
    {
        // nothing to do
    }

    void Transformation::pullEvent()
    {
        // nothing to do
    }

} // namespace ot


/* 
 * ------------------------------------------------------------
 *   End of Transformation.cxx
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
