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
/** header file for class TimestampGeneratorNode.
 *
 * @author Mathis Csisinko
 *
 * $Id: TimestampGeneratorNode.h 1890 2007-06-12 14:44:57Z mathis $
 * @file                                                                   */
/* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section timestampgenerator TimestampGenerator
 * The TimestampGenerator either acts as a simple event source generating a
 * dummy timestamp event or takes events generated from a child and repeats
 * them every time after a given timeout value. This is very handy in various
 * time dependend operations, e.g. you want to perform some actions at
 * instants, when no input events are generated. It has the following
 * attributes :
 * @li @c timeout timeout after which the last event is regenerated
 *
 * An example element looks like this :
 * @verbatim
 <TimestampGenerator timeout="1000">
 <Any EventGenerator element type>
 </TimestampGenerator>@endverbatim
*/

#ifndef _TIMESTAMPGENERATORNODE_H
#define _TIMESTAMPGENERATORNODE_H

#include "../OpenTracker.h"


#ifndef OT_NO_TIMESTAMPGENERATOR_SUPPORT

namespace ot {

	/**
	 * TimestampGeneratorNode resubmits generated or artificially creates
	 * events periodically after a configurable timeout value.
	 * 
	 * @ingroup common
	 */

    class OPENTRACKER_API TimestampGeneratorNode: public Node
    {
        friend class TimestampGeneratorModule;

        // Members
    protected:
        /// timeout between two events
        double timeOut;

        /// last event
		Event event;

        // Methods
    protected:
        /** simple constructor, sets members to initial values
         * @param timeOut timeout between two events */
        TimestampGeneratorNode(double timeOut): Node(),timeOut(timeOut)
        {}

        bool calcEvent();

		Event & getEvent() { return event; }
	public:    
        /** tests for EventGenerator interface being present. Overriden in this
         * node to return this.
         * @return always this */
        virtual int isEventGenerator()
		{
            return 1;
		}
        /** this method is called by the EventGenerator to update it's observers
         * @param event new event
         * @param generator the calling EventGenerator */
        virtual void onEventGenerated(Event &event,Node &generator);
	
	void pushEvent();
	void pullEvent();
	};

} // namespace ot


#endif //OT_NO_TIMESTAMPGENERATOR_SUPPORT


#endif
