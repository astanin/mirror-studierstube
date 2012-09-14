
#ifndef SOSIMPLE_MOUSE_H
#define SOSIMPLE_MOUSE_H

#include "SoSimpleDevice.h"
#include <Inventor/events/SoButtonEvent.h> 

class SoMouseButtonEvent;
class SoLocation2Event;


class SOSIMPLE_API SoSimpleMouse : public SoSimpleDevice {
  SOSIMPLE_OBJECT_HEADER(SoSimpleMouse, SoSimpleDevice);

public:
  enum Events {
    BUTTON_PRESS    = 0x01,
    BUTTON_RELEASE  = 0x02,
    POINTER_MOTION  = 0x04,
    BUTTON_MOTION   = 0x08,

    ALL_EVENTS = BUTTON_PRESS | BUTTON_RELEASE | POINTER_MOTION | BUTTON_MOTION
  };

  SoSimpleMouse(int eventmask = ALL_EVENTS);
  virtual ~SoSimpleMouse(void);

  virtual void enable(HWND widget, SoSimpleEventHandler * handler, void * closure);
  virtual void disable(HWND widget, SoSimpleEventHandler * handler, void * closure);

  virtual const SoEvent * translateEvent(MSG * event);

private:
	SoLocation2Event* makeLocationEvent(MSG* msg);
	SoMouseButtonEvent* makeButtonEvent(MSG* msg, SoButtonEvent::State state);

	int eventmask;
	SoMouseButtonEvent* buttonevent;
	SoLocation2Event* locationevent;
};


#endif // ! SOSIMPLE_MOUSE_H
