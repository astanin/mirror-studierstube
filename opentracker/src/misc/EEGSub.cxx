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
/** source file for EEGSib filter class.
 *
 * @author Alexander Bornik bornik@icg.tugraz.at
 *
 * $Id $
 * @file                                                                   */
/* ======================================================================= */

// this will remove the warning 4786
#include <OpenTracker/tool/disable4786.h>

#include <cstdlib>
#include <iostream>

#include <ace/Log_Msg.h>


#include <cmath>
#include <OpenTracker/misc/EEGSub.h>

namespace ot {

    EEGSub::EEGSub(const double &ifrequency, const unsigned int iharmonics, 
                   const double &isamplerate)
        : frequency(ifrequency),
          samplerate(isamplerate),
          harmonics(iharmonics)
    {
        logPrintI("EEGSub::EEGSub frequency: %f harmonics: %u smplrate: %f\n",ifrequency, iharmonics, isamplerate);

        las.clear();
        unsigned int i;

        // create all harmonics
        for (i=1; i <= harmonics; i++)
        {
            las.push_back(EEGLas(frequency*i, samplerate));
        }
    }

    double EEGSub::filter(const double &value)
    {
        std::vector<EEGLas>::iterator it;
        double retval = 0.0;

        //logPrintI("EEGSub(%lf)::filter(%lf)", frequency, value); 

        for (it = las.begin(); it!= las.end(); it++)
        {
            retval += it->filter(value);
        }
        
        //printf(" ... done (%lf)\n",  retval);

        return retval;
    }
} // namespace ot


/* 
 * ------------------------------------------------------------
 *   End of EEGSub.cxx
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
