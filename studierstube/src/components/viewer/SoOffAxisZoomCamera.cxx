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
/** SoOffAxisZoomCamera.cxx contains the implementation of class SoOffAxisZoomCamera
  *
  * @author Markus Sareika
  *
  * $Id: SoOffAxisZoomCamera.cxx 
  * @file                                                                   
  */
 /* ======================================================================== */

#include <stb/base/OS.h>

#include <iostream>

#ifdef STB_IS_WINDOWS
#  include <SoWinEnterScope.h>
#  include <windows.h>
#endif

#include <Inventor/elements/SoModelMatrixElement.h>
#include <Inventor/elements/SoViewingMatrixElement.h>
#include <Inventor/elements/SoProjectionMatrixElement.h>
#include <Inventor/elements/SoViewVolumeElement.h>
#include <Inventor/elements/SoFocalDistanceElement.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/SbLinear.h>

#ifndef __APPLE__
#include <GL/gl.h>
#include <GL/glu.h>
#else
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#endif

#ifdef STB_IS_WINDOWS
#  include <SoWinLeaveScope.h>
#endif

#include <stb/components/viewer/SoOffAxisZoomCamera.h>



BEGIN_NAMESPACE_STB

SO_NODE_SOURCE(SoOffAxisZoomCamera);


// Inventor class initialization.

void SoOffAxisZoomCamera::initClass()
{
	SO_NODE_INIT_CLASS(SoOffAxisZoomCamera, SoCamera, "Camera");
}

// The constructor inits the fields of the node.

SoOffAxisZoomCamera::SoOffAxisZoomCamera() 
{
	SO_NODE_CONSTRUCTOR(SoOffAxisZoomCamera);

	// pos and rot of camera location
	SO_NODE_ADD_FIELD(pos, (0.0f, 0.0f, 1.0f));
	SO_NODE_ADD_FIELD(rot, (SbRotation(SbVec3f(0.0f, 0.0f, 1.0f), 0.0f)));
	// fieldOfView of camera
	SO_NODE_ADD_FIELD(zoomFactor, (1.0f) );

	// size of projection plane
	SO_NODE_ADD_FIELD(size, (1.0f, 1.0f) );
	// eye posistion in world space
	SO_NODE_ADD_FIELD(eyepointPosition, (0.0f,0.0f,1.0f));
    // pre-warping matrix for oblique tiled projection
    SO_NODE_ADD_FIELD(matrix, (1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1));

	orientation.setValue(SbVec3f(0,0,1), 0);
	nearDistance = 0.16f;
	// position of projection plane in world space
	position.setValue(SbVec3f(0,0,0));
	setName("offAxisZoomCamera");
}

SoOffAxisZoomCamera::~SoOffAxisZoomCamera()
{
}

// Scales the height of the camera's projection plane.

void SoOffAxisZoomCamera::scaleHeight(float scaleFactor)
{
    SbVec2f ppSize = size.getValue();
    ppSize[1] = ppSize[1] * scaleFactor;
    size.setValue(ppSize);
}

void SoOffAxisZoomCamera::viewBoundingBox(const SbBox3f &/*box*/,
                                         float /*aspect*/, float /*slack*/)
{
    // NIL
}

#ifdef __COIN__

