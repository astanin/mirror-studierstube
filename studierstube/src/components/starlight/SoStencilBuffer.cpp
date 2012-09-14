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
 * source for stencil buffer node
 *
 * @author Gerhard Reitmayr
 *
 * $Id: SoStencilBuffer.cpp 8 2004-07-14 12:24:04Z tamer $
 * @file                                                                   */
/* ======================================================================= */

#include <stb/base/OS.h>

#ifdef STB_IS_WINDOWS
#  include <windows.h>
#endif

#ifndef __APPLE__
#include <GL/gl.h>
#else
#include <OpenGL/gl.h>
#endif

#include <Inventor/actions/SoGLRenderAction.h>

#include <stb/components/starlight/SoStencilBuffer.h>


SO_NODE_SOURCE(SoStencilBuffer);

SoStencilBuffer::SoStencilBuffer(void)
{
    SO_NODE_CONSTRUCTOR(SoStencilBuffer);
    
    SO_NODE_DEFINE_ENUM_VALUE(Ops, KEEP);
    SO_NODE_DEFINE_ENUM_VALUE(Ops, ZERO);
    SO_NODE_DEFINE_ENUM_VALUE(Ops, REPLACE);
    SO_NODE_DEFINE_ENUM_VALUE(Ops, INCR);
    SO_NODE_DEFINE_ENUM_VALUE(Ops, DECR);
    SO_NODE_DEFINE_ENUM_VALUE(Ops, INVERT);

    SO_NODE_DEFINE_ENUM_VALUE(Func, NEVER);
    SO_NODE_DEFINE_ENUM_VALUE(Func, LESS);
    SO_NODE_DEFINE_ENUM_VALUE(Func, LEQUAL);
    SO_NODE_DEFINE_ENUM_VALUE(Func, GREATER);
    SO_NODE_DEFINE_ENUM_VALUE(Func, GEQUAL);
    SO_NODE_DEFINE_ENUM_VALUE(Func, EQUAL);
    SO_NODE_DEFINE_ENUM_VALUE(Func, NOTEQUAL);
    SO_NODE_DEFINE_ENUM_VALUE(Func, ALWAYS);

    SO_NODE_ADD_FIELD(enable, (TRUE));
    SO_NODE_ADD_FIELD(function,(ALWAYS));
    SO_NODE_ADD_FIELD(ref,(0));
    SO_NODE_ADD_FIELD(mask, (0));
    SO_NODE_ADD_FIELD(bits,(255));
    SO_NODE_ADD_FIELD(fail,(NEVER));
    SO_NODE_ADD_FIELD(zFail,(NEVER));
    SO_NODE_ADD_FIELD(zPass,(NEVER));
    SO_NODE_ADD_FIELD(clearValue, (0));
    SO_NODE_ADD_FIELD(clear,(FALSE));
    
    SO_NODE_SET_SF_ENUM_TYPE(function, Func);
    SO_NODE_SET_SF_ENUM_TYPE(fail,Ops);
    SO_NODE_SET_SF_ENUM_TYPE(zFail, Ops);
    SO_NODE_SET_SF_ENUM_TYPE(zPass, Ops);
}

SoStencilBuffer::~SoStencilBuffer()
{
}

void SoStencilBuffer::initClass(void)
{
    static int first = 1;
    if (first) {
        first = 0;
        SO_NODE_INIT_CLASS(SoStencilBuffer, SoNode, "Node");
    }
}

void SoStencilBuffer::GLRender(SoGLRenderAction * action)
{
    if(!this->enable.isIgnored())
    {
        SoGLStencilBufferElement::setEnable(action->getState(), this->enable.getValue());
    }
    if(!this->function.isIgnored())
    {
        SoGLStencilBufferElement::setFunc(action->getState(),
            (SoGLStencilBufferElement::Func) this->function.getValue(),
            this->ref.getValue(), 
            this->mask.getValue());
    }
    if(!this->bits.isIgnored())
    {
        SoGLStencilBufferElement::setMask(action->getState(), this->bits.getValue());
    }
    if(!(this->fail.isIgnored() || this->zFail.isIgnored() || this->zPass.isIgnored()))
    {
        SoGLStencilBufferElement::setOps(action->getState(), 
            (SoGLStencilBufferElement::Ops) this->fail.getValue(),
            (SoGLStencilBufferElement::Ops) this->zFail.getValue(),
            (SoGLStencilBufferElement::Ops) this->zPass.getValue());
    }
    if(!this->clear.isIgnored() && this->clear.getValue())
    {
        glClearStencil(this->clearValue.getValue());
        glClear( GL_STENCIL_BUFFER_BIT );
    }
}
