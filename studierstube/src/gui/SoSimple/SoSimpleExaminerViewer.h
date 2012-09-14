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


#ifndef _STB_SOSIMPLEEXAMINERVIEWER_H_
#define _STB_SOSIMPLEEXAMINERVIEWER_H_

#include "SoSimpleComponent.h"
#include "SoSimpleFullViewer.h"
#include "SoSimpleViewer.h"

#include <Inventor/SbColor.h>
#include <Inventor/SoSceneManager.h>
#include <Inventor/nodes/SoNode.h> 
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/actions/SoGLRenderAction.h>

#include <gl/gl.h>


class SoSimpleDevice;
class SoSimpleMouse;
class SoSimpleKeyboard;


class SOSIMPLE_API SoSimpleExaminerViewer : public SoSimpleFullViewer
{
	SOSIMPLE_OBJECT_HEADER(SoSimpleExaminerViewer, SoSimpleFullViewer);

public:
	SoSimpleExaminerViewer(HWND parent = NULL, const char * name = NULL, SbBool embed = TRUE, SoSimpleFullViewer::BuildFlag flag = BUILD_ALL, SoSimpleViewer::Type type = BROWSER);
	~SoSimpleExaminerViewer();

	virtual SbBool isDecoration();
	virtual void setDecoration(const SbBool);

	virtual void setTransparencyType(SoGLRenderAction::TransparencyType type);

	virtual void setBackgroundColor(const SbColor & color);
	virtual const SbColor & getBackgroundColor(void) const;

	virtual void setClearBeforeRender(SbBool enable, SbBool zbEnable=TRUE);

	virtual void setSceneGraph(SoNode * root);
	virtual SoNode *getSceneGraph(void);

	virtual SoSceneManager* getSceneManager(void) const  {  return sceneManager;  }

	virtual void redraw();
	virtual void render();

	virtual void destroy();

	void registerDevice(SoSimpleDevice* device);
	void unregisterDevice(SoSimpleDevice* device);

	// dummy methods that are ignored...
	//
	virtual void glLockNormal(void);
	virtual void glUnlockNormal(void);
	virtual void setQuadBufferStereo(const SbBool enable);
	virtual SbBool isQuadBufferStereo(void) const;
	virtual void setStencilBuffer(const SbBool enable);
	virtual SbBool getStencilBuffer(void) const;
	virtual void setAlphaChannel(const SbBool enable);
	virtual SbBool getAlphaChannel(void) const;
	virtual void setViewing(const SbBool enable);

	void processEvent(MSG * event);

protected:
	virtual void actualRedraw();
	void createGLContext(HWND window);
	void setDevicesWindowSize(const SbVec2s size);

	SbBool invokeAppCB(MSG * event);
	const SoEvent* getSoEvent(MSG* event);
	static LRESULT eventHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

	SbBool clear;
	SbBool clearZBuffer;
	HDC hDC;
	HGLRC hRC;

	SoSceneManager*		sceneManager;
	SoSeparator*		sceneroot;
	SoNode*				scenegraph;
	SoDirectionalLight*	headlight;

	SbPList*			devicelist;
	SoSimpleMouse*		mouse;
	SoSimpleKeyboard*	keyboard;
};


#endif //_STB_SOSIMPLEEXAMINERVIEWER_H_