// Doc in superclass.
SbViewVolume SoOffAxisZoomCamera::getViewVolume(float useaspectratio) const
{
    SbViewVolume vv;

   	// read values of this camera
	float nearDist = nearDistance.getValue();
	float farDist = farDistance.getValue();
	SbVec2f ppSize = size.getValue();
	SbVec3f epos = eyepointPosition.getValue();

	bool flipH = false, flipV = false;

	if ((nearDist <= 0) || (ppSize[0] == 0) || (ppSize[1] == 0))
	{
		return vv;
	}

	// negative values for size result in flipped image
	if (ppSize[0] < 0) {
		ppSize[0] = -ppSize[0];
		flipV = true;
	}
	if (ppSize[1] < 0) {
		ppSize[1] = -ppSize[1];	
		flipH = true;
	}

	// set zoom offset
	ppSize[0] *= zoomFactor.getValue();
	ppSize[1] *= zoomFactor.getValue();

	if (useaspectratio != 0.0) 
	{
		float cameraAspect = ppSize[0]/ppSize[1];
		ppSize.setValue((useaspectratio/cameraAspect) * ppSize[0], ppSize[1]);
	}
	
	// world to PP-space transformation
	SbMatrix invOrientation, invPosition;
	invPosition.setTranslate(-position.getValue());
	invOrientation.setRotate(orientation.getValue().inverse());

	// transform eyepointPosition from world to PP-space
	SbVec3f relEpos;
	(invPosition*invOrientation).multVecMatrix(epos, relEpos);

    // if eye is behind projection plane (no valid camera)
	if (relEpos[2] <= 0) 
	{
		return vv;
	}

    SbLine lLB, lRT;
	SbVec3f fLB, fRT;
	// create lines through left-bottom and right-top corners of projection plane	
	lLB.setValue(SbVec3f(0,0,0),SbVec3f(-(ppSize[0]/2.0f)-relEpos[0],-(ppSize[1]/2.0f)-relEpos[1],-relEpos[2]));
	lRT.setValue(SbVec3f(0,0,0),SbVec3f((ppSize[0]/2.0f)-relEpos[0],(ppSize[1]/2.0f)-relEpos[1],-relEpos[2]));

	// intersect lines through projection plane corners with near plane
	SbPlane nearPlane(SbVec3f(0,0,1), -nearDist);	
	nearPlane.intersect(lLB, fLB);
	nearPlane.intersect(lRT, fRT);

	// get inersection points 
	float l,b,r,t,n;
	fLB.getValue(l,b,n);
	fRT.getValue(r,t,n);

	// flip image if flags are set
	if (flipH) {
		float tmp = b;
		b = t;
		t = tmp;
	}
	if (flipV) {
		float tmp = l;
		l = r;
		r = tmp;
	}

    vv.frustum(l,r,b,t,nearDist,farDist);

    vv.rotateCamera(this->orientation.getValue());
    vv.translateCamera(this->eyepointPosition.getValue());

    return vv;
}


// This method projects a given source point (3D) to the screen (2D).
int SoOffAxisZoomCamera::projectToScreen(const SbVec3f &src, SbVec2f &dst) const
{    
    SbVec3f res;
    SbBox3f box(0,0,0,1,1,1);

    getViewVolume().projectToScreen(src, res);
    if( box.intersect(res))
    {
        dst[0] = res[0];
        dst[1] = res[1];
        return TRUE;
    }
    // the original implementation does this also, therefore we added it
    // to be compatible.
    dst[0] = -100;
    dst[1] = -100;
    return FALSE;
}


void SoOffAxisZoomCamera::GLRender(SoGLRenderAction* action)
{
		glClear(GL_DEPTH_BUFFER_BIT);
		SoCamera::GLRender(action);
}


// Overrides the inherited method, to do picking for off axis camera.
void SoOffAxisZoomCamera::rayPick(SoRayPickAction * action)
{
	// Overridden to calculate the coordinates of the ray within the current camera settings
	//SoOffAxisZoomCamera::doPickAction(action);
	SoCamera::doAction(action);

	// check for a non-empty view volume
	SbViewVolume vv = this->getViewVolume(1.0f);
	if (vv.getDepth() != 0.0f &&
		vv.getWidth() != 0.0f &&
		vv.getHeight() != 0.0f) {
			action->computeWorldSpaceRay();
	}
}

////// Overridden to set up the viewing and projection matrices.
////void SoOffAxisZoomCamera::doPickAction(SoAction * action)
////{
////	SoState * state = action->getState();
////
////	SbViewVolume vv = this->getViewVolume(1.0f);
////
////	SbMatrix affine, proj;
////	if (vv.getDepth() == 0.0f || vv.getWidth() == 0.0f || vv.getHeight() == 0.0f) {
////		 Handle empty scenes.
////		affine = proj = SbMatrix::identity();
////	}
////	else {
////		vv.getMatrices(affine, proj);
////
////		 fix transform here
////		////////////////////// TODO ////////////////////////////////////////
////		SoTransform viewerLocation = SoStbCamera::getTransform();
////		SbMatrix mm;
////		mm.setTransform( pos.getValue(), rot.getValue(), SbVec3f(1.f, 1.f, 1.f));
////
////		vv.transform(mm);
////		affine.multRight(mm.inverse());
////	}
////	SoViewVolumeElement::set(state, this, vv);
////	SoProjectionMatrixElement::set(state, this, proj);
////	SoViewingMatrixElement::set(state, this, affine);
////	SoFocalDistanceElement::set(state, this, this->focalDistance.getValue());
////}

#else ///////////////////////// not __COIN__

// method returning an approximate SbViewVolume.
SbViewVolume SoOffAxisZoomCamera::getViewVolume(float useAspectRatio) const
{
    SbViewVolume view;

    SbVec2f ppSize = size.getValue();

    float camAspect = (useAspectRatio != 0.0 ? useAspectRatio :
    ppSize[0]/ppSize[1]);

    SbVec3f temp = eyepointPosition.getValue() - position.getValue();
    float angle = (float)(2.0f * acos((ppSize[1]/2)/ temp.length()));
    
    // Set up the perspective camera.
    view.perspective(angle, camAspect,
        nearDistance.getValue(), farDistance.getValue());
    
    // Note that these move the camera rather than moving objects
    // relative to the camera.
    view.rotateCamera(orientation.getValue());
    view.translateCamera(eyepointPosition.getValue());

    return view;
}

