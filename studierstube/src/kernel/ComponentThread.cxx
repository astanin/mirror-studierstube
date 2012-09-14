/* ========================================================================
* Copyright (C) 2005  Graz University of Technology
*
* This framework is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This framework is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this framework; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*
* For further information please contact Dieter Schmalstieg under
* <schmalstieg@icg.tu-graz.ac.at> or write to Dieter Schmalstieg,
* Graz University of Technology, Inffeldgasse 16a, A8010 Graz,
* Austria.
* ========================================================================
* PROJECT: Studierstube
* ======================================================================== */
/* @author Denis Kalkofen
*
* $Id: ComponentThread.cxx 25 2005-11-28 16:11:59Z denis $
* @file                                                                   */
/* ======================================================================= */


#include <stb/base/fixWinCEIssues.h>
#include <stb/kernel/ComponentThread.h>
#include <stb/kernel/Kernel.h>
#include <stb/base/fixWinXPIssues.h>
#include <stb/base/OS.h>
#include <ace/Thread.h>
#include <ace/Synch.h>

BEGIN_NAMESPACE_STB

ComponentThread::ComponentThread()
{
   mutex = new ACE_Thread_Mutex;
}

ComponentThread::~ComponentThread()
{
   delete mutex;
}

void 
ComponentThread::lock()
{
    mutex->acquire();
}


void 
ComponentThread::unlock()
{
    mutex->release();
}

void 
ComponentThread::start()
{
    threadHandle = new ACE_hthread_t();
    if(ACE_Thread::spawn((ACE_THR_FUNC)thread_func,
        this, 	
        THR_NEW_LWP|THR_JOINABLE, 	
        0, 	
        (ACE_hthread_t*)threadHandle,
        0, 	
        0, 	
        0
        )==-1)
    { 
        logPrintW("Spawning thread failed.\n"); 
    }

}    

    // stops the thread and closes the module

void 
ComponentThread::close()
{
#ifdef STB_IS_WINDOWS
    ACE_Thread::join( (ACE_hthread_t*)threadHandle );
#endif

#ifdef STB_IS_LINUX
    ACE_Thread::join( (ACE_hthread_t)threadHandle );
#endif

    // ACE_Thread::cancel( *(ACE_thread_t *)thread );
    delete ((ACE_hthread_t *)threadHandle);
}

END_NAMESPACE_STB
