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
/** source file for LinmouseSource.
*
* @authors Michele Fiorentino Alexander Bornik
* starting from cWiimote 0.2 by Kevin Forbes 
* $Id$
* @file input/WiiSource.cxx                                                    */
/* ======================================================================= */

// this is a linux implementation


#include <OpenTracker/OpenTracker.h>
#include <OpenTracker/input/WiiSource.h>

#ifdef USE_WII_SUPPORT

namespace ot {

   void WiiSource::pushEvent() 
   {
      lock();
      if( changed == 1 )
      {	
         updateObservers( event );
         changed = 0;
      }
      unlock();

   }

   void WiiSource::pullEvent() 
   {
      ///in future put code to manage more than one wii in a row
   }
}

#endif //#ifdef USE_WII_SUPPORT

/* 
* ------------------------------------------------------------
*   End of wiiSource.cxx
* ------------------------------------------------------------
*/
