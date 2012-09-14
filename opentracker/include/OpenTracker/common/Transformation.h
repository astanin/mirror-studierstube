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
/** header file for Transformation Node.
 *
 * @author Gerhard Reitmayr
 *
 * $Id: Transformation.h 1807 2007-04-12 10:35:50Z bornik $
 * @file                                                                   */
/* ======================================================================= */

#ifndef _TRANSFORMATION_H
#define _TRANSFORMATION_H

#include "../OpenTracker.h"

/**
 * A Transform is the abstract superclass for all transform nodes. It provides 
 * the transformation for all three interfaces. By relying on its child to 
 * provide the interfaces implementation it also automatically mirrors its child
 * interfaces. This makes it not typesafe in that the interfaces of its child
 * and parent have to match but this has to be provided by the configuration
 * file DTD.
 * @author Gerhard Reitmayr
 * @ingroup common
 */

namespace ot {

    class OPENTRACKER_API Transformation : public Node
    {
        // Members
    protected:
        /// local event variable
        Event localEvent;

        // Methods
    protected:
        /** transforms a event. Simplifies implementing other Transformations
         * as it is only needed to override this method. It stores the
         * transformed event in the localEvent member variable and returns
         * a pointer to it.
         * @param event pointer to original event
         * @return pointer to localEvent member
         */
        virtual Event* transformEvent( Event* event) = 0;

        /** constructor method
         */
        Transformation();

    public:
        /**
         * returns the event number n back in time starting with the
         * newest for n = 0
         */
        virtual Event& getEvent(unsigned int number = 0);

        virtual Event& getEventNearTime(double time);   

        /** returns the size of the queue. Uses the childs
         * implementation, if possible.
         * @return size of queue */
        virtual unsigned int getSize();

        /** the function evaluation method.
         * @param time the point in time at which function is evaluated
         * @return reference to event value */
        virtual Event& getEventAtTime(double time);

        /** tests for EventGenerator interface being present. Returns the
         * result of the childs implementation of this method.
         * @return 1 if child implements EventGenerator, 0 otherwise */
        virtual int isEventGenerator() ;

        /** tests for EventQueue interface being present. Returns the
         * result of the childs implementation of this method.
         * @return 1 if child implements EventQueue, 0 otherwise */
        virtual int isEventQueue() ;

        /** tests for TimeDependend interface being present. Returns the
         * result of the childs implementation of this method.
         * @return 1 if child implements TimeDependend, 0 otherwise */
        virtual int isTimeDependend() ;

        /**
         * this method is called by the EventGenerator to update it's observers.
         * This class computes a transformed event, stores it in its local variable
         * and notifies its observers in turn, propagating the change.
         */
        virtual void onEventGenerated( Event& event, Node& generator);

        void pushEvent();
        void pullEvent();
    };

} // namespace ot

#endif



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