// This method projects a given source point (3D) to the screen (2D).

int SoOffAxisZoomCamera::projectToScreen(const SbVec3f &src, SbVec2f &dst) const
{
	// read values of this camera
	SbVec2f ppSize = size.getValue();
	SbVec3f epos = eyepointPosition.getValue();

	if ((ppSize[0] <= 0) || (ppSize[1] <= 0)) {
		dst[0] = -100;
		dst[1] = -100;
		return FALSE;
	}

	// transform eyepointPosition from world to PP-space
	SbVec3f temp;
	temp = epos - position.getValue();
	SbVec3f relEpos;
	orientation.getValue().inverse().multVec(temp, relEpos);

	// transform src-Point from world to PP-space
	temp = src - position.getValue();
	SbVec3f relSrc;
	orientation.getValue().inverse().multVec(temp, relSrc);

	// create ray through relative eyeposition and source-point
	SbLine projectionRay;
	projectionRay.setValue(relEpos, relSrc);
	
	// if src is behind eye OR 
	// if eye is in front of projection plane (no valid camera)
	if ((relEpos[2] < relSrc[2]) || (relEpos[2] <= 0)) {
		dst[0] = -100;
		dst[1] = -100;
		return FALSE;
	}

	// intersect projectionRay with projection plane
	SbVec3f dest;
	SbPlane projectionPlane(SbVec3f(0,0,1), 0);	
	projectionPlane.intersect(projectionRay, dest);

 	// convert destination-point(on projection plane) to normalized screen coordinates
	dst[0] = dest[0]/ppSize[0] + 0.5f;
	dst[1] = dest[1]/ppSize[1] + 0.5f;
	
	int valid = TRUE;
	
	for (int i=0; i<2; i++) 
		if (dst[i]<0.0 || dst[i]>1.0) 
			valid = FALSE;
	return valid;
}


void 
SoOffAxisZoomCamera::doAction(SoAction* action)
{
	// read values of this camera
	SbVec3f epos = eyepointPosition.getValue();

	// world to PP-space tansformation
	SbMatrix invOrientation, invPosition;
	invPosition.setTranslate(-position.getValue());
	invOrientation.setRotate(orientation.getValue().inverse());

	// transform eyepointPosition from world to PP-space
	SbVec3f relEpos;
	(invPosition*invOrientation).multVecMatrix(epos, relEpos);

	// off-axis translation and eyepoint z-translation (PP-space to frustum-space transformation)  
	SbMatrix OffAxisTrans;
	OffAxisTrans.setTranslate(-relEpos);

	// get ModelMatrix (note: any transformation before the camera node 
	//	is interpreted as movement of the camera (like in Inventor) 	 
	SbMatrix mm = SoModelMatrixElement::get(action->getState());
	// create ViewingMatrix 
	// FIXME: SGI/OIV has a bug reg. the calculation of the ViewingModelMatrix.
	//        the following code adapts. 20030820 tamer.
#ifdef __COIN__
	SbMatrix vm = invPosition*invOrientation*OffAxisTrans*mm.inverse();
#else
	SbMatrix vm = mm.inverse()*invPosition*invOrientation*OffAxisTrans;
#endif

	// set ViewingMatrix 
	SoViewingMatrixElement::set(action->getState(), this, vm);
}


// Abort callback for this node.

SoGLRenderAction::AbortCode abortRendering(void*)
{
    return SoGLRenderAction::ABORT;
}


// Overrides the inherited method to implement a new SoGLRenderAction behavior.

