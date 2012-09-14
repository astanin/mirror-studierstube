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
/** header file for GKTransformNode Node.
 *
 * @author Gerhard Reitmayr
 *
 * $Id: GKTransformNode.h 1547 2006-10-25 10:33:10Z veas $
 * @file                                                                   */
/* ======================================================================= */

/**
 * @page transform_nodes Transform Node Reference
 * @section gktransform GKTransform
 *
 * This transformation node transforms GPS position data into a local
 * Gausz - Krueger map projection. The main parameters are the
 * two semi diameters a and b of the ellipsoid which are specified in meters, 
 * the meridian around which the grid coordinates should be computed, and for
 * parameter values alpha, beta, gamma, delta. 
 * It expects the data in ellipsoid coordinate format as the Latitue, 
 * Longitute and Height data in the components of the position value, just as 
 * the @ref gpssource node produces them. It can be used to transform data 
 * to a GK map or from a GK map into ellipsoid coordinates relative to the
 * given ellipsoid.
 *
 * The configuration elements are obtained by using the base name @c GKTransform
 * and prepending it with one of the prefixes @c Event | @c Queue | @c Time 
 * as described in the @ref transform node.
 *
 * It has the following attributes:
 * @li @c a the first semi diameter given in meters. This is required
 * @li @c b the second semi diameter given in meters. 
 *        If not specified it is set to the value of @c a
 * @li @c meridian the meridian of the GK projection to use. This is given in 
 *        degree rather than radiants ! the default value is 34 which is 
 used around Vienna, Austria
 * @li @c alpha a required transformation parameter
 * @li @c beta a required transformation parameter
 * @li @c gamma a required transformation parameter
 * @li @c delta a required transformation parameter
 * @li @c mode (to | from) specifies whether to transform to or from GK
 *        coordinates. The default is @c to.
 *
 * See @ref ellipsoidtransform for some good values for a, b
 * For a transformation to the Austrian map datum, use the following values :
 * @li @c meridian = 28 | 31 | 34, 34 is best for Vienna and east Austria.
 * @li @c alpha = 111120.6196
 * @li @c beta = 15988.6385
 * @li @c gamma = 16.73
 * @li @c delta = 0.0218
 *
 * An example element looks like this :
 * @verbatim
 <EventGKTransform a="6377397.155" b="6356078.963" mode="to" meridian="34" alpha="111120.6196" beta="15988.6385" gamma="16.73" delta="0.0218">
 <Any EventGenerator element type>
 </EventGKTransform>@endverbatim
*/

#ifndef _GKTRANSFORMNODE_H
#define _GKTRANSFORMNODE_H

#include "Transformation.h"

/**
 * This class implements a transformation between an ellipsoid coordinate system
 * and the Gausz - Krueger (GK) coordinates. The corresponding element is 
 * @ref gktransform .
 * @author Gerhard Reitmayr
 * @ingroup common
 */


#ifndef OT_NO_GK_SUPPORT


namespace ot {

    class OPENTRACKER_API GKTransformNode : public Transformation 
    {
    public:
        double a;
        double b;
        double meridian;
        double alpha, beta, gamma, delta;
        enum Mode { to = 0, from } mode;

    protected:
        GKTransformNode( double a_ , double b_, double m_, 
                         double alpha_, double beta_, double gamma_, double delta_, 
                         Mode mode_ );

        /** transforms a event either to or from an ellipsoid 
         * coordinate system to a cartesian one.
         * @param event pointer to original event
         * @return pointer to localEvent member
         */
        virtual Event* transformEvent( Event* event);

        friend class CommonNodeFactory;
    };

} // namespace ot {


#endif //OT_NO_GK_SUPPORT


#endif // !defined(_GKTRANSFORMNODE_H)


/* 
 * ------------------------------------------------------------
 *   End of GKTransformNode.h
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
