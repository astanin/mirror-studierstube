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
 * implementation for stencil buffer element
 *
 * @author Gerhard Reitmayr
 *
 * $Id: SoGLStencilBufferElement.cpp 8 2004-07-14 12:24:04Z tamer $
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

#include <stb/components/starlight/GLtools.h>
#include <stb/components/starlight/SoGLStencilBufferElement.h>


SO_ELEMENT_SOURCE(SoGLStencilBufferElement);

void SoGLStencilBufferElement::initClass(void)
{
    static int first = 1;
    if (first) {
        first = 0;
        SO_ELEMENT_INIT_CLASS(SoGLStencilBufferElement, inherited);
        SO_ENABLE(SoGLRenderAction, SoGLStencilBufferElement);
    }
}

SoGLStencilBufferElement::~SoGLStencilBufferElement(void)
{
}

void SoGLStencilBufferElement::init(SoState * state)
{
    inherited::init(state);
    this->enable = FALSE;
    this->func = ALWAYS;
    this->ref = 0;
    this->mask = 0;
    this->bits = 0;
    this->fail = KEEP;
    this->zfail = KEEP;
    this->zpass = KEEP;    
    this->statebits = 0;
    this->updategl( ENABLE | FUNC | MASK | OP );
}

void SoGLStencilBufferElement::push(SoState * state)
{
    SoGLStencilBufferElement * prev = (SoGLStencilBufferElement*)this->getNextInStack();
    this->enable = prev->enable;
    this->func = prev->func;
    this->ref = prev->ref; 
    this->mask = prev->mask;
    this->bits = prev->bits;
    this->fail = prev->fail;
    this->zfail = prev->zfail;
    this->zpass = prev->zpass;
    this->statebits = prev->statebits;
    prev->capture(state);
}

void SoGLStencilBufferElement::pop(SoState * /*state*/, const SoElement * prevTopElement)
{
    SoGLStencilBufferElement * prev = (SoGLStencilBufferElement*)prevTopElement;
    if ( (*this) != (*prev)) {
        this->updategl( ENABLE | FUNC | MASK | OP );
    }
}

void SoGLStencilBufferElement::setEnable(SoState * state, const SbBool enable )
{
    SoGLStencilBufferElement * elem = (SoGLStencilBufferElement*)
        SoElement::getElement(state, classStackIndex);

    if( enable != elem->enable )
    {
        elem->enable = enable;
        elem->statebits |= ENABLE;
        elem->updategl( ENABLE );
    }
}

void SoGLStencilBufferElement::setFunc(SoState * state, const Func func, const int32_t ref, const uint32_t mask )
{
    SoGLStencilBufferElement * elem = (SoGLStencilBufferElement*)
        SoElement::getElement(state, classStackIndex);
    
    if( func != elem->func || ref != elem->ref || mask != elem->mask )
    {
        elem->func = func;
        elem->ref = ref;
        elem->mask = mask;
        elem->statebits |= FUNC;
        elem->updategl( FUNC );
    }
}

void SoGLStencilBufferElement::setMask(SoState * state, const uint32_t bits)
{
    SoGLStencilBufferElement * elem = (SoGLStencilBufferElement*)
        SoElement::getElement(state, classStackIndex);
    
    if( bits != elem->bits )
    {
        elem->bits = bits;
        elem->statebits |= MASK;
        elem->updategl( MASK );
    }
}

void SoGLStencilBufferElement::setOps(SoState * state, const Ops fail, const Ops zfail, const Ops zpass )
{
    SoGLStencilBufferElement * elem = (SoGLStencilBufferElement*)
        SoElement::getElement(state, classStackIndex);
    
    if( fail != elem->fail || zfail != elem->zfail || zpass != elem->zpass )
    {
        elem->fail = fail;
        elem->zfail = zfail;
        elem->zpass = zpass;
        elem->statebits |= OP;
        elem->updategl( OP );
    }
}

SbBool SoGLStencilBufferElement::matches(const SoElement * element) const
{
    const SoGLStencilBufferElement * elem = (const SoGLStencilBufferElement*) element;
    return ((*this) == (*elem)); 
}

SoElement * SoGLStencilBufferElement::copyMatchInfo(void) const
{
    SoGLStencilBufferElement * elem = (SoGLStencilBufferElement*)
        this->getTypeId().createInstance();
    elem->enable = this->enable;
    elem->func = this->func;
    elem->ref  = this->ref; 
    elem->mask = this->mask;
    elem->bits = this->bits;
    elem->fail = this->fail;
    elem->zfail = this->zfail;
    elem->zpass = this->zpass;
    elem->statebits = this->statebits;
    return elem;
}

void SoGLStencilBufferElement::updategl( uint32_t updatebits ) const
{

    if( updatebits & ENABLE )
    {
        if (enable) {
            glEnable(GL_STENCIL_TEST);
            checkGLErrors();
        }
        else
        {
            glDisable(GL_STENCIL_TEST);
            checkGLErrors();
        }
    }
    if( updatebits & FUNC )
    {
        GLint glref = ref;
        GLuint glmask = mask;
        switch(func) {
        case NEVER:
            glStencilFunc( GL_NEVER, glref, glmask );
        	break;
        case LESS:
            glStencilFunc( GL_LESS, glref, glmask );
        	break;
        case LEQUAL:
            glStencilFunc( GL_LEQUAL, glref, glmask );
            break;
        case GREATER:
            glStencilFunc( GL_GREATER, glref, glmask );
            break;
        case GEQUAL:
            glStencilFunc( GL_GEQUAL, glref, glmask );
            break;
        case EQUAL:
            glStencilFunc( GL_EQUAL, glref, glmask );
            break;
        case NOTEQUAL:
            glStencilFunc( GL_NOTEQUAL, glref, glmask );
            break;
        case ALWAYS:
            glStencilFunc( GL_ALWAYS, glref, glmask );
            break;
        default:
            assert(NULL && "SoGLStencilBufferElement undefined stencil function!");
        }
        checkGLErrors();
    }
    if( updatebits & MASK )
    {
        glStencilMask( bits );
        checkGLErrors();
    }
    if ( updatebits & OP) 
    {
        static const GLenum functions[6] = { GL_KEEP, GL_ZERO, GL_REPLACE, GL_INCR, GL_DECR, GL_INVERT };
        GLenum glfail = functions[fail];
        GLenum glzfail = functions[zfail];
        GLenum glzpass = functions[zpass];
        glStencilOp( glfail, glzfail, glzpass );
        checkGLErrors();
    }
}
