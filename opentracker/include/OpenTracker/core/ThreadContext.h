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
/** header file for threaded Context class
 *
 * @author Alexander Bornik
 *
 * $Id$
 * @file                                                                   */
/* ======================================================================= */

#ifndef _THREADCONTEXT_H
#define _THREADCONTEXT_H

#ifndef SWIG
#include <ace/Condition_Thread_Mutex.h>
#include <ace/Thread_Mutex.h>
#include <ace/Synch.h>
#endif

#include "../dllinclude.h"

#include "Node.h"
#include "Module.h"
#include "NodeFactoryContainer.h"
#include "VideoUser.h"
#include "Context.h"


//class ACE_Thread_Mutex;
//class ACE_Condition_Thread_Mutex;
//class ACE_Condition<ACE_Thread_mutex>;


namespace ot {

    /**
     * This class represents a threaded verion of  the context class.
     
     * @author Gerhard Reitmayr
     * @ingroup core
     */

    class OPENTRACKER_API ThreadContext : public Context
    {
        typedef ACE_Thread_Mutex thread_mutex_type;
        typedef ACE_Mutex mutex_type;
        typedef ACE_Condition<ACE_Mutex > condition_type;
        
        // members
    protected:
        enum ActionType {
            POLL = 0,
            RATE = 1,
            DEMAND = 2,
            STOP = 3,
            QUIT = 255
        };

        int action_type;
        double action_rate;
        bool inloop;

        void * thread;
        
        thread_mutex_type * thread_mutex;
        mutex_type * action_mutex;
        condition_type * action_cond;
        bool action_cond_val;
        mutex_type * loopend_mutex;
        condition_type * loopend_cond;
        bool loopend_cond_val;

        
        // methods        
    public:

        /** a constructor method.
         * @param init If init is not equal to 0, it instantiates all known modules and
         *        factories, adds them to its local containers and also takes care of
         *        removing them again in the destructor.*/
        ThreadContext( int init = 0 );

        /** destructor method clears containers and removes any modules instantiated in
         * the default setup, if cleanUp is st. */
        virtual ~ThreadContext();

    protected:
        /** enters a critical section. Use this method to protect your operations
         * from another thread. This is not a recursive lock, do not call it
         * several times without unlocking !*/
        void thlock();
       /** leaves a critical section. Use this method to release the protection.
         */
        void thunlock();    
        /** This method implements the dispatcher, which runs one of the maint 
         *  loops in the thread. */
        void runDispatcher();

    public:
        /** This method implements the main loop and runs until it is stopped
         * somehow. Then it calls close() on all modules. */
        virtual void run();

        /** This method a main loop at a fixed rate until it is stopped
         * somehow. Then it calls close() on all modules. */
        virtual void runAtRate(double rate);

        /** This is a data-driven implementation of the main loop */
        virtual void runOnDemand();

        /** stop the main loop, not the thread */
        virtual void stopLoop();

    protected:
        static void thread_func( void * data )
        {
            ((ThreadContext*)data)->runDispatcher();
        };

        friend class ConfigurationParser;
    };

} // namespace ot

#endif

/*
 * ------------------------------------------------------------
 *   End of ThreadContext.h
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
