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
/** source file for StaticTransformation Node.
 *
 * @author Gerhard Reitmayr
 *
 * $Id: StaticTransformation.cxx 2089 2007-11-12 17:53:33Z samset $
 * @file                                                                   */
/* ======================================================================= */

// this will remove the warning 4786
#include <OpenTracker/tool/disable4786.h>

#include <OpenTracker/common/StaticTransformation.h>
#include <OpenTracker/common/CommonNodeFactory.h>

namespace ot {

    // default constructor method.

    StaticTransformation::StaticTransformation()
    {
        translation[0] = 0;
        translation[1] = 0;
        translation[2] = 0;
        rotation[0] = 0;
        rotation[1] = 0;
        rotation[2] = 0;
        rotation[3] = 1;
        scale[0] = 1;
        scale[1] = 1;
        scale[2] = 1;
        confidence = 1;
        usePos = false;
        useOrient = false;
    }

    // constructor method.

    StaticTransformation::StaticTransformation(float translation_[3], float scale_[3],
                                               float rotation_[4], bool usePos_, bool useOrient_ )
        : Transformation(), usePos( usePos_ ), useOrient( useOrient_ )
    {
        for( int i = 0; i < 3; i ++ )
        {
            this->translation[i] = translation_[i];
            this->scale[i] = scale_[i];
            this->rotation[i] = rotation_[i];
        }
        this->rotation[3] = rotation_[3];

        confidence = 1;
    }

    // transforms a event.

    Event* StaticTransformation::transformEvent( Event* event )
    {
        //logPrintI("StaticTransformation::transformEvent\n");
#ifdef USE_LIVE
        lock();
#endif
        //logPrintI("transformEvent locked\n");
        // transform the position of the event
        if( usePos )
        {
            MathUtils::rotateVector( copyA2V(rotation, 4),  event->getPosition(), localEvent.getPosition() );
            localEvent.getPosition()[0] = localEvent.getPosition()[0]*scale[0] + translation[0];
            localEvent.getPosition()[1] = localEvent.getPosition()[1]*scale[1] + translation[1];
            localEvent.getPosition()[2] = localEvent.getPosition()[2]*scale[2] + translation[2];
            //logPrintI("position transformed\n");            
        }
		        else {
            localEvent.getPosition()[0] = event->getPosition()[0]*scale[0];
            localEvent.getPosition()[1] = event->getPosition()[1]*scale[1];
            localEvent.getPosition()[2] = event->getPosition()[2]*scale[2];
        }
        // transform the orientation of the event
        if( useOrient )
        {
            MathUtils::multiplyQuaternion( copyA2V(rotation, 4), event->getOrientation(), localEvent.getOrientation() );
            //logPrintI("orientation transformed\n");            
        }
		        else {
            localEvent.getOrientation() = event->getOrientation();
        }
        try {
            localEvent.getConfidence() = event->getConfidence() * confidence;
        } catch (std::invalid_argument) {
            logPrintE("no confidence value");
        }
        localEvent.getButton() = event->getButton();
        localEvent.copyAllButStdAttr(*event);
        localEvent.timeStamp();
#ifdef USE_LIVE
        unlock();
#endif
        //logPrintI("StaticTransformation::transformEvent about to return\n");
        return &localEvent;
    }

#ifdef USE_LIVE
    void StaticTransformation::set_attribute(const char* _key, const char* _value) {
        lock();
        if (strcmp(_key, "rotation") == 0) {
            std::string val(_value);
            int ret = CommonNodeFactory::parseRotation(val, "quaternion", rotation);
        } else if (strcmp(_key, "translation") == 0) {
            std::string val(_value);
            int ret = CommonNodeFactory::parseVector(val, translation);
        } 
        unlock();
    }

    void StaticTransformation::set_translationXYZ(CORBA::Float x, CORBA::Float y, CORBA::Float z) {
        lock();
        translation[0] = x;
        translation[1] = y;
        translation[2] = z;
        unlock();
    }

    void StaticTransformation::set_translation(const OTGraph::Position3f pos) {
        lock();
        translation[0] = pos[0];
        translation[1] = pos[1];
        translation[2] = pos[2];
        unlock();
    }

    OTGraph::Position3f_slice* StaticTransformation::get_translation() {
        OTGraph::Position3f_slice* pos = OTGraph::Position3f_alloc();
        lock();
        pos[0] = translation[0];
        pos[1] = translation[1];
        pos[2] = translation[2];
        unlock();
        return pos;
    }

    void StaticTransformation::get_translationXYZ(CORBA::Float&x, CORBA::Float& y, CORBA::Float& z) {
        lock();
        x = translation[0];
        y = translation[1];
        z = translation[2];
        unlock();
    }

    void StaticTransformation::set_quaternion(const OTGraph::Quaternion4f ori) {
        lock();
        rotation[0] = ori[0];
        rotation[1] = ori[1];
        rotation[2] = ori[2];
        rotation[3] = ori[3];
        unlock();
    }
    
    void StaticTransformation::set_quaternionXYZW(CORBA::Float qx, CORBA::Float qy, CORBA::Float qz, CORBA::Float qw) {
        lock();
        rotation[0] = qx;
        rotation[1] = qy;
        rotation[2] = qz;
        rotation[3] = qw;
        unlock();
    }
    
    OTGraph::Quaternion4f_slice* StaticTransformation::get_quaternion() {
        OTGraph::Quaternion4f_slice* ori = OTGraph::Quaternion4f_alloc();
        lock();
        ori[0] = rotation[0];
        ori[1] = rotation[1];
        ori[2] = rotation[2];
        ori[3] = rotation[3];
        unlock();
        return ori;
    }
    
    void StaticTransformation::get_quaternionXYZW(CORBA::Float& qx, CORBA::Float& qy, CORBA::Float& qz, CORBA::Float& qw) {
        lock();
        qx = rotation[0];
        qy = rotation[1];
        qz = rotation[2];
        qw = rotation[3];
        unlock();        
    }

    OTGraph::Position3f_slice* StaticTransformation::get_scale() {
        OTGraph::Position3f_slice* s = OTGraph::Position3f_alloc();
        lock();
        s[0] = scale[0];
        s[1] = scale[1];
        s[2] = scale[2];
        unlock();
        return s;
    }
    
    void StaticTransformation::get_scaleXYZ(CORBA::Float& sx, CORBA::Float& sy, CORBA::Float& sz) {
        lock();
        sx = scale[0];
        sy = scale[1];
        sz = scale[2];
        unlock();
    }

    void StaticTransformation::set_scaleXYZ(CORBA::Float sx, CORBA::Float sy, CORBA::Float sz) {
        lock();
        scale[0] = sx;
        scale[1] = sy;
        scale[2] = sz;
        unlock();
    }
    
    void StaticTransformation::set_scale(const OTGraph::Position3f s) {
        lock();
        scale[0] = s[0];
        scale[1] = s[1];
        scale[2] = s[2];
        unlock();
    }
#endif //USE_LIVE
} //namespace ot


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
