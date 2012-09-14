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
/** The header file for the SoStudierstubeViewer class.
*
* @author Denis Kalkofen
*
* $Id: SoStudierstubeViewer.h 25 2005-11-28 16:11:59Z denis $
* @file                                                                   */
/* ======================================================================= */

#ifndef _SOSTUDIERSTUBEVIEWER_H_
#define _SOSTUDIERSTUBEVIEWER_H_

#include <stb/components/viewer/guiDefines.h>
#include <stb/components/viewer/ViewerDefines.h>
#include <stb/base/macros.h>
#include <stb/base/OS.h>

#include SOGUI_EXAMINERVIEWER_H

#ifdef USE_SOQT
#  include GuiWidget_H
#endif

#include <Inventor/nodes/SoDirectionalLight.h> 
#include <stb/components/video/Video.h>

#ifdef STB_IS_WINDOWS
#  include <windows.h>
#  include <time.h>
#elif defined(STB_IS_LINUX)
#ifndef __APPLE__
#  include <GL/glx.h>
#else
#  include <AGL/agl.h>
#endif
#endif

BEGIN_NAMESPACE_STB

class VIEWER_API SoStudierstubeViewer : public SoGuiExaminerViewer
{
public:
   /** The constructor of the viewer:
    *    
    */
    SoStudierstubeViewer(GuiWidget widget);
    
    /**
     * Destructor, does some clean up work.
     */
    virtual ~SoStudierstubeViewer();

	////////////////////////////////////////////
	/////////////// Head Light /////////////////
	////////////////////////////////////////////
    /**
     * Overrides inherited method to set a custom headlight.
     * @param on TRUE  turn headlight on
     *            FALSE turn headlight on
     */
    virtual void setHeadlight(SbBool on);

    /**
     * Returns whether the headlight is turned on or off.
     * @return TRUE  headlight is turned on
     * FALSE headlight is turned off
     */
    SbBool isHeadlight() const;

    void showMouse(bool mouseOn);
    /**
     *  Returns the SoDirectionalLight-instance used as headlight.
     * @return Pointer to SoDirectionalLight-instance used as headlight.
     * (NULL if headlight isn't created yet)
     */
    SoDirectionalLight *getHeadlight() const;

	//////////////////////////////////////////////////
	/////////////// WindowDecoration /////////////////
	//////////////////////////////////////////////////
    /**
     * This method turns the decorations of a window on/off.
     * @param on TRUE  turn decorations on
     *        on FALSE turn decorations off
     */
    void setWindowDecoration(SbBool on);

	/**
     * Returns whether the window decoration are turned on or off.
     * @return TRUE  decorations are turned on
     *         FALSE decorations are turned off
     */
    inline SbBool isWindowDecoration() const { return this->isWindowDecorationActive; }

	//////////////////////////////////////////////////
	/////////////// Position, Size, OnTop  ///////////
	//////////////////////////////////////////////////
    /**
     * This method sets a widget on top of all others and keeps it there.
     * @param on TRUE  set on top
     *        on FALSE keep as it is
     */
    void setWindowOnTop(SbBool on);


    /**
     * This method sets the position and the size of a window on the desktop.
     * @param x x position of the window on the desktop
     * @param y y position of the window on the desktop
     * @param width window width in pixels
     * @param height window height in pixels
     */
    void setWindowPosSize(int x, int y, int width, int height);

    //void setOVGLContext(Video* video);

    //bool isOVGLContext(){return isGLContextShared;}

    int countTriangles();

    void printTriangles(bool onOff);

    void printFrameRate(bool onOff);

protected:
	virtual void actualRedraw(void);


#ifdef STB_IS_WINDOWS
    HGLRC curGLContext;
    HDC   curDC;
#elif defined(STB_IS_LINUX)
#ifndef __APPLE__
    GLXDrawable drawable;
    Display* dsp;
    GLXContext ovGLContext;
#else
    AGLDrawable drawable;
    AGLContext ovGLContext;
#endif
#endif
	void redraw ();
    //bool isVideoGLContext;
	//bool shareGLContextWithVideo;
    //bool isGLContextShared;
    
    Video* videoComponent;
private:
	/** Stores a flag to indicate if window decorations are activated */
    SbBool isWindowDecorationActive;
	
    /** The headlight managed by the viewer */
    SoDirectionalLight *headlight;

    bool showTriangleCount;

    bool showFrameRate;
#ifdef STB_IS_WINDOWS
    DWORD frameRateLastTime, frameRateCtr;
#endif

    bool showCursor;
};

END_NAMESPACE_STB

//----------------------------------------------------------------------------
#endif//_SOSTUDIERSTUBEVIEWER_H_

