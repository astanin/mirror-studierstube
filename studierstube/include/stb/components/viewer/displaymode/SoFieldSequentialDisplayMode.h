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
/** The header file for the SoFieldSequentialDisplayMode class.
*
* @author Denis Kalkofen
*
* $Id: SoFieldSequentialDisplayMode.h 25 2005-11-28 16:11:59Z denis $
* @file                                                                   */
/* ======================================================================= */

#ifndef _SoFIELDSEQUENTIALDISPLAYMODE_H_
#define _SoFIELDSEQUENTIALDISPLAYMODE_H_

#include <stb/components/viewer/displaymode/SoDisplayMode.h>
#include <stb/base/macros.h>
#include <stb/base/OS.h>

#ifdef STB_IS_WINDOWS
#  define WIN32_LEAN_AND_MEAN
#  include <windows.h>
#endif

#ifndef __APPLE__
#include <GL/gl.h>
#else
#include <OpenGL/gl.h>
#endif

#include <Inventor/fields/SoSFBool.h> 
#include <Inventor/fields/SoSFEnum.h> 

BEGIN_NAMESPACE_STB

class  SoFieldSequentialDisplayMode : public SoDisplayMode
{
    SO_NODE_HEADER(SoFieldSequentialDisplayMode);

public:
    /** Inventor class initialization */
    static void initClass();

    /** The constructor inits the field of the node */
    SoFieldSequentialDisplayMode();

    enum BufferType {
        LEFT   = 0,
	    RIGHT };

  
    SoSFEnum buffer;

protected:
	
	virtual void GLRender(SoGLRenderAction *action);

	~SoFieldSequentialDisplayMode();

    virtual bool activate();

	virtual void setViewer(SoStudierstubeViewer* aViewer);

	static bool isQuadBufferAvailable;

	static bool isInit;
};

END_NAMESPACE_STB

//----------------------------------------------------------------------------
#endif//

