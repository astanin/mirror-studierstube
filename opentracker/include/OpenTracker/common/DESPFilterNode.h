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
/** header file for DESPFilter Node.
 *
 * @author Erick Mendez
 * @author Gerhard Schall
 *
 * $Id: DESPFilterNode.h 1168 2006-06-14 05:55:21Z bornik $
 * @file                                                                   */
/* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section despfilter DESPFilter
 * DESPFilter implements a filtering algorithm similar to kalman, but simpler and faster

 Example usage/defaults:

 @verbatim
 \<DESPFilter filter-position="0.5" filter-orientation="0.3">
 ... (any Event Source)
 \</DESPFilter>
 @endverbatim
*/

#ifndef _DESPFILTERNODE_H
#define _DESPFILTERNODE_H

#include "../OpenTracker.h"
#include "DESP.h"

/**
 * @author Erick Mendez
 * @author Gerhard Schall
 * @ingroup common
 */


#ifndef OT_NO_DESPFILTER_SUPPORT


namespace ot {

    class OPENTRACKER_API DESPFilterNode : public Node   
    {

    private:
        PoseDESP poseDesp;
    protected:
        bool init;
        float filterPos;     ///< Position Filter
        float filterOri;     ///< Orientation Filter

	    // internal variables
        Event currentEvent;  ///< The event we're currently working with
        Event targetEvent;   ///< The one we will be forwarding
        Event lastEvent;     ///< The last one we worked with

        int frameCtr;        ///< A counter for the last time we received a new frame

    public:
        /** constructor method. It sets initial values for the treshhold and type */
        DESPFilterNode( float filterPos_ = 0.0, float filterOri_ = 0.0 );

        /** tests for EventGenerator interface being present and returns
         * 1, if present.
         * @return always 1 */
        virtual int isEventGenerator()
        {
            return 1;
        }

        /**
         * This method is called by any child node. 
	     * @param event the event value passed
	     * @param generator the node generating the event
         */
        virtual void onEventGenerated( Event& event, Node & generator);                                
        void pushEvent();
        void pullEvent();
    };

} // namespace ot


#endif //OT_NO_DESPFILTER_SUPPORT

#endif //_DESPFILTERNODE_H

/* 
 * ------------------------------------------------------------
 *   End of DESPFilterNode.h
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
