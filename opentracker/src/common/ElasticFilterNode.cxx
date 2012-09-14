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
 * @author Flo Ledermann
 *
 * $Id: ElasticFilterNode.cxx 1807 2007-04-12 10:35:50Z bornik $
 * @file                                                                   */
/* ======================================================================= */

// this will remove the warning 4786
#include <OpenTracker/tool/disable4786.h>

#include <OpenTracker/common/ElasticFilterNode.h>


#ifndef OT_NO_ELASTICFILTER_SUPPORT


namespace ot {

    // constructor method
    ElasticFilterNode::ElasticFilterNode( float force_, float damp_, int frequency_, int offset_ )
        : currentEvent(), vEvent()
    {
        frequency = frequency_;
        offset = offset_;

        force = force_;
        damp = damp_;

        init = false;
    } 

    // tests the confidence value and only forwards passing events
    void ElasticFilterNode::onEventGenerated( Event& event, Node & generator )
    {
        //printf("ElasticFilterNode::onEventGenerated() \n");
        if (!init){
            currentEvent = event;
            init = true;

        }
        targetEvent = event;
        // we have to pass on the other information as well
        currentEvent.getButton() = event.getButton();
        currentEvent.getConfidence() = event.getConfidence();
    }

    void ElasticFilterNode::pushEvent() {
        // maybe we should us absolute timing, but for now we try without
        // double curTime = OSUtils::currentTime();
        // double dt = lastTime - curTime;
        if (init) {
            //printf("ElasticFilterNode::push() \n");

            Event dEvent;

            // calculate current offset from target
            dEvent.getPosition()[0] = targetEvent.getPosition()[0] - currentEvent.getPosition()[0];
            dEvent.getPosition()[1] = targetEvent.getPosition()[1] - currentEvent.getPosition()[1];
            dEvent.getPosition()[2] = targetEvent.getPosition()[2] - currentEvent.getPosition()[2];

            // calculate velocity
            vEvent.getPosition()[0] += (float)(dEvent.getPosition()[0] * force * 0.1);
            vEvent.getPosition()[1] += (float)(dEvent.getPosition()[1] * force * 0.1);
            vEvent.getPosition()[2] += (float)(dEvent.getPosition()[2] * force * 0.1);
		
            // damp velocity
            vEvent.getPosition()[0] *= 1.0f - damp;
            vEvent.getPosition()[1] *= 1.0f - damp;
            vEvent.getPosition()[2] *= 1.0f - damp;
     
            // add velocity to current event
            currentEvent.getPosition()[0] += vEvent.getPosition()[0];
            currentEvent.getPosition()[1] += vEvent.getPosition()[1];
            currentEvent.getPosition()[2] += vEvent.getPosition()[2];
				
            MathUtils::slerp(currentEvent.getOrientation(), targetEvent.getOrientation(), force*(1-damp)*0.1f, dEvent.getOrientation());
		
            for (int i=0; i< 4; i++)
                currentEvent.getOrientation()[i] = dEvent.getOrientation()[i]; // copy ??

            currentEvent.timeStamp();
            updateObservers(currentEvent);
        }
    }

    void ElasticFilterNode::pullEvent()
    {
        // nothing to do
    }
} // namespace ot


#else
#pragma message(">>> OT_NO_BUTTON_SUPPORT")
#endif //OT_NO_ELASTICFILTER_SUPPORT


/* 
 * ------------------------------------------------------------
 *   End of ElasticFilterNode.cxx
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
