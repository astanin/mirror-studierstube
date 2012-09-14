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
/**
*
* @author Daniel Wagner
*
* $Id$
* @file                                                                   */
/* ======================================================================= */


#ifndef _STB_SOSIMPLECOMPONENT_H_
#define _STB_SOSIMPLECOMPONENT_H_


#include "SoSimpleObject.h"
#include "SoSimpleCursor.h"


class SoSimpleComponent;
typedef void SoSimpleComponentCB(void * user, SoSimpleComponent * component);


/// Base class of SoSimpleExaminerViewer - does windowing
class SOSIMPLE_API SoSimpleComponent : public SoSimpleObject
{
	SOSIMPLE_OBJECT_ABSTRACT_HEADER(SoSimpleComponent, SoSimpleObject);

public:
	SoSimpleComponent();

	virtual void hide();
	virtual void show();

	virtual SbBool isVisible();

	void setSize(const SbVec2s size);
	SbVec2s getSize(void) const;

	virtual void setTitle(const char * const title);
	virtual const char * getTitle(void) const;

	virtual void setComponentCursor(const SoSimpleCursor & cursor);

	HWND getWidget(void) const;

	HWND getShellWidget(void) const;

	virtual void setWindowCloseCallback(SoSimpleComponentCB * const func, void * const user = NULL);

	static void initClasses(void);

protected:
	HWND hWnd;
	SbString title;

	SoSimpleComponentCB * closeCB;
	void * closeCBdata;
};


#endif //_STB_SOSIMPLECOMPONENT_H_
