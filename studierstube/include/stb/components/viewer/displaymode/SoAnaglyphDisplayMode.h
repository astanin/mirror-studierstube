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
/** The header file for the SoAnaglyphDisplay class.
*
* @author Denis Kalkofen
*
* $Id: SoAnaglyphDisplay.h 25 2005-11-28 16:11:59Z denis $
* @file                                                                   */
/* ======================================================================= */

#ifndef _SOANAGLYPHDISPLAYMODE_H_
#define _SOANAGLYPHDISPLAYMODE_H_

#include <Inventor/fields/SoSFColor.h>

#include <stb/components/viewer/displaymode/SoDisplayMode.h>

BEGIN_NAMESPACE_STB

class  SoAnaglyphDisplayMode : public SoDisplayMode
{
    SO_NODE_HEADER(SoAnaglyphDisplayMode);


public:
    /** Inventor class initialization */
    static void initClass();

    /** The constructor inits the field of the node */
    SoAnaglyphDisplayMode( );

	~SoAnaglyphDisplayMode( );

    /** Specifys the color of the left eye lens (RGB).
        Default value is (1,0,0) = RED
        @warning all RGB-values < 1 will be interpreted as 0
                 (hence only 0 and 1 are useful values)
    */      
    SoSFColor lensColor;

	void setLensColor(SbColor newColor);

protected:
	virtual void GLRender(SoGLRenderAction *action);

};

END_NAMESPACE_STB

//----------------------------------------------------------------------------
#endif//_SOANAGLYPHDISPLAYMODE_H_

