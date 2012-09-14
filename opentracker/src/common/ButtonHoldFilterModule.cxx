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
/** source file for ButtonHoldFilterModule module.
 *
 * @author Thomas Psik
 *
 * $Id: ButtonHoldFilterModule.cxx 2103 2007-12-19 16:39:50Z bornik $
 * @file                                                                   */
/* ======================================================================= */

// this will remove the warning 4786
#include <OpenTracker/tool/disable4786.h>

#include <cstdlib>
#include <OpenTracker/common/ButtonHoldFilterModule.h>
#include <OpenTracker/common/ButtonHoldFilterModule.h>

#include <OpenTracker/common/ButtonHoldFilterNode.h>

#include <cstdio>
#include <iostream>

//using namespace std;
#include <ace/Log_Msg.h>

// Destructor method


#ifndef OT_NO_BUTTON_SUPPORT


namespace ot {

	OT_MODULE_REGISTER_FUNC(ButtonHoldFilterModule){
		OT_MODULE_REGISTRATION_DEFAULT(ButtonHoldFilterModule , "ButtonHoldFilterConfig" );
	}



    ButtonHoldFilterModule::~ButtonHoldFilterModule()
    {
        nodes.clear();
    }

    // This method is called to construct a new Node.

    Node * ButtonHoldFilterModule::createNode( const std::string& name, const StringTable& attributes)
    {
        if( name.compare("ButtonHoldFilter") == 0 )
        {
            int offDuration;
            int num = sscanf(attributes.get("offDuration").c_str(), " %i", &offDuration );
            if( num == 0 ){
                offDuration = 2;
            }

            ButtonHoldFilterNode * source = new ButtonHoldFilterNode(offDuration  );
            nodes.push_back( source );
            logPrintI("Built ButtonHoldFilter node \n");

            initialized = 1;
            return source;
        }
        return NULL;
    }

    // pushes events into the tracker tree.
    void ButtonHoldFilterModule::pushEvent()
    {        
        cycle++;
    }

} // namespace ot


#else
#pragma message(">>> OT_NO_BUTTON_SUPPORT")
#endif //OT_NO_BUTTON_SUPPORT

/* 
 * ------------------------------------------------------------
 *   End of ButtonHoldFilterModule.cxx
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
