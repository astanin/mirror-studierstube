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
/** header file for UltraTrakSource Node.
 *
 * @author Rainer Splechtna
 *
 * $Id: UltraTrakSource.h 1807 2007-04-12 10:35:50Z bornik $
 * @file                                                                   */
/* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section UltraTrakSource UltraTrakSource
 * The UltraTrakSource node is a simple EventGenerator that inserts events from
 * the UltraTrak into the tracker tree. The UltraTrakSource element has the 
 * following attribute:
 * @li @c number the stations number, between 0 and any positive number
 *  
 * An example element looks like this :
 * @verbatim
 <UltraTrakSource number="1" />@endverbatim
*/

#ifndef _UltraTrakSource_H
#define _UltraTrakSource_H

#include "../OpenTracker.h"

/**
 * This class implements a simple node that inserts new events from the UltraTrak
 * into the tracker tree.
 * @author Rainer Splechtna
 * @ingroup Network
 */


#ifndef OT_NO_ULTRATRACK_SUPPORT


namespace ot {

    class OPENTRACKER_API UltraTrakSource : public Node
    {
        // Members
    public:
        /// the new event
        Event event;
        int number;
        bool modified;

        // Methods
    protected:
        /** constructor */
        UltraTrakSource() : Node()
        { number = -1; modified = true; }

    public:                
        /** tests for EventGenerator interface being present. Is overriden to
         * return 1 always.
         * @return always 1 */
        virtual int isEventGenerator()
        {
            return 1;
        }  
        
        void pushEvent();
        void pullEvent();
    
        friend class UltraTrakModule;
    };

} // namespace ot


#endif //OT_NO_ULTRATRACK_SUPPORT


#endif

/* 
 * ------------------------------------------------------------
 *   End of UltraTrakSource.h
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
