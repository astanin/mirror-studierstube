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
/** source file for ButtonOp Node.
 *
 * @author Gerhard Reitmayr
 *
 * $Id: ButtonOpNode.cxx 1807 2007-04-12 10:35:50Z bornik $
 * @file                                                                   */
/* ======================================================================= */

// this will remove the warning 4786
#include <OpenTracker/tool/disable4786.h>
#include <OpenTracker/common/ButtonOpNode.h>

//using namespace std;

// constructor method.


#ifndef OT_NO_BUTTON_SUPPORT


namespace ot {

    ButtonOpNode::ButtonOpNode( const Op & op )
        : Node(), arg1(0), arg2(0), operation( op )
    {
    }

    int ButtonOpNode::isEventGenerator()
    {
        return 1;
    }

    // this method is called by the EventGenerator to update it's observers.

    void ButtonOpNode::onEventGenerated( Event& event, Node& generator)
    {
        if( generator.isNodePort() == 1 )
        {
            if( generator.getType().compare("Arg1") == 0 )
                arg1 = event.getButton();
            else
                arg2 = event.getButton();
            result = event;
            switch( operation )
            {
                case OR :
                    result.getButton() = arg1 | arg2;
                    break;
                case AND :
                    result.getButton() = arg1 & arg2;
                    break;
            }
            updateObservers( result );
        }
    }

    void ButtonOpNode::pushEvent()
    {
        // nothing to do
    }

    void ButtonOpNode::pullEvent()
    {
        // nothing to do
    }

} // namespace ot


#else
#pragma message(">>> OT_NO_BUTTON_SUPPORT")
#endif //OT_NO_BUTTON_SUPPORT

/* 
 * ------------------------------------------------------------
 *   End of ButtonOpNode.cxx
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
