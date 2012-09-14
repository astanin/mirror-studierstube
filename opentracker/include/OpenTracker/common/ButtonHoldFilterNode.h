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
/** header file for ButtonHoldFilter Node.
 *
 * @author Thomas Psik tomp@ims.tuwien.ac.at
 *
 * $Id: ButtonHoldFilterNode.h 1807 2007-04-12 10:35:50Z bornik $
 * @file                                                                   */
/* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section buttonholdfilter ButtonHoldFilter
 * ButtonHoldFilter simulates an time delayed release of a buttonsource. It was necessary to introduce this filter to 
 * filter out short drops when using a radio transmission. The filter will hold the "on" event even if there is a drop of 
 * &quot;n&quot; off's. After &quot;n&quot; off's have pssed through the filter it will change the event to off. If there is a "on" in between, 
 * the count will start again. Please notice that there will be a delay in the button-release action due to this filter.
 *

 Example_ for one bit(n=3) - over time)
 @verbatim
 input : 11110111001101000011111
 output: 11111111111111111011111
 @endverbatim
 Example_ for one bit(n=1) - over time)
 @verbatim
 input : 11110111001101000011111
 output: 11111111101111100011111
 @endverbatim

 Example usage/defaults:

 @verbatim
 \<ButtonHoldFilter offDuration="2" >
 ... (any Event Source)
 \</ButtonHoldFilter>
 @endverbatim
*/

#ifndef _ButtonHoldFilterNode_H
#define _ButtonHoldFilterNode_H

#include "../OpenTracker.h"

/**
 * @author Thomas Psik
 * @ingroup common
 */


#ifndef OT_NO_BUTTON_SUPPORT


namespace ot {

    class OPENTRACKER_API ButtonHoldFilterNode : public Node   
    {

    protected:
        bool init;
        /// number of off's necessary to show off in outgoing event
        int offDuration;

        // internal variables
        Event currentEvent; ///
        int targetButtonEvent; ///
        int offCounter[16]; /// counter for each bit

        /** constructor method. It sets initial values for the treshhold and type */
        ButtonHoldFilterNode( int offDuration = 2);

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

        friend class ButtonHoldFilterModule;
    };

} // namespace ot


#endif //OT_NO_BUTTON_SUPPORT


#endif

/* 
 * ------------------------------------------------------------
 *   End of ButtonHoldFilterNode.h
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
