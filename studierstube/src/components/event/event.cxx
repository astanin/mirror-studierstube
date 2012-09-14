/* ======================================================================== 
 * Copyright (C) 2006  Graz University of Technology
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
 * For further information please contact 
 * Dieter Schmalstieg
 * <schmalstieg@icg.tu-graz.ac.at>
 * Graz University of Technology, 
 * Institute for Computer Graphics and Vision,
 * Inffeldgasse 16a, 8010 Graz, Austria.
 * ========================================================================
 * PROJECT: Studierstube
 * ======================================================================== */
/** 
  * 
  * @author Denis Kalkofen
  *
  * $Id: event.cpp 44 2005-07-18 15:03:44Z bara $
  * @file                                                                   */
 /* ======================================================================= */


#include <iostream>
#include <stb/components/event/event.h>
#include <stb/components/event/SoEventAction.h>
#include <stb/components/event/SoEventGroup.h>
#include <stb/components/event/SoEventKit.h>
#include <stb/components/event/SoEventNode.h>
#include <stb/components/event/SoEventRoot.h>
#include <stb/components/event/SoEventSeparator.h>
#include <stb/components/event/SoEventSource.h>
#include <stb/components/event/SoInputEvent.h>
#include <stb/components/event/SoOpenTrackerSource.h>
#include <stb/components/event/SoTrackedArtifactKit.h>
#include <stb/components/event/SoTrakEngine.h>
#include <stb/components/event/SoMultimodalEngine.h>
#include <stb/components/event/SoColorBlobTrakEngine.h>
#include <stb/components/event/EventBus.h>
#include <stb/kernel/ComponentManager.h>
//#include <stb/components/starlight/starlight.h>
#include <stb/kernel/Kernel.h>
#include <stb/kernel/ComponentManager.h>
#include <stb/kernel/SceneManager.h>
#include <stb/kernel/Profiler.h>

#ifdef HAVE_OPENVIDEO
#  include <openvideo/State.h>
#endif


#ifndef USE_OT_2_0
#include <OpenTracker.h>
#include <core/Context.h>
#else
#include <OpenTracker/OpenTracker.h>
#include <OpenTracker/core/Context.h>
#endif

// check if we actually have the correct OpenTracker version the users wants...
//
#ifdef USE_OT_1_1
#  if defined(OPENTRACKER_VERSION_MAJOR) && defined(OPENTRACKER_VERSION_MINOR)
#    if (OPENTRACKER_VERSION_MAJOR==1) && (OPENTRACKER_VERSION_MINOR==1)
#      pragma message(">>> Building against OpenTracker version 1.1 - version check OK")
#    else
#      pragma message ("ERROR: Wrong OpenTracker version. This is not OpenTracker 1.1")
#      error ("ERROR: Wrong OpenTracker version. This is not OpenTracker 1.1")
#    endif
#  else
#    pragma message ("ERROR: Should build against OpenTracker 1.1, but found unknown OpenTracker version. Please update to latest OpenTracker version")
#    error ("ERROR: Should build against OpenTracker 1.1, but found unknown OpenTracker version. Please update to latest OpenTracker version")
#  endif	
#else
#  if defined(OPENTRACKER_VERSION_MAJOR) && defined(OPENTRACKER_VERSION_MINOR)
#    if (OPENTRACKER_VERSION_MAJOR==1) && (OPENTRACKER_VERSION_MINOR==3)
#      ifdef WIN32
#        pragma message(">>> Building against OpenTracker version 1.3 - version check OK")
#      endif
#    elif (OPENTRACKER_VERSION_MAJOR==2) && (OPENTRACKER_VERSION_MINOR==0)
#      ifdef WIN32
#        pragma message(">>> Building against OpenTracker version 2.0 - version check OK")
#      endif
#    else
#      ifdef WIN32
#        pragma message ("ERROR: Wrong OpenTracker version. This is not OpenTracker 1.3")
#      endif
#      error ("ERROR: Wrong OpenTracker version. This is not OpenTracker 1.3")
#    endif
#  else
#    pragma message ("ERROR: Should build against OpenTracker 1.3, but found unknown OpenTracker version. Please update to latest OpenTracker version")
//#    error ("ERROR: Should build against OpenTracker 1.3, but found unknown OpenTracker version. Please update to latest OpenTracker version")
#  endif
#endif


#ifdef WIN32
#ifdef HAVE_OPENVIDEO
#  pragma message(">>> Creating event system with video input support")
#else
#  pragma message(">>> Creating event system without video input support")
#endif
#endif

//class Starlight;

CREATE_COMPONENT_FUNC(Event)

BEGIN_NAMESPACE_STB

Event* Event::instance=NULL;

Event::Event()
{
    configFile="";
    sinkName="";
	otSource = NULL;
	vu_updateCtr = 0;
    paused=false;
    processing=1; // Initialized to TIME
}

Event::~Event()
{
    //nil
}


