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
/** source file for InterpolatorModule module.
 *
 * @author Flo Ledermann
 *
 * $Id: InterpolatorModule.cxx 2103 2007-12-19 16:39:50Z bornik $
 * @file                                                                   */
/* ======================================================================= */


#include <OpenTracker/OpenTracker.h>


#ifndef OT_NO_INTERPORLATORMODULE_SUPPORT


// this will remove the warning 4786
#include <OpenTracker/tool/disable4786.h>
#include <cstdlib>

#include <OpenTracker/common/InterpolatorModule.h>
#include <OpenTracker/common/InterpolatorModule.h>

#include <OpenTracker/common/ElasticFilterNode.h>

#include <cstdio>
#include <iostream>

#include <ace/Log_Msg.h>


namespace ot {


	OT_MODULE_REGISTER_FUNC(InterpolatorModule){
		OT_MODULE_REGISTRATION_DEFAULT(InterpolatorModule, "InterpolatorConfig" );
	}



    // Destructor method

    InterpolatorModule::~InterpolatorModule()
    {
        nodes.clear();
    }

    // This method is called to construct a new Node.

    Node * InterpolatorModule::createNode( const std::string& name, 
                                           const StringTable& attributes)
    {
        if( name.compare("ElasticFilter") == 0 )
        {
            int frequency;
            int offset;
            float force = 0.1f, damp = 0.1f;
            int num = sscanf(attributes.get("frequency").c_str(), " %i", &frequency );
            if( num == 0 ){
                frequency = 1;
            }
            num = sscanf(attributes.get("offset").c_str(), " %i", &offset );
            if( num == 0 ){
                offset = 0;
            }
            num = sscanf(attributes.get("force").c_str(), " %f", &force );
            num = sscanf(attributes.get("damp").c_str(), " %f", &damp );
            ElasticFilterNode * source = new ElasticFilterNode( force, damp, frequency, offset );
            nodes.push_back( source );        
            ACE_DEBUG((LM_INFO, ACE_TEXT("ot:InterpolatorModule: Built ElasticFilter node\n")));
            initialized = 1;
            return source;
        }
        return NULL;
    }

    // pushes events into the tracker tree.
    void InterpolatorModule::pushEvent()
    {
        cycle++;
    }

} // namespace ot {


#else
#pragma message(">>> OT_NO_INTERPORLATORMODULE_SUPPORT")
#endif // OT_NO_INTERPORLATORMODULE_SUPPORT


/* 
 * ------------------------------------------------------------
 *   End of InterpolatorModule.cxx
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
