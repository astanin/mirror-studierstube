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
 * For further information please contact Denis Kalkofen under
 * <kalkofen@icg.tu-graz.ac.at> or write to Denis Kalkofen,
 * Graz University of Technology, Inffeldgasse 16a, A8010 Graz,
 * Austria.
 * ========================================================================
 * PROJECT: OpenVideo
 * ======================================================================== */
/** The source file for the VideoWrapperSrc class.
  *
  * @author Denis Kalkofen
  * 
  * $Id: VideoWrapperSrc.cxx 232 2007-03-13 11:04:16Z bornik $
  * @file                                                                   
  * ======================================================================= */

#include <openvideo/nodes/VideoWrapperSrc.h>
#include <openvideo/openVideo.h>

#ifdef ENABLE_VIDEOWRAPPERSRC

#include <VideoWrapper.h>
#include <openvideo/Manager.h>

#ifdef WIN32
	#ifdef _DEBUG
		#pragma comment(lib,"VideoWrapperd.lib")
	#else	
		#pragma comment(lib,"VideoWrapper.lib")
	#endif
#else //WIN32

#endif

#include <openvideo/State.h>

namespace openvideo {



// DSVLSrcBuffer gives DSVLSrc full access to openvideo::Buffer
class VideoWrapperSrcBuffer : public Buffer
{
	friend class VideoWrapperSrc;
public:
	VideoWrapperSrcBuffer(State* state, unsigned short video) : hVideo(video)
	{
		int bytesPerPixel = PixelFormat::getBitsPerPixel(state->format) / 8;		// assumes simple pixelformat (x times 8 bits)
		int stride = bytesPerPixel*state->width;
		bufferSize = stride*state->height;
		width = state->width;
		height = state->height;
		format = state->format;

		// VideoWrapper does not have doublebuffering, so we need to do copies...
		//
		buffer = new unsigned char[bufferSize];
	}

	~VideoWrapperSrcBuffer()
	{
		delete buffer;
	}

	bool getNewFrame()
	{
		timeval timestamp;
		unsigned char* buf = NULL;

		// try to get a new frame
		VIDEO_getFrame(hVideo, &buf, &timestamp);
		if(buf)
		{
			memcpy(buffer, buf, bufferSize);
			updateCtr++;

			// since we made a copy we can immediately release the frame...
			VIDEO_releaseFrame(hVideo);
			return true;
		}
		return false;
	}

protected:
	unsigned short	hVideo;
	int				bufferSize;
};


// DSVLSrcState gives DSVLSrc full access to openvideo::State
class VideoWrapperSrcState : public State
{
public:
	BufferVector& getBuffers()  {  return buffers;  }
	void setCurrentBuffer(Buffer* buf)  {  currentBuffer = buf;  }
};

#define VideoWrapper_State(_STATE)    reinterpret_cast<VideoWrapperSrcState*>(_STATE)


VideoWrapperSrc::VideoWrapperSrc()
{
	name = typeName = "VideoWrapperSrc";
	strcpy(libId,"");
	strcpy(formatId,"");
	cameraNum=width=height=frameRate=format=0;
	scale=0.0;
	numBuffers = 2;
}
	
VideoWrapperSrc::~VideoWrapperSrc()
{
}

void 
VideoWrapperSrc::initPixelFormats()
{
	pixelFormats.push_back(PIXEL_FORMAT(FORMAT_R8G8B8));
}


void
VideoWrapperSrc::init()
{
	logPrintS("Building VideoWrapperSrc\n");

	char camInitString[256];//="vc: 0 320x240 15 UYVY 0";
	sprintf(camInitString,"%s: %i %ix%i %i %s %f\n",libId,cameraNum,width,height,frameRate,formatId,scale);;
	logPrintI("camInitString= %s",camInitString);

	if(VIDEO_openVideo(camInitString, &g_hVideo)!= VW_SUCCESS)
	{
		logPrintE("VideoWrapperSrc failed to open video\n");
	}

//	VIDEO_setPropertyLong(g_hVideo,CAMERAPROP_BRIGHT,1);


	// start video
	if(VIDEO_startVideo(g_hVideo)!= VW_SUCCESS)
	{
		logPrintE("VideoWrapperSrc failed to start video\n");
	}

	// get video size
	VIDEO_getWidth(g_hVideo, &width);
	VIDEO_getHeight(g_hVideo, &height);

	// get pixel format
	VIDEO_getPixelFormat(g_hVideo, &format);

	state = new VideoWrapperSrcState();
	state->clear();
	state->width = width;
	state->height = height;
	state->format = PixelFormat::fromOGL(format);
	assert(state->format != FORMAT_UNKNOWN);

	for(int i=0; i<numBuffers; i++)
		VideoWrapper_State(state)->getBuffers().push_back(new VideoWrapperSrcBuffer(state, g_hVideo));
}


void
VideoWrapperSrc::preProcess()
{
}


void
VideoWrapperSrc::process()
{
	if(VideoWrapperSrcBuffer* buffer = reinterpret_cast<VideoWrapperSrcBuffer*>(state->findFreeBuffer()))
	{
		if(buffer->getNewFrame())
		{
			VideoWrapper_State(state)->setCurrentBuffer(buffer);
		}
		//else
		//	logPrintE("VideoWrapperSrc: did not get a new frame\n");
	}
	else
		logPrintW("VideoWrapperSrc all frames locked, can not read a new camera image!\n");
}


void
VideoWrapperSrc::postProcess()
{
}


bool 
VideoWrapperSrc::setParameter(std::string key, std::string value)
{
	if(Node::setParameter(key,value)) return true;

	if(key=="libId")
	{
		strcpy(this->libId,value.c_str());
		return true;
	}
	else if(key=="cameraNum")
	{
		this->cameraNum=atoi(value.c_str());
		return true;
	}
	else if(key=="width")
	{
		this->width=atoi(value.c_str());
		return true;
	}
	else if(key=="height")
	{
		this->height=atoi(value.c_str());
		return true;
	}
	else if(key=="frameRate")
	{
		this->frameRate=atoi(value.c_str());
		return true;
	}
	else if(key=="formatId")
	{
		strcpy(formatId,value.c_str());
		return true;
	}
	else if(key=="scale")
	{
		this->scale=(float)atof(value.c_str());
		return true;
	}

	if(key=="num-buffers")
	{
		numBuffers = atoi(value.c_str());
		if(numBuffers<1)
			numBuffers = 1;
		if(numBuffers>MAX_BUFFERS)
			numBuffers = MAX_BUFFERS;
		return true;
	}

	return false;
}


}  // namespace openvideo


#endif //ENABLE_VIDEOWRAPPERSRC
