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
/** header file for ConsoleSink Node.
 *
 * @author Gerhard Reitmayr
 *
 * $Id: ConsoleSink.h 1807 2007-04-12 10:35:50Z bornik $
 * @file                                                                   */
/* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section consolesink ConsoleSink
 * The ConsoleSink node is a simple EventObserver / EventGenerator that passes
 * events on and stores a copy of the last one. It is polled in regular
 * intervalls by the @ref consolemodule and its event is printed out to the console.
 * It has the following attributes :
 * @li @c comment a line of text describing the output
 * @li @c active on (on|off) flag defining whether this sink should store and output
 *        data and be displayed or not.
 *
 * An example element looks like this :
 * @verbatim
 <ConsoleSink comment="my Console node">
 <Any EventGenerator element type>
 </ConsoleSink>@endverbatim
*/

#ifndef _CONSOLESINK_H
#define _CONSOLESINK_H

#include "../OpenTracker.h"

/**
 * This class implements a simple node that stores a copy of the last
 * event it received and passed on for output to the console. The 
 * associated ConsoleModule polls the nodes regularly and prints
 * out the last event stored.
 * @author Gerhard Reitmayr
 * @ingroup common
 */

namespace ot {

    class OPENTRACKER_API ConsoleSink : public Node
    {
        // Members
    public:
        /// comment line
        std::string comment;
        /// the event that is stored
        Event event;
        /// flag whether event was changed since last display
        int changed;
        /// flag whether it is displayed or not
        int active;
        /// flag for forwarding event to system (win only sofar)
        int toSysMouse;

        // Methods
    protected:
        /** constructor method,sets commend member
         * @param comment_ the comment line to use */
        ConsoleSink( const std::string & comment_, int active_ = 0 ) :
            Node(), 
            comment( comment_ ),
            changed( 0 ),
            active( active_ ),
            toSysMouse( 0 )
	{}

    public:
    
    
        /** tests for EventGenerator interface being present. Is overriden to
         * return 1 always.
         * @return always 1 */
        virtual int isEventGenerator()
	{
            return 1;
	}
    
        /**
         * this method notifies the object that a new event was generated.
         * It stores a copy of the received event and passes the event on
         * to its observers.
         * @param event reference to the new event. Do not change the
         *        event values, make a copy and change that !
         * @param generator reference to the EventGenerator object that
         *        notified the EventObserver.
         */
        virtual void onEventGenerated( Event& e, Node& generator)
	{
            if( active )
	    {
                event = e;
                changed = 1;
	    }
            updateObservers( event );
	}

        void pushEvent() {};
        void pullEvent() {};

        friend class ConsoleModule;
    };

} // namespace ot

#endif

/* 
 * ------------------------------------------------------------
 *   End of ConsoleSink.h
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
