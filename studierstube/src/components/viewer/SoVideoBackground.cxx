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
/** The header file for the SoVideoBackground class.
  *
  * @author Denis Kalkofen
  *
  * $Id: SoVideoBackground.cxx 25 2005-11-28 16:11:59Z denis $
  * @file                                                                   */
 /* ======================================================================= */

#include <stb/components/viewer/SoVideoBackground.h>
//#include <stb/components/viewer/SoDisplay.h>
//#include <stb/components/viewer/Viewer.h>
//#include <stb/components/viewer/SoStudierstubeViewer.h>
#include <stb/kernel/Kernel.h>
#include <stb/kernel/ComponentManager.h>
#include <stb/kernel/Profiler.h>
#include <stb/components/video/Video.h>
#include <stb/base/OS.h>

//#include <stb/components/event/SoOpenTrackerSource.h>
#include <stb/kernel/SceneManager.h>


#  include <openvideo/nodes/VideoSink.h>
#  include <openvideo/nodes/VideoSinkSubscriber.h>
#  include <openvideo/State.h>



#ifdef STB_IS_WINDOWS
#  define WIN32_LEAN_AND_MEAN
#  include <windows.h>
#endif

#ifndef __APPLE__
#include <GL/gl.h>
#include <GL/glu.h>
#else
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#endif

#ifdef STB_IS_WINCE
// load intrinsics for ARM instructions
#include <Armintr.h>
#else
#define _PreLoad(x)
#endif


BEGIN_NAMESPACE_STB


static void scale_2x_RGB555(const unsigned short *src, unsigned short *dst, int width, int height);
static void scale_2x_RGB565(const unsigned short *src, unsigned short *dst, int width, int height);


SO_NODE_SOURCE(SoVideoBackground);


void
SoVideoBackground::initClass()
{ 
   SO_NODE_INIT_CLASS(SoVideoBackground, SoNode, "Node");
}


// used to calculate best texture size
static int
getNextPowerOfTwo(unsigned int nValue)
{
	if(nValue<=1)		return 1;
	if(nValue<=2)		return 2;
	if(nValue<=4)		return 4;
	if(nValue<=8)		return 8;
	if(nValue<=16)		return 16;
	if(nValue<=32)		return 32;
	if(nValue<=64)		return 64;
	if(nValue<=128)		return 128;
	if(nValue<=256)		return 256;
	if(nValue<=512)		return 512;
	if(nValue<=1024)	return 1024;
	if(nValue<=2048)	return 2048;

	return 0;
}





SoVideoBackground::SoVideoBackground()
{
    SO_NODE_CONSTRUCTOR(SoVideoBackground);
    SO_NODE_ADD_FIELD(sinkName, (""));

	sinkNameSensor = new SoFieldSensor(&SoVideoBackground::refreshSinkNameCB, this);

    texInfo = NULL;
    initialized = false;

}

SoVideoBackground::~SoVideoBackground()
{
	delete texInfo;
	for(int i=0;i<texInfos.size();i++) delete texInfos[i];
	
}

void  
SoVideoBackground::refreshSinkNameCB(void* data, SoSensor* sensor)
{
	SoVideoBackground *self= (SoVideoBackground *)data;
	self->handleChangedSink();
}

void SoVideoBackground::handleChangedSink()
{
	for(int i=0;i<texInfos.size();i++)
	{
		if( !strcmp(texInfos[i]->nameID.c_str(), sinkName.getValue().getString()) )
		{
			texInfo = texInfos[i];
			logPrintD("SoVideoBackground: changed to sink %s \n", texInfos[i]->nameID.c_str());
			return;
		}
	}
	logPrintD("SoVideoBackground: create new texture \n");	
	texInfo=NULL;
}


void 
SoVideoBackground::GLRender(SoGLRenderAction*)
{
	if(!initialized)
		initialized = init();

	drawTexture();
}


bool
SoVideoBackground::init()
{
	if(Video* video = Video::instance)
	{
		video->vp_registerVideoUser(this);
		sinkNameSensor->attach(&this->sinkName);
		return true;
	}
    return false;
}


void
SoVideoBackground::vu_init(const openvideo::Buffer& /*frame*/, stb::string *givenSinkName)
{
}


void
SoVideoBackground::vu_update(const openvideo::Buffer& frame, stb::string *givenSinkName, bool forceUpdate)
{

#ifdef _IS_KLIMTES_
     if (bufferSynchronizer == NULL)
	    updateTexture(frame);
    else
	    bufferSynchronizer->assign(const_cast<openvideo::Buffer*>(&frame));
#else
    // If this is not our sink then return
   if ((!sinkName.getValue().getLength()==0)&& sinkName.getValue().compareSubString(givenSinkName->c_str())) return;
	updateTexture(frame);
#endif

}


