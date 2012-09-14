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
/** The header file for the SoStbCameraControlMode class.
*
* @author Denis Kalkofen
*
* $Id: SoStbCameraControlMode.cxx 25 2005-11-28 16:11:59Z denis $
* @file                                                                   */
/* ======================================================================= */

#include <stb/components/viewer/controlmode/SoStbCameraControlMode.h>

//----------------------------------------------------------------------------

BEGIN_NAMESPACE_STB


SO_NODE_SOURCE(SoStbCameraControlMode);

//----------------------------------------------------------------------------
// Inventor class initialization 

void 
SoStbCameraControlMode::initClass()
{
    SO_NODE_INIT_CLASS(SoStbCameraControlMode, SoNode, "Node");
}

//----------------------------------------------------------------------------
// Abstract constructor. 

SoStbCameraControlMode::SoStbCameraControlMode()
{
	SO_NODE_CONSTRUCTOR(SoStbCameraControlMode);
	stbCamera=NULL;
     activated=false;
	// add fields
}

//----------------------------------------------------------------------------
// Abstract destructor. 

SoStbCameraControlMode::~SoStbCameraControlMode()
{
    // nil
}

bool 
SoStbCameraControlMode::activate()
{
	//nil
	return false;
}

void 
SoStbCameraControlMode::setStbCamera(SoStbCamera *soStbCam)
{
	this->stbCamera=soStbCam;
}

END_NAMESPACE_STB

