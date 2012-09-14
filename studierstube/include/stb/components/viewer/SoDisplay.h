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
/** The header file for the SoDisplay class.
*
* @author Denis Kalkofen
*
* $Id: SoDisplay.h 25 2005-11-28 16:11:59Z denis $
* @file                                                                   */
/* ======================================================================= */

#ifndef _SoDisplay_H_
#define _SoDisplay_H_

#include <Inventor/nodes/SoGroup.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>

#include <Inventor/actions/SoGLRenderAction.h> 
#include <Inventor/sensors/SoTimerSensor.h> 
#include <Inventor/fields/SoSFInt32.h> 
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/fields/SoSFColor.h>
#include <Inventor/fields/SoMFNode.h>
#include <Inventor/fields/SoSFNode.h>
#include <Inventor/fields/SoSFString.h>
#include <stb/components/viewer/guiDefines.h>
#include <stb/components/viewer/SoStbCamera.h>
#include SOGUICOMPONENT_H
#include <vector>
#include <stb/base/macros.h>
#include <stb/components/viewer/ViewerDefines.h>

//	SoDisplay
//=============
/************************************************************************
 * SoDisplay is the base class for a single viewer. 
 * An SoDisplay is used to  
 ************************************************************************/
BEGIN_NAMESPACE_STB

class SoStudierstubeViewer;


class VIEWER_API SoDisplay : public SoGroup
{
    SO_NODE_HEADER(SoDisplay);

public:
	/** 
	 * The constructor 
	 */
    SoDisplay();
    
    /** 
     * The destructor 
     */
    virtual ~SoDisplay();
    
    /** 
     * Inventor class initialization 
     */
    static void initClass();
    
    ////////////////////////////////////
    ////////////// DISPLAY FIELDS //////
    /////////////////////////////////////
    /** The scene graph */
    SoSFNode sceneGraph;
    
    /** Specifies whether the viewer's headlight is turned on or off. */ 
    SoSFBool  useRefCamera;

    SoSFBool  quadBuffering;
    //////////////////////////////////////////
    ////////////// Viewer Configuration //////
    //////////////////////////////////////////
    /** This enum just replicates the names and values of the 
     *   SoGLRenderAction TransparencyType-enum, to be able to use
     *   the enum names as values for the transparencyType-field of this class
     *   in an IV-file.
     */
    enum TransparencyType {
        SCREEN_DOOR         = SoGLRenderAction::SCREEN_DOOR,
	    ADD                 = SoGLRenderAction::ADD,
	    DELAYED_ADD         = SoGLRenderAction::DELAYED_ADD,
	    SORTED_OBJECT_ADD   = SoGLRenderAction::SORTED_OBJECT_ADD,
	    BLEND               = SoGLRenderAction::BLEND,
	    DELAYED_BLEND       = SoGLRenderAction::DELAYED_BLEND,
	    SORTED_OBJECT_BLEND = SoGLRenderAction::SORTED_OBJECT_BLEND,
        NONE                                = SoGLRenderAction::NONE,
        SORTED_OBJECT_SORTED_TRIANGLE_ADD   = SoGLRenderAction::SORTED_OBJECT_SORTED_TRIANGLE_ADD,
        SORTED_OBJECT_SORTED_TRIANGLE_BLEND = SoGLRenderAction::SORTED_OBJECT_SORTED_TRIANGLE_BLEND,
        SORTED_LAYERS_BLEND = SoGLRenderAction::SORTED_LAYERS_BLEND
    };

	
	/** Specifies the position of the upper-left corner of the viewer window.
        (in pixels) 
        note: (0,0) is the upper-left corner of the screen.
    */
    SoSFInt32 xoffset, yoffset;
    
    /** Specifies the width and height of the viewer window (in pixels). */
    SoSFInt32 width, height;
    
    /** Specifies whether the viewer's headlight is turned on or off. */ 
    SoSFBool  headlight;
    
    /** Specifies the intensity of the viewer's headlight (0.0 - 1.0). */
    SoSFFloat  headlightIntensity;
    
    /** Specifies the transparency type, the viewer shall use.
        The default value is BLEND.
        note: see documentation of SoGLRenderAction for description
	of all possible transparencyTypes
    */
    SoSFEnum transparencyType;

	/// if enabled the viewer will clear the background before rendering
	SoSFBool clearBackGround;

    
    /** Specifies the color of the viewer's background. */
    SoSFColor backgroundColor;
    
    /** Specifies visibility of the mouse cursor in within the window. default
     * value is TRUE.
     * @note works only with Qt currently */
    SoSFBool showMouse;
    
    /** Specifies whether the window should be decorated or not
     * (have borders, title bar, etc. ). Default value is TRUE. 
     * This is independend of the display mode.*/
    SoSFBool windowBorder;
    
    /** Specifies whether the viewer should be decorated or not, 
     * this enables the buttons and wheel widgets around the render area.
     * Default value is TRUE. 
     * This is independend of the display mode.*/	
    SoSFBool decoration;
    
    /** Specifies whether the viewer should be on top of all other windows or not, 
     * Default value is FALSE. 
     * This is independend of the display mode.*/	
    SoSFBool windowOnTop;
    
    /** Specifies whether to enable the stencil buffer of the viewer.
     * Default value is FALSE. */
    SoSFBool stencilBuffer;
    
    /** Specifies whether to allow manipulation of objects on screen or
    * to immediately receive keyboard and mouse events
    * Default value is TRUE. */
    SoSFBool isViewing;

    /** Specifies whether to display the current frame rate
    * Default value is FALSE. */
    SoSFBool showFrameRate;

    /** Specifies whether to display the number of polygons being rendered
    * Default value is FALSE. */
    SoSFBool showTriangleCount;
    //////////////////////////////////////
    ///////////// API ////////////////////
    /** 
     *	Creates an instance of SoStudierstubeViewer and adds the reference camera as well as 
     *	all the stbCameras to the viewer's scene graph. If a stbCamera has no content assigned to it, 
     *	'createViewer' will set the display's content to be used as the camera's content 
     */
    virtual void createViewer();
    
    /************************************************************************/
    /*                                                                      */
    /************************************************************************/
	SoStudierstubeViewer* getViewer();

    /************************************************************************/
    /*                                                                      */
    /************************************************************************/
    static void exitViewer(void*, SoGuiComponent* viewer);

    /************************************************************************/
    /*                                                                      */
	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	SoNode* findType(const SoType type);

    /************************************************************************/
    bool find(SoNode *node);


    /************************************************************************/
    /*                                                                      */
    /************************************************************************/
    SoPerspectiveCamera* getReferenceCamera();

 protected:
    SoStudierstubeViewer* viewer;
    
    /**
     *	SoStudierstubeViewer's root node.
     */
    SoSeparator* displayRoot;
    
    /**
     *	SoStudierstubeViewer's  default camera.
     */
    SoPerspectiveCamera *examCam;
    
    /** 
     *'readInstance' calls createViewer() to create a new studierstubeviewer while reading display from a file
     */
    virtual SbBool readInstance(SoInput *in, unsigned short flags);
    
 };

 END_NAMESPACE_STB

//----------------------------------------------------------------------------
#endif//_SODISPLAYKIT_H_