bool
SoVideoBackground::createTexture(const openvideo::Buffer& buffer)
{


	texInfo = new VideoBackgroundTexInfo;

	if(!openvideo::PixelFormat::toOGL(buffer.getFormat(), texInfo->format, texInfo->internalFormat))
	{
		delete texInfo;
		texInfo = NULL;
		logPrintE("SoVideoBackground:ERROR: unsupported pixel-format\n");
		return false;
	}

	texInfo->updateCtr = 0;
	texInfo->imgWidth = buffer.getWidth();
	texInfo->imgHeight = buffer.getHeight();
	texInfo->texWidth = getNextPowerOfTwo(buffer.getWidth());
	texInfo->texHeight = getNextPowerOfTwo(buffer.getHeight());

	bool	flip_h = false, flip_v = true;
	float	u_rt = texInfo->imgWidth / (float)texInfo->texWidth,
			v_rt = texInfo->imgHeight / (float)texInfo->texHeight;

	texInfo->u0 = (flip_h ? u_rt : 0   );
	texInfo->u1 = (flip_h ?    0 : u_rt);
	texInfo->v0 = (flip_v ? v_rt : 0   );
	texInfo->v1 = (flip_v ?    0 : v_rt);

	// Create the OpenGL texture
	//
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &texInfo->texID);
	glBindTexture(GL_TEXTURE_2D, texInfo->texID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
#if defined(_IS_KLIMTES_)
	assert(texInfo->internalFormat==GL_UNSIGNED_SHORT_5_6_5);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texInfo->texWidth, texInfo->texHeight, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, NULL);
#else
	glTexImage2D(GL_TEXTURE_2D, 0, texInfo->internalFormat, texInfo->texWidth, texInfo->texHeight, 0, texInfo->format, GL_UNSIGNED_BYTE, NULL);
#endif
    glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);

	texInfo->nameID = std::string(sinkName.getValue().getString());
	texInfos.push_back(texInfo);
	return true;


	return false;
}


void
SoVideoBackground::updateTexture(const openvideo::Buffer& buffer)
{
	STB_PROFILER_AUTOMEASURE(VIDEO_BACKGROUND)

#ifndef _IS_KLIMTES_
        glPushAttrib(GL_ALL_ATTRIB_BITS);
#endif 



	// init 1st texture
	if(!texInfo)
	{
		if(!createTexture(buffer))
			return;
	}

	if(buffer.getUpdateCounter() != texInfo->updateCtr)
	{
// 		printf("tex-update: %d\n", buffer.getUpdateCounter());
		texInfo->updateCtr = buffer.getUpdateCounter();

		glEnable(GL_TEXTURE_2D);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texInfo->texID);
#if defined(_IS_KLIMTES_)
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, texInfo->imgWidth, texInfo->imgHeight, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, (void*)buffer.getPixels());
#else
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, texInfo->imgWidth, texInfo->imgHeight, texInfo->format, GL_UNSIGNED_BYTE, (void*)buffer.getPixels());
#endif
        glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);

		//GLenum e;
		//while ((e = glGetError ()) != GL_NO_ERROR)
		//{
		//	printf("checkGLErrors(): GL error: %s\n", gluErrorString(e));
		//}

		touch();
	}


#ifndef _IS_KLIMTES_
    glPopAttrib();
#endif 


}


void
SoVideoBackground::drawTexture()
{
	STB_PROFILER_AUTOMEASURE(VIDEO_BACKGROUND)


	if(!texInfo)
		return;

	/////////preGLCalls()
	glPushMatrix();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
#ifdef _IS_KLIMTES_
	// glPushAttrib() is not available in OpenGLES
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	glDisable(GL_LIGHTING);
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_BLEND);
#else
	glPushAttrib(GL_ALL_ATTRIB_BITS);
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
    glDisable(GL_ALPHA_TEST);
    glDisable(GL_BLEND);
    glDisable(GL_CULL_FACE);
#endif
	///////////////////////    

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glEnable(GL_TEXTURE_2D);

	// redraw texture
	glBindTexture(GL_TEXTURE_2D,  texInfo->texID);

	glBegin(GL_QUADS);
		glTexCoord2f(texInfo->u0,texInfo->v0); glVertex3f(-1.0f, -1.0f,  0.0f);
		glTexCoord2f(texInfo->u1,texInfo->v0); glVertex3f( 1.0f, -1.0f,  0.0f);
		glTexCoord2f(texInfo->u1,texInfo->v1); glVertex3f( 1.0f,  1.0f,  0.0f);
		glTexCoord2f(texInfo->u0,texInfo->v1); glVertex3f(-1.0f,  1.0f,  0.0f);
	glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);

	glDisable(GL_TEXTURE_2D);

	///postGLCalls()
#ifdef _IS_KLIMTES_
	// glPopAttrib() is not available in OpenGLES
	glEnable(GL_BLEND);
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_LIGHTING);
	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);
#else
	glPopAttrib();
#endif
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}


