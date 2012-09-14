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
  * $Id: event.h 44 2005-07-18 15:03:44Z bara $
  * @file                                                                   */
 /* ======================================================================= */

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the EVENT_DLL_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// EVENT_DLL_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.


#ifndef _EVENT_H_
#define _EVENT_H_


#if defined(WIN32) || defined(_WIN32_WCE)
#  pragma warning(disable:4251)
#  pragma warning(disable:4290)
#  ifdef EVENT_EXPORTS
#    define EVENT_API __declspec(dllexport)
#  else
#    define EVENT_API __declspec(dllimport)
#  endif
#else
#  define EVENT_API
#endif


#include <stb/base/macros.h>
#include <stb/kernel/Component.h>
#include <stb/kernel/VideoUser.h>
#include <Inventor/fields/SoMFName.h>
#include <Inventor/fields/SoMFString.h>

/**
*	
*/
class SoTrakEngine;
class SoMultimodalEngine;
class SoColorBlobTrakEngine;
class SoTrackedArtifactKit;
class SoTrakEngine;
class SoOpenTrackerSource;
class SoEventGroup;
class SoEventKit;
class SoEventNode;
class SoEventSeparator;
class SoEventSource;
class SoInputEvent;
class SoEventRoot;
class SoEventAction;
class EventBus;


BEGIN_NAMESPACE_STB


class Event : public stb::Component, public stb::VideoUser
{
public:
    /**
    *     The Constructor	
    */
    Event();
    /**
    *     The destructor.
    */
    virtual ~Event();

    /// Called before the application is destructed.
    virtual bool init();
    /// Called before the application is destructed.
    virtual void shutDown();
    //
    virtual void setParameter(stb::string key, std::string value);

    virtual SoTrakEngine*           createSoTrakEngine();
	virtual SoMultimodalEngine*		createSoMultimodalEngine();
    virtual SoColorBlobTrakEngine*  createSoColorBlobTrakEngine();
    virtual SoTrackedArtifactKit*   createSoTrackedArtifactKit();
    virtual SoOpenTrackerSource*    createSoOpenTrackerSource();
    virtual SoEventGroup*           createSoEventGroup();
    virtual SoEventKit*             createSoEventKit();
    virtual SoEventNode*            createSoEventNode();
    virtual SoEventSeparator*       createSoEventSeparator();
    virtual SoEventSource*          createSoEventSource();
    virtual SoInputEvent*           createSoInputEvent();
    virtual SoEventRoot*            createSoEventRoot();
    virtual SoEventAction*          createSoEventAction();

    virtual EventBus&               getEventBus();


	// Declare that this component is a video user
	virtual VideoUser* getVideoUserInterface()   {  return this;  }


	/// Implement the stb::VideoUser interface
    virtual void vu_init(const openvideo::Buffer& frame, stb::string *givenSinkName);
    virtual void vu_update(const openvideo::Buffer& frame, stb::string *givenSinkName, bool forceUpdate = false);
	virtual VideoUser::UPDATE_MODE vu_getUpdateMode() const  {  return VideoUser::UPDATE_BEFORE_RENDER;  }
    SoOpenTrackerSource *getOpentrackerSource() { return otSource; };

    /// The following four functions enable tracking freezing
    void pause() { paused=true; };
    void resume() { paused=false; };
    void togglePause(){ paused=!paused; };
    bool isPaused() { return paused; };
    void setKey(SoMFName *newKey) { key=newKey; };
    void setValue(SoMFString *newValue) { value=newValue; };
    SoMFName * getKey() { return key; };
    SoMFString * getValue() { return value; };

	static Event* instance;
protected:	
    stb::string configFile; 
    stb::string sinkName;
    int processing;
private:
	SoOpenTrackerSource*	otSource;
	unsigned int			vu_updateCtr;
    bool paused;
    SoMFName *key;
    SoMFString *value;

	
};// class 


END_NAMESPACE_STB


#endif//_EVENT_H_


//========================================================================
// End of Event.h 
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
