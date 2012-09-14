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
/** source file for DESPFilter node.
 *
 * @author Erick Mendez
 * @author Gerhard Schall
 *
 * $Id: DESPFilterNode.cxx 1168 2006-06-14 05:55:21Z bornik $
 * @file                                                                   */
/* ======================================================================= */

// this will remove the warning 4786
#include <OpenTracker/tool/disable4786.h>

#include <OpenTracker/common/DESPFilterNode.h>

#ifndef OT_NO_DESPFILTER_SUPPORT

namespace ot {

    // constructor method
    DESPFilterNode::DESPFilterNode( float filterPos_, float filterOri_ )
    {
        filterPos = filterPos_;
        filterOri = filterOri_;
        frameCtr=0;

        init = false;
    } 

    // Applies the filter to incoming events
    void DESPFilterNode::onEventGenerated( Event& event, Node & generator )
    {
        if (!init)
        {
            // Makes an initial copy
            lastEvent.getPosition()=event.getPosition();
            lastEvent.getOrientation()=event.getOrientation();
            lastEvent.getConfidence()=event.getConfidence();
            lastEvent.time=event.time;

            // Sets the given filtering val
            poseDesp.setAlpha(filterPos,filterOri);
            init = true;
        }

        // The current event we're working on
        currentEvent.getPosition()=event.getPosition();
        currentEvent.getOrientation()=event.getOrientation();
        currentEvent.getConfidence()=event.getConfidence();
        currentEvent.time=event.time;

        // The event we will be passing
        targetEvent.getPosition()=event.getPosition();
        targetEvent.getOrientation()=event.getOrientation();
        targetEvent.getConfidence()=event.getConfidence();
        targetEvent.time=event.time; 

        if (lastEvent.time!=currentEvent.time) // Check if a new event arrived
        {
            if (frameCtr>0) // Check if previously no events had arrived
            {
                poseDesp.reinit();
                frameCtr=0;
            }

            // Apply the DESP prediction
            poseDesp.observe(currentEvent.getPosition(), currentEvent.getOrientation());
            poseDesp.predict(0, targetEvent.getPosition(), targetEvent.getOrientation());
        }
        else // If no new event has arrived
        {
            // Keep track for how long no new event has arrived
            frameCtr++;

            // Apply the DESP prediction
            poseDesp.predict(frameCtr, targetEvent.getPosition(), targetEvent.getOrientation());
        }

        // Keep a copy of the last received event
        lastEvent.getPosition()=currentEvent.getPosition();
        lastEvent.getOrientation()=currentEvent.getOrientation();
        lastEvent.getConfidence()=currentEvent.getConfidence();
        lastEvent.time=currentEvent.time;

        // Update the observers
        updateObservers( targetEvent );
    }

    void DESPFilterNode::pushEvent()
    {
        // nothing to do
    }

    void DESPFilterNode::pullEvent()
    {
        // nothing to do
    }
} // namespace ot


#else
#pragma message(">>> No DESP filtering supported")
#endif //OT_NO_DESPFILTER_SUPPORT


/* 
 * ------------------------------------------------------------
 *   End of DESPFilterNode.cxx
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
