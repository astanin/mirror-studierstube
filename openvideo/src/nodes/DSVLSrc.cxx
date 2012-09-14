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
/** The source file for the DSVLSrc class.
  *
  * @author Daniel Wagner
  * 
  * $Id: DSVLSrc.cxx 252 2007-06-13 14:34:11Z mendez $
  * @file                                                                   
  * ======================================================================= */

#include <openvideo/nodes/DSVLSrc.h>
#include <openvideo/openVideo.h>

#ifdef ENABLE_DSVLSRC

#include <DSVL.h>
#include <openvideo/Manager.h>

#ifdef OV_IS_WINXP
#  ifdef OV_IS_DEBUG
#    pragma message (">>> Linking against debug build of DSVL")
#    pragma comment(lib,"DSVL.lib")
#  else	
#    pragma message (">>> Linking against release build of DSVL")
#    pragma comment(lib,"DSVL.lib")
#  endif
#else //WIN32

#endif

#include <openvideo/State.h>
#include <assert.h>


namespace openvideo {

/*
 * Added a couple of functions (thanks Daniel), to handle exceptions
 */

int filter(const char* nWhere, unsigned int nCode, struct
           _EXCEPTION_POINTERS *nEp)
{
    // log what happened...
    return EXCEPTION_EXECUTE_HANDLER;
}

static const char* getExceptionStringFromCode(unsigned int code)
{
    switch(code)
    {
    case EXCEPTION_ACCESS_VIOLATION:
        return "EXCEPTION_ACCESS_VIOLATION";

    case EXCEPTION_BREAKPOINT:
        return "EXCEPTION_BREAKPOINT";

    case EXCEPTION_DATATYPE_MISALIGNMENT:
        return "EXCEPTION_DATATYPE_MISALIGNMENT";

    case EXCEPTION_SINGLE_STEP:
        return "EXCEPTION_SINGLE_STEP";

    case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
        return "EXCEPTION_ARRAY_BOUNDS_EXCEEDED";

    case EXCEPTION_FLT_DENORMAL_OPERAND:
        return "EXCEPTION_FLT_DENORMAL_OPERAND";

    case EXCEPTION_FLT_DIVIDE_BY_ZERO:
        return "EXCEPTION_FLT_DIVIDE_BY_ZERO";

    case EXCEPTION_FLT_INEXACT_RESULT:
        return "EXCEPTION_FLT_INEXACT_RESULT";

    case EXCEPTION_FLT_INVALID_OPERATION:
        return "EXCEPTION_FLT_INVALID_OPERATION";

    case EXCEPTION_FLT_OVERFLOW:
        return "EXCEPTION_FLT_OVERFLOW";

    case EXCEPTION_FLT_STACK_CHECK:
        return "EXCEPTION_FLT_STACK_CHECK";

    case EXCEPTION_FLT_UNDERFLOW:
        return "EXCEPTION_FLT_UNDERFLOW";

    case EXCEPTION_INT_DIVIDE_BY_ZERO:
        return "EXCEPTION_INT_DIVIDE_BY_ZERO";

    case EXCEPTION_INT_OVERFLOW:
        return "EXCEPTION_INT_OVERFLOW";

    case EXCEPTION_PRIV_INSTRUCTION:
        return "EXCEPTION_PRIV_INSTRUCTION";

    case EXCEPTION_NONCONTINUABLE_EXCEPTION:
        return "EXCEPTION_NONCONTINUABLE_EXCEPTION";

    default:
        return "UNKNOWN";
    }
}


static const char*
getDSVLPixelFormatString(PIXELFORMAT format)
{
	switch(format)
	{
	default:
	case PIXELFORMAT_UNKNOWN:
		return "PIXELFORMAT_UNKNOWN";
	case PIXELFORMAT_UYVY:
		return "PIXELFORMAT_UYVY";
	case PIXELFORMAT_YUY2:
		return "PIXELFORMAT_YUY2";
	case PIXELFORMAT_RGB565:
		return "PIXELFORMAT_RGB565";
	case PIXELFORMAT_RGB555:
		return "PIXELFORMAT_RGB555";
	case PIXELFORMAT_RGB24:
		return "PIXELFORMAT_RGB24";
	case PIXELFORMAT_RGB32:
		return "PIXELFORMAT_RGB32";
	case PIXELFORMAT_INVALID:
		return "PIXELFORMAT_INVALID";
	case PIXELFORMAT_QUERY:
		return "PIXELFORMAT_QUERY";
	}
}


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



// DSVLSrcFrame gives DSVLSrc full access to openvideo::Buffer
class DSVLSrcBuffer : public Buffer
{
friend class DSVLSrc;
public:
	DSVLSrcBuffer(DSVL_VideoSource* src, bool flipVert, State* state) : source(src), flipV(flipVert)
	{
		checkedOut = false;
		dsvlBuffer = copyBuffer = NULL;
		width = state->width;
		height = state->height;
		format = state->format;
	}

