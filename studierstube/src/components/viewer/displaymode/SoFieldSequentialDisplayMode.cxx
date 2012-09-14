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
* $Id: SoFieldSequentialDisplayMode.cxx 25 2005-11-28 16:11:59Z denis $
* @file                                                                   */
/* ======================================================================= */

#include <stb/components/viewer/displaymode/SoFieldSequentialDisplayMode.h>
#include <stb/base/OS.h>

#ifdef STB_IS_WINDOWS
#  define WIN32_LEAN_AND_MEAN
#  include <windows.h>
#endif
#include <stb/kernel/StbLogger.h>
#include <stb/components/viewer/Viewer.h>
#include <stb/components/viewer/SoStudierstubeViewer.h>
#include <stb/components/viewer/displaymode/SoFieldSequentialDisplayMode.h>
#include <stb/components/viewer/SoDisplay.h>
#include <stb/components/viewer/SoStudierstubeViewer.h>

BEGIN_NAMESPACE_STB

bool SoFieldSequentialDisplayMode::isInit=false;
bool SoFieldSequentialDisplayMode::isQuadBufferAvailable=false;

SO_NODE_SOURCE(SoFieldSequentialDisplayMode);

//----------------------------------------------------------------------------
// Inventor class initialization.

void
SoFieldSequentialDisplayMode::initClass()
{
    SO_NODE_INIT_CLASS(SoFieldSequentialDisplayMode, SoDisplayMode, "SoDisplayMode");
}

//----------------------------------------------------------------------------
// The constructor inits the field of the node.

SoFieldSequentialDisplayMode::SoFieldSequentialDisplayMode()
{
    SO_NODE_CONSTRUCTOR(SoFieldSequentialDisplayMode);
    
    SO_NODE_ADD_FIELD(buffer, (LEFT)); 
    
    SO_NODE_DEFINE_ENUM_VALUE(BufferType, LEFT);
    SO_NODE_DEFINE_ENUM_VALUE(BufferType, RIGHT);
    
    SO_NODE_SET_SF_ENUM_TYPE(buffer, BufferType);
    
	isInit=false;
}

//----------------------------------------------------------------------------

SoFieldSequentialDisplayMode::~SoFieldSequentialDisplayMode()
{
    // nil
}

void 
SoFieldSequentialDisplayMode::setViewer(SoStudierstubeViewer* aViewer)
{
    printf("SoFieldSequentialDisplayMode::setViewer\n");
	SoDisplayMode::setViewer(aViewer);
	if(!SoFieldSequentialDisplayMode::isInit){
		SoFieldSequentialDisplayMode::isInit=true;		
		if(viewer->isQuadBufferStereo()){
			isQuadBufferAvailable=true;
		}
		else{            
			viewer->setQuadBufferStereo(false);
            stb::logPrintW("SoFieldSequentialDisplayMode :: no quadbuffer found");
		}
	}
}

void 
SoFieldSequentialDisplayMode::GLRender(SoGLRenderAction * /*action*/)
{
#ifdef _IS_KLIMTES_
	assert(false && "Not supported with OpenGL ES !!!");
#else
    if(!activated)
    {
        activated=activate();
    }

	if(isQuadBufferAvailable)
	{
		glFlush();
		switch(buffer.getValue())//select new buffer
		{
			case LEFT:
				glDrawBuffer(GL_BACK_LEFT);
				break;
			case RIGHT:
				glDrawBuffer(GL_BACK_RIGHT);
				break;
		}
		//glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);//clear buffers
		glFlush();//force execution of gl commands
	}
#endif
}

bool 
SoFieldSequentialDisplayMode::activate()
{
    SoDisplay *dsp=Viewer::findSoDisplay(this);
    if(!dsp){
        return false;
    }
    
    setViewer(dsp->getViewer());

    return true;
}

END_NAMESPACE_STB
