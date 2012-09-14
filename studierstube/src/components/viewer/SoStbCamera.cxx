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
/** The header file for the StbCamera class.
*
* @author Denis Kalkofen
*
* $Id: StbCamera.cxx 25 2005-11-28 16:11:59Z denis $
* @file                                                                   */
/* ======================================================================= */

#include <stb/components/viewer/SoStbCamera.h>

#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTransformSeparator.h>

#include <stb/kernel/Kernel.h>

#include <stb/components/viewer/Viewer.h>
#include <stb/components/viewer/SoStudierstubeViewer.h>
#include <stb/components/viewer/SoDisplay.h>
#include <stb/components/viewer/SoStudierstubeViewer.h>
#include <stb/components/viewer/displaymode/SoDisplayMode.h>
#include <stb/components/viewer/SoViewport.h>
#include <stb/components/viewer/controlmode/SoStbCameraControlMode.h>

#include <stb/kernel/SoStbScene.h>


BEGIN_NAMESPACE_STB

SO_KIT_SOURCE(SoStbCamera);

//----------------------------------------------------------------------------
/** Inventor class initialization */

void
SoStbCamera::initClass()
{	
	SO_KIT_INIT_CLASS(SoStbCamera, SoBaseKit, "SoBaseKit");
}

//----------------------------------------------------------------------------
/** The constructor inits all fields. */
SoStbCamera::SoStbCamera() 
{
    SO_KIT_CONSTRUCTOR(SoStbCamera);
    
    // SO_KIT_ADD_CATALOG_ENTRY(
    // name,className,nullByDefault,parentName,rightSiblingName,isPublic
    //--------------- SoUser structure -------------------------------------------
    //						  kitRoot (SoStbCamera)
    //							 | 
    //                           | 
    //                           |
    //                 |----------
    //                 |						
    //				 xfSep		  
    //                 |
    //		|-------------|---------|--------|----------|-----------|
    //    transform  controlMode  offset  camera  zoomTransform  headUpNode
    //----------------------------------------------------------------------------
 
    SO_KIT_ADD_CATALOG_ENTRY(xfSep,			SoTransformSeparator,	FALSE,	this,	"" ,			FALSE);	
	SO_KIT_ADD_CATALOG_ENTRY(headUpNode,	SoSeparator,			TRUE,   xfSep,	"",				TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(zoomTransform,	SoTransform,			TRUE,	xfSep,	headUpNode,		TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(camera,		SoCamera,				TRUE,	xfSep,  zoomTransform,	TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(offset,		SoTransform,			TRUE,	xfSep,	camera,			FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(controlMode,	SoStbCameraControlMode,	TRUE,	xfSep,	offset,			TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(transform,		SoTransform,			FALSE,  xfSep,	controlMode,	TRUE);

	SO_KIT_ADD_FIELD( name,	(""));
    SO_KIT_INIT_INSTANCE();

    setSearchingChildren(TRUE);
    activated=false;
}

/** The destructor */
SoStbCamera::~SoStbCamera()
{
	//nil
}

bool 
SoStbCamera::activate()
{
	// set this StbCameras name
	this->setName( SbName(name.getValue()) );

	return activateControlMode();
}

SoCamera* 
SoStbCamera::getCamera()
{
	return (SoCamera*)getAnyPart("camera",false,false,false);
}

SoTransform* 
SoStbCamera::getTransform()
{
	return (SoTransform*)getAnyPart("transform",false,false,false);
}

SoNode* 
SoStbCamera::getControlMode()
{
	return getAnyPart("controlMode",false,false,false);
}

bool
SoStbCamera::activateControlMode()
{	
	if(controlMode.getValue())
	{
		if(!(SoCamera*)getAnyPart("camera",false,false,false)){
			logPrintE("StbCameraControlMode: 'no camera found to control'\n ");
			return false;
		}

		((SoStbCameraControlMode*)controlMode.getValue())->setStbCamera(this);

        if(!((SoStbCameraControlMode*)controlMode.getValue())->activate())
		    return false;

		return true;
	}
	return false;
}

void 
SoStbCamera::GLRender(SoGLRenderAction* action)
{
    if(!activated)
    {
        activated=activate();
        //return ;
    }

    SoBaseKit::GLRender(action);
}

END_NAMESPACE_STB

//void 
//SoStbCamera::setSoDisplay(SoDisplay *aDisplay)
//{
//	soDisplay=aDisplay;
//}
//
//SoDisplay*
//SoStbCamera::getSoDisplay()
//{
//	return soDisplay;
//}

