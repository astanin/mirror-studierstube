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
/** Source file for DESPFilter Node.
 *
 * @author Daniel Wagner
 *
 * $Id: DESP.cpp 25 2006-08-05 21:54:47Z daniel $
 * ======================================================================== */


#include <OpenTracker/common/DESP.h>

namespace ot {


    DESP::DESP(float nAlpha) : spt(1e10), spt2(1e10), alpha(nAlpha)
    {
    }


    void DESP::reinit()
    {
	spt = spt2 = 1e10;
    }


    void DESP::observe(float nValue)
    {
	if(spt==1e10 && spt2==1e10)
            spt = spt2 = nValue;

	float spt_new = alpha*nValue + (1-alpha)*spt;
	float spt2_new = alpha*spt_new + (1-alpha)*spt2;

	spt = spt_new;
	spt2 = spt2_new;
    }


    float DESP::predict(int delta_t)
    {
	if(delta_t==0)
            return 2*spt - spt2;
	else
            return (2+(alpha*delta_t)/(1-alpha))*spt - (1+(alpha*delta_t)/(1-alpha))*spt2 ;
    }



    PoseDESP::PoseDESP()
    {
    }

    void PoseDESP::setAlpha(float nAlphaPos, float nAlphaRot)
    {
	for(int i=0; i<3; i++)
            pos[i].setAlpha(nAlphaPos);
	for(int i=0; i<4; i++)
            rot[i].setAlpha(nAlphaRot);
    }


    void PoseDESP::reinit()
    {
	for(int i=0; i<3; i++)
            pos[i].reinit();
	for(int i=0; i<4; i++)
            rot[i].reinit();
    }


    void PoseDESP::observe(std::vector<float> position_, std::vector<float> quaternion_)
    {
	if(pos[0].getAlpha()>0.0f)
            for(int i=0; i<3; i++)
                pos[i].observe(position_[i]);

	if(rot[0].getAlpha()>0.0f)
	{
            for(int i=0; i<4; i++)
                rot[i].observe(quaternion_[i]);
	}
    }


    void PoseDESP::predict(int delta_t, std::vector<float> &position_, std::vector<float> &quaternion_)
    {
	if(pos[0].getAlpha()>0.0f)
            for(int i=0; i<3; i++)
                position_[i] = pos[i].predict(delta_t);

	if(rot[0].getAlpha()>0.0f)
	{
            for(int i=0; i<4; i++)
                quaternion_[i]= rot[i].predict(delta_t);
	}
    }


}  // namespace ot

/* 
 * ------------------------------------------------------------
 *   End of DESP.cxx
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
