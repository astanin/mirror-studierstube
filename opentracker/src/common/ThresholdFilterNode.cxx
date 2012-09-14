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
/** source file for ThresholdFilter Node.
 *
 * @author Gerhard Reitmayr
 * 
 * $Id: ThresholdFilterNode.cxx 1807 2007-04-12 10:35:50Z bornik $
 * @file                                                                   */
/* ======================================================================= */

// this will remove the warning 4786
#include <OpenTracker/tool/disable4786.h>

#include <OpenTracker/common/ThresholdFilterNode.h>
#include <cmath>


#ifndef OT_NO_THRESHOLDFILTER_SUPPORT


namespace ot {

    // constructor method.

    ThresholdFilterNode::ThresholdFilterNode( const float posmin, const float posmax, 
                                              const float rotmin, const float rotmax )
        : Node(), positionMin( posmin ), positionMax( posmax ), 
          rotationMin( rotmin ), rotationMax( rotmax )
    {
    }

    int ThresholdFilterNode::isEventGenerator()
    {
        return 1;
    }

    // this method is called by the EventGenerator to update it's observers.

    void ThresholdFilterNode::onEventGenerated( Event& event, Node& generator)
    {
        float deltaPos = (float)sqrt(
                                     (event.getPosition()[0]-lastEvent.getPosition()[0])*(event.getPosition()[0]-lastEvent.getPosition()[0]) +
                                     (event.getPosition()[1]-lastEvent.getPosition()[1])*(event.getPosition()[1]-lastEvent.getPosition()[1]) +
                                     (event.getPosition()[2]-lastEvent.getPosition()[2])*(event.getPosition()[2]-lastEvent.getPosition()[2]));
        float deltaRot = (float)MathUtils::angle( event.getOrientation(), lastEvent.getOrientation(), 4);
        if((( positionMin <= deltaPos ) && ( deltaPos <= positionMax )) ||
           (( rotationMin <= deltaRot ) && ( deltaRot <= rotationMax )))
        {
            lastEvent = event;
            updateObservers( lastEvent );
        }
    }

    void ThresholdFilterNode::pushEvent()
    {
        // nothing to do
    }

    void ThresholdFilterNode::pullEvent()
    {
        // nothing to do
    }
} // namespace ot


#endif //OT_NO_THRESHOLDFILTER_SUPPORT


/* 
 * ------------------------------------------------------------
 *   End of ThresholdFilterNode.h
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
