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
/** The header file for the GLUTSink class.
  *
  * @author Denis Kalkofen
  *
  * $Id: GLUTSink.h 148 2006-05-26 14:53:05Z daniel $
  * @file                                                                   */
 /* ======================================================================= */
#ifndef _GLUTSINK_H
#define _GLUTSINK_H
#include <openvideo/openVideo.h>
#ifdef ENABLE_GLUTSINK
#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

#ifdef LINUX
#include <GL/glx.h>
#endif

#include <openvideo/Node.h>
#include <openvideo/State.h>


namespace openvideo {


//class Buffer;


/**
*@ingroup nodes
*	GLUTSink implements an OpenVideo node to display a certain video stream. 
*	It displays the frame buffer of the context element of it's first input node as a 2dtexture which gets updated every time a traversal takes place.
*	The glut environment runs in it's own thread. it permanently checks in it's idle function if something has changed which forces a redraw of one of the glut windows.
*
*	\n
*   Supported Pixel Formats: \n
*	@li: R8G8B8
*/
class GLUTSink : 
	public Node
{
public:
	/**
	*	constructor
	*/
    GLUTSink();
    
	/**
	*	destructor
	*/
    ~GLUTSink();

	/**
	*	sets its context to use the context of its first parent and initializes the texture paramaters.
	*	start() also adds itself to the static list of "glutSinks", which is later used by the glut thread to access all the glutsink nodes.
	*	if the glut environment isn't up and running yet, start() will create a new thread where the main glut loop runs in.
	*/
	virtual void start();

	/**
	*	Is called once before the (process)traversal starts.
	*	Init should be used to implement any initializations a specific node needs.
	*/
	virtual void init();

	/**
	*	Sets a flag which is read by glut's idle function, which afterwards calls a 'glutPostRedisplay()'.
	*	We do it this way because glut's idle function runs in the same thread than gluts main display function.
	*	'process()' then waits untill the main display function updates the texture with the new video data.
	*/
    virtual void process();

	virtual void initPixelFormats();

    /**
    *	static list off all glut sinks currently running. 
    */
    static std::vector<openvideo::GLUTSink*> glutSinks;

#ifdef WIN32
    HGLRC getGLContext();
    HDC   getDeviceHandle();
#endif
#ifdef LINUX
    Display* getDisplay();
    GLXContext getGLContext();
    GLXDrawable getDeviceHandle();
#endif

protected:
	/**
	*	glut's main loop
	*/
    static void* mainLoop(void *);

	/**
	*	glut's main display function
	*/
    static void mainDisplayFunc ();

	/**
	*	glut's idle function
	*/
    static void idleFunc ();

	/**
	*	A flag to indicate glut's idle function we need a redraw.
	*/
    static bool glutRedraw;

	/**
	*	A flag to prevent multiple calls to start the glut environment.
	*/
    static bool isGlutThread;

	/**
	*	The sinks window handle.
	*/
    int winHandle;

	/**
	*	A flag to indicate this sink needs an update of it's video texture. 
	*	it is set in the 'process()' function and used by the main window function to find the sink which invokes the redraw.
	*/
    bool updateVideo;
	
	/**
	*	function which updated the texture with the new video frame.
	*/
    void updateTexture();

	/**
	*	applies the texture.
	*/
    void redraw();

	/**
	*	self explaining 
	*/
    int width,height,originX,originY;

	/**
	*	texture format.
	*/
	int format;

	/**
	*	Internal texture format. defines the number of color components in the texture. valid values = 1,2,3, or 4.
	*/
	int internalFormat;

    /**
	*	open gl's texture id
	*/
    unsigned int video_texture_id;

	/**
	*	Defines a video flip.
	*/
    bool flip_h, flip_v;
	
	/**
	*	Define the current ratio of the max. texture size and the current video size.
	*/
    float t_u0, t_u1;
    float t_v0, t_v1;
    
	/**
	*	maximal texture size
	*/
    enum {TEXTURE_WIDTH = 1024, TEXTURE_HEIGHT = 1024};

	BufferSynchronizer	bufferSychronizer;
	unsigned int		updateCtr;


#ifdef LINUX
    GLXDrawable dc;
    Display* dsp;
    GLXContext glContext;
#endif

#ifdef WIN32
    HGLRC glContext;
    HDC dc;
#endif

};

}//namespace openvideo {
#endif //#ifdef ENABLE_GLUTSINK
#endif

