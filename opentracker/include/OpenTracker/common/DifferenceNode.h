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
/** header file for class DifferenceNode.
 *
 * @author Mathis Csisinko
 *
 * $Id: DifferenceNode.h 1807 2007-04-12 10:35:50Z bornik $
 * @file                                                                   */
/* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section Difference Difference
 * The Difference node allows to calculate the difference of attributes of
 * two events temporally separated by a given time. An attribute name has to
 * be supplied to specify, which attribute is involved in calculating the
 * difference. It has the following attributes :
 * @li @c timeframe time between events in difference calculation
 * @li @c attributeName attribute to focus on in difference calculation
 *
 * An example element looks like this :
 * @verbatim
 <Difference timeframe="1000" attributeName="button">
 <Any EventGenerator element type>
 </Difference>@endverbatim
 *
 * @note As child nodes belong to the EventGenerator interface group, the
 * difference might not be actually what you expect, since past event lookup
 * can be inaccurate, when there's no continuous flow of events. Use @ref
 * timestampgenerator in this case for improved behaviour.
*/

#ifndef _DIFFERENCENODE_H
#define _DIFFERENCENODE_H

#include "../OpenTracker.h"


#ifndef OT_NO_DIFFERENCE_SUPPORT


#include "../core/EventQueueImplementation.h"


namespace ot {

	/**
	 * DifferenceNode calculates the difference between two attributes of
	 * temporally seperated events.
	 * 
	 * @ingroup common
	 */
    class OPENTRACKER_API DifferenceNode: public Node,protected EventQueueImplementation
    {
        friend class CommonNodeFactory;

        // Members
    protected:
		std::string attributeName;
        /// time frame value
        double timeFrame;

        // Methods
    protected:
        /** constructor method.
         * @param attributeName attribute to focus on in difference calculation
         * @param timeFrame time frame value for difference calculation */
		DifferenceNode(const std::string &attributeName,double timeFrame): Node(),EventQueueImplementation(),attributeName(attributeName),timeFrame(timeFrame)
		{}
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


#endif //OT_NO_DIFFERENCE_SUPPORT


#endif
