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
 * For further information please contact Dieter Schmalstieg under
 * <schmalstieg@icg.tu-graz.ac.at> or write to Dieter Schmalstieg,
 * Graz University of Technology, Inffeldgasse 16a, A8010 Graz,
 * Austria.
 * ========================================================================
 * PROJECT: Studierstube
 * ======================================================================== */
/** SoOffAxisZoomCamera.h contains the definition of class SoOffAxisZoomCamera
  *
  * @author Markus Sareika
  *
  * $Id: SoOffAxisZoomCamera.h
  * @file                                                                   
  */
 /* ======================================================================== */

#ifndef __SOOFFAXISZOOMCAMERA_H__
#define __SOOFFAXISZOOMCAMERA_H__

#include <Inventor/fields/SoSFVec2f.h>
#include <Inventor/nodes/SoCamera.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFMatrix.h>
#include <stb/base/macros.h>
#include <stb/base/OS.h>
#include <stb/components/viewer/ViewerDefines.h>
#include <stb/components/viewer/SoStbCamera.h>

#ifdef STB_IS_WINDOWS
#  include <SoWinLeaveScope.h>
#endif

/**
This class represents a camera, which is very flexible and its main 
advantage over the cameras provided by OpenIventor is its ability to render 
off-axis.

An SoOffAxisZoomCamera consists of two logical parts:

The eyepoint (viewpoint):
The field 'eyepointPosition' specifys the location of the viewpoint. 

The projection plane (a rectangular area, with specified 'size', 'position' 
and 'orientation'):
The field 'position' specifys the location of the center of the area.
The field 'orientation' specifys the orientation of the projection plane, 
defined as rotation of the plane's normal from its default (0,0,1). 
The field 'size' specifys the size (width, height) of the area.

The eyepoint and the projection plane can be positioned independently from 
each other and define (in conjunction with the near and far plane) a
viewing frustum. 
If the eyepoint lies on the normal through the center of the area, 
the camera will render like an SoPerspectiveCamera.

@warning If the eyepoint lies within the positive half-space of the projection 
         plane, the camera will render, otherwise the camera will abort the 
         SoRenderAction, hence nothing will be rendered. Under Coin it will
         render a default view.
@warning Because the eyepoint and the projection plane are independent from 
         each other, the whole camera can't be moved by only changing one 
         field (e.g. position, when using a standard perspective or 
         orthographic camera). Use an SoTransform-node placed before the 
         camera in the scene-graph to move the whole camera.

@author Rainer Splechtna
@ingroup camera
@ingroup core
*/

BEGIN_NAMESPACE_STB

class VIEWER_API SoOffAxisZoomCamera : public SoCamera
{
	SO_NODE_HEADER(SoOffAxisZoomCamera);

// Fields
public:
    /** Position of the eyepoint (viewpoint) */
	SoSFVec3f eyepointPosition;
    /** size[0] is the width, size[1] is the height of the projection area
    */
	SoSFVec3f pos;
	SoSFRotation rot;
	SoSFFloat zoomFactor;

	SoSFVec2f size;

    SoSFMatrix matrix;


public:
    /** Inventor class initialization */
	static void	initClass();
    /** The constructor inits the fields of the node */
	SoOffAxisZoomCamera();

protected:
    /** The destructor does nothing. */
	virtual ~SoOffAxisZoomCamera();

public:

    /** Scales the height of the camera's projection plane.
        @param scaleFactor for the height of the camera's projection plane. 
    */
	void scaleHeight(float scaleFactor);

    /** method returning default SbViewVolume.
        @param useAspectRatio
        @return default SbViewVolume
        */
    SbViewVolume getViewVolume(float useAspectRatio = 0.0) const;
    
    /** This method projects a given source point (3D) to the screen (2D).

        @param src 3D-point (in world coordinates) 
        @param dst src projected to screen (in normalized screen coordinates)
	    @return FALSE (0) if src is off screen
                TRUE (1) if src is on screen
    */
	int projectToScreen(const SbVec3f &src, SbVec2f &dst) const;

protected:
    virtual void viewBoundingBox(const SbBox3f &box,
					float aspect, float slack);
	virtual void GLRender(SoGLRenderAction *action);

	virtual void rayPick(SoRayPickAction * action);

	//virtual void doPickAction(SoAction * action);
	
#ifndef __COIN__

protected:
    /** Overrides the inherited method to implement a new
        SoGLRenderAction behavior.
        @param action the SoGLRenderAction calling this method
    */
    virtual void GLRender(SoGLRenderAction* action);
    /** Overrides the inherited method, to do nothing when called.
        @param action the SoRayPickAction calling this method
    */
    virtual void rayPick(SoRayPickAction *action);
    /** Overrides the inherited method to call the 
	    doAction method of this class.
        @param action the SoGetBoundingBoxAction calling this method
    */
    virtual void getBoundingBox(SoGetBoundingBoxAction *action);
    /** Overrides the inherited method to set the SoViewingMatrixElement 
	    correctly. It is only called from within other methods of this class.
        @param action the action passed to this method
    */
    virtual void doAction(SoAction *action);
#endif // NOT __COIN__
};

END_NAMESPACE_STB

#ifdef STB_IS_WINDOWS
#  include <SoWinEnterScope.h>
#endif

#endif // __SOOFFAXISZOOMCAMERA_H__
