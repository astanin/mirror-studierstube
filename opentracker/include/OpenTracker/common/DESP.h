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
/** header file for DESPFilter Node.
 *
 * @author Daniel Wagner
 *
 * $Id: DESP.h 25 2006-08-05 21:54:47Z daniel $
 * ======================================================================== */


#ifndef __DESP_H__
#define __DESP_H__

#include "../dllinclude.h"

#ifndef SWIG
#include <vector>
#endif

namespace ot {


    /// Double Exponential Smoothing Prediction for a scalar value
    /**
     *  Filtering similar to Kalman but a lot simpler and faster.
     */
    class OPENTRACKER_API DESP
    {
    public:
        DESP(float nAlpha=0.5f);

        void setAlpha(float nAlpha)  {  alpha = nAlpha;  }

        float getAlpha() const  {  return alpha;  }

        void observe(float nValue);

        float predict(int delta_t);

        void reinit();

    protected:
        float spt, spt2;
        float alpha;
    };


    /// Double Exponential Smoothing Prediction for a pose matrix
    /**
     *  Internally converts the pose matrix to position and orientation,
     *  which are each filtered using scalar DESP (component-wise).
     *  Then rebuilds the matrix.
     */
    class OPENTRACKER_API PoseDESP
    {
    public:
        PoseDESP();

        void setAlpha(float nAlphaPos, float nAlphaRot);

        void observe(std::vector<float> position_, std::vector<float> quaternion_);

        void predict(int delta_t, std::vector<float> &position_, std::vector<float> &quaternion_);

        void reinit();

    protected:
        DESP pos[3];
        DESP rot[4];
    };


}  // namespace ot


#endif //__DESP_H__

/* 
 * ------------------------------------------------------------
 *   End of DESP.h
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
