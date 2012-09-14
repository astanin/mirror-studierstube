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


#include "SoSimpleDevice.h"
#include "SoSimpleKeyboard.h"
#include "SoSimpleMouse.h"

#include <Inventor/events/SoEvent.h>


SOSIMPLE_OBJECT_ABSTRACT_SOURCE(SoSimpleDevice);


long SoSimpleDevice::modifierKeys = 0;
SbVec2s SoSimpleDevice::lasteventposition;


struct SoSimpleDeviceHandlerInfo {
	HWND widget;
	SoSimpleEventHandler *handler;
	void *closure;
};

typedef SbBool HandlerCB(SoSimpleEventHandler* handler, HWND widget, MSG* event, void* handlerclosure);


void
SoSimpleDevice::initClasses(void)
{
	SoSimpleDevice::initClass();
	SoSimpleKeyboard::initClass();
	SoSimpleMouse::initClass();
}


SoSimpleDevice::SoSimpleDevice()
{
	handlers = NULL;
	widgetsize = SbVec2s(0, 0);
}


SoSimpleDevice::~SoSimpleDevice()
{
	if(handlers)
	{
		for(int i=0; i<handlers->getLength(); i++)
		{
			SoSimpleDeviceHandlerInfo *info = (SoSimpleDeviceHandlerInfo*)(*handlers)[i];
			delete info;
		}
		delete handlers;
	}
}


void
SoSimpleDevice::setWindowSize(const SbVec2s size)
{
	widgetsize = size;
}


SbVec2s
SoSimpleDevice::getWindowSize(void) const
{
	return widgetsize;
}


void
SoSimpleDevice::setEventPosition(SoEvent* event, int x, int y) const
{
	lasteventposition = SbVec2s(x, y);
	event->setPosition(SbVec2s(x, widgetsize[1] - y - 1));
}


SbVec2s
SoSimpleDevice::getLastEventPosition()
{
	return lasteventposition;
}


void
SoSimpleDevice::addEventHandler(HWND widget, SoSimpleEventHandler* handler, void * closure)
{
	if(handlers == NULL)
		handlers = new SbPList;

	SoSimpleDeviceHandlerInfo * info = new SoSimpleDeviceHandlerInfo;

	info->widget = widget;
	info->handler = handler;
	info->closure = closure;
	handlers->append(info);
}

void
SoSimpleDevice::removeEventHandler(HWND widget, SoSimpleEventHandler *handler, void * closure)
{
	if(handlers)
	{
		for(int i=0; i<handlers->getLength(); i++)
		{
			SoSimpleDeviceHandlerInfo * info = (SoSimpleDeviceHandlerInfo *) (*handlers)[i];
			if((info->widget == widget) && (info->handler == handler) && (info->closure == closure))
			{
				delete info;
				handlers->remove(i);
				return;
			}
		}
	}
}


/*void
SoSimpleDevice::invokeHandlers(HandlerCB* cb, MSG * event)
{
	if(handlers)
	{
		for(int i=0; i<handlers->getLength(); i++)
		{
			SoSimpleDeviceHandlerInfo * info = (SoSimpleDeviceHandlerInfo *)(*handlers)[i];
			cb(info->handler, info->widget, event, info->closure);
		}
	}
}*/



void
SoSimpleDevice::invokeHandlers(MSG * event)
{
}
