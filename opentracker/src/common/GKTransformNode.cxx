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
/** source file for GKTransformNode Node.
 *
 * @author Gerhard Reitmayr
 *
 * $Id: GKTransformNode.cxx 1547 2006-10-25 10:33:10Z veas $
 * @file                                                                   */
/* ======================================================================= */

// this will remove the warning 4786
#include <OpenTracker/tool/disable4786.h>

#include <OpenTracker/common/GKTransformNode.h>
#include <cmath>


#ifndef OT_NO_GK_SUPPORT


namespace ot {

    GKTransformNode::GKTransformNode( double a_ , double b_, double m_,
                                      double alpha_, double beta_, double gamma_, double delta_,
                                      Mode mode_ ) :
        a( a_ ),
        b( b_ ),
        meridian( m_ * MathUtils::GradToRad ),  // we get the meridian in degrees
        alpha( alpha_ ),
        beta( beta_ ),
        gamma( gamma_ ),
        delta( delta_ ),
        mode( mode_ )
    {}

    Event* GKTransformNode::transformEvent( Event* event)
    {
        // the zero meridian of the GK map projection goes through Ferro,
        // a Canarian island 17° 40' left of Greenwhich zero meridian.
        const double ferro = (17.0 + 40.0 / 60.0) * MathUtils::GradToRad;
        // there are corrections applied to the resulting x (north) value
        // 5000 km ( 5000000 m ) are subtracted
        const double falseNorthing = 5000000.0;

        if( to == mode )
        {
            double B = event->getPosition()[0];
            double sinB = sin(B);
            double cosB = cos(B);
            double L = event->getPosition()[1];
            double dL = ( L + ferro - meridian );

            double Sm = alpha * B / MathUtils::GradToRad - beta * sin(2.0 * B) + gamma * sin(4.0 * B) - delta * sin(6.0 * B);

            double e2 = 1.0 - (b*b) / (a*a);
            double N = a / (sqrt( 1.0 - e2 * sinB * sinB ));
            double eta2 = ((a*a) / (b*b) - 1)*cosB*cosB;
            double t = tan( B );
            double corr_x = 1.0 + (dL*dL)*(cosB*cosB)*(5.0 - t*t + 9.0*eta2 + 4.0*eta2*eta2) / 12.0 +
                pow(dL,4.0)*pow(cosB,4.0)*(61.0-58.0*t*t+pow(t,4.0)+270.0*eta2 - 330.0*t*t*eta2) / 360.0;
            double corr_y = 1.0 + (dL*dL)*(cosB*cosB)*(1.0 - t*t + eta2) / 6.0 +
                pow(dL,4.0)*pow(cosB,4.0)*(5.0 - 18.0*t*t + pow(t,4.0) + 14.0*eta2 - 58.0*t*t*eta2) / 120.0;
            double x = Sm + N * dL * dL * sinB * cosB * corr_x / 2.0 - falseNorthing;
            double y = N * dL * cosB * corr_y;


            localEvent.getPosition()[0] = (float)x;
            localEvent.getPosition()[1] = (float)y;
        }
        else
        {
            double x = event->getPosition()[0];
            double y = event->getPosition()[1];
            double xa = x * MathUtils::GradToRad / alpha;
            double Bf = (xa + beta*sin(2*xa) + gamma*sin(4*xa) + delta*sin(6*xa)) * MathUtils::GradToRad;
            double t = tan(Bf);
            double t2 = t*t;
            double e2 = 1 - (b*b) / (a*a);
            double eta2 = ((a*a) / (b*b) - 1)*cos(Bf)*cos(Bf);
            double W = sqrt(1 - e2 *sin(Bf)*sin(Bf));
            double N = a / W;
            double M = b*b / (a*W*W*W);
            double corr_b = 1 - (y*y)*(5 + 3*t2 + eta2 - 9*t2*eta2)/(12*N*N) +
                (y*y*y*y)*(61+90*t2+45*t2*t2)/(360*N*N*N*N);
            double corr_l = 1 - (y*y)*(1 + 2*t2 + eta2)/(6*N*N) +
                (y*y*y*y)*(5 + 28*t2 + 24*t2*t2)/(120*N*N*N*N);
            double B = Bf - (y*y*t)*corr_b/(2*M*N);
            double L = y*corr_l/(N*cos(Bf)) + meridian - ferro;

            localEvent.getPosition()[0] = (float)B;
            localEvent.getPosition()[1] = (float)L;
        }

        // height over the ellipsoid surface is identical to the height over the map
        localEvent.getPosition()[2] = event->getPosition()[2];

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
#pragma message(">>> OT_NO_GK_SUPPORT")
#endif //OT_NO_GK_SUPPORT

/* 
 * ------------------------------------------------------------
 *   End of GKTransformNode.cxx
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
