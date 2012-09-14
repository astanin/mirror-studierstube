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


#include "SoSimpleKeyboard.h"
#include <Inventor/events/SoKeyboardEvent.h>

#include "KeyMapping.h"


SOSIMPLE_OBJECT_ABSTRACT_SOURCE(SoSimpleKeyboard);


#ifndef MK_ALT
#define MK_ALT 0x02
#endif

static SbDict * translatetable = NULL;



SoSimpleKeyboard::SoSimpleKeyboard(int nEventmask)
{
	kbdevent = new SoKeyboardEvent;
	eventmask = nEventmask;

	if (translatetable == NULL) {
		translatetable = new SbDict;

		int i=0;
		while (WinToSoMapping[i].from != VK_UNKNOWN) {
			translatetable->enter(WinToSoMapping[i].from,(void *)WinToSoMapping[i].to);
			i++;
		}
	}
}


SoSimpleKeyboard::~SoSimpleKeyboard()
{

}


void
SoSimpleKeyboard::enable(HWND widget, SoSimpleEventHandler * callbackproc, void * data)
{
	// keyboards can not be enabled or disabled
}


void
SoSimpleKeyboard::disable(HWND widget, SoSimpleEventHandler * callbackproc, void * data)
{
	// keyboards can not be enabled or disabled
}


const SoEvent *
SoSimpleKeyboard::translateEvent(MSG * msg)
{
	if ((msg->message != WM_KEYDOWN) && (msg->message != WM_KEYUP) && (msg->message != WM_SYSKEYDOWN) && (msg->message != WM_SYSKEYUP))
		return NULL;

	SoButtonEvent::State state =  ((msg->message == WM_KEYDOWN) || (msg->message == WM_SYSKEYDOWN)) ? SoButtonEvent::DOWN : SoButtonEvent::UP;

	kbdevent->setState(state);

	kbdevent->setTime(SbTime::getTimeOfDay());

	void * sokey;
	if (translatetable->find(msg->wParam, sokey)) {
		kbdevent->setKey((SoKeyboardEvent::Key)(int)sokey);
	}
	else {
		kbdevent->setKey(SoKeyboardEvent::ANY);
		return NULL;
	}

	// modifiers:
	if ((int)sokey == SoKeyboardEvent::LEFT_SHIFT) {
		if (state == SoButtonEvent::UP) {
			SoSimpleDevice::modifierKeys &= ~MK_SHIFT;
			//return NULL;
		}
		else {    // SoButtonEvent::DOWN
			if (SoSimpleDevice::modifierKeys & MK_SHIFT) return NULL;
			SoSimpleDevice::modifierKeys |= MK_SHIFT;
		}
	}

	if ((int)sokey == SoKeyboardEvent::LEFT_CONTROL) {
		if (state == SoButtonEvent::UP) {
			SoSimpleDevice::modifierKeys &= ~MK_CONTROL;
			//return NULL;
		}
		else {    // SoButtonEvent::DOWN
			if (SoSimpleDevice::modifierKeys & MK_CONTROL) return NULL;
			SoSimpleDevice::modifierKeys |= MK_CONTROL;
		}
	}

	if ((int)sokey == SoKeyboardEvent::LEFT_ALT) {
		if (state == SoButtonEvent::UP) {
			SoSimpleDevice::modifierKeys &= ~MK_ALT;
			//return NULL;
		}
		else {    // SoButtonEvent::DOWN
			if (SoSimpleDevice::modifierKeys & MK_ALT) return NULL;
			SoSimpleDevice::modifierKeys |= MK_ALT;
		}
	}

	SbVec2s pos = this->getLastEventPosition();
	this->setEventPosition(kbdevent, pos[0], pos[1]);

	kbdevent->setShiftDown((SoSimpleDevice::modifierKeys & MK_SHIFT) ? TRUE : FALSE);
	kbdevent->setCtrlDown((SoSimpleDevice::modifierKeys & MK_CONTROL) ? TRUE : FALSE);
	kbdevent->setAltDown((SoSimpleDevice::modifierKeys & MK_ALT) ? TRUE : FALSE);

	return kbdevent;
}
