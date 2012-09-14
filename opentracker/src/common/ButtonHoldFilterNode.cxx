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
/** source file for ElasticFilter node.
 *
 * @author Thomas Psik
 *
 * $Id: ButtonHoldFilterNode.cxx 1807 2007-04-12 10:35:50Z bornik $
 * @file                                                                   */
/* ======================================================================= */

#include <cstdlib>

// this will remove the warning 4786
#include <OpenTracker/tool/disable4786.h>
#include <OpenTracker/common/ButtonHoldFilterNode.h>


#ifndef OT_NO_BUTTON_SUPPORT


namespace ot {

    // constructor method
    ButtonHoldFilterNode::ButtonHoldFilterNode( int offDuration_)
        : currentEvent()
    {
        offDuration = offDuration_;
        init = false;
        for (int i=0; i< 16; i++) 
            offCounter[i] = 0;
    } 

    // tests the confidence value and only forwards passing events
    void ButtonHoldFilterNode::onEventGenerated( Event& event, Node & generator )
    {
        //printf("ButtonHoldFilterNode::onEventGenerated() \n");
        if (!init){
            init = true;
        }

        // we have to pass on the information 
        targetButtonEvent = event.getButton();
        currentEvent = event;
    }

    void ButtonHoldFilterNode::pushEvent() {
        if (init) {
            //printf("ButtonHoldFilterNode::push() \n");
            int updatedButtonEvent = 0;

            for (int i=0; i< 8; i++)
            {
                unsigned char bit = (targetButtonEvent >>i) & 1;
			
                // if bit is off incr counter
                if (!bit) 
                {
                    if (offCounter[i] < offDuration)
                        updatedButtonEvent |= 1<<i;
                    // else - button has been off at least offDuration times -> dont set bit -> button is off

                    offCounter[i]++;

                } 
                else
                {
                    offCounter[i] = 0; // reset counter on "on"
                    // set bit in button event
                    updatedButtonEvent |= 1<<i;
                }
            }
    
            currentEvent.getButton() = updatedButtonEvent;
            currentEvent.timeStamp();
            updateObservers(currentEvent);
        }
    }

    void ButtonHoldFilterNode::pullEvent() 
    {
        /// nothing to do here
    }

} // namespace ot


#else
#pragma message(">>> OT_NO_BUTTON_SUPPORT")
#endif //OT_NO_BUTTON_SUPPORT

/* 
 * ------------------------------------------------------------
 *   End of ButtonHoldFilterNode.cxx
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
