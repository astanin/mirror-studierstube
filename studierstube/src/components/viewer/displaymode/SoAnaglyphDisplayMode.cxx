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
/** The header file for the SoAnaglyphDisplay class.
*
* @author Denis Kalkofen
*
* $Id: SoAnaglyphDisplay.cxx 25 2005-11-28 16:11:59Z denis $
* @file                                                                   */
/* ======================================================================= */

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

#include <stb/components/viewer/displaymode/SoAnaglyphDisplayMode.h>
#include <stb/components/viewer/displaymode/SoGLColorBufferElement.h>

BEGIN_NAMESPACE_STB


SO_NODE_SOURCE(SoAnaglyphDisplayMode);

//----------------------------------------------------------------------------
// Inventor class initialization.

void
SoAnaglyphDisplayMode::initClass()
{
    SO_NODE_INIT_CLASS(SoAnaglyphDisplayMode, SoDisplayMode, "SoDisplayMode");
}

//----------------------------------------------------------------------------
// The constructor 

SoAnaglyphDisplayMode::SoAnaglyphDisplayMode()
{
    SO_NODE_CONSTRUCTOR(SoAnaglyphDisplayMode);
    SO_NODE_ADD_FIELD(lensColor, (1,0,0)); // default red
   
}
//----------------------------------------------------------------------------

SoAnaglyphDisplayMode::~SoAnaglyphDisplayMode()
{
    // nil
}



//----------------------------------------------------------------------------
// Switches to left buffer (using a color mask)    
void 
SoAnaglyphDisplayMode::setLensColor(SbColor newColor)
{
	this->lensColor=newColor;
}


void 
SoAnaglyphDisplayMode::GLRender(SoGLRenderAction *action)
{
//	
	SoGLColorBufferElement::setMask(action->getState(),SbVec4f(	lensColor.getValue()[0],
																lensColor.getValue()[1],
																lensColor.getValue()[2],
																1.0));
}

END_NAMESPACE_STB