	~DSVLSrcBuffer()
	{
        delete [] copyBuffer;
	}

	bool getNewFrame(unsigned int ctr)
	{
		DWORD wait_result = source->WaitForNextSample(100/60);

		if(wait_result!=0)			// time-out?
			return false;

		if(checkedOut)
		{
			// keeping DSVL buffsers is currently disabled
			// so this should never be called!
			assert(false);
			source->CheckinMemoryBuffer(mbHandle);
			checkedOut = false;
		}

		if(SUCCEEDED(source->CheckoutMemoryBuffer(&mbHandle, &dsvlBuffer)))
		{
			checkedOut = true;

			if(mbHandle.n <= sampleCtr)
			{
				// only keep new frames
				//
				source->CheckinMemoryBuffer(mbHandle);
				checkedOut = false;
				return false;
			}

			sampleCtr = mbHandle.n;

			if(flipV)
			{
				flipImage();
				buffer = copyBuffer;
			}
			else
			{
				copyImage();
				buffer = copyBuffer;
				//buffer = dsvlBuffer;
			}

			// FIXME-Daniel-20060530: currently there is a bug with keeping DSVL's memory buffers
			//                        so we always copy the video frames and release the buffer immediatelly
			source->CheckinMemoryBuffer(mbHandle);
			checkedOut = false;

			updateCtr = ctr;
			return true;
		}

		logPrintW("DSVLSrc failed to get video frame\n");
		return false;
	}

	void flipImage()
	{
		int bytesPerPixel = PixelFormat::getBitsPerPixel(format) / 8;		// assumes simple pixel-format (x times 8 bits)
		int stride = bytesPerPixel*width;

		if(copyBuffer==NULL)
			copyBuffer = new unsigned char[stride*height];

		openvideo::flipImage(dsvlBuffer, copyBuffer, stride, height);
	}


	void copyImage()
	{
		int bytesPerPixel = PixelFormat::getBitsPerPixel(format) / 8;		// assumes simple pixel-format (x times 8 bits)
		int stride = bytesPerPixel*width;

		if(copyBuffer==NULL)
			copyBuffer = new unsigned char[stride*height];

		memcpy(copyBuffer, dsvlBuffer, height*stride);
	}

protected:
	DSVL_VideoSource	*source;
	MemoryBufferHandle	mbHandle;
	unsigned char		*dsvlBuffer, *copyBuffer;
	bool				flipV;
	bool				checkedOut;
	static unsigned int	sampleCtr;
};

unsigned int DSVLSrcBuffer::sampleCtr = 0;


// DSVLSrcState gives DSVLSrc full access to openvideo::State
class DSVLSrcState : public State
{
public:
	~DSVLSrcState()
	{
		for(size_t i=0; i<buffers.size(); i++)
			delete buffers[i];
		buffers.clear();
	}

	BufferVector& getBuffers()  {  return buffers;  }

