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
/** The header file for the SoTrackedViewpointMobileDisplayControlMode class.
*
* @author Denis Kalkofen
*
* $Id: SoTrackedViewpointMobileDisplayControlMode.cxx 25 2005-11-28 16:11:59Z denis $
* @file                                                                   */
/* ======================================================================= */

#include <stb/components/viewer/controlmode/SoTrackedViewpointMobileDisplayControlMode.h>

#include <stb/components/viewer/controlmode/MultRotRot.h>
#include <stb/components/viewer/SoOffAxisCamera.h>
#include <stb/components/viewer/SoDisplay.h>
#include <stb/kernel/Kernel.h>
#include <stb/kernel/ComponentManager.h>
#include <stb/components/event/event.h>
#include <stb/components/event/SoTrakEngine.h>

#include <Inventor/nodes/SoTransform.h>
#include <Inventor/engines/SoTransformVec3f.h> 


BEGIN_NAMESPACE_STB



SO_NODE_SOURCE(SoTrackedViewpointMobileDisplayControlMode);


void
SoTrackedViewpointMobileDisplayControlMode::initClass()
{
    SO_NODE_INIT_CLASS(SoTrackedViewpointMobileDisplayControlMode, SoStbCameraControlMode, "SoStbCameraControlMode");
}

SoTrackedViewpointMobileDisplayControlMode::SoTrackedViewpointMobileDisplayControlMode()
{
    SO_NODE_CONSTRUCTOR(SoTrackedViewpointMobileDisplayControlMode);

    SO_NODE_ADD_FIELD(viewpointTrackerKey, ("")); 
	SO_NODE_ADD_FIELD(viewpointTrackerValue, ("")); 
    SO_NODE_ADD_FIELD(displayTrackerKey, ("")); 
    SO_NODE_ADD_FIELD(displayTrackerValue, ("")); 

	SO_NODE_ADD_FIELD(eyeOffset, (0.0, 0.0, 0.0));
	SO_NODE_ADD_FIELD(displayOffset, (0.0, 0.0, 0.0)); 
	SO_NODE_ADD_FIELD(displayRotationOffset, (SbVec3f( 0.f, 0.f, 1.f ), 0.f )); 


}


SoTrackedViewpointMobileDisplayControlMode::~SoTrackedViewpointMobileDisplayControlMode()
{
	//nil
}


bool 
SoTrackedViewpointMobileDisplayControlMode::activate()
{
	if(stbCamera==NULL)
		return false;


    Event* event=(Event*)(Kernel::getInstance()->getComponentManager()->load("Event"));
    if(!event)
    {
		logPrintE("failed to load event system\n");
        return false;
    }

    trHead=event->createSoTrakEngine();
    trDisplay=event->createSoTrakEngine();
    if(!trHead || !trDisplay)
    {
		logPrintE("SoTrackedDisplayControlMode could not get an SoTrakEngine\n");
        return false;
    }
    trHead->key.set1Value(0,viewpointTrackerKey.getValue());
    trHead->value.set1Value(0,viewpointTrackerValue.getValue());

    trDisplay->key.set1Value(0,displayTrackerKey.getValue());;
    trDisplay->value.set1Value(0,displayTrackerValue.getValue());;

    connectHeadTracker(trHead);
    connectDisplayTracker(trDisplay);
	return true;
}

//----------------------------------------------------------------------------
void
SoTrackedViewpointMobileDisplayControlMode::disconnectHeadTracker()
{
	((SoOffAxisCamera*)stbCamera->getCamera())->eyepointPosition.disconnect();
}

void 
SoTrackedViewpointMobileDisplayControlMode::connectHeadTracker(SoTrakEngine* tracker)
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
SoTrackedViewpointMobileDisplayControlMode::connectHeadTracker(SoSFVec3f *trackerTranslation,
										 SoSFRotation *trackerRotation)
{
	disconnectHeadTracker();

	// use engine to create tracker to world transformation matrix
	SoComposeMatrix *ctw = new SoComposeMatrix;
	ctw->translation.connectFrom(trackerTranslation);
	ctw->rotation.connectFrom(trackerRotation);

	connectHeadTrackerStep2(ctw);
}


//----------------------------------------------------------------------------
void
SoTrackedViewpointMobileDisplayControlMode::connectHeadTrackerStep2(SoComposeMatrix *ctw)
{
	// use engines to transform offsets
	SoTransformVec3f *te;
	te = new SoTransformVec3f;
	te->vector.connectFrom(&eyeOffset);
	te->matrix.connectFrom(&ctw->matrix);

	// connect eyepointPositions to transformed offsets
	((SoOffAxisCamera*)stbCamera->getCamera())->eyepointPosition.connectFrom(&te->point);
    //stbCamera->getTransform()->translation.connectFrom(&te->point);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void 
SoTrackedViewpointMobileDisplayControlMode::connectDisplayTracker(SoTrakEngine* tracker)
{
    disconnectDisplayTracker();

    // use engine to create tracker to world transformation matrix
    SoComposeMatrix *ctw = new SoComposeMatrix;
    ctw->translation.connectFrom(&tracker->translation);
    ctw->rotation.connectFrom(&tracker->rotation);

    connectDisplayTrackerStep2(ctw);

    // use engines to calculate rotations	
    MultRotRot *md = new MultRotRot;
    md->rotationA.connectFrom(&displayRotationOffset);
    md->rotationB.connectFrom(&tracker->rotation);

    // connect orientations to calculated rotations
    ((SoOffAxisCamera*)stbCamera->getCamera())->orientation.connectFrom(&md->product);
    // stbCamera->getTransform()->rotation.connectFrom(&md->product);
}


//----------------------------------------------------------------------------

void
SoTrackedViewpointMobileDisplayControlMode::disconnectDisplayTracker()
{
	((SoOffAxisCamera*)stbCamera->getCamera())->position.disconnect();
	((SoOffAxisCamera*)stbCamera->getCamera())->orientation.disconnect();
}



//----------------------------------------------------------------------------
void
SoTrackedViewpointMobileDisplayControlMode::connectDisplayTracker(SoSFVec3f *trackerTranslation,
											SoSFRotation *trackerRotation)
{
	disconnectDisplayTracker();

	// use engine to create tracker to world transformation matrix
	SoComposeMatrix *ctw = new SoComposeMatrix;
	ctw->translation.connectFrom(trackerTranslation);
	ctw->rotation.connectFrom(trackerRotation);

	connectDisplayTrackerStep2(ctw);

	// use engines to calculate rotations	
	MultRotRot *md = new MultRotRot;
	md->rotationA.connectFrom(&displayRotationOffset);
	md->rotationB.connectFrom(trackerRotation);

	// connect orientations to calculated rotations
	((SoOffAxisCamera*)stbCamera->getCamera())->orientation.connectFrom(&md->product);

}

//----------------------------------------------------------------------------
void
SoTrackedViewpointMobileDisplayControlMode::connectDisplayTrackerStep2(SoComposeMatrix *ctw)
{
	// use engines to transform offsets
	SoTransformVec3f *tdo;
	tdo = new SoTransformVec3f;
	tdo->vector.connectFrom(&displayOffset);
	tdo->matrix.connectFrom(&ctw->matrix);

	// connect positions to transformed offsets
	((SoOffAxisCamera*)stbCamera->getCamera())->position.connectFrom(&tdo->point);		
}

END_NAMESPACE_STB
