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
* $Id: SoGLPolygonStippleElement.h 25 2005-11-28 16:11:59Z denis $
* @file                                                                   */
/* ======================================================================= */

#ifndef _SOGLPOLYGONSTIPPLEELEMENT_H_
#define _SOGLPOLYGONSTIPPLEELEMENT_H_

#include <Inventor/elements/SoElement.h>
#include <Inventor/elements/SoSubElement.h>
#include <Inventor/SbLinear.h> 
#ifndef __APPLE__
#include <GL/gl.h>
#else
#include <OpenGL/gl.h>
#endif
#include <stb/base/macros.h>



BEGIN_NAMESPACE_STB

class SoGLPolygonStippleElement : public SoElement 
{    
    SO_ELEMENT_HEADER(SoGLPolygonStippleElement);
public:
    static void initClass(void);
	
	/// sets the stencil mask to define to which planes to write
    static void setMask(SoState * state, GLubyte *mask);

	static void setEnable(SoState * state, bool value);

protected:
    virtual ~SoGLPolygonStippleElement();
    
public:
	virtual void init(SoState * state);

	virtual void push(SoState * state);

    virtual void pop(SoState * state, const SoElement * prevTopElement);
    
    virtual SbBool matches(const SoElement * element) const;

	virtual SoElement * copyMatchInfo(void) const;
     
    bool operator==( const SoGLPolygonStippleElement & arg ) const;
    bool operator!=( const SoGLPolygonStippleElement & arg ) const;

private:	
	enum State {
        ENABLE = 1,
        MASK   = 2
    };
	GLubyte *mask;
	bool enable;
	void updategl( uint32_t updatebits ) const;
};

inline bool SoGLPolygonStippleElement::operator==( const SoGLPolygonStippleElement & arg ) const
{
    return ( 
        this->enable   == arg.enable &&
		this->mask		== arg.mask

    );
}

inline bool SoGLPolygonStippleElement::operator!=( const SoGLPolygonStippleElement & arg ) const
{
    return !operator==(arg);
}

END_NAMESPACE_STB

#endif //