/// Called before the application is destructed.
bool 
Event::init()
{   
	Event::instance=this;
	logPrintS("Event Initialized\n");
    if(isInit)
        return isInit;
    isInit=true;
    //get viewer's parameter
    retrieveParameter();

//#ifndef STB_IS_WINCE
//	// FIXME: do we really need to load this? Daniel 20060605
//    Starlight* starlight=(Starlight*)(stb::Kernel::getInstance()->getComponentManager()->load("Starlight"));
//    if(!starlight)
//    {
//		// FIXME: Error Message should be displayed here. Mendez 20060315
//        return false;
//    }
//#endif
    SoColorBlobTrakEngine::initClass();
	SoMultimodalEngine::initClass();
    SoTrakEngine::initClass();
    SoTrackedArtifactKit::initClass();
    SoInputEvent::initClass();
    SoEventSource::initClass();
    SoOpenTrackerSource::initClass();
    SoEventGroup::initClass();
    SoEventKit::initClass();
    SoEventNode::initClass();
    SoEventSeparator::initClass();
    SoEventAction::initClass();
    SoEventRoot::initClass();


    otSource=new SoOpenTrackerSource;
    otSource->ref();
    otSource->configuration.setValue(stb::Kernel::getInstance()->getConfig(configFile).c_str());

    if (processing==0)
    {
        otSource->processing=SoOpenTrackerSource::IDLE;
    }
    else if (processing==1)
    {
        otSource->processing=SoOpenTrackerSource::TIME;
        otSource->interval=SbTime(0.01f);
    }
    else if (processing==2)
    {
        otSource->processing=SoOpenTrackerSource::POLL;
    }

    stb::Kernel::getInstance()->getSceneManager()->setTrackerSource(otSource);


    key=new SoMFName;
    value=new SoMFString;

    return isInit;
}

void 
Event::setParameter(stb::string key, std::string value)
{
    if(key=="configFile")
    {
        configFile=value;
    }
    else if((key.find("sinkName",0)!=string::npos)|(key.find("ovSinkName",0)!=string::npos))
    {// Deprecating the usage of ovSinkName in favor of sinkName. Mendez 20070614
        sinkName=value;
    }
    else if(key=="processing")
    {
        if ((value=="IDLE")||(value=="idle"))
        {
            processing=0;
        }
        else if ((value=="TIME")||(value=="time"))
        {
            processing=1;
        }
        else if ((value=="POLL")||(value=="poll"))
        {
            processing=2;
        }
    }


    //else if()
    //{
    //}
}

/// Called before the application is destructed.
void 
Event::shutDown()
{
    otSource->shutDown();
}

SoTrakEngine*
Event::createSoTrakEngine()
{
    return new SoTrakEngine();
}

SoMultimodalEngine*
Event::createSoMultimodalEngine()
{
	return new SoMultimodalEngine();
}

SoColorBlobTrakEngine*
Event::createSoColorBlobTrakEngine()
{
    return new SoColorBlobTrakEngine();
}

SoTrackedArtifactKit* 
Event::createSoTrackedArtifactKit()
{
    return new SoTrackedArtifactKit();
}


SoOpenTrackerSource* 
Event::createSoOpenTrackerSource()
{
    return new SoOpenTrackerSource();
}

SoEventGroup* 
Event::createSoEventGroup()
{
    return new SoEventGroup();
}

SoEventKit* 
Event::createSoEventKit()
{
    return new SoEventKit();
}

SoEventNode* 
Event::createSoEventNode()
{
    return new SoEventNode();
}

SoEventSeparator* 
Event::createSoEventSeparator()
{
    return new SoEventSeparator();
}

SoEventSource* 
Event::createSoEventSource()
{
    return new SoEventSource();
}

SoInputEvent* 
Event::createSoInputEvent()
{
    return new SoInputEvent();
}

SoEventRoot* 
Event::createSoEventRoot()
{
    return new SoEventRoot();
}

SoEventAction* 
Event::createSoEventAction()
{
    return new SoEventAction();
}

EventBus& 
Event::getEventBus()
{
    return EventBus::getSingleton();
}


void
Event::vu_init(const openvideo::Buffer& frame, stb::string *givenSinkName)
{
}

void
Event::vu_update(const openvideo::Buffer& frame, stb::string *givenSinkName, bool forceUpdate)
{
#ifdef HAVE_OPENVIDEO

    if (!isInit) return;

    // If this is not our sink then return
    if ((!sinkName.empty())&(sinkName.find(givenSinkName->c_str(),0)==string::npos)) return;

	assert(otSource);

	STB_PROFILER_AUTOMEASURE(OT_VIDEO_FEED)

	if(ot::Context* context = otSource->getContext())
	{
		// only update if this is a fresh video image!
		//
		if(frame.getUpdateCounter() != vu_updateCtr)
		{
			// OpenTracker's pixel-format was carefully chosen to be compatible to OpenVideo's.
			// Since the Stb4 pixel-format is compatible to OpenVideo's pixel-format too, we can
			// simple cast here...
			//
			context->newVideoFrame(frame.getPixels(), frame.getWidth(), frame.getHeight(), static_cast<ot::PIXEL_FORMAT>(frame.getFormat()));
			vu_updateCtr = frame.getUpdateCounter();
		}
	}

#endif
}



END_NAMESPACE_STB

//========================================================================
// End of Event.cxx
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

