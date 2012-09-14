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
/** The header file for the SchedulerBase class.
*
* @author Denis Kalkofen
*
* $Id: SchedulerBase.cxx 215 2006-10-13 10:06:28Z veas $
* @file                                                                   */
/* ======================================================================= */

#include <openvideo/SchedulerBase.h>
#include <TinyXML/tinyxml.h>

using namespace openvideo;


SchedulerBase::SchedulerBase()
{
   mode=POLL;
   updateRate=30;
}

SchedulerBase::~SchedulerBase()
{
   //nil
}

void 
SchedulerBase::parseConfiguration(TiXmlElement* element)
{
    const char		     *_mode = element->Attribute("scheduleMode"),
                   *_updateRate = element->Attribute("updateRate");

    if(_mode)
    {
        if(!strcmp(_mode,("idle"))){
            mode=IDLE;		
        }
        else if(!strcmp(_mode,"timer"))
            mode=TIMER;		
        else if(!strcmp(_mode,"poll"))
            mode=POLL;
    }
    if(_updateRate)
    {
        updateRate=(float)atof(_updateRate);
    }
}

void 
SchedulerBase::init()
{
    //nil
}

void
SchedulerBase::run()
{
    switch(mode)
    {   
    case IDLE:
        scheduleIdle();
        break;
    case TIMER:
        scheduleTimer();
        break;
    case POLL:
        schedulePoll();
        break;
    }
}

void
SchedulerBase::stop()
{
    switch(mode)
    {   
    case IDLE:
        unscheduleIdle();
        break;
    case TIMER:
        unscheduleTimer();
        break;
    case POLL:
        unschedulePoll();
        break;
    }
}

//========================================================================
// End of SchedulerBase.cxx
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
