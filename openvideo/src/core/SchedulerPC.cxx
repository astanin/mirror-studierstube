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
* $Id: SchedulerPC.cxx 223 2006-11-06 14:18:27Z mendez $
* @file                                                                   */
/* ======================================================================= */

#include <stdlib.h>
#include <openvideo/SchedulerPC.h>
#include <openvideo/TimerACE.h>
#include <openvideo/Manager.h>
#include <ace/Thread_Mutex.h>
#include <ace/Condition_Thread_Mutex.h>

using namespace openvideo;


SchedulerPC::SchedulerPC()
{
   exitIdle=false;
}

SchedulerPC::~SchedulerPC()
{
  if(timer)
      delete timer;
  if(updateLock)
      delete updateLock;
  if(updateLockCond)
      delete updateLockCond;
}

void 
SchedulerPC::scheduleIdle()
{
    while(!exitIdle)
    {
        Manager::update(NULL);
    }
}
void 
SchedulerPC::scheduleTimer()
{
    timer=new TimerACE();
    timer->schedule(Manager::update,
                    NULL,
                    double(1.0/(double)updateRate));

    logPrintS("Starting main loop\n");
    timer->runEventLoop();
}

void 
SchedulerPC::schedulePoll()
{
    updateLock=new ACE_Thread_Mutex();
    updateLockCond=new ACE_Condition_Thread_Mutex(*updateLock);
    updateLockCond->wait();
}

void 
SchedulerPC::unscheduleIdle()
{
    exitIdle=true;
}

void 
SchedulerPC::unscheduleTimer()
{
    if(timer)
        delete timer;
}

void 
SchedulerPC::unschedulePoll()
{
    updateLockCond->broadcast();
    if(updateLock)
        delete updateLock;
    if(updateLockCond)
        delete updateLockCond;
}



//========================================================================
// End of SchedulerPC.cxx
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
