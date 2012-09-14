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
/** header file string fixer
 *
 * @author Daniel Wagner
 *
 * $Header$
 * @file                                                                   */
/* ======================================================================= */


#ifndef __STRINGFIXER_HEADERFILE__
#define __STRINGFIXER_HEADERFILE__

#include <cstring>

class StringFixer
{
    enum {
        MAX_LEN = 8192			// 8K should be enough...
    };

public:
    StringFixer(const char* nString, ...)
    {
        str = new char[MAX_LEN];

        va_list marker;
        va_start(marker, nString);
        vsprintf(str, nString, marker);
    }

    /*StringFixer(const unsigned short* nString, ...)
      {
      char* strc = new char[wcslen(nString)+1];
      int len = wcslen(nString);

      for(int i=0; i<=len; i++)
      strc[i] = (char)nString[i];

      str = new char[MAX_LEN];

      va_list marker;
      va_start(marker, nString);
      vsprintf(str, strc, marker);
      delete strc;
      }*/

    const char* get() const
    {
        return str;
    }

    ~StringFixer()
    {
        delete str;
    }

protected:
    char* str;
};

#ifdef _WIN32_WCE
#define FIX_STRING(X) StringFixer(X).get() 
#else
#define FIX_STRING(X) X
#endif //_WIN32_WCE


#endif //__STRINGFIXER_HEADERFILE__

/* 
 * ------------------------------------------------------------
 *   End of StringFixer.h
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
