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
 * $Id: SoDisplay.cxx 25 2005-11-28 16:11:59Z denis $
 * @file                                                                   */
/* ======================================================================= */
#include <stb/components/viewer/SoDisplay.h>

#include <stb/kernel/Studierstube.h>
#include <stb/components/viewer/Viewer.h>
#include <stb/components/viewer/guiDefines.h>
#include <stb/components/viewer/SoStudierstubeViewer.h>

#include <stb/components/viewer/SoStbCamera.h>
#include <stb/components/viewer/SoOffAxisCamera.h>
#include <stb/components/viewer/SoViewport.h>

#include SOGUI_H
#include SOGUI_CURSOR_H

#include <Inventor/actions/SoSearchAction.h>


BEGIN_NAMESPACE_STB

SO_NODE_SOURCE(SoDisplay);

//----------------------------------------------------------------------------
/** Inventor class initialization */
void
SoDisplay::initClass()
{	
    SO_NODE_INIT_CLASS(SoDisplay, SoNode, "SoGroup");
}

//----------------------------------------------------------------------------
/** The constructor inits all fields. */
SoDisplay::SoDisplay()
    : viewer(NULL)
{
    SO_NODE_CONSTRUCTOR(SoDisplay);
    
    SO_NODE_ADD_FIELD(sceneGraph, (NULL));    
    //////// StudierstubeViewer Configurations
	//SO_NODE_ADD_FIELD(name, ("")); 
    SO_NODE_ADD_FIELD(xoffset, (0)); 
    SO_NODE_ADD_FIELD(yoffset, (0)); 
    SO_NODE_ADD_FIELD(width, (640)); 
    SO_NODE_ADD_FIELD(height, (480)); 
    SO_NODE_ADD_FIELD(transparencyType, (SoDisplay::BLEND)); 
    SO_NODE_ADD_FIELD(headlight, (TRUE)); 
    SO_NODE_ADD_FIELD(headlightIntensity, (1.0f)); 
    SO_NODE_ADD_FIELD(backgroundColor, (0,0,0)); 
    SO_NODE_ADD_FIELD(clearBackGround, (TRUE));
    SO_NODE_ADD_FIELD(showMouse, (TRUE));
    SO_NODE_ADD_FIELD(windowBorder, (TRUE));
    SO_NODE_ADD_FIELD(decoration, (TRUE));
    SO_NODE_ADD_FIELD(windowOnTop, (FALSE));
    SO_NODE_ADD_FIELD(stencilBuffer, (FALSE));
    SO_NODE_ADD_FIELD(useRefCamera, (TRUE));
    SO_NODE_ADD_FIELD(isViewing,(TRUE));
    SO_NODE_ADD_FIELD(showFrameRate,(FALSE));
    SO_NODE_ADD_FIELD(showTriangleCount,(FALSE));
    SO_NODE_ADD_FIELD(quadBuffering,(FALSE));

    
    SO_NODE_DEFINE_ENUM_VALUE(TransparencyType, SCREEN_DOOR);
    SO_NODE_DEFINE_ENUM_VALUE(TransparencyType, ADD);
    SO_NODE_DEFINE_ENUM_VALUE(TransparencyType, DELAYED_ADD);
    SO_NODE_DEFINE_ENUM_VALUE(TransparencyType, SORTED_OBJECT_ADD);
    SO_NODE_DEFINE_ENUM_VALUE(TransparencyType, BLEND);
    SO_NODE_DEFINE_ENUM_VALUE(TransparencyType, DELAYED_BLEND);
    SO_NODE_DEFINE_ENUM_VALUE(TransparencyType, SORTED_OBJECT_BLEND);
    SO_NODE_DEFINE_ENUM_VALUE(TransparencyType, NONE);
    SO_NODE_DEFINE_ENUM_VALUE(TransparencyType, SORTED_OBJECT_SORTED_TRIANGLE_ADD);
    SO_NODE_DEFINE_ENUM_VALUE(TransparencyType, SORTED_OBJECT_SORTED_TRIANGLE_BLEND);
    SO_NODE_DEFINE_ENUM_VALUE(TransparencyType, SORTED_LAYERS_BLEND);
    SO_NODE_SET_SF_ENUM_TYPE(transparencyType, TransparencyType);

    examCam=NULL;
}

