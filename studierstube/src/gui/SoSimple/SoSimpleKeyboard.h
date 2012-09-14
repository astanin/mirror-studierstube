
#ifndef SOSIMPLE_KEYBOARD_H
#define SOSIMPLE_KEYBOARD_H


#include "SoSimpleDevice.h"


class SoKeyboardEvent;

#define SO_SIMPLE_ALL_KEYBOARD_EVENTS SoSimpleKeyboard::ALL

class SOSIMPLE_API SoSimpleKeyboard : public SoSimpleDevice {
  SOSIMPLE_OBJECT_HEADER(SoSimpleKeyboard, SoSimpleDevice);

public:
  enum Events {
    KEY_PRESS = 0x01,
    KEY_RELEASE = 0x02,
    ALL_EVENTS = KEY_PRESS | KEY_RELEASE
  };

  SoSimpleKeyboard(int eventmask = ALL_EVENTS);
  virtual ~SoSimpleKeyboard(void);

  virtual void enable(HWND widget, SoSimpleEventHandler * handler, void * closure);
  virtual void disable(HWND widget, SoSimpleEventHandler * handler, void * closure);

  virtual const SoEvent * translateEvent(MSG * event);

private:
  //class SoSimpleKeyboardP * pimpl;
  //friend class SoSimpleKeyboardP;
  //friend class SoGuiKeyboardP;

	int eventmask;
	SoKeyboardEvent* kbdevent;
};


#endif // ! SOSIMPLE_KEYBOARD_H
