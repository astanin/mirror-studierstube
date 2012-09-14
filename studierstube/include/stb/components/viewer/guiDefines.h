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
* @author Denis Kalkofen
*
* $Id: guiDefines.h 25 2005-11-28 16:11:59Z denis $
* @file                                                                   */
/* ======================================================================= */

#ifndef _GUIDEFINES_H_
#define _GUIDEFINES_H_

#include <stb/base/OS.h>

#if defined(USE_SOQT)

#ifdef STB_IS_WINDOWS
#  define SOQT_DLL 1
#endif

#define GuiWidget_H <qwidget.h> 

#define SOGUICOMPONENT_H <Inventor/Qt/SoQtComponent.h>
#define SOGUI_H <Inventor/Qt/SoQt.h>
#define SOGUI_CURSOR_H <Inventor/Qt/SoQtCursor.h>
#define SOGUI_EXAMINERVIEWER_H <Inventor/Qt/viewers/SoQtExaminerViewer.h>

#define SoGui SoQt
#define SoGuiComponent SoQtComponent
#define GuiWidget QWidget*
#define SoGuiCursor SoQtCursor
#define SoGuiViewer SoQtViewer
#define SoGuiFullViewer SoQtFullViewer
#define SoGuiRenderArea SoQtRenderArea
#define SoGuiExaminerViewer SoQtExaminerViewer

#elif defined(USE_SOWIN)

#ifdef STB_IS_WINDOWS
#  define SOWIN_DLL 1
#endif

#define SOGUICOMPONENT_H <Inventor/Win/SoWinComponent.h>
#define SOGUI_H <Inventor/Win/SoWin.h>
#define SOGUI_CURSOR_H <Inventor/Win/SoWinCursor.h>
#define SOGUI_EXAMINERVIEWER_H <Inventor/Win/viewers/SoWinExaminerViewer.h>

#define SoGui SoWin
#define SoGuiComponent SoWinComponent
#define GuiWidget HWND
#define SoGuiCursor SoWinCursor
#define SoGuiViewer SoWinViewer
#define SoGuiFullViewer SoWinFullViewer
#define SoGuiRenderArea SoWinRenderArea
#define SoGuiExaminerViewer SoWinExaminerViewer

/*#elif USE_SOGL

#define SOGUICOMPONENT_H <stb/sogl/SoGLComponent.h>
#define SOGUI_H <stb/sogl/SoGL.h>
#define SOGUI_CURSOR_H <stb/sogl/SoGLCursor.h>
#define SOGUI_EXAMINERVIEWER_H <stb/sogl/SoGLExaminerViewer.h>

#define SoGui SoGL
#define SoGuiComponent SoGLComponent
#define GuiWidget HWND
#define SoGuiCursor SoGLCursor
#define SoGuiViewer SoGLViewer
#define SoGuiFullViewer SoGLFullViewer
#define SoGuiRenderArea SoGLRenderArea
#define SoGuiExaminerViewer SoGLExaminerViewer*/

#elif defined(USE_SOSIMPLE)

#define SOGUICOMPONENT_H <SoSimple/SoSimpleExaminerViewer.h>
#define SOGUI_H <SoSimple/SoSimple.h>
#define SOGUI_CURSOR_H <SoSimple/SoSimpleCursor.h>
#define SOGUI_EXAMINERVIEWER_H <SoSimple/SoSimpleExaminerViewer.h>

#define SoGui SoSimple
#define SoGuiComponent SoSimpleComponent
#define GuiWidget HWND
#define SoGuiCursor SoSimpleCursor
#define SoGuiViewer SoSimpleViewer
#define SoGuiFullViewer SoSimpleFullViewer
#define SoGuiRenderArea SoSimpleRenderArea
#define SoGuiExaminerViewer SoSimpleExaminerViewer

#else // no window binding defined

#  error "ERROR: please define one of the following USE_SOQT, USE_SOWIN, USE_SOGL"

#endif // end of windows bindings

#endif //GUIDEFINES_H
