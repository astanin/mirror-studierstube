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
/** The header file for the SoGLPolygonStippleElement class.
*
* @author Denis Kalkofen
*
* $Id: SoGLPolygonStippleElement.cxx 25 2005-11-28 16:11:59Z denis $
* @file                                                                   */
/* ======================================================================= */

#include <stb/base/OS.h>

#ifdef STB_IS_WINDOWS
#  define WIN32_LEAN_AND_MEAN
#  include <windows.h>
#endif

#include <Inventor/actions/SoGLRenderAction.h>
#include <stb/components/viewer/displaymode/SoGLPolygonStippleElement.h>

BEGIN_NAMESPACE_STB


SO_ELEMENT_SOURCE(SoGLPolygonStippleElement);

void SoGLPolygonStippleElement::initClass(void)
{
    static int first = 1;
    if (first) {
        first = 0;
        SO_ELEMENT_INIT_CLASS(SoGLPolygonStippleElement, SoElement);
        SO_ENABLE(SoGLRenderAction, SoGLPolygonStippleElement);
    }
}

SoGLPolygonStippleElement::~SoGLPolygonStippleElement(void)
{
	delete mask;
}

void 
SoGLPolygonStippleElement::init(SoState * state)
{
    SoElement::init(state);
	this->enable=false;
	this->mask=new GLubyte[32*32];
}

void SoGLPolygonStippleElement::push(SoState * state)
{
    SoGLPolygonStippleElement * prev = (SoGLPolygonStippleElement*)this->getNextInStack();
	this->enable= prev->enable;
    this->mask = prev->mask;
    prev->capture(state);
}

void SoGLPolygonStippleElement::pop(SoState * /*state*/, const SoElement * prevTopElement)
{
    SoGLPolygonStippleElement * prev = (SoGLPolygonStippleElement*)prevTopElement;
    if ( (*this) != (*prev)) {
        this->updategl( ENABLE | MASK );
    }
}
SbBool 
SoGLPolygonStippleElement::matches(const SoElement * element) const
{
    const SoGLPolygonStippleElement * elem = (const SoGLPolygonStippleElement*) element;
    return ((*this) == (*elem)); 
}

SoElement * 
SoGLPolygonStippleElement::copyMatchInfo(void) const
{
    SoGLPolygonStippleElement * elem = (SoGLPolygonStippleElement*)
    this->getTypeId().createInstance();
  
    return elem;
}

void SoGLPolygonStippleElement::updategl( uint32_t updatebits ) const
{
	if( updatebits & ENABLE )
    {
        if (this->enable)
            glEnable(GL_POLYGON_STIPPLE);
        else
            glDisable(GL_POLYGON_STIPPLE);
    }
	if( updatebits & MASK )
	{
		glPolygonStipple(mask);
	}
}
  
void SoGLPolygonStippleElement::setEnable(SoState * state, bool value)
{
	SoGLPolygonStippleElement * elem = (SoGLPolygonStippleElement*)SoElement::getElement(state, classStackIndex);
    
    if( value != elem->enable )
    {
        elem->enable = value;
        elem->updategl(ENABLE);
    }
}

void 
SoGLPolygonStippleElement::setMask(SoState * state,GLubyte *newMask)
{
	SoGLPolygonStippleElement * elem = (SoGLPolygonStippleElement*)SoElement::getElement(state, classStackIndex);
    
    if( newMask != elem->mask )
    {
        elem->mask = newMask;
        elem->updategl(MASK);
    }
}

END_NAMESPACE_STB
