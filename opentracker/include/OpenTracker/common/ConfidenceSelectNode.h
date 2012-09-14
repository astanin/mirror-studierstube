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
/** header file for ConfidenceSelect Node.
 *
 * @author Gerhard Reitmayr
 *
 * $Id: ConfidenceSelectNode.h 1807 2007-04-12 10:35:50Z bornik $
 * @file                                                                   */
/* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section confidenceselect ConfidenceSelect
 *
 * The ConfidenceSelect node will filter events based on their confidence value
 * relative to confidence values seen before. It can be used to select the
 * event with the highest confidence value within a short time interval. It is
 * a simple EventGenerator node.
 *
 * It will store the confidence value of an
 * event received and filter out any events that arrive within a certain time span
 * set by the timeout parameter and that have a confidence value below ( or higher )
 * than the stored one. After the timeout it will again store the next event received.
 * If an event passes the test, the events confidence value is stored and
 * the timeout starts again. Two modes are possible : @c high filters out lower values and
 * @c low filters out higher values. The element has the following attributes :
 * @li @c timeout the timeout value in milliseconds
 * @li @c type high either @c high or @c low to set the type of filtering
 *
 * An example element looks like this :
 * @verbatim
 <ConfidenceSelect timeout="100" type="high" >
 ...any event node
 </ConfidenceFilter>@endverbatim
*/

#ifndef _CONFIDENCESELECTNODE_H
#define _CONFIDENCESELECTNODE_H

#include "../OpenTracker.h"

/**
 * This class implements a filter based on the confidence value of events relative to
 * earlier confidence values seen. It is similar to the ConfidenceFilterNode.
 * @author Gerhard Reitmayr
 * @ingroup common
 */


#ifndef OT_NO_CONFIDENCE_SUPPORT


namespace ot {

    class OPENTRACKER_API ConfidenceSelectNode : public Node   
    {

    protected:
        /// timeout value
        double timeout;
        /// last time stamp seen
        double lastTimeStamp;
        /// last confidence value seen
        float lastConfidence;
        /// the type of filter
        enum types { HIGH = 0, LOW } type;
    
        /** constructor method. It sets initial values for the treshhold and type */
        ConfidenceSelectNode( double timeout_ = 100, types type_ = HIGH );

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


#endif // OT_NO_CONFIDENCE_SUPPORT


#endif


/* 
 * ------------------------------------------------------------
 *   End of ConfidenceSelectNode.h
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
