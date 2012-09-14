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
*
* $Id$
* @file                                                                   */
/* ======================================================================= */

/**
* @page Nodes Node Reference
* @section WiiSource WiiSource
* The WiiSource node is a simple EventGenerator that outputs the
* current position and button event of the Wii. It is driven by
* the @ref WiiModule. 
*
* An example element looks like this :
* @verbatim
<WiiSource/>
@endverbatim
*/

#ifndef _WIISOURCE_H
#define _WIISOURCE_H

#include <OpenTracker/OpenTracker.h>

#ifdef USE_WII_SUPPORT

#include<OpenTracker/input/WiiHandler.h>

namespace ot {

   /**
   * This class implements a simple source that sets its valid flag in
   * regular intervals and updates any EventObservers. 
   * @author Alexander Bornik
   * @ingroup input
   */
   class OPENTRACKER_API WiiSource : public Node {

      // Members
   public:   
      /// source device name
      std::string devname;
      /// the event that is posted to the EventObservers
      Event event;
      /// a flag to indicate whether it was changed during processing
      int changed;

      // Methods
      /** simple constructor, sets members to initial values */
      WiiSource( WiiHandler* wiimote, std::string devname_ ="wii") : Node(), devname(devname_), changed(1)
      {}

      /** tests for EventGenerator interface being present. Is overriden to
      * return 1 always.
      * @return always 1 */
      virtual int isEventGenerator()
      {
         return 1;
      }

      void pushEvent( );
      void pullEvent( );
   };

}  // namespace ot

#endif //#ifdef USE_WII_SUPPORT
#endif //_WIISOURCE_H

/* 
* ------------------------------------------------------------
*   End of LinMouseSource.h
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
