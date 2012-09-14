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
/** The header file for the SchedulerBase class.  
 *  
 * @author Denis Kalkofen  
 *  
 * $Id: SchedulerBase.h 25 2005-11-28 16:11:59Z denis $  
 * @file                                                                   */  
/* ======================================================================= */  

#ifndef _OV_SCHEDULER_H_
#define _OV_SCHEDULER_H_

#include <stb/base/macros.h>
#include <stb/base/OS.h>
#include <stb/kernel/Studierstube.h>

class SoIdleSensor;
class SoTimerSensor;
class TiXmlAttribute;

BEGIN_NAMESPACE_STB


class GUIBinder;


/**@ingroup kernel
* The Scheduler's main loop is based on coins main loop implementation which comes with the gui-binding.
*/
class KERNEL_API Scheduler
{
public:
    /**
     *     The Constructor	
     */
    Scheduler();

    /**
     *     The Destructor calls unschedule().
     */
    virtual ~Scheduler();


	/// Initializes and runs the GUI binding
	virtual void run(GUIBinder* guiBinder);

    /// Stops the GUI binding
    virtual void stop(GUIBinder* guiBinder);

	 virtual void readConfiguration(TiXmlAttribute* attribute);


protected:	
    /**
     * Set up the sensor which will be used to update the kernel.
     * schedule() calls either scheduleIdleSensor or scheduleTimerSensor()
     */
    virtual void schedule();

    /**
     * remove the sensor which is used to update the kernel.
     */
    virtual void unschedule();
    
    /**
     * The different schedule mods. 
     */
    enum MODE {
        IDLE=0,
        TIMER=1,
		MANUAL=2
    };

    /************************************************************************
     * Set up and start the idle sensor 
     ************************************************************************/
    virtual void scheduleIdleSensor();

    /************************************************************************
     * Set up and start the timer sensor 
     ************************************************************************/
    virtual void scheduleTimerSensor();


	virtual void scheduleManualSensor();

	virtual void unscheduleManualSensor();

    /************************************************************************
     * The idle sensor
     ************************************************************************/
    SoIdleSensor *idle;

    /************************************************************************
     * The timer sensor
     ************************************************************************/
    SoTimerSensor *timer;

    /************************************************************************
     * Flag to indicate whether the scheduler is started already or not
     ************************************************************************/
    bool scheduled;
    
    /************************************************************************
     * Holds the configured schedule mode (currently either timer or idle)
     ************************************************************************/
    MODE mode;

    /************************************************************************
     * The update rate for the timer sensor
     ************************************************************************/
    float updateRate;

    /************************************************************************
     * The library handle to the gui binding. hModule is defined in OS.h and implemented accordingly to the used operating system.
     ************************************************************************/
    hModule libHandle;
    
private:
	GUIBinder* guiBinder;

};// class


END_NAMESPACE_STB


#endif//_OV_SCHEDULER_H_


//========================================================================
// End of SchedulerBase.h 
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
