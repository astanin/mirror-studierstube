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
/** header file for ButtonOp Node.
 *
 * @author Gerhard Reitmayr
 * 
 * $Id: ButtonOpNode.h 1807 2007-04-12 10:35:50Z bornik $
 * @file                                                                   */
/* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section buttonopnode ButtonOp
 * The ButtonOp node implements a boolean operation on the button events of 
 * its two input ports. It saves the last button events from each input and
 * outputs either the logical AND or OR result of the two events, whenever
 * a new event from either input is received.
 *
 * The element has the following attributes :
 * @li @c op (OR | AND) whether it should do an OR or AND.
 *
 * An example element looks like this :
 * @verbatim
 <ButtonOp op="OR">
 <Arg1>
 <any event generator ...>
 </Arg1>
 <Arg2>
 <any event generator ...>
 </Arg2>
 </ButtonOp>@endverbatim
*/

#ifndef _BUTTONOPNODE_H
#define _BUTTONOPNODE_H

#include "../OpenTracker.h"

/**
 * The ButtonOp node implements a boolean operation on two button events.
 * It uses two NodePorts named 'Arg1' and 'Arg2'.
 * @author Gerhard Reitmayr
 * @ingroup common
 */


#ifndef OT_NO_BUTTON_SUPPORT


namespace ot {

    class OPENTRACKER_API ButtonOpNode
        : public Node
    {
        // Members
    protected:
        /// last saved event variable
        Event result;
        /// last button events
        unsigned short arg1, arg2;
        /// the operation to perform
        enum Op { OR, AND } operation;

        // Methods
    protected:

        /** constructor method
         */
        ButtonOpNode( const Op & op_  );

    public:

        /** tests for EventGenerator interface being present. Returns the
         * result of the childs implementation of this method.
         * @return 1 if child implements EventGenerator, 0 otherwise */
        virtual int isEventGenerator() ;

        /**
         * this method is called by the EventGenerator to update it's observers.
         */
        virtual void onEventGenerated( Event& event, Node& generator);

        void pushEvent();
        void pullEvent();

        friend class CommonNodeFactory;
    };

} // namespace ot


#endif //OT_NO_BUTTON_SUPPORT


#endif


/* 
 * ------------------------------------------------------------
 *   End of ButtonOpNode.h
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
