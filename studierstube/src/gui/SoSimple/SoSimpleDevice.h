
#ifndef SOSIMPLE_DEVICE_H
#define SOSIMPLE_DEVICE_H

#include <Inventor/SbLinear.h>
#include "SoSimpleObject.h"
#include <windows.h>


typedef LRESULT SoSimpleEventHandler(HWND, UINT, WPARAM, LPARAM);
class SoEvent;


class SOSIMPLE_API SoSimpleDevice : public SoSimpleObject
{
	SOSIMPLE_OBJECT_ABSTRACT_HEADER(SoSimpleDevice, SoSimpleObject);

public:
	virtual ~SoSimpleDevice();

	virtual void enable(HWND w, SoSimpleEventHandler * handler, void * closure) = 0;
	virtual void disable(HWND w, SoSimpleEventHandler * handler, void * closure) = 0;

	virtual const SoEvent * translateEvent(MSG * event) = 0;

	void setWindowSize(const SbVec2s size);
	SbVec2s getWindowSize(void) const;

	static void initClasses(void);

protected:
	SoSimpleDevice();

	void setEventPosition(SoEvent * event, int x, int y) const;
	static SbVec2s getLastEventPosition(void);

	void addEventHandler(HWND, SoSimpleEventHandler *, void *);
	void removeEventHandler(HWND, SoSimpleEventHandler *, void *);
	void invokeHandlers(MSG * event);

	static long modifierKeys;

private:
	SbVec2s widgetsize;
	SbPList *handlers;

	static SbVec2s lasteventposition;
};


#endif // !SOSIMPLE_DEVICE_H
