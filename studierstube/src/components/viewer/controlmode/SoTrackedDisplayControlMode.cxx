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
/** The header file for the SoTrackedDisplayControlMode class.
*
* @author Denis Kalkofen
*
* $Id: SoTrackedDisplayControlMode.cxx 25 2005-11-28 16:11:59Z denis $
* @file                                                                   */
/* ======================================================================= */

#include <stb/components/viewer/controlmode/SoTrackedDisplayControlMode.h>

#include <stb/kernel/Kernel.h>
#include <stb/kernel/ComponentManager.h>
#include <stb/components/event/event.h>
#include <stb/components/event/SoTrakEngine.h>

BEGIN_NAMESPACE_STB


SO_NODE_SOURCE(SoTrackedDisplayControlMode);

//----------------------------------------------------------------------------
// Inventor class initialization.

void
SoTrackedDisplayControlMode::initClass()
{
    SO_NODE_INIT_CLASS(SoTrackedDisplayControlMode, SoStbCameraControlMode, "SoStbCameraControlMode");
}
//----------------------------------------------------------------------------
// The constructor creates and stores a reference to a new SoTrakEngine.

SoTrackedDisplayControlMode::SoTrackedDisplayControlMode()
{
    SO_NODE_CONSTRUCTOR(SoTrackedDisplayControlMode);

	SO_NODE_ADD_FIELD(displayTrackerKey, (""));
    SO_NODE_ADD_FIELD(displayTrackerValue, (""));
	SO_NODE_ADD_FIELD(locked, (false));
}

//----------------------------------------------------------------------------
// The destructor unrefs the internal SoTrakEngine. 

SoTrackedDisplayControlMode::~SoTrackedDisplayControlMode()
{
	//nil
}

bool 
SoTrackedDisplayControlMode::activate()
{
    if(stbCamera==NULL)
	return false;
    
    Event* event=(Event*)(Kernel::getInstance()->getComponentManager()->load("Event"));
    if(!event)
    {
		logPrintE("failed to load event system\n");
        return false;
    }

    tre=event->createSoTrakEngine();
    if(!tre)
    {
		logPrintE("SoTrackedDisplayControlMode could not get an SoTrackEngine\n");
        return false;
    }
    tre->key.set1Value(0,displayTrackerKey.getValue());
    tre->value.set1Value(0,displayTrackerValue.getValue());

    stbCamera->getTransform()->translation.connectFrom(&tre->translation);
    stbCamera->getTransform()->rotation.connectFrom(&tre->rotation);

    return true;
}
void SoTrackedDisplayControlMode::GLRender(SoGLRenderAction *action)
{
    if(locked.getValue()&&tre!=NULL)
    {
        tre->locked.setValue(true);
    }else{
        tre->locked.setValue(false);
    }
}

END_NAMESPACE_STB
