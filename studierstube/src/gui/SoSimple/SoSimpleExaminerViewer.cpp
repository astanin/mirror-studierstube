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
#include "SoSimpleMouse.h"
#include "SoSimpleKeyboard.h"


SOSIMPLE_OBJECT_ABSTRACT_SOURCE(SoSimpleExaminerViewer);


static void renderCB(void * user, SoSceneManager * manager);
static LRESULT CALLBACK windowFunc(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
static HWND createWindow();


SoSimpleExaminerViewer::SoSimpleExaminerViewer(HWND parent, const char * name, SbBool embed, SoSimpleFullViewer::BuildFlag flag, SoSimpleViewer::Type type)
{
	hWnd = SoSimple::createMainWindow(windowFunc);
	::SetWindowLong(hWnd, GWL_USERDATA, (LONG)this);

	hDC = NULL;
	hRC = NULL;

	createGLContext(hWnd);

	clear = TRUE;
	clearZBuffer = TRUE;

	devicelist = new SbPList;

	mouse = new SoSimpleMouse;
	registerDevice(mouse);

	keyboard = new SoSimpleKeyboard;
	registerDevice(keyboard);

	sceneroot = new SoSeparator;
	sceneroot->setName("sosimple->sceneroot");

	headlight = new SoDirectionalLight;
	headlight->setName("sosimple->headlight");
	headlight->direction.setValue(1.0f, -1.0f, -10.0f);
	sceneroot->addChild(headlight);

	sceneManager = new SoSceneManager;
	sceneManager->setRenderCallback(renderCB, this);
	sceneManager->activate();
	sceneManager->setSceneGraph(sceneroot);

	scenegraph = NULL;
}


SoSimpleExaminerViewer::~SoSimpleExaminerViewer()
{
	delete sceneManager;
}


void
SoSimpleExaminerViewer::setClearBeforeRender(SbBool enable, SbBool zbEnable)
{
	clear = enable;
	clearZBuffer = zbEnable;
}


SbBool
SoSimpleExaminerViewer::isDecoration()
{
	return FALSE;
}


void
SoSimpleExaminerViewer::setDecoration(const SbBool)
{

}


void
SoSimpleExaminerViewer::setStencilBuffer(const SbBool enable)
{
	// ignored
}


SbBool
SoSimpleExaminerViewer::getStencilBuffer(void) const
{
	// ignored
	return FALSE;
}


void
SoSimpleExaminerViewer::setAlphaChannel(const SbBool enable)
{
	// ignored
}


SbBool
SoSimpleExaminerViewer::getAlphaChannel(void) const
{
	// ignored
	return FALSE;
}

void
SoSimpleExaminerViewer::setViewing(const SbBool enable)
{
	// ignored
}

void
SoSimpleExaminerViewer::setTransparencyType(SoGLRenderAction::TransparencyType type)
{
	sceneManager->getGLRenderAction()->setTransparencyType(type);
}


void
SoSimpleExaminerViewer::setBackgroundColor(const SbColor & color)
{
	sceneManager->setBackgroundColor(color);
}


const SbColor&
SoSimpleExaminerViewer::getBackgroundColor(void) const
{
	return sceneManager->getBackgroundColor();
}


void
SoSimpleExaminerViewer::setSceneGraph(SoNode* root)
{
	if(scenegraph)
		sceneroot->removeChild(scenegraph);

	sceneroot->addChild(root);
	scenegraph = root;

	sceneManager->scheduleRedraw();
}


SoNode*
SoSimpleExaminerViewer::getSceneGraph(void)
{
	return scenegraph;
}


void
SoSimpleExaminerViewer::setDevicesWindowSize(const SbVec2s size)
{
	if(!devicelist)
		return;
	const int num = devicelist->getLength();
	for (int i = 0; i < num; i++)
		((SoSimpleDevice*)(*devicelist)[i])->setWindowSize(size);
}


void
SoSimpleExaminerViewer::glLockNormal(void)
{
	// ignored
}


void
SoSimpleExaminerViewer::glUnlockNormal(void)
{
	// ignored
}


void
SoSimpleExaminerViewer::setQuadBufferStereo(const SbBool enable)
{
	// ignored
}


SbBool
SoSimpleExaminerViewer::isQuadBufferStereo(void) const
{
	// ignored
	return FALSE;
}


void
SoSimpleExaminerViewer::actualRedraw()
{
	assert(sceneManager);
	if(!isVisible())
		return;
	sceneManager->render(clear, clearZBuffer);
}



void
SoSimpleExaminerViewer::redraw()
{
	if(!isVisible())
		return;

	//sceneManager->setBackgroundColor(SbColor(1.0f, 0.0f, 0.0f));

	glLockNormal();
	actualRedraw();
	::SwapBuffers(hDC);
	glUnlockNormal();
}


void
SoSimpleExaminerViewer::render(void)
{
	redraw();
}


void
SoSimpleExaminerViewer::createGLContext(HWND window)
{
	hDC = ::GetDC(window);

	static	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		16,											// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// 16Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};

	GLuint		PixelFormat;			// Holds The Results After Searching For A Match

	if(!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	// Did Windows Find A Matching Pixel Format?
		return;

	if(!SetPixelFormat(hDC,PixelFormat,&pfd))		// Are We Able To Set The Pixel Format?
		return;

#ifdef STB_IS_WINCE
	klesSetScreenOrientation(KLIMTES_SCREEN_90CW);
#endif

	if(!(hRC = wglCreateContext(hDC)))
		return;

	if(!wglMakeCurrent(hDC, hRC))
		return;

	glEnable(GL_DEPTH_TEST);
}


/*void
SoSimpleExaminerViewer::setEventCallback(SoSimpleRenderAreaEventCB* func, void * user)
{
	appeventhandler = func;
	appeventhandlerdata = user;
}*/


SbBool
SoSimpleExaminerViewer::invokeAppCB(MSG * event)
{
	// currently not supported
	//

	//if(appeventhandler!=NULL)
	//	return appeventhandler(appeventhandlerdata, event);
	return FALSE;
}


const SoEvent*
SoSimpleExaminerViewer::getSoEvent(MSG * event)
{
	if(!devicelist)
		return (SoEvent*)NULL;

	const SoEvent* soevent = NULL;
	const int num = devicelist->getLength();

	for(int i=0; (i<num) && (soevent==NULL); i++)
		soevent = ((SoSimpleDevice*)(*this->devicelist)[i])->translateEvent(event);

	return soevent;
}


void
SoSimpleExaminerViewer::registerDevice(SoSimpleDevice* device)
{
	int idx = devicelist->find(device);
	if (idx != -1)
		return;

	devicelist->append(device);
	device->enable(hWnd, &SoSimpleExaminerViewer::eventHandler, (void*)this);
	device->setWindowSize(getSize());
}


void
SoSimpleExaminerViewer::unregisterDevice(SoSimpleDevice* device)
{
	assert(devicelist);

	const int idx = devicelist->find(device);
	if(idx == -1)
		return;

	devicelist->remove(idx);
	device->disable(hWnd, NULL, NULL);
}


LRESULT
SoSimpleExaminerViewer::eventHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// FIXME: the function gets registered and all in SoWinRenderArea,
	// so why does event handling differ in SoWin versus SoXt and SoQt?
	// 20030411 mortene.
	assert(FALSE && "should not be in use");
	return 0;
}


void
SoSimpleExaminerViewer::processEvent(MSG * event)
{
	if(invokeAppCB(event))
		return;

	const SoEvent* soevent = getSoEvent(event);

	if (soevent != NULL) {
		SbBool processed = sceneManager->processEvent(soevent);
		if(processed)
			return;
	}

	//inherited::processEvent(event);
}


void
renderCB(void * closure, SoSceneManager * manager)
{
	assert(closure && manager);
	SoSimpleExaminerViewer *thisptr = (SoSimpleExaminerViewer*) closure;
	if(manager == thisptr->getSceneManager()) {
		thisptr->render();
	} else {
#if SOWIN_DEBUG
		SoDebugError::post("SoWinRenderAreaP::renderCB",
			"invoked for unknown SoSceneManager (%p)", manager);
#endif // SOWIN_DEBUG
		manager->setRenderCallback(NULL, NULL);
		return;
	}

	//if (!thisptr->isAutoRedraw())
	//	manager->setRenderCallback(NULL, NULL);
}

void SoSimpleExaminerViewer::destroy() {
	wglDeleteContext(hRC);
	DestroyWindow(hWnd);
}

static LRESULT CALLBACK
windowFunc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
	LRESULT retval = 0;
	BOOL handled = FALSE;

	switch (message) {
		case WM_DESTROY:
			retval = SoSimple::onDestroy(window, message, wparam, lparam);
			handled = TRUE;
			break;

		case WM_QUIT:
			retval = SoSimple::onQuit(window, message, wparam, lparam);
			if(SoSimpleExaminerViewer* viewer = (SoSimpleExaminerViewer*)::GetWindowLong(window, GWL_USERDATA))
			{
				viewer->destroy();
			}
			handled = TRUE;
			break;

		case WM_KEYDOWN:
			SoSimple::exitMainLoop();
			if(SoSimpleExaminerViewer* viewer = (SoSimpleExaminerViewer*)::GetWindowLong(window, GWL_USERDATA))
			{
				viewer->destroy();
			}
			handled = TRUE;
			break;


	}

	
	if(SoSimpleExaminerViewer* viewer = (SoSimpleExaminerViewer*)::GetWindowLong(window, GWL_USERDATA))
	{
		MSG msg;

		msg.hwnd = window;
		msg.message = message;
		msg.lParam = lparam;
		msg.wParam = wparam;
		msg.time = ::GetTickCount();
		msg.pt.x = 88;
		msg.pt.x = 99;

		viewer->processEvent(&msg);
	}


	if (handled)
		return retval;

	return DefWindowProc(window, message, wparam, lparam);
}