//----------------------------------------------------------------------------
/** The destructor */
SoDisplay::~SoDisplay()
{
    if(viewer)
    {
        viewer->hide();
        delete viewer;
    }
}

//----------------------------------------------------------------------------
//
void 
SoDisplay::exitViewer(void*, SoGuiComponent* viewer)
{
    SoDisplay* display=Viewer::findSoDisplay(((SoStudierstubeViewer*)viewer)->getSceneGraph());
    Viewer::removeSoDisplay(display);
    //    delete viewer;
}


//----------------------------------------------------------------------------
// see SoDisplay.h
void 
SoDisplay::createViewer()
{
    //	set up the viewer's basic graph
    //
    //							   root
    //								|
    //          |---------------------
    //         examCam    
    //
    displayRoot=new SoSeparator();
    this->addChild(displayRoot);

	// set this displays name
	//this->setName( SbName(name.getValue()) );

	// on wince, the viewport is (for whatever reason) not correctly set up -- 20060914 flo
#ifdef STB_IS_WINCE
	SoViewport * viewport = new SoViewport();
	viewport->setViewportRegion(SbVec2s(0,0),SbVec2s(640,480));
	displayRoot->addChild(viewport);
#endif

    if(useRefCamera.getValue()){
        examCam= new SoPerspectiveCamera();
        displayRoot->addChild(examCam);
    }
    if(sceneGraph.getValue()){
		displayRoot->addChild(sceneGraph.getValue());
    }

    ////////////////////////////////////////
    // create StudierstubeViewer
    ////////////////////////////////////////
    viewer=new SoStudierstubeViewer(NULL);

    if (quadBuffering.getValue())
    {
        viewer->setQuadBufferStereo(true);
    }        
    ///////////////////////////////////////////////////
    // add all the stbCameras to the viewer's root node
    // for (int i=0;i<stbCameraList.getNum();i++)
    // {
    //    if(stbCameraList[i]!=NULL 
    //    && stbCameraList[i]->isOfType(SoStbCamera::getClassTypeId()))
    //    {
    //     SoStbCamera *stbCamera=(SoStbCamera *)stbCameraList[i];
    //     //Set reference camera's parameter to match with the OffAxisCamera of the first StbCamera 
    //     if(	i==0 
    //      && stbCamera->getCamera()
    //      && stbCamera->getCamera()->isOfType(SoOffAxisCamera::getClassTypeId())
    //      )
    //     {
    //      SoPerspectiveCamera *refCam = (SoPerspectiveCamera*)examCam;
    //      SoOffAxisCamera *cam=(SoOffAxisCamera*)stbCamera->getCamera();
    //      refCam->aspectRatio		=	cam->size.getValue()[0]/cam->size.getValue()[1];
    //      refCam->focalDistance	=	cam->focalDistance;
    //      refCam->nearDistance	=	cam->nearDistance;
    //      refCam->farDistance		=	cam->farDistance;
    //      refCam->viewportMapping =	cam->viewportMapping;
    //      refCam->orientation		=	cam->orientation;
    //     }

    //     ///////////////////////////////////////////////////////////////////////////////////////
    //   ///////////// Tell the StbCamera about SoDisplay's reference camera  ///////////////////
    //     stbCamera->setSoDisplay(this);
    //     stbCamera->setReferenceCamera(examCam);

    //     ///////////////////////////////////////////////
    //     ///////////// Set Content  ///////////////////
    //     //set the displays content only if no content has been defined for the StbCamera
    //     if(!stbCamera->hasContent())
    //     {
    //      stbCamera->setContent(content.getValue());
    //     }
    //        stbCamera->activateControlMode();
    //        stbCamera->activateDisplayMode(viewer);
    //     ///////////////////////////////////////////////////////////
    //     //////////// Add StbCamera to StudierstubeViewer's root ////
    //     displayRoot->addChild(stbCamera);	
    //    }
    //}
  
    //////////////////////////////////////////
    ////  Configure  StudierstubeViewer 
    //////////////////////////////////////////

	//windowBorder
	//viewer->setWindowDecoration(windowBorder.getValue());
	//if (!windowBorder.getValue())
	//	flags|=Qt::FramelessWindowHint;

	//set chosen flags for qt window
	//mainWin->setWindowFlags(flags);

    ////windowDecoreation
    viewer->setWindowDecoration(windowBorder.getValue());
    if( viewer->isDecoration() != decoration.getValue())
        viewer->setDecoration( decoration.getValue());
	
    //widownOnTop
    viewer->setWindowOnTop(windowOnTop.getValue());
	
    //stencilBuffer
    if (stencilBuffer.getValue() == TRUE)
    {
        viewer->setStencilBuffer(TRUE);
    }

    //window pos & size
    viewer->setWindowPosSize(xoffset.getValue(), yoffset.getValue(),
                             width.getValue()  , height.getValue());
   
    //transparency
    viewer->setTransparencyType(
                                (SoGLRenderAction::TransparencyType)transparencyType.getValue());
	
    //title
    viewer->setTitle(STUDIERSTUBE_VERSION_STRING);
    
    // scene lighting
    //viewer->setHeadlight(headlight.getValue());
    //if (viewer->getHeadlight())
    // viewer->getHeadlight()->intensity.setValue(headlightIntensity.getValue());
       

    //background color 
    viewer->setBackgroundColor(backgroundColor.getValue());

    //clearing background on/off
    viewer->setClearBeforeRender(clearBackGround.getValue());


    // set mouse cursor
    if( showMouse.getValue() == FALSE )
    {
        viewer->setComponentCursor(SoGuiCursor::getBlankCursor());
        viewer->showMouse(false);
    }

    // Set the Viewing
    if(!isViewing.getValue())
        viewer->setViewing(isViewing.getValue());
    
    //////////////////////////////////////////
    //// start StudierstubeViewer
    //// set viewer's root node and show the studierstubeViewer
    //////////////////////////////////////////
    viewer->setSceneGraph(displayRoot);

    viewer->printTriangles(showTriangleCount.getValue() == TRUE);
    viewer->printFrameRate(showFrameRate.getValue() == TRUE);
    viewer->setWindowCloseCallback(exitViewer);
    viewer->show();
}

