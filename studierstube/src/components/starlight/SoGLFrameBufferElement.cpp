/* ========================================================================
 * Copyright (C) 2000-2003  Vienna University of Technology
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
 * <schmalstieg@ims.tuwien.ac.at> or write to Dieter Schmalstieg,
 * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
 * Austria.
 * ========================================================================
 * PROJECT: stbapi
 * ======================================================================== */
/**
 * implementation for frame buffer element
 *
 * @author Gerhard Reitmayr
 *
 * $Id: SoGLFrameBufferElement.cpp 24 2005-02-10 19:38:46Z reitmayr $
 * @file                                                                   */
/* ======================================================================= */

#include <stb/base/OS.h>

#include <assert.h>
#ifdef STB_IS_WINDOWS
#  include <windows.h>
#endif

#ifndef __APPLE__
#include <GL/gl.h>
#else
#include <OpenGL/gl.h>
#endif

#include <Inventor/actions/SoGLRenderAction.h>
#include <stb/components/starlight/SoGLFrameBufferElement.h>

SO_ELEMENT_SOURCE(SoGLFrameBufferElement);

void SoGLFrameBufferElement::initClass(void)
{
    static int first = 1;
    if (first) {
        first = 0;
        SO_ELEMENT_INIT_CLASS(SoGLFrameBufferElement, inherited);
        SO_ENABLE(SoGLRenderAction, SoGLFrameBufferElement);
    }
}

SoGLFrameBufferElement::~SoGLFrameBufferElement(void)
{
}

void SoGLFrameBufferElement::init(SoState * state)
{
    inherited::init(state);
    this->mask = ALL;
    this->updategl();
}

void SoGLFrameBufferElement::push(SoState * state)
{
    SoGLFrameBufferElement * prev = (SoGLFrameBufferElement*)this->getNextInStack();
    this->mask = prev->mask;
    prev->capture(state);
}

void SoGLFrameBufferElement::pop(SoState * /*state*/, const SoElement * prevTopElement)
{
    SoGLFrameBufferElement * prev = (SoGLFrameBufferElement*)prevTopElement;
    if (this->mask != prev->mask ) {
        this->updategl();
    }
}

void SoGLFrameBufferElement::set(SoState * state, const Components mask )
{
    SoGLFrameBufferElement * elem = (SoGLFrameBufferElement*)
        SoElement::getElement(state, classStackIndex);
    
    if (mask != elem->mask ) {
        elem->mask = mask;
        elem->updategl();
    }
}

SbBool SoGLFrameBufferElement::matches(const SoElement * element) const
{
    const SoGLFrameBufferElement * elem = (const SoGLFrameBufferElement*) element;
    return (elem->mask == this->mask); 
}

SoElement * SoGLFrameBufferElement::copyMatchInfo(void) const
{
    SoGLFrameBufferElement * elem = (SoGLFrameBufferElement*)
        this->getTypeId().createInstance();
    elem->mask = this->mask;
    return elem;
}

void SoGLFrameBufferElement::updategl(void) const
{
    glColorMask( (this->mask & RED)>0, (this->mask & GREEN)>0, (this->mask & BLUE)>0, (this->mask & ALPHA)>0 );
}
