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
/** Header file for OSUtils class, a collection of common utility functions.
 *
 * @author Gerhard Reitmayr
 *
 * $Id: OSUtils.h 1547 2006-10-25 10:33:10Z veas $
 * @file                                                                   */
/* ======================================================================= */

#ifndef _OSUTILS_H
#define _OSUTILS_H

#include "../dllinclude.h"

/**
 * This class implements some utility functions, which are not provided by
 * other libraries. Currently these include time related functions to get
 * the current time and yield and sleep a specified amount of time.
 * @author Gerhard Reitmayr
 * @ingroup core
 */

namespace ot {

    class OPENTRACKER_API OSUtils
    {

    public:

        /** returns the current time in milliseconds since 1.1.1970. Someone said
         * this should probably be a long, I used a double because VRML does too.
         * @note check whether double or long is better
         * @return double containing milliseconds since ...*/
        static double currentTime();

        /** sleeps for the specified amount of milliseconds
         * and yields the execution to another thread. If 0 is specified, only
         * a yield is performed (implementation based on Sleep (Win32), sginap (SGI)).
         * The implementation is actually not accurate enough, because both systems
         * provide only timeslices of 1/100 of a second. So all sleep times will
         * actually be multiples of 1/100 of a second.
         * @arg time time to sleep in milliseconds
         */
        static void sleep( double time );
    };

} // namespace ot

#endif

/* 
 * ------------------------------------------------------------
 *   End of OSUtils.h
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
