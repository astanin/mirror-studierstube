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
  * $Id: SoVideoBackground.h 25 2005-11-28 16:11:59Z denis $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _SOVIDEOBACKGROUND_H_
#define _SOVIDEOBACKGROUND_H_

//#include <stb/components/viewer/ViewerDefines.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <stb/components/video/video.h>
#include <stb/base/macros.h>
#include <stb/kernel/VideoUser.h>
#ifndef __APPLE__
#include <GL/gl.h>
#else
#include <OpenGL/gl.h>
#endif

namespace openvideo{
  class VideoSink;
  class BufferSynchronizer;
}


BEGIN_NAMESPACE_STB


class Video;
class VideoBackgroundSinkSubscriber;
struct VideoBackgroundTexInfo;


class VIDEO_API SoVideoBackground : public SoNode, public stb::VideoUser
{
friend class VideoBackgroundSinkSubscriber;

	SO_NODE_HEADER(SoVideoBackground);

   struct VideoBackgroundTexInfo
{
	string nameID;
	GLuint texID;
	GLenum format;
	GLint internalFormat;

	int imgWidth,imgHeight;
	int texWidth,texHeight;
	float u0,v0, u1,v1;
    unsigned int updateCtr;
};

  public:
	static void initClass();

	SoVideoBackground();

	virtual ~SoVideoBackground();

	SoSFString sinkName;

	bool init();

	virtual void vu_init(const openvideo::Buffer& frame, stb::string *givenSinkName);
	virtual void vu_update(const openvideo::Buffer& frame, stb::string *givenSinkName, bool forceUpdate=false);
	virtual UPDATE_MODE vu_getUpdateMode() const  {  return VideoUser::UPDATE_BEFORE_RENDER;  }

protected:
	SoFieldSensor *sinkNameSensor;
	static void  refreshSinkNameCB(void* data, SoSensor* sensor);
	void handleChangedSink();

	virtual void GLRender(SoGLRenderAction *action);
	bool createTexture(const openvideo::Buffer& buffer);
	void updateTexture(const openvideo::Buffer& buffer);
	virtual void drawTexture();
	void blitIntoVideoMemory();

	VideoBackgroundTexInfo*	texInfo;
	std::vector<VideoBackgroundTexInfo*>texInfos;

	bool						initialized;

#ifdef _IS_KLIMTES_
	openvideo::BufferSynchronizer* bufferSynchronizer;
#endif
private:
	GLuint lastTexID;
};

END_NAMESPACE_STB

#endif //_SoVideoBackground_H_

