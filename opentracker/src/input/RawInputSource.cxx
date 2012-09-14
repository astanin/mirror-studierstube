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
/** source file for the raw input source node.
 *
 * @author Mathis Csisinko
 *
 * $Id: RawInputSource.cxx 1557 2006-10-25 11:50:04Z veas $
 * @file                                                                   */
/* ======================================================================= */

#include <OpenTracker/input/RawInputSource.h>

#ifdef USE_RAWINPUT
#ifdef WIN32

namespace ot {

    RawInputSource::RawInputSource(HANDLE hDevice): Node(),modified(false),event(),hDevice(hDevice)
    {
    }

    void RawInputSource::setButtonEvent(unsigned short button,unsigned short buttonMask)
    {
        if (buttonMask)
            {
                event.getButton() = (event.getButton() & ~ buttonMask) | button;
                event.timeStamp();
                modified = true;
            }
    }

    void RawInputSource::setMoveEvent(MoveMode moveMode,float x,float y,float z)
    {
        switch (moveMode)
            {
		case XYZAbsolute:
                    event.getPosition()[2] = z;
		case XYAbsolute:
                    event.getPosition()[0] = x;
                    event.getPosition()[1] = y;
                    event.timeStamp();
                    modified = true;
                    break;
		case XYRelative:
                    if (x != 0.f && y != 0.f)
			{
                            event.getPosition()[0] += x;
                            event.getPosition()[1] += y;
                            event.timeStamp();
                            modified = true;
			}
                    break;
		case ZRelative:
                    if (z != 0.f)
			{
                            event.getPosition()[2] += z;
                            event.timeStamp();
                            modified = true;
			}
            }
    }

    bool RawInputSource::calcEvent()
    {
        bool result = modified;
        modified = false;
        return result;
    }

} // namespace ot


#endif
#endif

/* ===========================================================================
   End of RawInputSource.cxx
   ===========================================================================
   Automatic Emacs configuration follows.
   Local Variables:
   mode:c++
   c-basic-offset: 4
   eval: (c-set-offset 'subeventment-open 0)
   eval: (c-set-offset 'case-label '+)
   eval: (c-set-offset 'eventment 'c-lineup-runin-eventments)
   eval: (setq indent-tabs-mode nil)
   End:
   =========================================================================== */
