

#include <PocketHAL/PocketHAL.h>
#include "PocketHAL/game.h"


#include <openVideo/Manager.h>
#include <openVideo/nodes/VideoSinkSubscriber.h>


class TestWinCE : public PHAL::Game, public openvideo::VideoSinkSubscriber
{
public:
    enum { UID = 0x0B6BE65F };

	enum {
		VIDEO_WIDTH = 320,
		VIDEO_HEIGHT = 240
	};

    TestWinCE();

protected:
	virtual bool OnInitialize();
	virtual void OnShutdown();
	virtual bool OnGameLoop();
	virtual void OnStylusDown( PHAL::Point p );

	virtual void initPixelFormats();
	virtual void update(openvideo::State* curState);

	void drawVideo(PHAL::Display* nDisplay, bool nZoom);

private:
	bool					rotateSamples,
							showZoomed;

	unsigned short			*videoPixels;

	openvideo::Manager		*manager;

	HANDLE					hThread;
	DWORD					dwThreadId;
};
