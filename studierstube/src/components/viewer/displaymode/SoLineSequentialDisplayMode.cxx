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
/** The header file for the SoLineSequentialDisplayMode class.
*
* @author Denis Kalkofen
*
* $Id: SoLineSequentialDisplayMode.cxx 25 2005-11-28 16:11:59Z denis $
* @file                                                                   */
/* ======================================================================= */

#include <stb/components/viewer/displaymode/SoLineSequentialDisplayMode.h>
#include <stb/base/OS.h>

#ifdef STB_IS_WINDOWS
#  define WIN32_LEAN_AND_MEAN
#  include <windows.h>
#endif

#include <stb/components/viewer/displaymode/SoGLPolygonStippleElement.h>

BEGIN_NAMESPACE_STB


SO_NODE_SOURCE(SoLineSequentialDisplayMode);

//----------------------------------------------------------------------------
// Inventor class initialization.

void
SoLineSequentialDisplayMode::initClass()
{
    SO_NODE_INIT_CLASS(SoLineSequentialDisplayMode, SoDisplayMode, "SoDisplayMode");
}

//----------------------------------------------------------------------------
//mask data structures for line interleaved rendering
static GLubyte evenmask[]= 
{
    0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff,
    0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff,
    0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff,
    0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff,
    0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff,
    0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff,
    0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff,
    0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff,
    0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff,
    0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff,
    0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff,
    0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff,
    0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff,
    0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff,
    0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff,
    0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff
};

static GLubyte oddmask[] = 
{
    0xff, 0xff, 0xff, 0xff,
    0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff,
    0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff,
    0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff,
    0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff,
    0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff,
    0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff,
    0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff,
    0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff,
    0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff,
    0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff,
    0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff,
    0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff,
    0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff,
    0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff,
    0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff,
    0x00, 0x00, 0x00, 0x00
};

//----------------------------------------------------------------------------
// The constructor inits the field of the node.

SoLineSequentialDisplayMode::SoLineSequentialDisplayMode()
{
    SO_NODE_CONSTRUCTOR(SoLineSequentialDisplayMode);
	SO_NODE_ADD_FIELD(lines, (EVEN)); 

	SO_NODE_DEFINE_ENUM_VALUE(LineType, EVEN);
    SO_NODE_DEFINE_ENUM_VALUE(LineType, ODD);
    
    SO_NODE_SET_SF_ENUM_TYPE(lines, LineType);
}

//----------------------------------------------------------------------------

SoLineSequentialDisplayMode::~SoLineSequentialDisplayMode()
{
    // nil
}

void 
SoLineSequentialDisplayMode::GLRender(SoGLRenderAction *action)
{
	if(lines.getValue()==EVEN)
		SoGLPolygonStippleElement::setMask(action->getState(),evenmask);
	else
		SoGLPolygonStippleElement::setMask(action->getState(),oddmask);

	SoGLPolygonStippleElement::setEnable(action->getState(),true);
}

END_NAMESPACE_STB

//----------------------------------------------------------------------------
//eof

/* ===========================================================================
    End of SoLineSequentialDisplayMode.cxx
   ===========================================================================
    Automatic Emacs configuration follows.
    Local Variables:
    mode:c++
    c-basic-offset: 4
    eval: (c-set-offset 'substatement-open 0)
    eval: (c-set-offset 'case-label '+)
    eval: (c-set-offset 'statement 'c-lineup-runin-statements)
    eval: (setq indent-tabs-mode nil)
    End:
=========================================================================== */
