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
/** header file for ToolSink Node.
 *
 * @author Gerhard Reitmayr
 *
 * $Id: ToolSink.h 1159 2006-06-13 13:40:36Z bornik $
 * @file                                                                   */
/* ======================================================================= */

#ifndef _TOOLSINK_H
#define _TOOLSINK_H

#include <OpenTracker.h>

/**
 * This class implements a simple node that stores a copy of the last
 * event it received and passed on for output to the console. The
 * associated ConsoleModule polls the nodes regularly and prints
 * out the last event stored.
 * @author Gerhard Reitmayr
 * @ingroup tool
 */
class ToolSink : public Node
{
    // Members
public:
    /// comment line
    std::string comment;
    /// the event that is stored
    Event event;
    /// flag whether event was changed since last display
    int changed;

    // Methods
protected:
    /** constructor method,sets commend member
     * @param comment_ the comment line to use */
    ToolSink( const std::string & comment_ ) :
        Node(),
        comment( comment_ ),
        changed( 0 )
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
     * @param event reference to the new event.
     * @param generator reference to the EventGenerator object that
     *        notified the EventObserver.
     */
    virtual void onEventGenerated( Event& event, Node& generator)
    {
        changed = 1;
        updateObservers( event );
    }

    friend class ToolIOModule;
};

#endif

/* 
 * ------------------------------------------------------------
 *   End of ToolSink.h
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
