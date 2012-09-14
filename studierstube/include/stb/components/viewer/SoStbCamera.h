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
* $Id: StbCamera.h 25 2005-11-28 16:11:59Z denis $
* @file                                                                   */
/* ======================================================================= */

#ifndef _SOSTBCAMERA_H_
#define _SOSTBCAMERA_H_

#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/fields/SoSFString.h>
#include <stb/base/macros.h>
#include <stb/components/viewer/ViewerDefines.h>

BEGIN_NAMESPACE_STB

class SoStudierstubeViewer;
class SoDisplay;

class VIEWER_API SoStbCamera : public SoBaseKit
{
public:
    SO_KIT_HEADER(SoStbCamera);
    
	SO_KIT_CATALOG_ENTRY_HEADER(xfSep);
	SO_KIT_CATALOG_ENTRY_HEADER(zoomTransform);
	SO_KIT_CATALOG_ENTRY_HEADER(headUpNode);
    SO_KIT_CATALOG_ENTRY_HEADER(transform);
    SO_KIT_CATALOG_ENTRY_HEADER(offset);
    SO_KIT_CATALOG_ENTRY_HEADER(controlMode);
	SO_KIT_CATALOG_ENTRY_HEADER(camera);

public:
	/** The constructor */
    SoStbCamera();

    /** The destructor does nothing */
    ~SoStbCamera();

    /** Inventor class initialization */
    static void initClass();

	/** 
	*	Return the SoStbCamera's camera.
	*/
	SoCamera* getCamera();

	/** 
	*	Return the SoStbCamera's transform node.
	*/
	SoTransform* getTransform();

	/** 
	*	Return the SoStbCamera's control mode node.
	*/
	SoNode* getControlMode();
	
    bool activate();

	/** Specifies the name of the node so it can be found e.g. by an application. */
	SoSFString name;

protected:
    /** Overrides the inherited method to implement a new
        SoGLRenderAction behavior. 
        @param action the SoGLRenderAction calling this method
    */
    bool activated;
    
    virtual void GLRender(SoGLRenderAction* action);
	
    /** 
    * The StbCamera's controlMode needs to be activated. 
    */
    bool activateControlMode();

};

END_NAMESPACE_STB

//----------------------------------------------------------------------------
#endif//_SOSTBCAMERA_H_

