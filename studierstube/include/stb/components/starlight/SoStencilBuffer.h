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
 * header for stencil buffer node
 *
 * @author Gerhard Reitmayr
 *
 * $Id: SoStencilBuffer.h 8 2004-07-14 12:24:04Z tamer $
 * @file                                                                   */
/* ======================================================================= */ 

#ifndef _SOSTENCILBUFFER_H_
#define _SOSTENCILBUFFER_H_

#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/nodes/SoSubNode.h>

#include "SoGLStencilBufferElement.h"

/**
 * This node sets the parameters of the stencil buffer. See the
 * relevant OpenGL functions for more details. You will have to activate
 * stencil buffer support in the viewer by setting @ref SoDisplayKit::stencilBuffer 
 * TRUE in the @ref SoDisplayKit.
 *
 * The stencil test is disabled by default and this node will turn on stencil tests by default. 
 * Therefore it is possible to have several instances without always specifying enable TRUE. 
 *
 * Note, that the whole element handling is probably not optimal yet. If in doubt, specify more
 * parameters with each instance you use to make sure you get the right state.
 *
 * File format syntax / defaults:
@verbatim
SoStencilBuffer {
    SoSFBool    enable   TRUE      # enable stencil buffer test

    SoSFEnum    function ALWAYS    # (NEVER|LESS|LEQUAL|GREATER|GEQUAL|EQUAL|NOTEQUAL|ALWAYS) stencil test function
    SoSFInt32   ref      0         # reference value for test
    SoSFInt32   mask     0         # mask for test

    SoSFInt32   bits     255       # allowed bits to write to

    SoSFEnum    fail     KEEP      # (KEEP|ZERO|REPLACE|INCR|DECR|INVERT) operation on failure of stencil test
    SoSFEnum    zFail    KEEP      # (KEEP|ZERO|REPLACE|INCR|DECR|INVERT) operation on failure of depth test
    SoSFEnum    zPass    KEEP      # (KEEP|ZERO|REPLACE|INCR|DECR|INVERT) operation on passing all tests

    SoSFBool    clear    FALSE     # initialize stencil buffer, if this node is traversed
    SoSFInt32   clearValue 0       # initialize stencil buffer with this value
}
@endverbatim
 *
 * @author Gerhard Reitmayr
 * @ingroup camera
 */
class STARLIGHT_API SoStencilBuffer : public SoNode {
    
    SO_NODE_HEADER(SoStencilBuffer);
    
public:

    /// static class initialization for Open Inventor framework
    static void initClass(void);

    /// constructor
    SoStencilBuffer(void);

    /// possible operations on the stencil buffer
    enum Ops {
        KEEP = SoGLStencilBufferElement::KEEP,
            ZERO = SoGLStencilBufferElement::ZERO,
            REPLACE = SoGLStencilBufferElement::REPLACE,
            INCR = SoGLStencilBufferElement::INCR,
            DECR = SoGLStencilBufferElement::DECR,
            INVERT = SoGLStencilBufferElement::INVERT
    };
    
    /// possible test functions
    enum Func {
        NEVER = SoGLStencilBufferElement::NEVER,
            LESS = SoGLStencilBufferElement::LESS,
            LEQUAL = SoGLStencilBufferElement::LEQUAL,
            GREATER = SoGLStencilBufferElement::GREATER,
            GEQUAL = SoGLStencilBufferElement::GEQUAL,
            EQUAL = SoGLStencilBufferElement::EQUAL,
            NOTEQUAL = SoGLStencilBufferElement::NOTEQUAL,
            ALWAYS = SoGLStencilBufferElement::ALWAYS
    };
    
    /// enable stencil buffer test
    SoSFBool enable;

    /// stencil buffer function to use
    SoSFEnum function;

    /// stencil buffer compare value
    SoSFInt32 ref;

    /// mask for stencil function
    SoSFInt32 mask;

    /// which stencil planes to use
    SoSFInt32 bits;
    
    /// operation on stencil test failure
    SoSFEnum fail;

    /// operation on z test failure
    SoSFEnum zFail;

    /// operation z test pass
    SoSFEnum zPass;

    /// value to initialize or clear the stencil buffer with
    SoSFInt32 clearValue;        

    /// clear buffer during traversal
    SoSFBool clear;

    virtual void GLRender(SoGLRenderAction * action);
    
protected:
    virtual ~SoStencilBuffer();
};

#endif // !_SOSTENCILBUFFER_H_
