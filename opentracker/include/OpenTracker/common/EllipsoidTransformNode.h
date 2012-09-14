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
/** header file for EllipsoidTransformNode Node.
 *
 * @author Gerhard Reitmayr
 *
 * $Id: EllipsoidTransformNode.h 1547 2006-10-25 10:33:10Z veas $
 * @file                                                                   */
/* ======================================================================= */

/**
 * @page transform_nodes Transform Node Reference
 * @section ellipsoidtransform EllipsoidTransform
 *
 * This transformation node transforms GPS position data between ellipsoid and
 * the corresponding cartesian coordinate system. The main parameters are the
 * two semi diameters a and b of the ellipsoid which are specified in meters.
 * For ellipsoid coordinate data it uses
 * the Latitue, Longitute and Height data in the components of
 * the position value, just as the @ref gpssource node produces them. 
 *
 * The configuration elements are obtained by using the base name @c EllipsoidTransform
 * and prepending it with one of the prefixes @c Event | @c Queue | @c Time 
 * as described in the @ref transform node.
 *
 * It has the following attributes:
 * @li @c a the first semi diameter given in meters. This is required
 * @li @c b the second semi diameter given in meters. 
 *           If not specified it is set to the value of @c a
 * @li @c @mode (toCartesian | toEllipsoid) specifies whether to transform to cartesian or
 *           ellipsoid coordinates. The default is @c toCartesian.
 *
 * Some handy values for a and b :
 * @li a = 6378137 , b = 6356752.31424523 - WGS84 used by GPS systems
 * @li a = 6377397.155 ,  b = 6356078.963 - Bessel used by Austrian map data
 *
 * An example element looks like this :
 * @verbatim
 <EventEllipsoidTransform a="6378137" b="6356752.31424523" mode="toCartesian">
 <Any EventGenerator element type>
 </EventEllipsoidTransform>@endverbatim
*/

#ifndef _ELLIPSOIDTRANSFORMNODE_H
#define _ELLIPSOIDTRANSFORMNODE_H

#include "../OpenTracker.h"

#ifndef OT_NO_ELLIPSOIDTRANSFORM_SUPPORT


#include "Transformation.h"

/** 
 * This class implements transformations between ellipsoid and cartesian
 * coordinate systems for GPS position data. It can be used as a
 * filter node for events, queues or time dependend nodes. 
 * @author Gerhard Reitmayr
 * @ingroup common
 * @todo fix principal value tests for trigonometric calculations
 */

namespace ot {

    class OPENTRACKER_API EllipsoidTransformNode : public Transformation  
    {
    public:
        /// the first semi diameter in meters
        double a;
        /// the second semi diameter in meters
        double b;
        /// mode flag to denote which direction the conversion will be
        enum Mode { toEllipsoid = 0, toCartesian } mode;

    protected:
        EllipsoidTransformNode( double a_ , double b_, Mode mode_ );

        /** transforms a event either to or from an ellipsoid 
         * coordinate system to a cartesian one.
         * @param event pointer to original event
         * @return pointer to localEvent member
         */
        virtual Event* transformEvent( Event* event);

        friend class CommonNodeFactory;
    };

} // namespace ot


#endif //OT_NO_ELLIPSOIDTRANSFORM_SUPPORT


#endif // !defined(_ELLIPSOIDTRANSFORMNODE_H)

/* 
 * ------------------------------------------------------------
 *   End of EllipsoidTransformNode.h
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
