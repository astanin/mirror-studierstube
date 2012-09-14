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
/** The header file for the ComponentThread class.  
*  
* @author Denis Kalkofen  
*  
* $Id: ComponentThread.h 25 2005-11-28 16:11:59Z denis $  
* @file                                                                   */  
/* ======================================================================= */  


#ifndef _COMPONENTTHREAD_H_
#define _COMPONENTTHREAD_H_

#include <stb/kernel/Component.h>
#include <stb/base/macros.h>


class ACE_Thread_Mutex;


BEGIN_NAMESPACE_STB


/**@ingroup kernel
*
* 
*/
class KERNEL_API ComponentThread : public Component
{
public:
	/**
	*     The destructor.
	*/
	virtual ~ComponentThread();

    /**
    * This method is called after initialisation is finished and before the
    * main loop is started. In this implementation it starts the thread. Be
    * sure to call this method from your subclasses start method to start the
    * thread !*/
    virtual void start();    

    /**
    * Closes the module. In this implementation it stops the thread. Be sure
    * to call this method from your subclasses close method to stop the
    * thread !*/
    virtual void close();     

    /************************************************************************/
    /*                                                                      */
    /************************************************************************/
    static const char* getBaseTypeID(){return "ComponentThread";}

protected:	
    /**
    *     The Constructor	
    */
    ComponentThread();

    /// Handle to module specific thread. This is a little bit tricky as we
    /// don't use the ACE definition but the same type.
    void * threadHandle;

    /// Mutex to implement lock, unlock behavior
    ACE_Thread_Mutex * mutex;

    /** Enters a critical section. Use this method to protect your operations
    * from another thread. This is not a recursive lock, do not call it
    * several times without unlocking !*/
    void lock();
    /** Leaves a critical section. Use this method to release the protection.
    */
    void unlock();

    /** The work method for the module thread. This is executed by the new
    * module thread. In this class it does nothing but subclasses should
    * override it to add their implementation. */
    virtual void run(){};    

    /** Static thread function passed to the actual thread. This calls
    * then run on the right instance. Do not use this yourself. */
    static void thread_func( void * data )
    {
        ((ComponentThread *)data)->run();
    };

};// class 


END_NAMESPACE_STB



#endif//_ComponentTHREAD_H_
