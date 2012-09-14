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
/** The Source file for the SoWindowHandling
*
* @author Erick Mendez
*
* $Id: SoFileEnv.cpp 2006-03-10 mendez $
* @file                                                                   */
/* ======================================================================= */

#include <stb/components/viewer/SoWindowHandling.h>
#include <stb/kernel/Kernel.h>
#include <stb/kernel/SceneManager.h>
#include <stb/kernel/ComponentManager.h>
#include <stb/components/video/Video.h>
#include <stb/components/event/event.h>
#include <stb/components/event/SoOpenTrackerSource.h>

using namespace std;

SO_KIT_SOURCE(SoWindowHandling);

SoWindowHandling::SoWindowHandling()
{
    SO_KIT_CONSTRUCTOR(SoWindowHandling);
    SO_KIT_ADD_FIELD(exit,     ());
    SO_KIT_ADD_FIELD(minimize, ());
    SO_KIT_ADD_FIELD(maximize, ());
    SO_KIT_ADD_FIELD(freezevideo, ());
    SO_KIT_ADD_FIELD(freezetracking, ());
    SO_KIT_ADD_FIELD(enableFreeze, (FALSE));
    SO_KIT_ADD_FIELD(wildcardKey, (""));
    SO_KIT_ADD_FIELD(wildcardValue, (""));

    SO_KIT_INIT_INSTANCE();

    exitSensor=new SoFieldSensor(SoWindowHandling::exitCB, this);
    minimizeSensor=new SoFieldSensor(SoWindowHandling::minimizeCB, this);
    maximizeSensor=new SoFieldSensor(SoWindowHandling::maximizeCB, this);
    freezevideoSensor=new SoFieldSensor(SoWindowHandling::freezevideoCB, this);
    freezetrackingSensor=new SoFieldSensor(SoWindowHandling::freezetrackingCB, this);
    wildcardKeySensor=new SoFieldSensor(SoWindowHandling::wildcardKeyCB, this);
    wildcardValueSensor=new SoFieldSensor(SoWindowHandling::wildcardValueCB, this);

	this->setUpConnections(TRUE, TRUE);
}

SoWindowHandling::~SoWindowHandling()
{

}

SbBool SoWindowHandling::setUpConnections(SbBool onoff, SbBool doitalways)
{
    if ((doitalways == FALSE) && (connectionsSetUp == onoff)) return onoff;
    if (onoff) 
    {
        // We connect AFTER base class.
        SoBaseKit::setUpConnections(onoff, doitalways);

        exitSensor->attach(&this->exit);
        minimizeSensor->attach(&this->minimize);
        maximizeSensor->attach(&this->maximize);
        freezevideoSensor->attach(&this->freezevideo);
        freezetrackingSensor->attach(&this->freezetracking);
        wildcardKeySensor->attach(&this->wildcardKey);
        wildcardValueSensor->attach(&this->wildcardValue);
    }
    else 
    {
        // We disconnect BEFORE base class.
        exitSensor->detach();
        minimizeSensor->detach();
        maximizeSensor->detach();
        freezevideoSensor->detach();
        freezetrackingSensor->detach();
        wildcardKeySensor->detach();
        wildcardValueSensor->detach();

        SoBaseKit::setUpConnections(onoff, doitalways);
    }
    return !(connectionsSetUp = onoff);
}

void SoWindowHandling::initClass(void)
{
    SO_KIT_INIT_CLASS(SoWindowHandling, SoBaseKit, "BaseKit");
}

void SoWindowHandling::exitCB(void *data, SoSensor *)
{
    SoWindowHandling *self= (SoWindowHandling *)data;
        //::exit(0);
    stb::Kernel::getInstance()->stop();
}

void SoWindowHandling::minimizeCB(void *data, SoSensor *)
{
    // minimize
    SoWindowHandling *self= (SoWindowHandling *)data;
}

void SoWindowHandling::maximizeCB(void *data, SoSensor *)
{
    // maximize
    SoWindowHandling *self= (SoWindowHandling *)data;
}

void SoWindowHandling::wildcardKeyCB(void *data, SoSensor *)
{
    // Set the wildcard event's key
    SoWindowHandling *self= (SoWindowHandling *)data;
    self->refreshPredicates();
}

void SoWindowHandling::wildcardValueCB(void *data, SoSensor *)
{
    // Set the wildcard event's key
    SoWindowHandling *self= (SoWindowHandling *)data;
    self->refreshPredicates();
}

void SoWindowHandling::freezevideoCB(void *data, SoSensor *)
{
    SoWindowHandling *self= (SoWindowHandling *)data;
    if (!self->enableFreeze.getValue()) return;
    // freeze the video
    stb::Video* video=(stb::Video*)(stb::Kernel::getInstance()->getComponentManager()->load("Video"));
    video->togglePause();
}

void SoWindowHandling::freezetrackingCB(void *data, SoSensor *)
{
    // freeze the tracking
    SoWindowHandling *self= (SoWindowHandling *)data;
    if (!self->enableFreeze.getValue()) return;

    stb::Event* eventInstance=(stb::Event*)(stb::Kernel::getInstance()->getComponentManager()->load("Event"));
    self->refreshPredicates();
    eventInstance->togglePause();
}

void SoWindowHandling::refreshPredicates()
{
    stb::Event* eventInstance=(stb::Event*)(stb::Kernel::getInstance()->getComponentManager()->load("Event"));
    eventInstance->setKey(&wildcardKey);
    eventInstance->setValue(&wildcardValue);
}
