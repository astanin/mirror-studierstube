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
 * source for frame buffer node
 *
 * @author Gerhard Reitmayr
 *
 * $Id: SoFrameBuffer.cpp 8 2004-07-14 12:24:04Z tamer $
 * @file                                                                   */
/* ======================================================================= */

#include <Inventor/actions/SoGLRenderAction.h>

#include <stb/components/starlight/SoFrameBuffer.h>


SO_NODE_SOURCE(SoFrameBuffer);

SoFrameBuffer::SoFrameBuffer(void)
{
    SO_NODE_CONSTRUCTOR(SoFrameBuffer);
    
    SO_NODE_DEFINE_ENUM_VALUE(Components, NONE);
    SO_NODE_DEFINE_ENUM_VALUE(Components, RED);
    SO_NODE_DEFINE_ENUM_VALUE(Components, GREEN);
    SO_NODE_DEFINE_ENUM_VALUE(Components, BLUE);
    SO_NODE_DEFINE_ENUM_VALUE(Components, ALPHA);
    SO_NODE_DEFINE_ENUM_VALUE(Components, ALL);
    
    SO_NODE_ADD_FIELD(mask, (SoFrameBuffer::ALL));
    
    SO_NODE_SET_SF_ENUM_TYPE(mask, Components);  
}

SoFrameBuffer::~SoFrameBuffer()
{
}

void SoFrameBuffer::initClass(void)
{
    static int first = 1;
    if (first) {
        first = 0;
        SO_NODE_INIT_CLASS(SoFrameBuffer, SoNode, "Node");
    }
}

void SoFrameBuffer::GLRender(SoGLRenderAction * action)
{
    if(!this->mask.isIgnored())
        SoGLFrameBufferElement::set(action->getState(),
            (SoGLFrameBufferElement::Components) this->mask.getValue());
}
