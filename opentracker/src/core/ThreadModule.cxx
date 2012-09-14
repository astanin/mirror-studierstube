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
/** source file for ThreadModule class.
 *
 * @author Gerhard Reitmayr
 *
 * $Id: ThreadModule.cxx 2049 2007-09-19 05:59:38Z bornik $
 * @file                                                                   */
/* ======================================================================= */


// a trick to avoid warnings when ace includes the STL headers
#ifdef WIN32
#pragma warning(disable:4244)
#pragma warning(disable:4786)
#endif

#include <cstdlib>
#include <string>

#include <ace/Thread.h>
#include <ace/Synch.h>

#include <OpenTracker/OpenTracker.h>
#include <OpenTracker/core/OtLogger.h>
#include <OpenTracker/core/ThreadModule.h>

namespace ot {

    // enters a critical section. 

    
    void ThreadModule::lockLoop()
    {
	mutex->acquire();
    }

    // leaves a critical section. 

    void ThreadModule::unlockLoop()
    {
	mutex->release();
    }
    

    // constructor
        
    ThreadModule::ThreadModule() :
        threadID(NULL),
        threadHandle(NULL)
    {
        mutex = new ACE_Thread_Mutex;
    }

    // destructor clears everything 

    ThreadModule::~ThreadModule()
    {
        delete mutex;
    }

    // starts the thread

    void ThreadModule::start()
    {
	threadID = new ACE_thread_t;
	threadHandle = new ACE_hthread_t;
	ACE_Thread::spawn((ACE_THR_FUNC)thread_func, 
                          this, 
                          THR_NEW_LWP | THR_JOINABLE,
                          (ACE_thread_t *)threadID,
						  (ACE_hthread_t *)threadHandle);
    logPrintI("ThreadID: %d, ThreadHandle: %d\n", 
            (*(ACE_thread_t*)threadID), 
            (*(ACE_hthread_t*)(threadHandle)));
    }    

    // stops the thread and closes the module

    void ThreadModule::close()
    {
        logPrintI("ThreadModule::close()\n");
#ifdef WIN32
		//ACE_thread_t *departed;
        if (threadHandle)
        {
            ACE_Thread::join( (*((ACE_hthread_t*)threadHandle)));
        }
#else
        if (threadID)
        {
            if (threadID)
            {
                ACE_Thread::join( (*(ACE_thread_t*)threadID) );
            }
        }
#endif
	// ACE_Thread::cancel( *(ACE_thread_t *)thread );
        
        if (threadID)
        {
#ifndef WIN32
            delete ((ACE_thread_t *)threadID);            
#endif
        }
        if (threadHandle)
        {
#ifndef WIN32            
            delete ((ACE_hthread_t *)threadHandle);
#endif
        }
    }

} // namespace ot

/* 
 * ------------------------------------------------------------
 *   End of ThreadModule.cxx
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
