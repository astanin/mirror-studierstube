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
/** header file for ARToolKitMultiMarkerSource source node.
 *
 * @author Daniel Wagner
 *
 * $Id$
 * @file                                                                   */
/* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section artoolkitmultimarkersource ARToolKitMultiMarkerSource
 * The ARToolKitMultiMarkerSource node is a simple EventGenerator that inserts events 
 * from tracking a visual multi-marker in a video image by using the ARToolKitPlus
 * library. See the @ref artoolkitplusmodule for information about how to
 * configure the module as a whole. It has the following elements :
 * @li @c cfg-file the file containing setup the marker patterns
 *
 * An example element looks like this :
 * @verbatim
 <ARToolKitMultiMarkerSource cfg-file="test_setup.cfg" />@endverbatim
*/

#ifndef _ARTOOLKITMULTIMARKERSOURCE_H
#define _ARTOOLKITMULTIMARKERSOURCE_H

#include "ARToolKitSource.h"


#if defined(USE_ARTOOLKIT) || defined(USE_ARTOOLKITPLUS)

namespace ot {

    /** An ARToolKitSource represents a single marker that is tracked by the 
     * artoolkit library.
     * @ingroup input
     */
    class OPENTRACKER_API ARToolKitMultiMarkerSource : public ARToolKitSource
    {

    public:
	      std::string cfgfile;    ///< filename of the multimarker config file
	      void* mmConfig;         ///< pointer to the multi-marker configuration structure (loaded from cfgfile)

        /** constructor method
         * @param cfgfile_ filname of artoolkit multimarker configuration
         * @param vertex_ vertex coordinates of the marker
         */
        ARToolKitMultiMarkerSource(std::string& cfgfile_, void* mmConfig_) : 
            cfgfile( cfgfile_ ),
            mmConfig(mmConfig_)
        {

           type = "ARToolKitPlusMultiMarkerSource";

        }

    };

} // namespace ot

#endif	// defined(USE_ARTOOLKIT) || defined(USE_ARTOOLKITPPC)

#endif  // _ARTOOLKITMULTIMARKERSOURCE_H

/* 
 * ------------------------------------------------------------
 *   End of ARToolKitMultiMarkerSource.h
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
