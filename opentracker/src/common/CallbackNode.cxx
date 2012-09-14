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
/** source file for Callback Node.
 *
 * @author Alexander Bornik
 *
 * $Id$
 * @file                                                                   */
/* ======================================================================= */

// this will remove the warning 4786
#include <OpenTracker/tool/disable4786.h>

#include <cstdlib>
#include <OpenTracker/common/CallbackNode.h>
#include <OpenTracker/common/CallbackModule.h>

#include <iostream>

//using namespace std;

#include <ace/Log_Msg.h>
#include <OpenTracker/tool/OT_ACE_Log.h>
#include <ace/Synch.h>

// called to construct a new Node. 

#ifndef OT_NO_CALLBACKMODULE_SUPPORT

#include <OpenTracker/core/OtLogger.h>

namespace ot {
  
    void CallbackNode::onEventGenerated( Event& event, Node& generator)
    {

        using namespace std;

		//logPrintW("CallbackNode:: onEventGenerated for node %s\n", (name.c_str()));

        // lock event for the time of the callback
        
        eventmutex.acquire();
        
        OTGlobalCallbackFunction *gcbfunc = NULL;
        void * gcbdata;

        logPrintI("CallbackNode::onEventGenerated: \n");
        // call to global callback function
      
        if (cbmodule != NULL) 
        {
            gcbfunc = cbmodule->getGlobalCallbackFunction();
            gcbdata = cbmodule->getGlobalCallbackData();
        }

        if (gcbfunc != NULL)
        {
            (*gcbfunc)(this, event, gcbdata);
        }
        // call node based callback function
        if ( function != NULL )
        {
			//logPrintW("CallbackNode:: calling the function\n");
            (*function)(this, event, data );
        }

        eventmutex.release();

        updateObservers( event );
    }
    
    void CallbackNode::pushEvent()
    {
        /// nothing to do here
    }
    
    void CallbackNode::pullEvent()
    {
        /// nothing to do here
    }
}  // namespace ot

#else
#pragma message(">>> OT_NO_CALLBACKMODULE_SUPPORT")
#endif //OT_NO_CALLBACKMODULE_SUPPORT


    /* 
     * ------------------------------------------------------------
     *   End of CallbackModule.cxx
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
