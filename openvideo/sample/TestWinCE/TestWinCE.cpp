
#include "TestWinCE.h"
#include "PocketHAL/utility.h"

#include <openVideo/nodes/VideoSink.h>
#include <ace/ACE.h>


PHAL_DECLARE_ENTRY_POINT( TestWinCE );


DWORD WINAPI
ManagerThreadFunc(LPVOID lpParam)
{
	openvideo::Manager* manager = (openvideo::Manager*)lpParam;

	manager->run();
	return 0;
}


TestWinCE::TestWinCE()
{
	ACE::init();

    m_config.m_appName     = TEXT("OpenVideo Sample");
    m_config.m_orientation = PHAL::ORIENTATION_ROTATE90CCW;

	videoPixels = new unsigned short[VIDEO_WIDTH*VIDEO_HEIGHT];

	rotateSamples = true;
	showZoomed = false;

	hThread = NULL;
	dwThreadId = 0;
}


bool
TestWinCE::OnInitialize()
{
	if (!Game::OnInitialize())
        return false;

	manager = openvideo::Manager::getInstance();
	manager->parseConfiguration("openvideo.xml");

	if(openvideo::Node* node = manager->getNode("Simple video out"))
	{
		//node->getName();
		openvideo::VideoSink* sink = (openvideo::VideoSink*)node;
		sink->subscribe(this);
	}

	//hThread = ::CreateThread(NULL, 0, ManagerThreadFunc, this, 0, &dwThreadId);
	manager->run();

    return true;
}


void
TestWinCE::OnShutdown()
{


	Game::OnShutdown();
}


bool
TestWinCE::OnGameLoop()
{
    PHAL::Display* display = GetDisplay();

    if (display->BeginScene())
    {
        // Render one frame
        Clear( display->GetBackBuffer(), PHAL::COLOR_BLACK );
		//drawVideo(display, showZoomed);
		DrawText( display->GetBackBuffer(), 0, 0, GetFPSString() );
        display->Swap();
    }

    return Game::OnGameLoop();
}


void
TestWinCE::OnStylusDown( PHAL::Point p )
{
	if(p.x<320)
		rotateSamples = !rotateSamples;
	else
		showZoomed = !showZoomed;

	//syncedSink.setRotateSamples(rotateSamples);
}


void dump(const char* nFileName, void* nData, unsigned int nNumBytes)
{
	if(FILE* fp = fopen(nFileName, "wb"))
	{
		fwrite(nData, 1, nNumBytes, fp);
		fclose(fp);
	}
}

void
TestWinCE::drawVideo(PHAL::Display* nDisplay, bool nZoom)
{
	//dump("small.raw", (void*)currentImage->getPixels(), 320*240*2);

	if(nZoom)
	{
		unsigned int *dst = (unsigned int *)nDisplay->GetBackBuffer()->GetPixels();
		const unsigned short *src = videoPixels;
		unsigned int pix0,pix1,pix2,pix3;

		for(int y=0; y<240; y++)
		{
			// optimized method with 4x loop unrolling and 4 pixels at once
			//
#if 1
			int w=320/4 + 1;
			while(--w)
			{
				// read 4 pixels
				//
				pix0 = *src++;
				pix1 = *src++;
				pix2 = *src++;
				pix3 = *src++;

				// duplicate each pixel into a full 32-bit word
				//
				pix0 |= (pix0<<16);
				pix1 |= (pix1<<16);
				pix2 |= (pix2<<16);
				pix3 |= (pix3<<16);

				// write all 16 pixels
				//
				dst[0] = dst[320] = pix0;
				dst[1] = dst[321] = pix1;
				dst[2] = dst[322] = pix2;
				dst[3] = dst[323] = pix3;

				dst+=4;
			}
#else		
			// slower, simpler method: no unrolling, no optimied loop variable
			//
			for(int x=0; x<320; x++)
			{
				unsigned int pix = *src++;
				pix |= (pix<<16);

				dst[0] = pix;
				dst[320] = pix;

				dst++;
			}
#endif
			dst += 320;
		}
	}
	else
	{
		unsigned char *dst = (unsigned char *)nDisplay->GetBackBuffer()->GetPixels(160,120);
		const unsigned short *src = videoPixels;

		for(int y=0; y<VIDEO_HEIGHT; y++)
		{
			memcpy(dst, src, VIDEO_WIDTH*2);
			dst += nDisplay->GetBackBuffer()->GetPitch();
			src += VIDEO_WIDTH;
		}
	}
}


void
TestWinCE::initPixelFormats()
{
	pixelFormats.push_back(FORMAT_R5G6B5);
	pixelFormats.push_back(FORMAT_R8G8B8);
	pixelFormats.push_back(FORMAT_B8G8R8);
}


void
TestWinCE::update(openvideo::State* curState)
{
	if(curState && curState->frame)
		memcpy(videoPixels, (void*)state->frame, VIDEO_WIDTH*VIDEO_HEIGHT*2);
}

