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
/** header file for Selection Node.
 *
 * @author Ivan Viola, Matej Mlejnek
 *
 * $Id: SelectionNode.h 1807 2007-04-12 10:35:50Z bornik $
 * @file                                                                   */
/* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section selection Selection
 * A Selection node is an EventGenerator node that listens to two other
 * EventGenerator nodes and selects preferred data by the following algorithm.
 * Data from the preferred input is always passed through. Moreover, the node
 * also remembers the timestamp of the last preferred event. If an event arrives
 * from the other input within a certain timeout period to the last preferred
 * event, it is not passed on. This way a stream of preferred events, with 
 * intermittend pauses smaller then the timeout will not be mixed with data
 * from the default source.
 * The preferred input EventGenerator is marked with wrapper tags.
 * It has the following attributes :
 * @li @c timeout, time period between two input events (in milliseconds)

 * and following inputs
 * @li @c Select marks the preferred EventGenerator
 * 
 * An example element using all wrapper elements looks like this :
 * @verbatim
 <Selection timeout="100">
 <Select>
 <One of any EventGenerator element type>
 </Select>
 <One of any EventGenerator element type>
 </Selection>@endverbatim
*/

#ifndef _SELECTIONNODE_H
#define _SELECTIONNODE_H

#include "../OpenTracker.h"

/**
 * A SelectionNode is an EventGenerator node that observes two marked input
 * nodes and selects the data provided by the input nodes. The mark decides
 * from which input node the data should be prefered. It is implemented using
 * wrapper nodes, timeout period and apropriate DTD definitions.
 * @author Ivan Viola, Matej Mlejnek
 * @ingroup common
 */


#ifndef OT_NO_SELECTION_SUPPORT


namespace ot {

    class OPENTRACKER_API SelectionNode : public Node   
    {

    protected:
        /// Variable for timeout value (in miliseconds)
        double timeOut;
        /// Variable for the last event timestamp from the prefered input node
        double lastTimeStamp;
    
        /** constructor method. It sets initial values for the timeout. */
        SelectionNode( double t );

    public:
        /** tests for EventGenerator interface being present and returns
         * 1, if present.
         * @return always 1 */
        virtual int isEventGenerator()
	{
            return 1;
	}

        /**
         * This method is called by any child node. Checks whether the incoming
         * event is from the wrapper node, otherwise it compares it against the
         * last event from the wrapper node and if it was created later then
         * the specified time out, it is passed on.
         * @param event the event value passed
         * @param generator the node generating the event
         */
        virtual void onEventGenerated( Event& event, Node & generator);                                
        void pushEvent();
        void pullEvent();

        friend class CommonNodeFactory;
    };

} // namespace ot


#endif //OT_NO_SELECTION_SUPPORT


#endif


/* 
 * ------------------------------------------------------------
 *   End of SelectionNode.h
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
