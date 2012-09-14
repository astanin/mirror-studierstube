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
/** The header file for the SoTrackedViewpointControlMode class.
*
* @author Denis Kalkofen
*
* $Id: SoTrackedViewpointControlMode.cxx 25 2005-11-28 16:11:59Z denis $
* @file                                                                   */
/* ======================================================================= */

//#include "SoTrackedViewpointControlMode.h"
#include <stb/components/viewer/controlmode/SoTrackedViewpointControlMode.h>
#include <stb/components/viewer/SoOffAxisCamera.h>
#include <stb/components/viewer/SoDisplay.h>
#include <stb/kernel/Kernel.h>
#include <stb/kernel/ComponentManager.h>
#include <stb/components/event/event.h>
#include <stb/components/event/SoTrakEngine.h>


#include <Inventor/nodes/SoTransform.h>
#include <Inventor/engines/SoTransformVec3f.h> 

BEGIN_NAMESPACE_STB


SO_NODE_SOURCE(SoTrackedViewpointControlMode);

//----------------------------------------------------------------------------
// Inventor class initialization.

void
SoTrackedViewpointControlMode::initClass()
{
    SO_NODE_INIT_CLASS(SoTrackedViewpointControlMode, SoStbCameraControlMode, "SoStbCameraControlMode");
}
//----------------------------------------------------------------------------
// The constructor creates and stores a reference to a new SoTrakEngine.

SoTrackedViewpointControlMode::SoTrackedViewpointControlMode()
{
    SO_NODE_CONSTRUCTOR(SoTrackedViewpointControlMode);

    SO_NODE_ADD_FIELD(viewpointTrackerKey, ("")); 
    SO_NODE_ADD_FIELD(viewpointTrackerValue, ("")); 

    SO_NODE_ADD_FIELD(eyeOffset, (0.0, 0.0, 0.0));
 
}

//----------------------------------------------------------------------------
// The destructor unrefs the internal SoTrakEngine.

SoTrackedViewpointControlMode::~SoTrackedViewpointControlMode()
{
   
}

bool 
SoTrackedViewpointControlMode::activate()
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
		logPrintE("SoTrackedDisplayControlMode could not get a SoTrackEngine\n");
        return false;
    }
    tre->key.set1Value(0,viewpointTrackerKey.getValue());
    tre->value.set1Value(0,viewpointTrackerValue.getValue());

	////set up connection
	connectHeadTracker(tre);

	return true;
}

//----------------------------------------------------------------------------
void
SoTrackedViewpointControlMode::disconnectHeadTracker()
{
	((SoOffAxisCamera*)stbCamera->getCamera())->eyepointPosition.disconnect();
}

//----------------------------------------------------------------------------
void 
SoTrackedViewpointControlMode::connectHeadTracker(SoTrakEngine *tracker)
{
    disconnectHeadTracker();

    // use engine to create tracker to world transformation matrix
    SoComposeMatrix *ctw = new SoComposeMatrix;
    ctw->translation.connectFrom(&tracker->translation);
    ctw->rotation.connectFrom(&tracker->rotation);

    connectHeadTrackerStep2(ctw);
}


//----------------------------------------------------------------------------
void
SoTrackedViewpointControlMode::connectHeadTracker(SoSFVec3f *trackerTranslation,
						  SoSFRotation *trackerRotation)
{
	disconnectHeadTracker();

	// use engine to create tracker to world transformation matrix
	SoComposeMatrix *ctw = new SoComposeMatrix;
	ctw->translation.connectFrom(trackerTranslation);
	ctw->rotation.connectFrom(trackerRotation);
	
	//let the user decide which offset he wants to use
	//eyeOffset.setValue(((SoOffAxisCamera*)stbCamera->getCamera())->eyepointPosition.getValue());

	connectHeadTrackerStep2(ctw);
}


//----------------------------------------------------------------------------
void
SoTrackedViewpointControlMode::connectHeadTrackerStep2(SoComposeMatrix *ctw)
{
	// use engines to transform offsets
	SoTransformVec3f *te;
	te = new SoTransformVec3f;
	te->vector.connectFrom(&eyeOffset);
	te->matrix.connectFrom(&ctw->matrix);

	// connect to transformed offsets
        ((SoOffAxisCamera*)stbCamera->getCamera())->eyepointPosition.connectFrom(&te->point);
	//stbCamera->getTransform()->translation.connectFrom(&te->point);
}

END_NAMESPACE_STB
