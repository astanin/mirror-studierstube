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
/** source file for LogModule.
 *
 * @author Gerhard Reitmayr
 *
 * $Id$
 * @file                                                                   */
/* ======================================================================= */

#include <cstdlib>
#include <OpenTracker/common/LogModule.h>

#ifndef _WIN32_WCE
#include <fstream>
#endif

#include <ace/Log_Msg.h>

namespace ot {

    LogModule::LogModule(void)
    {
    }

    void LogModule::init( StringTable & attributes,  ConfigNode * localTree)
    {
        Module::init( attributes, localTree );
    
        if( attributes.get("active").compare("off") == 0 )
            ACE_LOG_MSG->set_flags(ACE_Log_Msg::SILENT);
        else
            ACE_LOG_MSG->clr_flags(ACE_Log_Msg::SILENT);

        if( attributes.get("stderr").compare("on") == 0 )
            ACE_LOG_MSG->set_flags(ACE_Log_Msg::STDERR);
        else
            ACE_LOG_MSG->clr_flags(ACE_Log_Msg::STDERR);

        if( attributes.get("verbose").compare("full") == 0 )
            ACE_LOG_MSG->set_flags(ACE_Log_Msg::VERBOSE);
        else if( attributes.get("verbose").compare("lite") == 0 )
            ACE_LOG_MSG->set_flags(ACE_Log_Msg::VERBOSE_LITE);

        if( attributes.get("level").compare("info") == 0 )
        {
            ACE_LOG_MSG->priority_mask( LM_INFO | LM_NOTICE | LM_WARNING | LM_ERROR | LM_CRITICAL | LM_ALERT | LM_EMERGENCY, 
                                        ACE_Log_Msg::PROCESS );
        }
        else if( attributes.get("level").compare("error") == 0 )
        {
            ACE_LOG_MSG->priority_mask( LM_WARNING | LM_ERROR | LM_CRITICAL | LM_ALERT | LM_EMERGENCY,
                                        ACE_Log_Msg::PROCESS );
        }

#ifndef _WIN32_WCE
        if( attributes.get("file").compare("on") == 0 && attributes.containsKey("filename"))
        {
            ACE_OSTREAM_TYPE * file = new std::ofstream( attributes.get("filename").c_str());
            ACE_LOG_MSG->msg_ostream( file, 1 );
            ACE_LOG_MSG->set_flags( ACE_Log_Msg::OSTREAM );
        }
#endif
    }

} // namespace ot


/* 
 * ------------------------------------------------------------
 *   End of LogModule.h
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
