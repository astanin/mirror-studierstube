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
/** header file for MatrixTransform Node.
 *
 * @author Gerhard Reitmayr
 *
 * $Id: MatrixTransformation.h 1547 2006-10-25 10:33:10Z veas $
 * @file                                                                   */
/* ======================================================================= */

/**
 * @page transform_nodes Transform Node Reference
 * @section matrixtransform MatrixTransformation
 *
 * This section describes the MatrixTransformation configuration element. It
 * transforms the data by applying an affine transformation to the
 * the child's position data as post transformations. The transformation itself is
 * fixed and described by a 3x4 matrix which is set with the elements attributes.
 * Note that this node only acts on position information !!
 *
 * The configuration elements are obtained by using the base name
 * @c MatrixTransformation and prepending it with one of the prefixes described before.
 * They have the following attribute:
 * @li @c matrix   a 3x4 matrix specifying an affine transformation
 *
 * An example element looks like this :
 * @verbatim
 <EventMatrixTransform matrix="1 0 1 1 0 0 1 1 1 1 1 2">
 <Any EventGenerator element type>
 </EventMatrixTransform>@endverbatim
*/

#ifndef _MATRIXTRANSFORMATION_H
#define _MATRIXTRANSFORMATION_H

#include "Transformation.h"

/**
 * The MatrixTransformation class implements a static matrix post transformation on
 * the event of its child node. It implements the @ref matrixtransform nodes.
 * @author Gerhard Reitmayr
 * @ingroup common
 */

namespace ot {

    class OPENTRACKER_API MatrixTransformation : public Transformation
    {
    protected:

        /// stores the affine part of the transformation
        float matrix[3][4];

        /**
         * transforms a event. Overrides the Transformation implementation
         * to implement a different one.
         */
        virtual Event* transformEvent( Event* event) ;

        /** default constructor method sets values to implement identity
         * transformation */
        MatrixTransformation();

        /** constructor method
         * @param matrix_ contains 3x4 matrix for affine transformation
         */
        MatrixTransformation( float * matrix_[4] );

        /** constructor method
         * @param matrix_ contains 3x4 matrix for affine transformation
         */
        MatrixTransformation( float * matrix_ );

    public:


        /** returns the affine part of the Transformation
         * @return float pointer to 3x4 floats
         */
        float ** getMatrix()
	{
            return (float**)matrix;
	}


        /** sets the scale of the transformation. It is only
         * executed, if the transformation is of type to
         * change the position of any events. It copies the
         * passed float array into an internal structure.
         * @param data a 3x4 float array containing the scale
         */
        void setMatrix(float* data[4])
	{
            memcpy( matrix, data, sizeof(float)*12 );
	}

        friend class CommonNodeFactory;
    };

} // namespace ot

#endif

/* 
 * ------------------------------------------------------------
 *   End of MatrixTransformation.h
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
