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
/** source file for EllipsoidTransformNode Node.
 *
 * @author Gerhard Reitmayr
 *
 * $Id: EllipsoidTransformNode.cxx 1547 2006-10-25 10:33:10Z veas $
 * @file                                                                   */
/* ======================================================================= */

// this will remove the warning 4786
#include <OpenTracker/tool/disable4786.h>

#include <OpenTracker/common/EllipsoidTransformNode.h>


#ifndef OT_NO_ELLIPSOIDTRANSFORM_SUPPORT


#include <cmath>

//using namespace std;

namespace ot {

    EllipsoidTransformNode::EllipsoidTransformNode( double a_ , double b_, Mode mode_ ) :
        a( a_ ),
        b( b_ ),
        mode( mode_ )
    {}

    Event* EllipsoidTransformNode::transformEvent( Event* event)
    {
        if( toCartesian == mode )
        {
            double B = event->getPosition()[0];
            double L = event->getPosition()[1];
            double H = event->getPosition()[2];
            double e2 = 1 - (b*b) / (a*a);
            double N = a / (sqrt( 1 - e2 * sin( B )*sin( B )));
            localEvent.getPosition()[0] = (float)((N + H)*cos(B)*cos(L));
            localEvent.getPosition()[1] = (float)((N + H)*cos(B)*sin(L));
            localEvent.getPosition()[2] = (float)(((1 - e2)*N + H)*sin(B));				
        }
        else
        {
            double x = event->getPosition()[0];
            double y = event->getPosition()[1];
            double z = event->getPosition()[2];
            double e2 = 1 - (b*b) / (a*a);
            double L = 0;
            double B = 0;
            double H = 0;
            if( x != 0 ) 
            {
                L = atan( y / x );
                B = atan( z * cos( L ) / (x * (1 - e2)));  // interestingly, this is only an approximation, 
                // but it seems to work ?!
                double N = a / (sqrt( 1 - e2 * sin( B )*sin( B )));
                H = sqrt( x*x + y*y ) / cos(B) - N; // this avoids problems with L
            } 
            else if( y != 0 )
            {
                L = 1 / atan ( x / y );
                B = atan( z * sin( L ) / (y * (1 - e2)));  // interestingly, this is only an approximation,
                // but it seems to work ?!poles 
                double N = a / (sqrt( 1 - e2 * sin( B )*sin( B )));
                H = sqrt( x*x + y*y ) / cos(B) - N; // this avoids problems with L
            }
            else  // otherwise we are somewhere at the poles
            {
                if( z > 0 )
                {
                    B = MathUtils::Pi / 2;
                    H = z - b;
                }
                else if( z < 0 )
                {
                    B = - MathUtils::Pi / 2;
                    H = - b -z;
                }
            }
            localEvent.getPosition()[0] = (float)B;
            localEvent.getPosition()[1] = (float)L;
            localEvent.getPosition()[2] = (float)H;				
        }
        // copy the rest over
        // we don't deal with orientation so far...
        localEvent.copyAllButStdAttr(*event);

        localEvent.getOrientation() = event->getOrientation();
        localEvent.getConfidence() = event->getConfidence();
        localEvent.getButton() = event->getButton();
        localEvent.time = event->time;
        return & localEvent;
    }

} // namespace ot


#else
#pragma message(">>> OT_NO_ELLIPSOIDTRANSFORM_SUPPORT")
#endif 

/* 
 * ------------------------------------------------------------
 *   End of EllipsoidTransformNode.cxx
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
