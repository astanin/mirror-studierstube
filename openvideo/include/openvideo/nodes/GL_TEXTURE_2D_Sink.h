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
/** The header file for the GL_TEXTURE_2D_Sink class.
  *
  * @author Denis Kalkofen
  *
  * $Id: GL_TEXTURE_2D_Sink.h 206 2006-07-12 09:24:30Z mathis $
  * @file                                                                   */
 /* ======================================================================= */
#ifndef _GL_TEXTURE_2D_SINK_H
#define _GL_TEXTURE_2D_SINK_H
#include <openvideo/openVideo.h>
#ifdef  ENABLE_GL_TEXTURE_2D_SINK

#include <openvideo/Node.h>
class ACE_Mutex;

namespace openvideo {
/**
*@ingroup nodes
*	This node implements a 2d texture which is updated every time the node is traversed.
*	GL_TEXTURE_2D_Sink takes the context of it's first registered parent node. The node can be used to integrate an OpenVideo sink into another programm. therefore it implements a way to lock and unlock the texture it updates.
*
*   Supported Pixel Formats:
*	@li: R8G8B8
*	@li: B8G8R8
*	@li: R8G8B8X8
*	@li: B8G8R8X8
*	@li: L8
*/

class OPENVIDEO_API GL_TEXTURE_2D_Sink : 
    public openvideo::Node
{
public:
	/**
	*	constructor
	*/
	GL_TEXTURE_2D_Sink();

	/**
	*	destructor
	*/
	~GL_TEXTURE_2D_Sink();

	/**
	*	creates the texture.
	*/
	virtual void init();

	/**
	*	updates the texture with the video frame in it's context.	
	*/   
	virtual void process();

	/**
	*	aquires the mutex	
	*/   
	void acquire();

	/**
	*	releases the mutex.	
	*/   
	void release();

	/**
	*	texture coordinates: two of them are 0 and the other two indicate the 
	*	ratio between max. texture size and the current video size. flip_h, flip_v indicate which two of them are going to be 0. 
	*/
	float t_u0, t_u1,t_v0, t_v1;
	
	/**
	*	opengl's texture id.
	*/
	unsigned int get_video_texture_id();

	virtual void initPixelFormats();

	/**
	*	indicates whether the sink is started or not.
	*/
	bool isStarted() const { return isRunning; }

	/**
	*	wait synchronously until sink is started.
	*/
	void waitStarted() const { while (! isRunning); }

protected:
	volatile bool isRunning;

	/**
	*	mutex to lock the sink's data.
	*/
	ACE_Mutex* mutex;

	bool doubleBufferFlag;

	int buffer;

	unsigned int video_texture_id[1];
	/**
	*	video widht & height.
	*/
	int width,height;

	/**
	*	texture format.
	*/
	int format;

	/**
	*	Internal texture format. defines the number of color components in the texture. valid values = 1,2,3, or 4.
	*/
	int internalFormat;

	/**
	*	is the video fliped?
	*/
	bool flip_h, flip_v;

	/**
	*	max. texture size
	*/
	enum {TEXTURE_WIDTH = 1024, TEXTURE_HEIGHT = 1024};
};

} //namespace openvideo {

#endif // ENABLE_GL_TEXTURE_2D_SINK

#endif