void
SoVideoBackground::blitIntoVideoMemory()
{
	STB_PROFILER_AUTOMEASURE(VIDEO_BACKGROUND)

#if defined(_IS_KLIMTES_)
	if(openvideo::Buffer* buffer = bufferSynchronizer->getLocked())
	{
		if(!buffer->getPixels())
		{
			buffer->unlock();
			return;
		}

		if(unsigned short* oglPixels = (unsigned short*)klesGetBufferPixels())
		{
			int oglWidth = klesGetBufferWidth(),
				oglHeight = klesGetBufferHeight();

			if(oglWidth==buffer->getWidth()*2 && oglHeight==buffer->getHeight()*2)
				scale_2x_RGB565((unsigned short*)buffer->getPixels(), oglPixels, buffer->getWidth(),buffer->getHeight());
			else
			{
				// currently we only support 2x upscaling
				// TODO: support no scaling (1x) at all...
				assert(false);
			}
		}

		buffer->unlock();
	}
#endif // defined(HAVE_OPENVIDEO) && defined(_IS_KLIMTES_)
}



//////////////////////////////////////////////////////////////////////////
//
//      Tool Functions for blitting video images into video memory
//

static void
scale_2x_RGB555(const unsigned short *src, unsigned short *dst, int width, int height)
{
	// Converts each pixel from RGR565 to RGB555 during blitting.
	// We therefore need to shift the first two components one bit to the right
	//

	unsigned int *dst0 = reinterpret_cast<unsigned int*>(dst);					// destination pointer for even lines
	unsigned int *dst1 = reinterpret_cast<unsigned int*>(dst+2*width);			// destination pointer for odd lines
	int x, width_half = width/2;
	unsigned int srcRaw,srcFixed;

	for(int y=0; y<height; y++)
	{
		// do a fast scanline up-scale with loop unrolling, writing 8 pixels per loop
		//

		x = width_half+1;			// +1 for pre-decrement (faster than post-decrement)
		while(--x)
		{
			srcRaw = *src++;
			srcFixed = (srcRaw & 0x001F)  | ((srcRaw>>1) & 0x7FE0);

			srcFixed |= srcFixed<<16;

			// write 4 pixels at once
			*dst0++ = srcFixed;
			*dst1++ = srcFixed;


			srcRaw = *src++;
			srcFixed = (srcRaw & 0x001F)  | ((srcRaw>>1) & 0x7FE0);

			srcFixed |= srcFixed<<16;

			// write 4 pixels at once
			*dst0++ = srcFixed;
			*dst1++ = srcFixed;
		}

		dst0 += width;		// skip odd lines
		dst1 += width;		// skip even lines
	}
}


static void
scale_2x_RGB565(const unsigned short *src, unsigned short *dst, int width, int height)
{
	assert(width==320);

#define IMG_WIDTH 320
#define IMG_HEIGHT 240

	unsigned int* dstI = (unsigned int*)dst;

	for(int y=0; y<IMG_HEIGHT; y+=1)
	{
		// prefetch data for a line into cache
		//
		unsigned long *srcI0 = (unsigned long *)src, *srcI;
		srcI = srcI0 + IMG_WIDTH/2 - 32;
		while(srcI>=srcI0)
		{
			_PreLoad(srcI+0);
			_PreLoad(srcI+8);
			_PreLoad(srcI+16);
			_PreLoad(srcI+24);
			srcI -= 32;
		}

		// scale up a line
		//
		unsigned int *dstIEnd=dstI+IMG_WIDTH, c0,c1,c2,c3,c4,c5,c6,c7;

		do
		{
			// read 8 pixels (16 bytes)
			c0 = src[0];
			c1 = src[1];
			c2 = src[2];
			c3 = src[3];
			c4 = src[4];
			c5 = src[5];
			c6 = src[6];
			c7 = src[7];

			// double each pixel into two pixels
			c0 = c0 | (c0<<16);
			c1 = c1 | (c1<<16);
			c2 = c2 | (c2<<16);
			c3 = c3 | (c3<<16);
			c4 = c4 | (c4<<16);
			c5 = c5 | (c5<<16);
			c6 = c6 | (c6<<16);
			c7 = c7 | (c7<<16);

			// write exactly one cache line (32 bytes)...
			dstI[0] = c0;
			dstI[1] = c1;
			dstI[2] = c2;
			dstI[3] = c3;
			dstI[4] = c4;
			dstI[5] = c5;
			dstI[6] = c6;
			dstI[7] = c7;

			// write exactly one cache line (32 bytes)...
			dstI[IMG_WIDTH+0] = c0;
			dstI[IMG_WIDTH+1] = c1;
			dstI[IMG_WIDTH+2] = c2;
			dstI[IMG_WIDTH+3] = c3;
			dstI[IMG_WIDTH+4] = c4;
			dstI[IMG_WIDTH+5] = c5;
			dstI[IMG_WIDTH+6] = c6;
			dstI[IMG_WIDTH+7] = c7;

			src+=8;
			dstI+=8;
		} while(dstI < dstIEnd);

		dstI += IMG_WIDTH;
	}

#undef IMG_WIDTH
#undef IMG_HEIGHT
}


END_NAMESPACE_STB

