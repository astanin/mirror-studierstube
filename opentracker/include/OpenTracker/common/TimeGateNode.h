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
/** header file for TimeGate Node.
 *
 * @author Gerhard Reitmayr
 *
 * $Id: TimeGateNode.h 1807 2007-04-12 10:35:50Z bornik $
 * @file                                                                   */
/* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section timegate TimeGate
 * This node acts as a gate for an event flow. It will only let events through, if
 * there was an event received through the Gate input some time before. The exact
 * time intervall is given by the attribute timeframe (in milliseconds). It also
 * works the opposite way by blocking all events coming in within the time frame.
 * In contrast to the @ref selection , it does not pass on event data from the Gate.
 * It has the following attributes :
 * @li @c timeframe, time period between two input events (in milliseconds)
 * @li @c mode (pass|block) in @c pass mode it will only let events pass, that
 *        fall within a timeframe interval of an event received through the Gate
 *        input. In block mode it will block these events.
 *
 * and following inputs
 * @li @c Gate marks the gate that opens or closes the node for some time
 * 
 * An example element using all wrapper elements looks like this :
 * @verbatim
 <TimeGate timeframe="100" mode="pass">
 <Gate>
 <One of any EventGenerator element type>
 </Gate>
 <One of any EventGenerator element type>
 </TimeGate>@endverbatim
*/

#ifndef _TIMEGATENODE_H
#define _TIMEGATENODE_H

#include "../OpenTracker.h"

/**
 * A TimeGateNode is an EventGenerator node that observes two marked input
 * nodes and selects the data provided by the input nodes. The mark decides
 * from which input node the data should be prefered. It is implemented using
 * wrapper nodes, timeout period and apropriate DTD definitions.
 * @author Gerhard Reitmayr
 * @ingroup common
 */


#ifndef OT_NO_TIMEGATE_SUPPORT


namespace ot {

    class OPENTRACKER_API TimeGateNode : public Node   
    {

    protected:
        /// Variable for timeout value (in miliseconds)
        double timeframe;
        /// Variable for the last event timestamp from the prefered input node
        double lastTimeStamp;
        /// mode selection
        enum Mode { PASS , BLOCK } mode;
    
        /** constructor method. It sets initial values for the timeframe and mode. */
        TimeGateNode( double timeframe_, Mode mode_ );

    public:
        /** tests for EventGenerator interface being present and returns
         * 1, if present.
         * @return always 1 */
        virtual int isEventGenerator()
	{
            return 1;
	}

        /**
         * This method is called by any child node. 
         * @param event the event value passed
         * @param generator the node generating the event
         */
        virtual void onEventGenerated( Event& event, Node & generator);                                
        void pushEvent();
        void pullEvent();

        friend class CommonNodeFactory;
    };

} // namespace ot


#endif // OT_NO_TIMEGATE_SUPPORT


#endif


/* 
 * ------------------------------------------------------------
 *   End of TimeGateNode.h
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
