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
/** The source file for the ImageSrc class.
  *
  * @author Daniel Wagner
  * 
  * $Id: ImageSrc.cxx 251 2007-06-12 15:52:01Z mendez $
  * @file                                                                   
  * ======================================================================= */

#include <stdlib.h>
#include <openvideo/nodes/ImageSrc.h>
#include <openvideo/openVideo.h>

#ifdef ENABLE_IMAGESRC

#include <openvideo/Manager.h>

#ifdef OV_IS_WINXP
#  define  SIMAGE_NOT_DLL
#  ifdef OV_IS_DEBUG
#    pragma message (">>> Linking against debug build of simage")
#    pragma comment(lib,"simage1d.lib")
//#    pragma message (">>> Linking against debug build of LibJPEG")
//#    pragma comment(lib,"JPEGd.lib")
#  else	
#    pragma message (">>> Linking against release build of simage")
#    pragma comment(lib,"simage1.lib")
//#    pragma message (">>> Linking against release build of LibJPEG")
//#    pragma comment(lib,"JPEG.lib")
#  endif
#else //WIN32

#endif

#include <openvideo/State.h>
#include <ace/Time_Value.h>
#include <ace/OS.h>
#include <assert.h>
#include <simage.h>


namespace openvideo {


static void
flipImage(unsigned char* nSrc, unsigned char *nDst, int nStride, int nHeight)
{
	nSrc += nStride*(nHeight-1);

	for(int i=0; i<nHeight; i++)
	{
		memcpy(nDst, nSrc, nStride);
		nDst += nStride;
		nSrc -= nStride;
	}
}

inline unsigned short
convertPixel24To16(int nRed, int nGreen, int nBlue)
{
	return (unsigned short)(((nRed << 8) & (0x1F << 11)) | ((nGreen << 3) & (0x3F << 5)) | (nBlue >> 3));
}

static void
convertRGB24toRGB565(unsigned short* nDestData, const unsigned char* nSrcData, int nNumPixels)
{
	int	i,j;

	for(i=0,j=0; i<nNumPixels; i++,j+=3)
		nDestData[i] = convertPixel24To16(nSrcData[j+0], nSrcData[j+1], nSrcData[j+2]);
}



// ImageSrcFrame gives ImageSrc full access to openvideo::Buffer
class ImageSrcBuffer : public Buffer
{
friend class ImageSrc;
public:
	ImageSrcBuffer(unsigned char* pixels, State* state)
	{
		buffer = pixels;
		width = state->width;
		height = state->height;
		format = state->format;
	}

	~ImageSrcBuffer()
	{
		delete buffer;
		buffer = NULL;
	}

	void setUpdateCtr(unsigned int ctr)
	{
		updateCtr = ctr;
	}

protected:
};


// ImageSrcState gives ImageSrc full access to openvideo::State
class ImageSrcState : public State
{
public:
	~ImageSrcState()
	{
		for(size_t i=0; i<buffers.size(); i++)
			delete buffers[i];
		buffers.clear();
	}

	BufferVector& getBuffers()  {  return buffers;  }

	void setCurrentBuffer(Buffer* buf)  {  currentBuffer = buf;  }
};

#define IMAGE_State(_STATE)  reinterpret_cast<ImageSrcState*>(_STATE)


ImageSrc::ImageSrc()
{
	numBuffers = 0;
	width = 320;
	height = 240;
	name = typeName = "ImageSrc";
	updateCtr = 1;
	curIdx = 0;
	delay = 1.0f;
	manual = true;
	doNextPic = true;
	lastUpdate = new ACE_Time_Value;
}
	
ImageSrc::~ImageSrc()
{
	state->unlockAllBuffers();

	// FIXME: is it safe to delete this here?
	delete state;

	delete lastUpdate;
}

void 
ImageSrc::initPixelFormats()
{
	pixelFormats.push_back(PIXEL_FORMAT(FORMAT_R8G8B8));
	pixelFormats.push_back(PIXEL_FORMAT(FORMAT_R5G6B5));
}


void
ImageSrc::init()
{
	logPrintS("Building ImageSrc\n");


	state = new ImageSrcState();
	state->clear();
	state->format = curPixelFormat;
	state->width = width;
	state->height = height;

	for(size_t i=0; i<fileNames.size(); i++)
	{
		int w=0,h=0,numC=0;
		unsigned char *buf2=0, *buf1=0, *buf0 = simage_read_image(fileNames[i].c_str(), &w,&h,&numC);

		if(!buf0)
		{
			logPrintW("ImageSrc failed to load %s\n", fileNames[i].c_str());
			continue;
		}

		if(numC!=3)
		{
            logPrintW("ImageSrc: only images width 3 components supported, discarding %s\n", fileNames[i].c_str());
			simage_free_image(buf0);
			continue;
		}

		assert(w);
		assert(h);
		assert(numC);

		if(w!=width || h!=height)
		{
			buf1 = simage_resize(buf0, w,h,numC, width,height);
			simage_free_image(buf0);
		}
		else
			buf1 = buf0;

		if(curPixelFormat==FORMAT_R5G6B5)
		{
			unsigned char *buf1a = new unsigned char[width*height*2];
			convertRGB24toRGB565(reinterpret_cast<unsigned short*>(buf1a), buf1, width*height);
			simage_free_image(buf1);
			buf2 = new unsigned char[width*height*2];
			flipImage(buf1a, buf2, width*2, height);			// simage loads images y-flipped
			delete buf1a;
		}
		else
		{
			buf2 = new unsigned char[width*height*3];
			flipImage(buf1, buf2, width*3, height);				// simage loads images y-flipped
			simage_free_image(buf1);
		}

		//FILE* fp = fopen("dump.raw", "wb");
		//fwrite(buf2, width*height*2, 1, fp);
		//fclose(fp);

		IMAGE_State(state)->getBuffers().push_back(new ImageSrcBuffer(buf2, state));
        logPrintI("ImageSrc: image '%s' loaded\n", fileNames[i].c_str());
	}

	// make sure we have an update the first time process() is called...
	//
	lastUpdate->set(0.0);
}


void
ImageSrc::preProcess()
{
}


void
ImageSrc::process()
{
	ACE_Time_Value	currentTime = ACE_OS::gettimeofday(),
					dt = currentTime - *lastUpdate;

	if((dt.msec() > 1000.0f*delay && !manual) || doNextPic)
	{
		if(++curIdx >= IMAGE_State(state)->getBuffers().size())
			curIdx = 0;

		ImageSrcBuffer* buf = (ImageSrcBuffer*)IMAGE_State(state)->getBuffers()[curIdx];
		buf->setUpdateCtr(++updateCtr);

		IMAGE_State(state)->setCurrentBuffer(buf);

		*lastUpdate = currentTime;
		doNextPic = false;
	}
}

void
ImageSrc::postProcess()
{
}


bool 
ImageSrc::setParameter(std::string key, std::string value)
{
	if(Node::setParameter(key,value))
		return true;

	if(key=="width")
	{
		width = atoi(value.c_str());
		return true;
	}

	if(key=="height")
	{
		height = atoi(value.c_str());
		return true;
	}

	if(key=="delay")
	{
		if(value=="manual")
			manual = true;
		else
		{
			delay = (float)atof(value.c_str());
			manual = false;
		}
		return true;
	}

	if(key.substr(0, 5)=="image")
	{
		fileNames.push_back(value);
	}

	return false;
}


}  // namespace openvideo


#endif //ENABLE_IMAGESRC
