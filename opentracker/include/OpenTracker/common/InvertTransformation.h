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
/** header file for InvertTransform Nodes.
 *
 * @author Gerhard Reitmayr
 *
 * $Id: InvertTransformation.h 1547 2006-10-25 10:33:10Z veas $
 * @file                                                                   */
/* ======================================================================= */

/**
 * @page transform_nodes Transform Node Reference
 * @section inverttransform InvertTransform
 *
 * An InvertTransform inverts the affine base described by the tracking data.
 * Put another way, if you use the output of an InvertTransform and combine
 * it with the input using a DynamicTransform, you will get the canonical
 * base, i.e. position in the origin and orientation equal to identity.
 *
 * There are again three flavours, one for each interface type.
 * The configuration elements are obtained by using the base name @c InvertTransform
 * and prepending it with one of the prefixes described before.
 *
 * An example element looks like this :
 * @verbatim
 <EventInvertTransform>
 <Any EventGenerator element type>
 </EventInvertTransform>@endverbatim
*/

#ifndef _INVERTTRANSFORMATION_H
#define _INVERTTRANSFORMATION_H

#include "Transformation.h"

/**
 * The InverTransform class implements a transformation on the child node, that
 * inverts the affine base described by the tracking data. It has no additional
 * fields.
 * @author Gerhard Reitmayr
 * @ingroup common
 */

namespace ot {

    class OPENTRACKER_API InvertTransformation : public Transformation
    {
    protected:

        /**
         * transforms a event. Overrides the Transformation implementation
         * to implement the inversion
         */
        virtual Event* transformEvent( Event* event) ;

        /** default constructor method sets values to implement identity 
         * transformation */
        InvertTransformation()
	{}

        friend class CommonNodeFactory;
    };

} // namespace ot

#endif


/* 
 * ------------------------------------------------------------
 *   End of InvertTransformation.h
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
