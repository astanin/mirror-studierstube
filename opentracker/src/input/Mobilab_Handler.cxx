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
/** source file for Mobilab_Handler
 *
 * @author Alexander bornik
 * 
 * $Id$
 *
 * @file                                                                   */
/* ======================================================================= */

#include <OpenTracker/OpenTracker.h>

#ifndef EXCLUDE_MOBILAB_SUPPORT

#ifdef WIN32
#pragma warning(disable:4244)
#endif

#include <cstdlib>
#include <OpenTracker/tool/FixWinCE.h>
#include <OpenTracker/input/Mobilab_Handler.h>
#include <OpenTracker/input/MobilabDriver.h>

namespace ot {


    Mobilab_Handler::Mobilab_Handler(MobilabDriver * parent_) :
        parent( parent_ ),
        mobilabind( 0 )
    {
        logPrintI("Mobilab_Handler::Mobilab_Handler()\n");	
    }

    Mobilab_Handler::~Mobilab_Handler()
    {
        logPrintI("Mobilab_Handler::~Mobilab_Handler()\n");
    }

    int Mobilab_Handler::open( void * factory )
    {
        //reactor()->register_handler(SIGINT, this);
#ifdef WIN32
	return reactor()->register_handler(this);
#else
	return reactor()->register_handler(this, ACE_Event_Handler::READ_MASK);
#endif        
    }

    int	Mobilab_Handler::handle_input(ACE_HANDLE fd)
    {
        //logPrintI("Mobilab_Handler::handle_input\n");

        int ncnt;

        // try to read the difference to 16 bytes = data for all 8 channels
        ncnt = peer().recv( mobilabbuf + mobilabind, 16 - mobilabind);
        if (ncnt>0)
        {
            mobilabind += ncnt;
            
            // we are complete
            if (mobilabind == 16)
            {
                parent->newSample(mobilabbuf);
                mobilabind  = 0;
            }  
	}
#ifdef WIN32        
        else
        { 
            // this is a hack to avoid high resource consumption on windows !

            ACE_OS::sleep(ACE_Time_Value(0,20000));
        }
#endif
        return 0;
    }

    int Mobilab_Handler::handle_signal( int signum, siginfo_t *, ucontext_t * )
    {
        logPrintI("Mobilab_Handler::handle_signal\n");
        
        if (signum != 0)
        {
            ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("received signal %S\n"), signum));
            if (parent)
            {
                parent->sendStopTransferCommand();
            }
            return 0;
        }

        return 0;	
    }

} // namespace ot

#endif //#ifdef EXCLUDE_MOBILAB_SUPPORT

/* 
 * ------------------------------------------------------------
 *   End of Mobilab_Handler.cxx
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
