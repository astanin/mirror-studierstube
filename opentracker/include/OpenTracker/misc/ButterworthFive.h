/* ========================================================================
 * Copyright (c) 2007,
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
/** header file for ButterworthFive class.
 *
 * @author Alexander Bornik bornik@icg.tugraz.at
 *
 * $Id $
 * @file                                                                   */
/* ======================================================================= */


#ifndef _BUTTERWORTHFIVE_H
#define _BUTTERWORTHFIVE_H

#include <OpenTracker/OpenTracker.h>

namespace ot {

    static const double coefficientsa[6] = { 1.000000000000000,
                                             -4.920575523907497,
                                             9.685447082583096,
                                             -9.532811413309153,
                                             4.691584824703282,
                                             -0.923644961508006 };
    static const double coefficientsb[6] = { 1.0e-08 * 0.026755381243859,
                                             1.0e-08 * 0.133776906219296,
                                             1.0e-08 * 0.267553812438592,
                                             1.0e-08 * 0.267553812438592,
                                             1.0e-08 * 0.133776906219296,
                                             1.0e-08 * 0.026755381243859 };

    class OPENTRACKER_API ButterworthFive
    {
        // Members
    protected:
        double historyx[6];
        double historyy[6];
        unsigned long actindex;

        // Methods
    public:
        /** constructor method
         */
        ButterworthFive();

	/** takes an input value and returns the filtered value; */

	double filter(const double &value);
    };

} // namespace ot

#endif

/* 
 * ------------------------------------------------------------
 *   End of ButterworthFive.h
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
