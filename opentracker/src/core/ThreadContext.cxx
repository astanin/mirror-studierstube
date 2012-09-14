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
/** source file for the threaded Context class
 *
 * @author Alexander Bornik
 *
 * $Id$
 * @file                                                                   */     
/* ======================================================================= */
#ifdef WIN32
#pragma warning(disable:4244)
#endif

#include <cstdlib>
#include <OpenTracker/tool/FixWinCE.h>
#include <ace/OS.h>
#include <ace/FILE.h>
#include <ace/Condition_Thread_Mutex.h>
#include <ace/Thread.h>
#include <ace/Synch.h>
#include <ace/Thread_Mutex.h>
#include <algorithm>

#include <OpenTracker/OpenTracker.h>

#include <OpenTracker/core/ConfigurationParser.h>
#include <OpenTracker/common/CommonNodeFactory.h>
#include <OpenTracker/common/CallbackModule.h>

#include <OpenTracker/core/Configurator.h>
#include <OpenTracker/core/ThreadContext.h>
#include <OpenTracker/core/Context.h>


namespace ot {

    ThreadContext::ThreadContext(int init ) : Context(init)
    {
        using namespace std;
        logPrintI("ThreadContext::ThreadContext()\n");

        action_type = POLL;
        action_rate = 1.0;

        thread_mutex = new thread_mutex_type;
        action_mutex = new mutex_type;
        action_cond = new condition_type(*action_mutex);
        action_cond_val = false;
        loopend_mutex = new mutex_type;
        loopend_cond = new condition_type(*loopend_mutex);
        loopend_cond_val = false;

        
        inloop = false;

        thread = new ACE_thread_t;
        ACE_Thread::spawn((ACE_THR_FUNC)thread_func, 
                          this, 
                          THR_NEW_LWP | THR_JOINABLE,
                          (ACE_thread_t *)thread );	
    }

    // Destructor method.
    ThreadContext::~ThreadContext()
    {     
        using namespace std;
        using namespace ot;
        
        logPrintI("ThreadContext::~ThreadContext()\n");

        /// make sure that next action is causing dispatcher loop to exit
        thlock();

        action_type = QUIT;

        thunlock();
        
        /// stop the running event loop, thread wait for new action signal
        /// afterwards
        Context::stopLoop();

        if (inloop)
        { 
            loopend_mutex->acquire();
            while (!loopend_cond_val)
            {
                loopend_cond->wait();                
            }
            loopend_cond_val = false;
            loopend_mutex->release();
        }

        /// send action signal
        //action_cond->signal();

        /// wait until thread has closed down
        logPrintI(" waiting for thread to finish ...\n");
#ifdef WIN32
	ACE_Thread::join( (ACE_thread_t*)thread );
#else
	ACE_Thread::join( (ACE_thread_t)thread );
#endif
        logPrintI(" done.\n");

        delete ((ACE_thread_t *)thread);

        delete loopend_cond;
        delete loopend_mutex;        
        delete action_cond;
        delete action_mutex;
        delete thread_mutex;
        logPrintI("ThreadContext::~ThreadContext() done.\n");
    }

    // enters a critical section. 

    void ThreadContext::thlock()
    {
	thread_mutex->acquire();
    }

    // leaves a critical section. 

    void ThreadContext::thunlock()
    {
	thread_mutex->release();
    }

    void ThreadContext::runDispatcher()
    {
        using namespace std;
        bool havetoquit = false;

        while (!havetoquit)
        {
            logPrintI("Waiting for action command ...\n");
            
            action_mutex->acquire();

            while (!action_cond_val)
            {
                action_cond->wait();            
            }
            action_cond_val = false;
            action_mutex->release();

            thlock();
            int locactiontype = action_type;
            thunlock();
            
            inloop = true;

            switch (locactiontype)
            {
                case POLL:
                    Context::run();
                    break;
                case RATE:
                    Context::runAtRate(action_rate);
                    break;
                case DEMAND:
                    Context::runOnDemand();
                    break;
                case STOP:
                    Context::stopLoop();
                    break;
                case QUIT:
                    havetoquit = true;
                    break;
                default:
                    logPrintW("kThreadContext::runDispatcher: Unknown action type %d\n", locactiontype);
                    break;
            }
            logPrintI("  at end of loop.\n");

            inloop = false;

            loopend_mutex->acquire();
            loopend_cond_val = true;
            loopend_cond->signal();
            loopend_mutex->release();
        }

        logPrintI(" thread finished.\n");
        
    }

    void ThreadContext::run()
    {
        using namespace std;
        logPrintI("ThreadContext::run()\n");

        thlock();

        action_type = POLL;

        thunlock();

        action_mutex->acquire();
        action_cond_val = true;
        action_cond->signal();
        action_mutex->release();

    }

    void ThreadContext::runAtRate(double rate)
    {
        using namespace std;
        logPrintI("ThreadContext::runAtRate()\n");

        thlock();

        action_type = RATE;
        action_rate = rate;

        thunlock();

        action_mutex->acquire();
        action_cond_val = true;
        action_cond->signal();
        action_mutex->release();

    }

    void ThreadContext::runOnDemand()
    {
        using namespace std;
        logPrintI("ThreadContext::runOnDemand()\n");
     
        thlock();

        action_type = DEMAND;

        thunlock();

        action_mutex->acquire();
        action_cond_val = true;
        action_cond->signal();
        action_mutex->release();
    }

    void ThreadContext::stopLoop()
    {
        using namespace std;
        
        logPrintI("ThreadContext::stopRun()\n");
     
        thlock();

        action_type = STOP;

        thunlock();

        action_mutex->acquire();
        action_cond_val = true;
        action_cond->signal();
        action_mutex->release();
    }
    
} // namespace ot

/* 
 * ------------------------------------------------------------
 *   End of ThreadContext.cxx
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
