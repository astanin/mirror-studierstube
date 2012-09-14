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
/** The header file for the SoViewport class.
*
* @author Denis Kalkofen
*
* $Id: SoViewport.cxx 25 2005-11-28 16:11:59Z denis $
* @file                                                                   */
/* ======================================================================= */

#include <stb/components/viewer/SoViewport.h>

#include <Inventor/SbViewportRegion.h> 
#include <Inventor/elements/SoGLViewportRegionElement.h>
#include <stb/base/OS.h>

#ifdef STB_IS_WINDOWS
#  define WIN32_LEAN_AND_MEAN
#  include <windows.h>
#endif

#ifndef __APPLE__
#include <GL/gl.h>
#else
#include <OpenGL/gl.h>
#endif

BEGIN_NAMESPACE_STB


SO_NODE_SOURCE(SoViewport);


void
SoViewport::initClass()
{
    SO_NODE_INIT_CLASS(SoViewport, SoNode, "SoNode");
}

SoViewport::SoViewport(void)
{
	SO_NODE_CONSTRUCTOR(SoViewport);
	SO_NODE_ADD_FIELD(origin, (-1,-1));
	SO_NODE_ADD_FIELD(size, (-1,-1));
	changeViewportRegion=false;
}

SoViewport::~SoViewport(void)
{
	//nil
}

void 
SoViewport::setViewportRegion(SbVec2s origin, SbVec2s size)
{
	viewportRegion.setViewportPixels(origin,size);
	changeViewportRegion=true;
}

/** Returns the region's origin*/
SbVec2s 
SoViewport::getOrigin()
{
	return viewportRegion.getViewportOriginPixels();
}

/** Returns the region's size*/
SbVec2s
SoViewport::getSize()
{
	return viewportRegion.getViewportSizePixels();
}

void 
SoViewport::GLRender(SoGLRenderAction* action)
{
	//change the value of the viewportregion element in the state according to the 'viewportRegion' member variable
	if(changeViewportRegion)
		SoGLViewportRegionElement::set(action->getState(),viewportRegion);
		
}

SbBool 
SoViewport::readInstance(SoInput *in, unsigned short flags)
{
	if(!SoFieldContainer::readInstance(in, flags))
		return false;
	SbVec2s curOrigin=origin.getValue();
	SbVec2s curSize=size.getValue();
	
	// don't enable the viewportregion if  origin and size euqal -1
	if(		curOrigin[0]==-1 &&	curOrigin[1]==-1
		&&	curSize[0]  ==-1 &&	curSize[1]  ==-1)
		return true;
	
	this->setViewportRegion(origin.getValue(),size.getValue());

	return true;
}

END_NAMESPACE_STB
