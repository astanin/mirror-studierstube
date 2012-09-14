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
/** header file string aligner
 *
 * @author Daniel Wagner
 *
 * $Header$
 * @file                                                                   */
/* ======================================================================= */


#ifndef __STRINGALIGNER_HEADERFILE__
#define __STRINGALIGNER_HEADERFILE__

#ifdef WIN32
#pragma warning(disable:4311)
#endif

#include <cstring>

class StringAligner
{
public:
    StringAligner(const char* nPtr) : ptr(nPtr), myptr(0)
    {
#define USE_64_BIT
#ifdef USE_64_BIT
        if((reinterpret_cast<unsigned long>(nPtr) & 0x03f) != 0)
#else
            if((reinterpret_cast<unsigned int>(nPtr) & 0x03f) != 0)
#endif
                strcpy(myptr = new char[strlen(nPtr)+1], nPtr);
    }

    const char* get() const
    {
        return myptr ? myptr : ptr;
    }

    ~StringAligner()
    {
        if(myptr)
            delete myptr;
    }

protected:
    const char* ptr;
    char* myptr;
};

#ifdef _WIN32_WCE
#define ALIGN_STRING(str)  StringAligner(str).get()
#else
#define ALIGN_STRING(str) str
#endif //_WIN32_WCE


#endif //__STRINGALIGNER_HEADERFILE__

/* 
 * ------------------------------------------------------------
 *   End of StringAligner.h
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
