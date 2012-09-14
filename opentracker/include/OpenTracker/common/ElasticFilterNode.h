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
/** header file for ElasticFilter Node.
 *
 * @author Flo Ledermann ledermann@ims.tuwien.ac.at
 *
 * $Id: ElasticFilterNode.h 1807 2007-04-12 10:35:50Z bornik $
 * @file                                                                   */
/* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section elasticfilter ElasticFilter
 * ElasticFilter simulates an elastic connection between the incoming tracking data and
 * the data that is sent out to the parent nodes. It also interpolates the values between two
 * updates from the source, so this is also useful if you have a low update rate from
 * your tracking and still want smooth animation.

 Example usage/defaults:

 @verbatim
 \<ElasticFilter force="0.02" damp="0.5" frequency="1" offset="0">
 ... (any Event Source)
 \</ElasticFilter>
 @endverbatim
*/

#ifndef _ELASTICFILTERNODE_H
#define _ELASTICFILTERNODE_H

#include "../OpenTracker.h"

/**
 * @author Flo Ledermann
 * @ingroup common
 */


#ifndef OT_NO_ELASTICFILTER_SUPPORT


namespace ot {

    class OPENTRACKER_API ElasticFilterNode : public Node   
    {

    protected:
        bool init;
        /// frequency of updates
        int frequency;
        /// offset of first update relative to main loop start
        int offset;
        /// the event that is posted to the EventObservers
        float damp;     ///< damping factor
        float force;    ///< the strength of the spring in our model

	// internal variables
        Event currentEvent; ///
        Event targetEvent;  ///
        Event vEvent;       ///

        /** constructor method. It sets initial values for the treshhold and type */
        ElasticFilterNode( float force_ = 0.02, float damp_ = 0.5, int frequency_ = 1, int offset_ = 0);

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

        /** pushes event down the line. Needed to access protected
         * updateObservers method in EventGenerator */
        void pushEvent();
        void pullEvent();

        friend class InterpolatorModule;
    };

} // namespace ot


#endif //OT_NO_ELASTICFILTER_SUPPORT


#endif

/* 
 * ------------------------------------------------------------
 *   End of ElasticFilterNode.h
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
