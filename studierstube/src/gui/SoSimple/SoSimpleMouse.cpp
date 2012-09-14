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


#include "SoSimpleMouse.h"
#include <Inventor/events/SoLocation2Event.h>
#include <Inventor/events/SoMouseButtonEvent.h>


SOSIMPLE_OBJECT_ABSTRACT_SOURCE(SoSimpleMouse);

#ifndef WM_MOUSEWHEEL
#define WM_MOUSEWHEEL 0x020a
#endif // WM_MOUSEWHEEL

#ifndef MK_ALT
#define MK_ALT 0x02
#endif


SoSimpleMouse::SoSimpleMouse(int events)
{
	eventmask = events;
	buttonevent = NULL;
	locationevent = NULL;
}

SoSimpleMouse::~SoSimpleMouse()
{

}


void
SoSimpleMouse::enable(HWND widget, SoSimpleEventHandler * callbackproc, void * data)
{
	// mice can not be enabled or disabled
}


void
SoSimpleMouse::disable(HWND widget, SoSimpleEventHandler * callbackproc, void * data)
{
	// mice can not be enabled or disabled
}


SoLocation2Event *
SoSimpleMouse::makeLocationEvent(MSG * msg)
{
	static POINT prevPos = { 0xFFFF, 0xFFFF };
	if ((msg->pt.x == prevPos.x) && (msg->pt.y == prevPos.y)) {
		return NULL;
	}
	else {
		prevPos = msg->pt;
	}

	if (locationevent == NULL)
		locationevent = new SoLocation2Event;

	setEventPosition(locationevent, msg->pt.x, msg->pt.y);

	locationevent->setShiftDown((SoSimpleDevice::modifierKeys & MK_SHIFT) ? TRUE : FALSE);
	locationevent->setCtrlDown((SoSimpleDevice::modifierKeys & MK_CONTROL) ? TRUE : FALSE);
	locationevent->setAltDown((SoSimpleDevice::modifierKeys & MK_ALT) ? TRUE : FALSE);

	return locationevent;
}

SoMouseButtonEvent *
SoSimpleMouse::makeButtonEvent(MSG * msg, SoButtonEvent::State state)
{
	if(buttonevent == NULL)
		buttonevent = new SoMouseButtonEvent;

	buttonevent->setState(state);

	switch(msg->message) {
	  case WM_LBUTTONDOWN: // left button
	  case WM_LBUTTONUP:
		  buttonevent->setButton(SoMouseButtonEvent::BUTTON1);
		  break;

	  case WM_MBUTTONDOWN: // midbutton
	  case WM_MBUTTONUP:
		  buttonevent->setButton(SoMouseButtonEvent::BUTTON3);
		  break;

	  case WM_RBUTTONDOWN: // right button
	  case WM_RBUTTONUP:
		  buttonevent->setButton(SoMouseButtonEvent::BUTTON2);
		  break;

	  case WM_MOUSEWHEEL:
		  if (((int16_t)HIWORD(msg->wParam)) < 0) {  // HIWORD(wParam) == deltaZ
			  buttonevent->setButton(SoMouseButtonEvent::BUTTON4);
		  }
		  else {
			  buttonevent->setButton(SoMouseButtonEvent::BUTTON5);
		  }
		  break;

	  default:
		  buttonevent->setButton(SoMouseButtonEvent::ANY);
		  break;
	}

	if(msg->message == WM_MOUSEWHEEL) {
		SbVec2s pos = getLastEventPosition();
		setEventPosition(buttonevent, pos[0], pos[1]);
	} else {
		setEventPosition(buttonevent, msg->pt.x, msg->pt.y);
	}

	buttonevent->setShiftDown((SoSimpleDevice::modifierKeys & MK_SHIFT) ? TRUE : FALSE);
	buttonevent->setCtrlDown((SoSimpleDevice::modifierKeys & MK_CONTROL) ? TRUE : FALSE);
	buttonevent->setAltDown((SoSimpleDevice::modifierKeys & MK_ALT) ? TRUE : FALSE);

	return buttonevent;
}


const SoEvent *
SoSimpleMouse::translateEvent(MSG * msg)
{
	SoEvent * soevent = (SoEvent *) NULL;
	SoButtonEvent::State state = SoButtonEvent::UNKNOWN;

	switch (msg->message) {

  case WM_LBUTTONDOWN:
  case WM_MBUTTONDOWN:
  case WM_RBUTTONDOWN:
  case WM_MOUSEWHEEL:
	  if (!(eventmask & SoSimpleMouse::BUTTON_PRESS))
		  break;
	  state = SoButtonEvent::DOWN;
	  soevent = makeButtonEvent(msg, state);
	  break;

  case WM_LBUTTONUP:
  case WM_MBUTTONUP:
  case WM_RBUTTONUP:
	  if (!(eventmask & SoSimpleMouse::BUTTON_RELEASE))
		  break;
	  state = SoButtonEvent::UP;
	  soevent = makeButtonEvent(msg, state);
	  break;

  case WM_MOUSEMOVE:
	  if (!(eventmask & SoSimpleMouse::POINTER_MOTION))
		  break;
	  soevent = makeLocationEvent(msg);
	  break;

  case WM_SETFOCUS:
  case WM_KILLFOCUS:
  default:
	  break;

	}

	if(soevent)
	{
		long msec = ::GetTickCount();
		soevent->setTime(SbTime((msec / 1000), (msec % 1000) * 1000));
	}

	return (SoEvent*) soevent;
}