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
/** source file for Callback module.
 *
 * @author Gerhard Reitmayr
 *
 * $Id: CallbackModule.cxx 2103 2007-12-19 16:39:50Z bornik $
 * @file                                                                   */
/* ======================================================================= */

// this will remove the warning 4786
#include <OpenTracker/tool/disable4786.h>

#include <cstdlib>
#include <OpenTracker/common/CallbackModule.h>
#include <OpenTracker/common/CallbackNode.h>

#include <iostream>

//using namespace std;

#include <ace/Log_Msg.h>
#include <OpenTracker/tool/OT_ACE_Log.h>

// called to construct a new Node.


#ifndef OT_NO_CALLBACKMODULE_SUPPORT


namespace ot {


	OT_MODULE_REGISTER_FUNC(CallbackModule){
		OT_MODULE_REGISTRATION_DEFAULT(CallbackModule,"CallbackConfig" );
	}


    Node * CallbackModule::createNode( const std::string& name, 
                                       const StringTable& attributes)
    {
        if( name.compare("Callback") == 0 )
        {
            const std::string & nameVal = attributes.get("name");
            NodeMap::iterator it = nodes.find( nameVal );
            if( it == nodes.end())
            {
                CallbackNode * node = new CallbackNode( nameVal );
                // for the global callback function to work,
                // the nodes need to know the module
                node->cbmodule = this;
                // setup callback if they are in the lcbmap
                // This way a callback set, when the nod was not
                // there, still works
                fctmap_type::iterator lcbit = lcbmap.find(nameVal);
                if (lcbit != lcbmap.end())
                {
                    setCallback(nameVal, lcbit->second.first, 
                                lcbit->second.second);
                }
                nodes[nameVal] = node;
                logPrintI("Built Callback node %s.\n", nameVal.c_str());
                return node;
            }
        } 
        return NULL;
    }


    //  sets a callback on a certain node.

    void CallbackModule::getLocalCallbackMap(fctmap_type &fmap)
    {
        using namespace std;
        
        fmap = lcbmap;
        /*
        NodeMap::iterator mit;
        OTCallbackFunction * cbf = NULL;
        void * cbd = NULL;
        
        for (mit = nodes.begin(); 
             mit != nodes.end(); mit++)
        {
            cerr << mit->first << endl;

            cbf = ((CallbackNode *)(*mit).second)->function;
            cbd = ((CallbackNode *)(*mit).second)->data;

            if (cbf != NULL)
            {
                fmap[mit->first] = std::make_pair(cbf,cbd);
            }
            }
        */
    }

    void CallbackModule::setCallback( const std::string& name, OTCallbackFunction * function, void * data )
    {
        NodeMap::iterator it = nodes.find( name );
        if( it != nodes.end())
        {
            ((CallbackNode *)(*it).second)->function = function;
            ((CallbackNode *)(*it).second)->data = data;
        }
        lcbmap[name] = std::make_pair(function,data);
    }

    // sets a global callback function.
    void CallbackModule::setGlobalCallback(OTGlobalCallbackFunction *function, void * data)
    {
        gcbfunction = function;
        gcbdata = data;
    }

    OTGlobalCallbackFunction* 
    CallbackModule::getGlobalCallbackFunction() const
    {
        return gcbfunction;
    }

    void* CallbackModule::getGlobalCallbackData() const
    {
        return gcbdata;
    }

} // namespace ot


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
