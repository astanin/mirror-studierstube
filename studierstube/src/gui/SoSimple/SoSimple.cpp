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


#include "SoSimple.h"
#include "SoSimpleObject.h"

#include <Inventor/errors/SoError.h>
#include <Inventor/errors/SoDebugError.h> 
#include <Inventor/nodekits/SoNodeKit.h> 
#include <Inventor/SoInteraction.h> 
#include <Inventor/SoDB.h>
#include <Inventor/SbTime.h> 
#include <Inventor/sensors/SoSensor.h>

#include "PlatformTools.h"
#include <TCHAR.h>


#ifndef _WIN32_WCE
//#define USE_ORIGINAL_MAINLOOP
#endif

static SbString gAppName, gClassName;
static HWND gHWND = NULL;

static int timerSensorId = 0;
static int idleSensorId = 0;
static int delaySensorId = 0;

static bool pipeErrorMessagesToConsole = true;

static SoSensorCB* manualCallBack = NULL;


static void doIdleTasks(void);

static void CALLBACK timerSensorCB(HWND window, UINT message, UINT idevent, DWORD dwtime);
static void CALLBACK delaySensorCB(HWND window, UINT message, UINT idevent, DWORD dwtime);
static void CALLBACK idleSensorCB(HWND window, UINT message, UINT idevent, DWORD dwtime);
static void errorHandlerCB(const SoError * error, void * data);
static void sensorQueueChanged(void * cbdata);


extern "C" {

SOSIMPLE_API HWND
SoSimple_init(const char * appname, const char * classname)
{
	if(appname)
		gAppName  = appname;
	if(classname)
		gClassName = classname;

	SoSimple::init();
	return NULL;
}


SOSIMPLE_API void
SoSimple_setCallback(SoSensorCB* cb)
{
	manualCallBack = cb;
}


#ifdef USE_ORIGINAL_MAINLOOP

#pragma message("SoSimple uses original mainloop")

SOSIMPLE_API void
SoSimple_mainLoop()
{
	MSG msg;

	while (TRUE)
	{
		if (::GetQueueStatus(QS_ALLINPUT) != 0)		// if messagequeue != empty
		{		
			if (::GetMessage(& msg, NULL, 0, 0))		// if msg != WM_QUIT
			{
				::TranslateMessage(& msg);
				::DispatchMessage(& msg);
			}
			else
				break;								// msg == WM_QUIT
		}
		else if (idleSensorId != 0)
		{
			doIdleTasks();
		}
		else
		{
			::WaitMessage();
		}
	}

	SoSimple::done();
}

#else

#pragma message("SoSimple uses WinCE compatible mainloop")

SOSIMPLE_API void
SoSimple_mainLoop()
{
	MSG msg;

	for(;;)
	{
		while(::PeekMessage( &msg, 0, 0, 0, PM_REMOVE ))
		{
			if (msg.message == WM_QUIT)
			{
				SoSimple::done();
				return;
			}
			::TranslateMessage( &msg );
			::DispatchMessage( &msg );
		}

		if(idleSensorId!=0)
			doIdleTasks();

		SoSimple::doStbCallback();
	}

	SoSimple::done();
	return;
}

#endif // USE_ORIGINAL_MAINLOOP


}; // extern "C"



void
adjustWindowSize(int& nWidth, int& nHeight, DWORD nStyle, DWORD nExStyle)
{
	RECT rect;

	rect.left = 0;
	rect.top = 0;
	rect.right = nWidth;
	rect.bottom = nHeight;

	AdjustWindowRectEx(&rect, nStyle, FALSE, nExStyle);

	nWidth = rect.right - rect.left;
	nHeight = rect.bottom - rect.top;
}


void
SoSimple::doStbCallback()
{
	if(manualCallBack)
		(*manualCallBack)(NULL, NULL);
}


void
SoSimple::init()
{
	SoDB::init();
	SoNodeKit::init();
	SoSimpleObject::init();

	SoDebugError::setHandlerCallback(errorHandlerCB, NULL);

	SoDB::getSensorManager()->setChangedCallback(sensorQueueChanged, NULL);
}



void
SoSimple::setWidgetSize(HWND const widget, const SbVec2s size)
{
	UINT flags = SWP_NOMOVE | SWP_NOZORDER;

	DWORD style = GetWindowLong(widget, GWL_STYLE),
		  exStyle = GetWindowLong(widget, GWL_EXSTYLE);
	int width = size[0], height = size[1];

	adjustWindowSize(width,height, style, exStyle);
	SetWindowPos(widget, NULL, 0, 0, width, height, flags);
}


HWND
SoSimple::getShellWidget(const HWND w)
{
	return gHWND;
}

HWND
SoSimple::getTopLevelWidget()
{
	return gHWND;
}


void
SoSimple::exitMainLoop(void)
{
	PostQuitMessage(0);
}


void
SoSimple::done(void)
{
}


void
doIdleTasks(void)
{
	SoDB::getSensorManager()->processTimerQueue();
	SoDB::getSensorManager()->processDelayQueue(TRUE); // isidle = TRUE
	sensorQueueChanged(NULL);
}



LRESULT
SoSimple::onDestroy(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
	PostQuitMessage(0);
	return 0;
}

LRESULT
SoSimple::onQuit(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
	if (idleSensorId != 0) ::KillTimer(NULL, idleSensorId);
	if (timerSensorId != 0) ::KillTimer(NULL, timerSensorId);
	if (delaySensorId != 0) ::KillTimer(NULL, delaySensorId);

	idleSensorId = timerSensorId = delaySensorId = 0;

	::UnregisterClass(CHAR_TO_NATIVE(gClassName.getString()), NULL);
	return 0;
}



