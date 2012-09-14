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
/** The header file for the guiDefines.
*
* @author Daniel Wagner
*
* $Id$
* @file                                                                   */
/* ======================================================================= */


#ifndef _SOSIMPLE_SOSIMPLE_H
#define _SOSIMPLE_SOSIMPLE_H


#include "main.h"
#include "SoSimple.h"

#include <Inventor/SbVec2s.h>
#include <Inventor/SbString.h>


class SOSIMPLE_API SoSimple {
public:
	static void init();
	static void done();
	static void doStbCallback();

	static void exitMainLoop();

	static void setWidgetSize(HWND const widget, const SbVec2s size);
	static HWND getShellWidget(const HWND w);
	static HWND getTopLevelWidget();

	static HWND createMainWindow(WNDPROC nWindowFunc);

	static LRESULT onDestroy(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
	static LRESULT onQuit(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
};


#define SOSIMPLE_STATIC_SOTYPE_INIT = SoType::badType()


#endif //_SOSIMPLE_SOSIMPLE_H
