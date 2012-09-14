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
/** The header file for the SoDesktopControlMode class.
*
* @author Denis Kalkofen
*
* $Id: SoDesktopControlMode.cxx 25 2005-11-28 16:11:59Z denis $
* @file                                                                   */
/* ======================================================================= */

#include <stb/components/viewer/controlmode/SoDesktopControlMode.h>
#include <stb/components/viewer/Viewer.h>
#include <stb/components/viewer/SoDisplay.h>
#include <Inventor/nodes/SoPerspectiveCamera.h> 
#include <stb/components/viewer/SoOffAxisCamera.h>
#include <stb/components/viewer/SoOffAxisZoomCamera.h>
#include <stb/components/viewer/controlmode/MultRotRot.h>
#include <stb/kernel/Kernel.h>
//----------------------------------------------------------------------------

BEGIN_NAMESPACE_STB


SO_NODE_SOURCE(SoDesktopControlMode);

//----------------------------------------------------------------------------
// Inventor class initialization 

void 
SoDesktopControlMode::initClass()
{
    SO_NODE_INIT_CLASS(SoDesktopControlMode, SoStbCameraControlMode, "SoStbCameraControlMode");
}

//----------------------------------------------------------------------------
// Abstract constructor. 

SoDesktopControlMode::SoDesktopControlMode()
{
  SO_NODE_CONSTRUCTOR(SoDesktopControlMode);;
}

//----------------------------------------------------------------------------
// Abstract destructor. 

SoDesktopControlMode::~SoDesktopControlMode()
{
    // nil
}


bool 
SoDesktopControlMode::activate()
{
	if(stbCamera==NULL)
		return false;

    SoDisplay *dsp=Viewer::findSoDisplay(this);
    if(!dsp)
        return false;

    SoPerspectiveCamera *refCam = dsp->getReferenceCamera();
    if(!refCam)
        return false;
     
    stbCamera->getTransform()->translation.connectFrom(&refCam->position);
    stbCamera->getTransform()->rotation.connectFrom(&refCam->orientation);

	return true;
}

END_NAMESPACE_STB
