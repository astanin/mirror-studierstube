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
 * header for stencil buffer element
 *
 * @author Gerhard Reitmayr
 *
 * $Id: SoGLStencilBufferElement.h 8 2004-07-14 12:24:04Z tamer $
 * @file                                                                   */
/* ======================================================================= */

#ifndef _SOGLSTENCILBUFFERELEMENT_H_
#define _SOGLSTENCILBUFFERELEMENT_H_

#include <Inventor/elements/SoElement.h>
#include <Inventor/elements/SoSubElement.h>

#include "starlight.h"

/**
 * This element sets all stencil buffer related parameters. It 
 * uses glStencilFunc, glStencilMask and glStencilOp to do the typical
 * stencil buffer things. See the node @ref SoStencilBuffer to use 
 * this functionality during rendering.
 *
 * @author Gerhard Reitmayr
 * @ingroup camera
 */
class STARLIGHT_API SoGLStencilBufferElement : public SoElement {
    typedef SoElement inherited;
    
    SO_ELEMENT_HEADER(SoGLStencilBufferElement);
public:
    static void initClass(void);
protected:
    virtual ~SoGLStencilBufferElement();
    
public:
 
    /// operations on the stencil buffer
    enum Ops {
        KEEP = 0,
        ZERO,
        REPLACE,
        INCR,
        DECR,
        INVERT
    };

    /// stencil buffer test functions
    enum Func {
        NEVER,
        LESS,
        LEQUAL,
        GREATER,
        GEQUAL,
        EQUAL,
        NOTEQUAL,
        ALWAYS
    };

    /// enums to keep track of changed state
    enum State {
        ENABLE = 1,
        FUNC   = 2,
        MASK   = 4,
        OP     = 8
    };

    /// enables or disables stencil buffer tests
    static void setEnable(SoState * state, const SbBool enable );

    /// sets the test function and parameters
    static void setFunc(SoState * state, const Func func, const int32_t ref, const uint32_t mask );

    /// sets the stencil mask to define to which planes to write
    static void setMask(SoState * state, const uint32_t bits );

    /// sets the operations on the stencil buffere
    static void setOps(SoState * state, const Ops fail, const Ops zfail, const Ops zpass );

    virtual void init(SoState * state);
    virtual void push(SoState * state);
    virtual void pop(SoState * state, const SoElement * prevTopElement);
    
    virtual SbBool matches(const SoElement * element) const;
    virtual SoElement * copyMatchInfo(void) const;
    
    bool operator==( const SoGLStencilBufferElement & arg ) const;
    bool operator!=( const SoGLStencilBufferElement & arg ) const;

private:
    uint32_t statebits;
    SbBool enable;
    // parameters of glStencilFunc
    Func func;
    int32_t ref;
    uint32_t mask;
    // parameters of glStencilMask
    uint32_t bits;
    // parameters of glStencilOp
    Ops fail;
    Ops zfail;
    Ops zpass;
    
    void updategl( uint32_t updatebits ) const;
};

inline bool SoGLStencilBufferElement::operator==( const SoGLStencilBufferElement & arg ) const
{
    return ( 
        this->enable == arg.enable &&
        this->func   == arg.func   &&
        this->ref    == arg.ref    &&
        this->mask   == arg.mask   &&
        this->bits   == arg.bits   &&
        this->fail   == arg.fail   &&
        this->zfail  == arg.zfail  &&
        this->zpass   == arg.zpass   &&
        this->statebits == arg.statebits
    );
}

inline bool SoGLStencilBufferElement::operator!=( const SoGLStencilBufferElement & arg ) const
{
    return !operator==(arg);
}

#endif // !_SOGLSTENCILBUFFERELEMENT_H_