SbBool 
SoDisplay::readInstance(SoInput *in, unsigned short flags)
{
    if(!SoNode::readInstance(in, flags))
        return false;

    createViewer();
      
    return true;
}

SoStudierstubeViewer* 
SoDisplay::getViewer()
{
    return this->viewer;
}


SoPerspectiveCamera* 
SoDisplay::getReferenceCamera()
{
    return examCam;
}


bool
SoDisplay::find(SoNode *node)
{
    SoSearchAction sAction;
    sAction.reset();
    sAction.SoSearchAction::setNode(node);
    sAction.setSearchingAll(TRUE);
    sAction.apply(displayRoot);
    SoPath *path = sAction.getPath();
    if(path==NULL){
        return false;
    }  
    return true;
}

SoNode*
SoDisplay::findType(const SoType type)
{
	SoSearchAction sAction;
	sAction.reset();
	sAction.setType(type);
	sAction.setSearchingAll(TRUE);
	sAction.apply( ((SoStudierstubeViewer*)viewer)->getSceneGraph() );
	SoPath *path = sAction.getPath();
	if(path!=NULL){
		return sAction.getNode();
	}  
	return false;
}

//void
//SoDisplay::findStbCameras(SoPathList & list)
//{
//	SoType testType = SoStbCamera::getClassTypeId();
//	SoType testType2 = SoDisplay::getClassTypeId();
//	
//	SoSearchAction sAction;
//	sAction.reset();
//	sAction.setType(SoStbCamera::getClassTypeId());
//	sAction.setSearchingAll(TRUE);
//	sAction.apply( ((SoStudierstubeViewer*)viewer)->getSceneGraph() );
//	list = sAction.getPaths();
//}


END_NAMESPACE_STB

//----------------------------------------------------------------------
// End of SoDisplay.cxx
//----------------------------------------------------------------------
// Local Variables:
// mode: c++
// c-basic-offset: 4
// eval: (c-set-offset 'substatement-open 0)
// eval: (c-set-offset 'case-label '+)
// eval: (c-set-offset 'statement 'c-lineup-runin-statements)
// eval: (setq indent-tabs-mode nil)
// End:
//----------------------------------------------------------------------
