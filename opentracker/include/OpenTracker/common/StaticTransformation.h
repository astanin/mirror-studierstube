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
/** header file for StaticTransform Node.
 *
 * @author Gerhard Reitmayr
 *
 * $Id: StaticTransformation.h 2063 2007-10-16 10:54:39Z bornik $
 * @file                                                                   */
/* ======================================================================= */

/**
 * @page transform_nodes Transform Node Reference
 * @section transform Transformation
 *
 * This section describes the Transformation configuration element. It 
 * transforms the data by applying a rotation, scale and translation to
 * the child's data as post transformations. The transformation itself is
 * fixed and set with the elements attributes. Note that only the rotational
 * part acts on the child's orientation data.
 *
 * There are three variants of this transformation. The @c PositionTransform
 * element acts only on the position data and all attributes can be specified. 
 * @c OrientationTransform acts only on the orientation data and therefore only
 * a rotation can be specified. Finally the simple @c Transform acts on both and
 * again a full transformation can be specified.
 *
 * The configuration elements are obtained by using one of the base names
 * @c PositionTransform|OrientationTransform|Transform 
 * and prepending it with one of the prefixes described before.
 * They have the following attributes:
 * @li @c translation   0 0 0    
 * @li @c scale         1 1 1    
 * @li @c rotationtype (quaternion|matrix|euler) quaternion
 * @li @c rotation        0 0 0 1 
 *
 * An example element looks like this :
 * @verbatim
 <EventTransform rotationtype="euler" rotation="1 2 3" translation="0 0 0">
 <Any EventGenerator element type>
 </EventTransform>@endverbatim
*/

#ifndef _STATICTRANSFORMATION_H
#define _STATICTRANSFORMATION_H

#include "Transformation.h"

/**
 * The StaticTransformation class implements a static post transformation on
 * the event of its child node. It also is the base for other static implementations
 * such as the VirtualTransformation as it implements the same fields.
 * @author Gerhard Reitmayr
 * @ingroup common
 */

namespace ot {

    class OPENTRACKER_API StaticTransformation : public Transformation
    {
    protected:

        /// stores the translation of the transformation
        float translation[3];
        /// stores scale part of the transformation
        float scale[3];
        /// stores rotational part of the transformation
        float rotation[4];
        /// stores additional confidence value that is multiplied (used for DynamicTransforms)
        float confidence;
        /// flag whether to compute position updates
        bool usePos;
        /// flag whether to compute orientation updates
        bool useOrient;
    
        /**
         * transforms a event. Overrides the Transformation implementation
         * to implement a different one.
         */
        virtual Event* transformEvent( Event* event) ;

        /** default constructor method sets values to implement identity 
         * transformation */
        StaticTransformation();

    public:

        /** constructor method
         * @param translation_ sets translation
         * @param scale_ sets scale
         * @param rotation_ sets rotation
         */
        StaticTransformation(float translation_[3], float scale_[3], float rotation_[4], bool usePos_, bool useOrient_);

        /** returns the rotational part of the Transformation as a Quaternion.
         * @return float pointer to 4 floats containing quaternion
         */
        float* getRotation()
	{
            return (float*)rotation;
	}

        /** returns the scaling part of the Transformation 
         * @return float pointer to 3 floats 
         */
        float* getScale()
	{
            return (float *)scale;
	}

        /** returns the translational part of this Transformation 
         * @return float pointer to 3 float 
         */
        float* getTranslation()
	{
            return (float *)translation;
	}

        /** sets the rotation of the transformation. It copies the
         * passed float array into an internal structure.
         * @param data a 4 float array containing the rotation in
         *             quaternion format.
         */
        void setRotation(float * data)
	{
            memcpy( rotation, data, sizeof(float)*4 );
            put( "rotation", data, 4 );
            put( "rotationtype", "quaternion" );
	}

        /** sets the scale of the transformation. It is only
         * executed, if the transformation is of type to 
         * change the position of any events. It copies the
         * passed float array into an internal structure.
         * @param data a 3 float array containing the scale
         */
        void setScale(float * data)
	{
            if( usePos ) 
	    {
                memcpy( scale, data, sizeof(float)*3 );
                put( "scale", data, 3 );
	    }
	}

        /** sets the translation of the transformation. It is only
         * executed, if the transformation is of type to 
         * change the position of any events. It copies the
         * passed float array into an internal structure.
         * @param data a 3 float array containing the translation
         */
        void setTranslation(float * data)
	{
            if( usePos ) {
                memcpy( translation, data, sizeof(float)*3 );
                put( "translation", data, 3 );
            }
	}

#ifdef USE_LIVE
        virtual void set_attribute(const char* _key, const char* _value);

        virtual void set_translationXYZ(CORBA::Float x, CORBA::Float y, CORBA::Float z);
#ifndef SWIG
        virtual void set_translation(const OTGraph::Position3f pos);        
#endif
        virtual void get_translationXYZ(CORBA::Float& x, CORBA::Float& y, CORBA::Float& z);
        virtual OTGraph::Position3f_slice* get_translation();
#ifndef SWIG
        virtual void set_quaternion(const OTGraph::Quaternion4f ori);
#endif
        virtual void set_quaternionXYZW(CORBA::Float qx, CORBA::Float qy, CORBA::Float qz, CORBA::Float qw);
        virtual OTGraph::Quaternion4f_slice* get_quaternion();
        virtual void get_quaternionXYZW(CORBA::Float& qx, CORBA::Float& qy, CORBA::Float& qz, CORBA::Float& qw);
        virtual OTGraph::Position3f_slice* get_scale();
        virtual void get_scaleXYZ(CORBA::Float& sx, CORBA::Float& sy, CORBA::Float& sz);
        virtual void set_scaleXYZ(CORBA::Float sx, CORBA::Float sy, CORBA::Float sz);
#ifndef SWIG
        virtual void set_scale(const OTGraph::Position3f s);
#endif // SWIG
#endif
        friend class CommonNodeFactory;
    };

} // namespace ot

#endif

/* 
 * ------------------------------------------------------------
 *   End of StaticTransformation.h
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
