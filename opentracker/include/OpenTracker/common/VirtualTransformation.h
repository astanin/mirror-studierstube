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
/** header file for VirtualTransformation Node.
 *
 * @author Gerhard Reitmayr
 *
 * $Id: VirtualTransformation.h 1547 2006-10-25 10:33:10Z veas $
 * @file                                                                   */
/* ======================================================================= */

/**
 * @page transform_nodes Transform Node Reference
 *
 * @section virtualtransform VirtualTransformation
 *
 * This section describes the VirtualTransformation configuration element. It 
 * implements an offset in the child's affine space. That is the configured
 * translation and rotation are post transformed with the child's position
 * and orientation values. This effectively offsets the tracked point with
 * respect to the tracked affine base. Note that there is no scale, as a 
 * scaled base only makes sense, if there is a translational offset and in
 * that case the scale could be precomputed on the translational offset anyway.
 *
 * There are three variants of this transformation. The @c VirtualPositionTransform
 * element configures only a translational offset and accordingly only the
 * translation attribute can be specified. 
 * @c VirtualOrientationTransform configures a rotational offset and again only
 * the rotation attributes can be specified. 
 * Finally the simple @c VirtualTransform acts on both and
 * again a full transformation can be specified.
 * 
 * The configuration elements are obtained by using one of the base names
 * @c VirtualPositionTransform|VirtualOrientationTransform|VirtualTransform 
 * and prepending it with one of the prefixes described before.
 * They have the following attributes:
 * @li @c translation   0 0 0    (T)
 * @li @c rotationtype (quaternion|matrix|euler) quaternion
 * @li @c rotation        0 0 0 1 (R)
 * 
 *
 * An example element looks like this :
 * @verbatim
 <EventVirtualTransform rotationtype="euler" rotation="1 2 3" translation="0 0 0">
 <Any EventGenerator element type>
 </EventVirtualTransform>@endverbatim
*/

#ifndef _VIRTUALTRANSFORMATION_H
#define _VIRTUALTRANSFORMATION_H

#include "StaticTransformation.h"

/**
 * A VirtualTransformation acts differently from a common Transformation.
 * It is a local transformation of the events coordinate system.
 * Mathematically it is a pre-transformation as opposed to a
 * post-transformation like the VirtualTransformation.
 * @author Gerhard Reitmayr
 * @ingroup common
 */

namespace ot {

    class OPENTRACKER_API VirtualTransformation : public StaticTransformation
    {
        // Methods
    protected:
        /**
         * transforms a event. Overrides the Transformation implementation
         * to implement a different one.
         */
        virtual Event* transformEvent( Event* event) ;

        /** default constructor method sets transformation to identity */
        VirtualTransformation() : StaticTransformation()
	{}

    public:
        /** constructor method. */
        VirtualTransformation(float translation_[3], float scale_[3],
                              float rotation_[4], bool usePos_, bool useOrient_ )
            : StaticTransformation(translation_, scale_, rotation_, usePos_, useOrient_ )
	{}

        friend class CommonNodeFactory;
    };

} // namespace ot

#endif



/* 
 * ------------------------------------------------------------
 *   End of VirtualTransformation.h
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
