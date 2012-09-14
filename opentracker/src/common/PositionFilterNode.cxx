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
/** source file for PositionFilterNode.
 *
 * @author Thomas Psik tomp@ims.tuwien.ac.at
 *
 * $Id: PositionFilterNode.cxx 1807 2007-04-12 10:35:50Z bornik $
 * @file                                                                   */
/* ======================================================================= */


#include <OpenTracker/common/PositionFilterNode.h>


#ifndef OT_NO_POSITIONFILTER_SUPPORT


namespace ot {


    // constructor method.

    PositionFilterNode::PositionFilterNode( const float min_[3], const float max_[3] )
        : Node()
    {
        min[0] = min_[0];
        min[1] = min_[1];
        min[2] = min_[2];

        max[0] = max_[0];
        max[1] = max_[1];
        max[2] = max_[2];


    }

    int PositionFilterNode::isEventGenerator()
    {
        return 1;
    }

    // this method is called by the EventGenerator to update it's observers.

    void PositionFilterNode::onEventGenerated( Event& event, Node& generator)
    {
        if(
           min[0] < event.getPosition()[0] && event.getPosition()[0] < max[0]
           &&
           min[1] < event.getPosition()[1] && event.getPosition()[1] < max[1]
           &&
           min[2] < event.getPosition()[2] && event.getPosition()[2] < max[2]
           )
        {
            updateObservers( event );
        }
    }

    void PositionFilterNode::pushEvent() 
    {
        // nothing to do
    }
    
    void PositionFilterNode::pullEvent()
    {
        // nothing to do
    }

}  // namespace ot


#endif //OT_NO_POSITIONFILTER_SUPPORT


/* 
 * ------------------------------------------------------------
 *   End of PositionFilterNode.cxx
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
