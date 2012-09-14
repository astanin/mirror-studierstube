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
/** The Source file for the SoPhantomShape.
*
* @author Erick Mendez
*
* $Id: SoPhantomShape.cpp 2006-03-10 mendez $
* @file                                                                   */
/* ======================================================================= */

#include <stb/base/OS.h>

#ifdef STB_IS_WINDOWS
#  include <SoWinEnterScope.h>
#  include <windows.h>
#endif

#include <Inventor/actions/SoGLRenderAction.h>

#ifndef __APPLE__
#include <GL/gl.h>
#else
#include <OpenGL/gl.h>
#endif

#ifdef STB_IS_WINDOWS
#  include <SoWinLeaveScope.h>
#endif

#include <stb/components/starlight/SoPhantomShape.h>

SO_NODE_SOURCE(SoPhantomShape);

void SoPhantomShape::initClass(void)
{
	SO_NODE_INIT_CLASS(SoPhantomShape, SoSeparator, "Separator");
}

SoPhantomShape::SoPhantomShape(void)
{
	SO_NODE_CONSTRUCTOR(SoPhantomShape);
	SO_NODE_ADD_FIELD(enable, (TRUE));
}

SoPhantomShape::~SoPhantomShape()
{
}

void SoPhantomShape::GLRenderBelowPath(SoGLRenderAction * action)
{
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	SoSeparator::GLRenderBelowPath(action);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);	
}