	void setCurrentBuffer(Buffer* buf)  {  currentBuffer = buf;  }
};

#define DSVL_State(_STATE)  reinterpret_cast<DSVLSrcState*>(_STATE)


DSVLSrc::DSVLSrc()
{
	dsvlSource = NULL;

	name = typeName = "DSVLSrc";
	flipV = false;
	numBuffers = 2;
	updateCtr = 1;
}
	
DSVLSrc::~DSVLSrc()
{
	state->unlockAllBuffers();

	// FIXME: is it sage to delete this here?
	delete state;

	delete [] dsvlSource;
	dsvlSource = NULL;
}

void 
DSVLSrc::initPixelFormats()
{
	pixelFormats.push_back(PIXEL_FORMAT(FORMAT_B8G8R8));
	pixelFormats.push_back(PIXEL_FORMAT(FORMAT_R5G6B5));
}


void
DSVLSrc::init()
{
	logPrintS("Starting DSVLSrc\n");

	if(::CoInitialize(NULL) == S_FALSE)
	{
		logPrintE("DSVLSrc CoInitialize() failed\n");
		return;
	}

	dsvlSource = new DSVL_VideoSource();

	LONG	cap_width = 0;
	LONG	cap_height = 0;
	double	cap_fps = 0.0;
	PIXELFORMAT pf = PIXELFORMAT_UNKNOWN;

	if(FAILED(dsvlSource->BuildGraphFromXMLFile(const_cast<char*>(configFileName.c_str()))))
	{
		logPrintE("DSVL failed reading config file\n");
		return;
	}

	if(FAILED(dsvlSource->GetCurrentMediaFormat(&cap_width,&cap_height,&cap_fps,&pf)))
	{
		logPrintE("DSVL failed retrieving video configuration\n");
		return;
	}

	if(curPixelFormat==FORMAT_B8G8R8 || curPixelFormat==FORMAT_R5G6B5)
	{
		if((curPixelFormat==FORMAT_B8G8R8 && pf!=PIXELFORMAT_RGB24) || (curPixelFormat==FORMAT_R5G6B5 && pf!=PIXELFORMAT_RGB565))
		{
			logPrintE("DSVLSrc wrong configuration - openvideo wants %s but DSVL delivers %s",
													   PixelFormat::FormatToString(curPixelFormat).c_str(), getDSVLPixelFormatString(pf));
			return;
		}
	}
	else
	{
		logPrintE("DSVLSrc: only B8G8R8 and R5G6B5 are supported\n");
		return;
	}

	if(FAILED(dsvlSource->EnableMemoryBuffer(1,numBuffers)))
	{
		logPrintE("DSVL setting memory buffers\n");
		return;
	}

	if(FAILED(dsvlSource->Run()))
	{
		logPrintE("DSVL failed to run\n");
		return;
	}

	logPrintS("DSVL initialized with %d x %d at %d fps\n", cap_width, cap_height, (int)cap_fps);

	state = new DSVLSrcState();
	state->clear();
	state->width=cap_width;
	state->height=cap_height;
	state->format = curPixelFormat;

	for(int i=0; i<numBuffers; i++)
		DSVL_State(state)->getBuffers().push_back(new DSVLSrcBuffer(dsvlSource, flipV, state));
}


void
DSVLSrc::preProcess()
{
}

void
DSVLSrc::process()
{
#ifdef WIN32
__try {
#endif

	if(DSVLSrcBuffer* buffer = reinterpret_cast<DSVLSrcBuffer*>(state->findFreeBuffer()))
	{
		// our overloaded Buffer class does all the work...
		//
		if(buffer->getNewFrame(updateCtr))
		{
			DSVL_State(state)->setCurrentBuffer(buffer);
			updateCtr++;

			// Debug code to animate the video frame in case we don't get new video data
			// FIXME-Daniel-20060530: This should be removed in the future...
			static bool tst = false;
			static int c=0;
			if(tst)
			{
				memset((void*)buffer->getPixels(), c, 320*240*3);
				c = (c+1)&0xff;
			}
		}
	}
	else
		logPrintW("DSVLSrc all frames locked, can not read a new camera image!\n");
#ifdef WIN32
} __except(filter("RenderScenegraph", GetExceptionCode(), GetExceptionInformation())) {}
#endif


	//FILE* fp = fopen("dump.raw", "wb");
	//fwrite(state->frame, 1, state->width*state->height*3, fp);
	//fclose(fp);
}

void
DSVLSrc::postProcess()
{
}


bool 
DSVLSrc::setParameter(std::string key, std::string value)
{
	if(Node::setParameter(key,value))
		return true;

	if(key=="config-file")
	{
		configFileName = value;
		return true;
	}

	if(key=="flip-v")
	{
		flipV = (value=="true" || value=="TRUE");
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


#endif //ENABLE_DSVLSRC
