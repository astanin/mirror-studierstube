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
/** 
 *
 * @author Erick Mendez
 *
 * $Header$
 * @file                                                                   */
/* ======================================================================= */


#ifndef _OT_VIDEOUSER_H
#define _OT_VIDEOUSER_H

#ifndef SWIG
#include <vector>
#endif

namespace ot 
{


/// Chosen to be compatible to OpenVideo's pixel formats!
enum PIXEL_FORMAT {
    FORMAT_R8G8B8	= 0,
    FORMAT_B8G8R8	= 1,
    FORMAT_R8G8B8X8	= 2,
    FORMAT_B8G8R8X8	= 3,
    FORMAT_R5G6B5	= 4,
    FORMAT_L8		= 5,
    FORMAT_UNKNOWN	= 6,
    FORMAT_X8B8G8R8 = 7
};


class VideoUser
{
public:

virtual ~VideoUser() {
}

    /// provides a module with a new video frame. image is only valid for this function call!
    virtual void newVideoFrame(const unsigned char* image, int width, int height, 
                               PIXEL_FORMAT format) = 0;
};


typedef std::vector<VideoUser*> VideoUserVector;


} // namespace ot


#endif // _OT_VIDEOUSER_H

/* 
 * ------------------------------------------------------------
 *   End of VideoUser.h
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
