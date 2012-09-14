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
/** The header file for the Scheduler class.
*
* @author Denis Kalkofen
*
* $Id: Scheduler.cxx 25 2005-11-28 16:11:59Z denis $
* @file                                                                   */
/* ======================================================================= */


#include <stb/kernel/Scheduler.h>

#ifndef USE_TINYXMLMOD
#include <tinyxml/tinyxml.h>
#else
#include <TinyXML/tinyxml.h>
#endif
#include <Inventor/sensors/SoIdleSensor.h>
#include <Inventor/sensors/SoTimerSensor.h>
#include <stb/kernel/Kernel.h>
#include <stb/kernel/GUIBinder.h>


BEGIN_NAMESPACE_STB


Scheduler::Scheduler()
{
	mode=IDLE;
	updateRate=0.0;
	scheduled=false;
    idle=NULL;
    timer=NULL;
	guiBinder = NULL;
}


Scheduler::~Scheduler()
{
	unschedule();
}


void
Scheduler::run(GUIBinder* gui_binder)
{
	guiBinder = gui_binder;

	guiBinder->gb_init("Studierstube");
	schedule();
	guiBinder->gb_mainloop();
}

void
Scheduler::stop(GUIBinder* gui_binder)
{
    guiBinder = gui_binder;
    stb::logPrintI("Exiting GUI Binder\n");
    guiBinder->gb_exitMainLoop();
}

void
Scheduler::unschedule()
{
	if(!scheduled)
		return ;

	switch(mode)
	{
        case IDLE:
            idle->unschedule();
            delete idle;
	        break;
        case TIMER:
            timer->unschedule();
            delete timer;
	        break;
		case MANUAL:
			unscheduleManualSensor();
			break;
	}
	scheduled=false;
}


void 
Scheduler::schedule()
{
	if(scheduled)
		return ;

	switch(mode)
	{
	    case IDLE:
		    scheduleIdleSensor();
		    break;
	    case TIMER:
		    scheduleTimerSensor();
		    break;
		case MANUAL:
			scheduleManualSensor();
			break;
	}
	scheduled=true;
}


void 
Scheduler::readConfiguration(TiXmlAttribute* attribute)
{
	if(!stb::stricasecmp(attribute->Name(),"updateMode"))
	{
		if(!stb::stricasecmp(attribute->Value(),"idle"))
			mode=IDLE;
		else if(!stb::stricasecmp(attribute->Value(),"timer"))
			mode=TIMER;
		else if(!stb::stricasecmp(attribute->Value(),"manual"))
			mode=MANUAL;
	}
	else if(!stb::stricasecmp(attribute->Name(),"updateRate"))
	{
		updateRate=(float)atof(attribute->Value());
	}
}


void
Scheduler::scheduleIdleSensor()
{
    stb::logPrintD("Info: kernel->schedule idle \n");
	idle= new SoIdleSensor();
	idle->setFunction(Kernel::update);
	idle->schedule();
}


void
Scheduler::scheduleTimerSensor()
{
	stb::logPrintD("Info: kernel->schedule timer \n");
	timer=new SoTimerSensor();
	timer->setFunction(Kernel::update);
	timer->setInterval(updateRate);
	timer->schedule();
}


void
Scheduler::scheduleManualSensor()
{
	guiBinder->gb_registerManualCallback(Kernel::update);
}


void
Scheduler::unscheduleManualSensor()
{
	guiBinder->gb_registerManualCallback(NULL);

}


END_NAMESPACE_STB


//========================================================================
// End of Scheduler.cxx
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
