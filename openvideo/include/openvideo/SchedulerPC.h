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
* PROJECT: OpenVideo  
* ======================================================================== */  
/** The header file for the SchedulerPC class.  
*  
* @author Denis Kalkofen  
*  
* $Id: SchedulerPC.h 148 2006-05-26 14:53:05Z daniel $  
* @file                                                                   */  
/* ======================================================================= */  

#ifndef _OV_SCHEDULERPC_H_
#define _OV_SCHEDULERPC_H_

#include <openvideo/SchedulerBase.h>
class  ACE_Condition_Thread_Mutex;
class ACE_Thread_Mutex;

namespace openvideo{
class TimerACE;
/**
*@ingroup core
*	
*/
class SchedulerPC : public SchedulerBase
{
public:
    /**
    *     The Constructor	
    */
    SchedulerPC();

    /**
    *     The destructor.
    */
    virtual ~SchedulerPC();

protected:	
    virtual void scheduleIdle();
    virtual void scheduleTimer();
    virtual void schedulePoll();
    virtual void unscheduleIdle();
    virtual void unscheduleTimer();
    virtual void unschedulePoll();
    bool isStarted;
    TimerACE *timer;
    bool exitIdle;

    /**
    *	Mutex used in updateLockCond .
    */
    ACE_Thread_Mutex *updateLock;

    /** 
    *       A condition variable which waits for an update to updateLock.
    */
    ACE_Condition_Thread_Mutex *updateLockCond;
private:
	
};// class 
}//namespace openvideo 
#endif//_SCHEDULERPC_H_
//========================================================================
// End of SchedulerPC.h 
//========================================================================
// Local Variables:
// mode: c++
// c-basic-offset: 4
// eval: (c-set-offset 'substatement-open 0)
// eval: (c-set-offset 'case-label '+)
// eval: (c-set-offset 'statement 'c-lineup-runin-statements)
// eval: (setq indent-tabs-mode nil)
// End:
//========================================================================