void SoOffAxisZoomCamera::GLRender(SoGLRenderAction* action)
{
	// read values of this camera
	float nearDist = nearDistance.getValue();
	float farDist = farDistance.getValue();
	SbVec2f ppSize = size.getValue();
	SbVec3f epos = eyepointPosition.getValue();

	bool flipH = false, flipV = false;

	if ((nearDist <= 0) || (ppSize[0] == 0) || (ppSize[1] == 0))
	{
        action->setAbortCallback(abortRendering, NULL);
        action->abortNow();
        action->setAbortCallback(NULL, NULL);
		return;
	}

	// negative values for size result in flipped image
	if (ppSize[0] < 0) {
		ppSize[0] = -ppSize[0];
		flipV = true;
	}
	if (ppSize[1] < 0) {
		ppSize[1] = -ppSize[1];	
		flipH = true;
	}

	if (viewportMapping.getValue() == ADJUST_CAMERA) 
	{
		float cameraAspect = ppSize[0]/ppSize[1];
		float viewportAspect = action->getViewportRegion().getViewportAspectRatio();			
		ppSize.setValue((viewportAspect/cameraAspect) * ppSize[0], ppSize[1]);
	}
		
	// world to PP-space tansformation
	SbMatrix invOrientation, invPosition;
	invPosition.setTranslate(-position.getValue());
	invOrientation.setRotate(orientation.getValue().inverse());

	// transform eyepointPosition from world to PP-space
	SbVec3f relEpos;
	(invPosition*invOrientation).multVecMatrix(epos, relEpos);

	// if eye is behind projection plane (no valid camera)
	if (relEpos[2] <= 0) 
	{
		action->setAbortCallback(abortRendering, NULL);
		action->abortNow();
		action->setAbortCallback(NULL, NULL);
		return;
	}

	// off-axis translation and eyepoint z-translation (PP-space to frustum-space transformation)  
	SbMatrix OffAxisTrans;
	OffAxisTrans.setTranslate(-relEpos);

	// get ModelMatrix (note: any transformation before the camera node 
	//	is interpreted as movement of the camera (like in Inventor) 	 
	SbMatrix mm = SoModelMatrixElement::get(action->getState());
	// create ViewingMatrix 
#ifdef __COIN__
	SbMatrix vm = invPosition*invOrientation*OffAxisTrans*mm.inverse();
#else
	SbMatrix vm = mm.inverse()*invPosition*invOrientation*OffAxisTrans;
#endif
	// set ViewingMatrix 
	SoViewingMatrixElement::set(action->getState(), this, vm);

	SbLine lLB, lRT;
	SbVec3f fLB, fRT;
	// create lines through left-bottom and right-top corners of projection plane	
	lLB.setValue(SbVec3f(0,0,0),SbVec3f(-(ppSize[0]/2.0f)-relEpos[0],-(ppSize[1]/2.0f)-relEpos[1],-relEpos[2]));
	lRT.setValue(SbVec3f(0,0,0),SbVec3f((ppSize[0]/2.0f)-relEpos[0],(ppSize[1]/2.0f)-relEpos[1],-relEpos[2]));

	// intersect lines through projection plane corners with near plane
	SbPlane nearPlane(SbVec3f(0,0,1), -nearDist);	
	nearPlane.intersect(lLB, fLB);
	nearPlane.intersect(lRT, fRT);

	// get inersection points 
	float l,b,r,t,n;
	fLB.getValue(l,b,n);
	fRT.getValue(r,t,n);

	// flip image if flags are set
	if (flipH) {
		float tmp = b;
		b = t;
		t = tmp;
	}
	if (flipV) {
		float tmp = l;
		l = r;
		r = tmp;
	}

    // set projection Matrix using openGL calls
    glMatrixMode(GL_PROJECTION);

    // use a pre-warping matrix (normalwise identity matrix)
    // usefull for oblique tiled projection
    glLoadMatrixf(&(matrix.getValue().getValue()[0][0]));

	// use intersection point coordinates to create frustum-matrix
    glFrustum(l,r,b,t,nearDist,farDist);

    glMatrixMode(GL_MODELVIEW);

    float pMatr[16];
    glGetFloatv(GL_PROJECTION_MATRIX, pMatr);
    SbMatrix pm(    pMatr[0], pMatr[1], pMatr[2], pMatr[3],
                    pMatr[4], pMatr[5], pMatr[6], pMatr[7],
                    pMatr[8], pMatr[9], pMatr[10],pMatr[11],
                    pMatr[12],pMatr[13],pMatr[14],pMatr[15]);
    SoProjectionMatrixElement::set(action->getState(), this, pm);
}


void
SoOffAxisZoomCamera::getBoundingBox(SoGetBoundingBoxAction *action)
{
    doAction((SoAction *)action);
}


// Overrides the inherited method, to do nothing when called.
void
SoOffAxisZoomCamera::rayPick(SoRayPickAction *action)
{
    // NIL
}

#endif  // __COIN__

END_NAMESPACE_STB

/* ===========================================================================
    End of SoOffAxisZoomCamera.cxx
   ===========================================================================
    Automatic Emacs configuration follows.
    Local Variables:
    mode:c++
    c-basic-offset: 4
    eval: (c-set-offset 'substatement-open 0)
    eval: (c-set-offset 'case-label '+)
    eval: (c-set-offset 'statement 'c-lineup-runin-statements)
    eval: (setq indent-tabs-mode nil)
    End:
=========================================================================== */
