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
/** header file for ARToolKit source node.
 *
 * @author Gerhard Reitmayr
 *
 * $Id: ARToolKitSource.h 2124 2008-02-15 16:30:09Z veas $
 * @file                                                                   */
/* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section artoolkitsource ARToolKitSource
 * The ARToolKitSource node is an abstract class providing basic facilities.
 */

#ifndef _ARTOOLKITSOURCE_H
#define _ARTOOLKITSOURCE_H

#include "../OpenTracker.h"


#if defined(USE_ARTOOLKIT) || defined(USE_ARTOOLKITPLUS)

namespace ot {

  /** An ARToolKitSource is the base abstract class for ARToolKit library.
   * artoolkit library.
   * @ingroup input
   */
  class OPENTRACKER_API ARToolKitSource : public Node
    {

    public:
       Event event,  ///< the event pushed into the tree
	          buffer; ///< buffer variable for grabbing thread
       int modified;  ///< is a new update ready in the buffer ?

    public:
      /** tests for EventGenerator interface being present. Is overridden to
       * return 1 always.
       * @return always 1 */
      virtual int isEventGenerator()
	   {
	     return 1;
	   };  

     protected:
      ARToolKitSource() : Node(), modified(0)
      {

      }
  

  };

} // namespace ot

#endif	// defined(USE_ARTOOLKIT) || defined(USE_ARTOOLKITPPC)

#endif

/* 
 * ------------------------------------------------------------
 *   End of Transformation.cxx
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
