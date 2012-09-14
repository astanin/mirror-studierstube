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
/** source file for Filter Node.
 *
 * @author Gerhard Reitmayr
 * @todo check implementation of quaternion interpolation and document
 * $Id: FilterNode.cxx 1807 2007-04-12 10:35:50Z bornik $
 * @file                                                                   */
/* ======================================================================= */

// this will remove the warning 4786
#include <OpenTracker/tool/disable4786.h>

#include <OpenTracker/common/FilterNode.h>
#include <cmath>

// constructor method.

namespace ot {

    FilterNode::FilterNode( const std::vector<float> & weights_, const Type & type_ )
        : Node(), weights( weights_ ), type( type_ )
    {
    }

    int FilterNode::isEventGenerator()
    {
        return 1;
    }

    // this method is called by the EventGenerator to update it's observers.

    void FilterNode::onEventGenerated( Event& event, Node& generator)
    {
        Node * queue = getChild( 0 );
        if( queue != NULL && queue->getSize() == weights.size() )
        {
            double w,sum = 0;
            double pos[3] = {0, 0, 0 }, orient[3] = { 0, 0, 0 };
            double conf = 0;

            // referencerot for quaternion interpolation
            //float * referencerot= queue->getEvent( 0 ).orientation;
            std::vector<float> &referencerot = queue->getEvent(0).getOrientation();

            std::vector<float>::iterator it;
            for( it = weights.begin(); it != weights.end(); it++ )
            {
                Event & event = queue->getEvent( it - weights.begin());
                w = (*it);

                if( type != ORIENTATION )
                {
                    /*  The position is computed as the weighted average of the
                        positions in the queue. The average is not normalized, to
                        yield a more general filter.*/
                    pos[0] += event.getPosition()[0] * w;
                    pos[1] += event.getPosition()[1] * w;
                    pos[2] += event.getPosition()[2] * w;
                }

                if( type != POSITION )
                {
                    /* The orientation is computed as the normalized weighted average of the
                       orientations in the queue in log space. That is, they are transformed
                       to 3D vectors inside the unit hemisphere and averaged in this linear space.
                       The resulting vector is transformed back to a unit quaternion. */

                    if( MathUtils::dot(referencerot, event.getOrientation(), 4) < 0 )
                    {
                        event.getOrientation()[0] = -event.getOrientation()[0];
                        event.getOrientation()[1] = -event.getOrientation()[1];
                        event.getOrientation()[2] = -event.getOrientation()[2];
                        event.getOrientation()[3] = -event.getOrientation()[3];
                    }

                    double angle = acos( event.getOrientation()[3] );
                    double as = sin( angle );
                    if( as != 0 )
                        as = angle * w / as;
                    else
                        as = 0;					// lim x/(sin(x/2)) = 2 for x -> 0 ???
                    orient[0] += event.getOrientation()[0] * as;
                    orient[1] += event.getOrientation()[1] * as;
                    orient[2] += event.getOrientation()[2] * as;
                }

                /* confidence is also averaged */
                conf += event.getConfidence() * w;

                sum += w;

            }
            if( type != POSITION )
            {
                // calculate the length of the summed vector in log space
                // this is the angle of the result quaternion
                w = sqrt((orient[0]*orient[0] + orient[1]*orient[1] + orient[2]*orient[2])/(sum*sum));
                double as = 0;
                if( w != 0)
                    as = sin( w ) / w;
                localEvent.getOrientation()[0] = (float)(orient[0] * as);
                localEvent.getOrientation()[1] = (float)(orient[1] * as);
                localEvent.getOrientation()[2] = (float)(orient[2] * as);
                localEvent.getOrientation()[3] = (float)cos( w );
                MathUtils::normalizeQuaternion( localEvent.getOrientation() );
            }
            else 
            {
                localEvent.getOrientation()[0] = event.getOrientation()[0];
                localEvent.getOrientation()[1] = event.getOrientation()[1];
                localEvent.getOrientation()[2] = event.getOrientation()[2];
                localEvent.getOrientation()[3] = event.getOrientation()[3];
            }
        
            if( type != ORIENTATION )
            {
                // copy pos to event.position
                localEvent.getPosition()[0] = (float)pos[0];
                localEvent.getPosition()[1] = (float)pos[1];
                localEvent.getPosition()[2] = (float)pos[2];
            }
            else
            {
                localEvent.getPosition()[0] = event.getPosition()[0];
                localEvent.getPosition()[1] = event.getPosition()[1];
                localEvent.getPosition()[2] = event.getPosition()[2];			
            }
        
            localEvent.getConfidence() = (float)conf;

            localEvent.time = event.time;
            localEvent.getButton() = event.getButton();
            updateObservers( localEvent );
        }
        else
        {
            updateObservers( event );
        }
    }

    void FilterNode::pushEvent()
    {
        // nothing to do
    }
    
    void FilterNode::pullEvent()
    {
        // nothing to do
    }
} // namespace ot


/* 
 * ------------------------------------------------------------
 *   End of FilterNode.h
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
