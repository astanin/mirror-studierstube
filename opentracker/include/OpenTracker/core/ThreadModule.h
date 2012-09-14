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
/** header file for ThreadModule class.
 *
 * @author Gerhard Reitmayr
 *
 * $Id: ThreadModule.h 2049 2007-09-19 05:59:38Z bornik $
 * @file                                                                   */
/* ======================================================================= */

#ifndef _THREADMODULE_H
#define _THREADMODULE_H

#include <OpenTracker/dllinclude.h>

class ACE_Thread_Mutex;

#include "Module.h"

/**
 * ThreadModule is an abstract superclass for modules that require a 
 * multithreaded implementation. It provides a thread method that should be
 * overriden by subclasses to add their own thread implementation and
 * provides lock and unlock methods to synchronize access to shared resources
 * between the main thread and the module thread. More complicated 
 * concurrent access control has to be implemented by the subclasses.
 *
 * This class provides empty implementations for all its methods instead of
 * pure virtual functions, so that classes inheriting from it don't have to
 * implement all methods in a trivial way.
 * @author Gerhard Reitmayr
 * @ingroup core
 */

namespace ot {

    class OPENTRACKER_API ThreadModule : public Module
    {
        //members
    protected:
        /// handle to module specific thread. This is a little bit tricky as we
        /// don't use the ACE definition but the same type.
        void * threadID;
	void * threadHandle;

        /// Mutex to implement lock, unlock behavior
        ACE_Thread_Mutex * mutex;
        
        //methods
    protected:    
        /** enters a critical section. Use this method to protect your operations
         * from another thread. This is not a recursive lock, do not call it
         * several times without unlocking !*/
        void lockLoop();
        /** leaves a critical section. Use this method to release the protection.
         */
        void unlockLoop();
        /** the work method for the module thread. This is executed by the new
         * module thread. In this class it does nothing but subclasses should
         * override it to add their implementation. */
        virtual void run()
        {};    
        /** static thread function passed to the actual thread. This calls
         * then run on the right instance. Do not use this yourself. */
        static void thread_func( void * data )
        {
            ((ThreadModule *)data)->run();
        };

    public:
        /** constructor */
	ThreadModule();
        /** destructor */
        virtual ~ThreadModule();

        /**
         * This method is called after initialisation is finished and before the
         * main loop is started. In this implementation it starts the thread. Be
         * sure to call this method from your subclasses start method to start the
         * thread !*/
        virtual void start();    
        /**
         * closes the module. In this implementation it stops the thread. Be sure
         * to call this method from your subclasses close method to stop the
         * thread !*/
        virtual void close();            
    };

} // namespace ot

#endif

/* 
 * ------------------------------------------------------------
 *   End of ThreadModule.h
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
