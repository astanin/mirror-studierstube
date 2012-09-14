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


#include "SoSimpleExaminerViewer.h"
#include "PlatformTools.h"


SOSIMPLE_OBJECT_ABSTRACT_SOURCE(SoSimpleComponent);


SoSimpleComponent::SoSimpleComponent()
{
	closeCB = NULL;
	closeCBdata = NULL;
}


void
SoSimpleComponent::hide()
{
	::ShowWindow(hWnd, SW_HIDE);
}


void
SoSimpleComponent::show()
{
	::ShowWindow(hWnd, SW_SHOW);
}


SbBool
SoSimpleComponent::isVisible()
{
	return ::IsWindowVisible(hWnd);
}


void
SoSimpleComponent::setSize(const SbVec2s size)
{
	SoSimple::setWidgetSize(hWnd, size);
}


SbVec2s
SoSimpleComponent::getSize(void) const
{
	RECT rect;
	::GetWindowRect(hWnd, & rect);
	return SbVec2s((short)(rect.right - rect.left), (short)(rect.bottom - rect.top));
}


void
SoSimpleComponent::setComponentCursor(const SoSimpleCursor & cursor)
{
	// ignored
}


void
SoSimpleComponent::setTitle(const char * const nTitle)
{
	title = nTitle ? nTitle : "";

	::SetWindowText(hWnd, CHAR_TO_NATIVE(title.getString()));
}


const char*
SoSimpleComponent::getTitle(void) const
{
	return title.getString();
}


HWND
SoSimpleComponent::getWidget(void) const
{
	return hWnd;
}



HWND
SoSimpleComponent::getShellWidget(void) const
{
	return hWnd;
}


void
SoSimpleComponent::setWindowCloseCallback(SoSimpleComponentCB * const func, void * const user)
{
	closeCB = func;
	closeCBdata = user;
}


void
SoSimpleComponent::initClasses(void)
{
	SoSimpleComponent::initClass();
	SoSimpleViewer::initClass();
	SoSimpleFullViewer::initClass();
	SoSimpleExaminerViewer::initClass();
}