void
errorHandlerCB(const SoError * error, void * data)
{
	if (pipeErrorMessagesToConsole) {
		(void)printf("%s\n", error->getDebugString().getString());
	}
	else {
		SbString debugstring = error->getDebugString();
		::MessageBox(NULL, CHAR_TO_NATIVE(debugstring.getString()), TEXT("SoError"), MB_OK | MB_ICONERROR);
	}
}


void
sensorQueueChanged(void * cbdata)
{
	SoSensorManager * sensormanager = SoDB::getSensorManager();

	SbTime timevalue;
	if (sensormanager->isTimerSensorPending(timevalue)) {
		SbTime interval = timevalue - SbTime::getTimeOfDay();

		if (interval.getValue() < 0.0) interval.setValue(0.0);
		if (timerSensorId != 0) ::KillTimer(NULL, ::timerSensorId);

		timerSensorId =
			::SetTimer(NULL,
			/* ignored because of NULL first argument: */ 0,
			interval.getMsecValue(),
			(TIMERPROC)::timerSensorCB);
	}
	else if (::timerSensorId != 0) {
		::KillTimer(NULL, timerSensorId);
		timerSensorId = 0;
	}

	if (sensormanager->isDelaySensorPending()) {

		if (idleSensorId == 0) {
			idleSensorId =
				::SetTimer(NULL,
				/* ignored because of NULL first argument: */ 0,

				// FIXME: this seems like a rather bogus way
				// of setting up a timer to check when the
				// system goes idle. Should investigate how
				// this actually works, and perhaps if there
				// is some other mechanism we could
				// use. 20040721 mortene.
				0,

				(TIMERPROC)idleSensorCB);
		}

		if (delaySensorId == 0) {
			unsigned long timeout = SoDB::getDelaySensorTimeout().getMsecValue();
			delaySensorId =
				::SetTimer(NULL,
				/* ignored because of NULL first argument: */ 0,
				timeout,
				(TIMERPROC)delaySensorCB);
		}
	}
	else {
		if (idleSensorId != 0) {
			::KillTimer(NULL, idleSensorId);
			idleSensorId = 0;
		}

		if (delaySensorId != 0) {
			::KillTimer(NULL, delaySensorId);
			delaySensorId = 0;
		}
	}
}


void CALLBACK
timerSensorCB(HWND window, UINT message, UINT idevent, DWORD dwtime)
{
#if SOWIN_DEBUG && 0
	SoDebugError::postInfo("SoWin::timerSensorCB", "called");
#endif // SOWIN_DEBUG
	SoDB::getSensorManager()->processTimerQueue();
	sensorQueueChanged(NULL);
}


void CALLBACK
delaySensorCB(HWND window, UINT message, UINT idevent, DWORD dwtime)
{
#if SOWIN_DEBUG && 0
	SoDebugError::postInfo("SoWin::delaySensorCB", "called");
#endif // SOWIN_DEBUG
	SoDB::getSensorManager()->processDelayQueue(FALSE);
	sensorQueueChanged(NULL);
}


void CALLBACK
idleSensorCB(HWND window, UINT message, UINT idevent, DWORD dwtime)
{
#if SOWIN_DEBUG && 0
	SoDebugError::postInfo("SoWin::idleSensorCB", "called");
#endif // SOWIN_DEBUG

	doIdleTasks();
}


HWND
SoSimple::createMainWindow(WNDPROC nWindowFunc)
{

	TCHAR appName[256], className[256];

	_tcscpy(className, CHAR_TO_NATIVE(gClassName.getString()));
	_tcscpy(appName, CHAR_TO_NATIVE(gAppName.getString()));

	HINSTANCE hInstance = ::GetModuleHandle(0);

	WNDCLASS windowclass;
	windowclass.lpszClassName = className;
	windowclass.hInstance = hInstance;
	windowclass.lpfnWndProc = nWindowFunc;
	windowclass.style = 0;
	windowclass.lpszMenuName = NULL;
	windowclass.hIcon = NULL;
	windowclass.hCursor = NULL;
	windowclass.hbrBackground = NULL;
	windowclass.cbClsExtra = 0;
	windowclass.cbWndExtra = 4;

	ATOM reg = ::RegisterClass(&windowclass);

#ifdef _WIN32_WCE
	DWORD	style = WS_VISIBLE, // | WS_POPUP
			exStyle = WS_EX_TOPMOST;
	int		width = ::GetSystemMetrics(SM_CXSCREEN), height = ::GetSystemMetrics(SM_CYSCREEN);
#else
	DWORD	style = WS_CAPTION | WS_BORDER | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE | WS_POPUPWINDOW,
			exStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
	int		width = 640, height = 480;
#endif


	adjustWindowSize(width,height, style, exStyle);

	gHWND = ::CreateWindowEx(exStyle,
		className,
		appName,
		style,
		CW_USEDEFAULT, CW_USEDEFAULT,
		width,height,
		NULL,
		NULL,
		hInstance,
		NULL);

	if(gHWND==NULL)
	{
		DWORD err = ::GetLastError();
		err = err;
	}
	else
	{
		::ShowWindow(gHWND, SW_SHOW);
	    ::SetForegroundWindow(gHWND);

	    ::InvalidateRect(gHWND, NULL, TRUE); 
		::UpdateWindow(gHWND);
		::Sleep(200);
	}

	return gHWND;
}

