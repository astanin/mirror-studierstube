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
/** header file for ConfidenceFilter Node.
 *
 * @author Gerhard Reitmayr
 *
 * $Id: ConfidenceFilterNode.h 1807 2007-04-12 10:35:50Z bornik $
 * @file                                                                   */
/* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section confidencefilter ConfidenceFilter
 * A ConfidenceFilter nodes filters and throws away incoming events based on their 
 * confidence value.
 * It works either in high or low pass mode and has a configurable treshhold value.
 * In high pass mode it only passes events to its parent that have a confidence equal 
 * or greater than the treshhold value, and vice versa in low pass mode.
 * It has the following attributes :
 * @li @c treshhold the treshhold value in [0,1]
 * @li @c type high either high or low to denote the mode
 *
 * 
 * An example element using all wrapper elements looks like this :
 * @verbatim
 <ConfidenceFilter treshhold="0.8" type="high" >
 ...any event node
 </ConfidenceFilter>@endverbatim
*/

#ifndef _CONFIDENCEFILTERNODE_H
#define _CONFIDENCEFILTERNODE_H

#include "../OpenTracker.h"

/**
 * This class implements an event filter that uses the confidence value of an event to
 * filter out and throw away events.
 * @author Gerhard Reitmayr
 * @ingroup common
 */


#ifndef OT_NO_CONFIDENCE_SUPPORT


namespace ot {

    class OPENTRACKER_API ConfidenceFilterNode : public Node   
    {

    protected:
        /// treshhold value, in [0,1]
        float treshhold;
        /// the type of filter
        enum types { HIGH = 0, LOW } type;
    
        /** constructor method. It sets initial values for the treshhold and type */
        ConfidenceFilterNode( float treshhold_ = 0.5, types type_ = HIGH );

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
 *   End of ConfidenceFilterNode.h
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
