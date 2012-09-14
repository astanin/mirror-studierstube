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
/** The header file for the SoGLColorBufferElement class.
*
* @author Denis Kalkofen
*
* $Id: SoGLColorBufferElement.cxx 25 2005-11-28 16:11:59Z denis $
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

#include <iostream>

#include <Inventor/actions/SoGLRenderAction.h>

#include <stb/components/viewer/displaymode/SoGLColorBufferElement.h>

BEGIN_NAMESPACE_STB


SO_ELEMENT_SOURCE(SoGLColorBufferElement);

void SoGLColorBufferElement::initClass(void)
{
    static int first = 1;
    if (first) {
        first = 0;
        SO_ELEMENT_INIT_CLASS(SoGLColorBufferElement, SoElement);
        SO_ENABLE(SoGLRenderAction, SoGLColorBufferElement);
    }
}

SoGLColorBufferElement::~SoGLColorBufferElement(void)
{
}

void 
SoGLColorBufferElement::init(SoState * state)
{
    SoElement::init(state);
	this->mask.setValue(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
}

void SoGLColorBufferElement::push(SoState * state)
{
    SoGLColorBufferElement * prev = (SoGLColorBufferElement*)this->getNextInStack();
    this->mask = prev->mask;
    prev->capture(state);
}

void SoGLColorBufferElement::pop(SoState * /*state*/, const SoElement * prevTopElement)
{	
    SoGLColorBufferElement * prev = (SoGLColorBufferElement*)prevTopElement;
    if ( (*this) != (*prev)) {
        this->updategl( );
    }
}

SbBool 
SoGLColorBufferElement::matches(const SoElement * element) const
{
    const SoGLColorBufferElement * elem = (const SoGLColorBufferElement*) element;
    return ((*this) == (*elem)); 
}

SoElement * 
SoGLColorBufferElement::copyMatchInfo(void) const
{
    SoGLColorBufferElement * elem = (SoGLColorBufferElement*)this->getTypeId().createInstance();
  
    return elem;
}

void SoGLColorBufferElement::updategl( ) const
{
 
	GLboolean  r,g,b,a;
	r=g=b=a=GL_TRUE;
	if(mask[0]<1.0){
		r=GL_FALSE;
	}
	if(mask[1]<1.0){
		g=GL_FALSE;
	}
	if(mask[2]<1.0){
		b=GL_FALSE;
	}
	if(mask[3]<1.0){
		a=GL_FALSE;
	}
	glColorMask(r,g,b,a);
}

void 
SoGLColorBufferElement::setMask(SoState * state, SbVec4f newMask)
{
	SoGLColorBufferElement * elem = (SoGLColorBufferElement*)SoElement::getElement(state, classStackIndex);//push if non exists
    if(elem && (newMask != elem->mask) )
    {
        elem->mask = newMask;
        elem->updategl();
    }
}

END_NAMESPACE_STB
