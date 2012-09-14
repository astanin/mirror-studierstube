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
/** source file for Callforward module.
 *
 * @author Alexander Bornik
 *
 * $Id$
 * @file                                                                   */
/* ======================================================================= */

// this will remove the warning 4786
#include <OpenTracker/tool/disable4786.h>

#include <cstdlib>
#include <OpenTracker/common/CallforwardModule.h>
#include <OpenTracker/common/CallforwardNode.h>
#include <iostream>

#include <ace/Log_Msg.h>
#include <OpenTracker/tool/OT_ACE_Log.h>

#ifndef OT_NO_CALLFORWARDMODULE_SUPPORT

namespace ot {
        
    OT_MODULE_REGISTER_FUNC(CallforwardModule){
            OT_MODULE_REGISTRATION_DEFAULT(CallforwardModule,"CallforwardConfig" );
	}


    Node * CallforwardModule::createNode( const std::string& name,
                                          const StringTable& attributes)
    {
        if( name.compare("Callforward") == 0 )
        {
            const std::string & nameVal = attributes.get("name");
            NodeMap::iterator it = nodes.find( nameVal );
            if( it == nodes.end())
            {
                CallforwardNode * node = new CallforwardNode( nameVal );
                nodes[nameVal] = node;
                logPrintI("Built Callforward node %s.\n", nameVal.c_str());
                return node;
            }
        } 
        return NULL;
    }

    bool CallforwardModule::callForward(const std::string& name, Event &event)
    {
        using namespace std;
       
        NodeMap::iterator it = nodes.find( name );
        //logPrintI(" CallforwardModule::callForward('%s')\n", name.c_str());

        if( it != nodes.end())
        {
            //logPrintI(" found '(%s):'", name.c_str());
            CallforwardNode * cfnode =
                dynamic_cast<CallforwardNode*>((*it).second);
            
            if (cfnode)
            {
                //event.printout();
                cfnode->setEvent(event);
                if (context != NULL)
                {
                    // notify main loop
		    if (context->doSynchronization())
		    {
                        context->dataSignal();
                        context->consumedWait();
                        //logPrintI("CallforwardModule::event processed!\n");
		    }
                }
            }
            else
            {
                //logPrintI(" could not find '(%s):'", name.c_str());
                return false;
            }
        }
        else
        {
            //cout << " callforward node with name " 
            //     << name <<" not found!" << endl;
            return false;
        }

        return true;
    } 

   void CallforwardModule::pushEvent()
    {
        /// nothing to do here
    }

} // namespace ot


#else
#pragma message(">>> OT_NO_CALLFORWARDMODULE_SUPPORT")
#endif //OT_NO_CALLFORWARDMODULE_SUPPORT


/* 
 * ------------------------------------------------------------
 *   End of CallforwardModule.cxx
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
