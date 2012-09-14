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
/** The source file for the GLUTSink class.
  *
  * @author Denis Kalkofen
  * 
  * $Id: GLUTSink.cxx 232 2007-03-13 11:04:16Z bornik $
  * @file                                                                   
  * ======================================================================= */

#include <openvideo/nodes/GLUTSink.h>
#include <openvideo/openVideo.h>

#ifdef ENABLE_GLUTSINK

#include <openvideo/State.h>
#include <openvideo/Manager.h>

#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/glu.h>

#if defined(WIN32)&&(_MSC_VER>=1400)
// somehow ::TryEnterCriticalSection() is not declared in release build...
// (needed by ACE)
BOOL TryEnterCriticalSection(LPCRITICAL_SECTION lpCriticalSection);
#endif //WIN32

#include <ace/OS.h>
#include <ace/Thread.h>
#include <ace/Condition_Thread_Mutex.h>
#include <ace/Thread_Mutex.h>
#include <ace/Mutex.h>

#include <iostream>

#if defined(OV_IS_WINXP) && !defined(_IS_KLIMTES_)
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")
#pragma comment(lib,"glut32.lib")
#endif

using namespace openvideo;

bool GLUTSink::isGlutThread=false;
std::vector<GLUTSink*> GLUTSink::glutSinks;
bool GLUTSink::glutRedraw=false;


GLUTSink::GLUTSink()
{
	name = typeName = "GLUTSink";
	state=NULL;
	originX=originY=0;
	width=height=0;
	updateVideo=false;
	internalFormat=0;
    glContext=NULL;
	updateCtr = 0;
#ifdef LINUX
    dsp=NULL;    
#endif

}

#ifdef WIN32
HGLRC 
GLUTSink::getGLContext()
{
    return glContext;
}

HDC   
GLUTSink::getDeviceHandle()
{
    return dc;
}
#endif 

#ifdef LINUX
GLXContext
GLUTSink::getGLContext()
{
    return glContext;
}

GLXDrawable
GLUTSink::getDeviceHandle()
{
    return dc;
}

Display*
GLUTSink::getDisplay() {
    return dsp;
}
#endif 

void 
GLUTSink::initPixelFormats()
{
	//format_r8g8b8	= 0,
	//format_b8g8r8	= 1,
	//format_r8g8b8x8	= 2,
	//format_b8g8r8x8	= 3,
	//format_l8		= 5,
	this->pixelFormats.push_back(PIXEL_FORMAT(FORMAT_R8G8B8));
	this->pixelFormats.push_back(PIXEL_FORMAT(FORMAT_B8G8R8));
	this->pixelFormats.push_back(PIXEL_FORMAT(FORMAT_R8G8B8X8));
	this->pixelFormats.push_back(PIXEL_FORMAT(FORMAT_B8G8R8X8));
	this->pixelFormats.push_back(PIXEL_FORMAT(FORMAT_L8));
}

GLUTSink::~GLUTSink()
{
}


void*
GLUTSink::mainLoop(void *)
{

    glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE );


	//wait 'till all glutsinks are loaded ='till the process() call happens

    for(int i=0;i<(int)GLUTSink::glutSinks.size();i++)
    {
		GLUTSink *glutSink=GLUTSink::glutSinks[i];
		glutInitWindowPosition (glutSink->originX, glutSink->originY );
		int w,h;
		if(glutSink->width==0)
			w=1;
		else
			w=glutSink->width;
		if( glutSink->height==0)
			h=1;
		else
			h=glutSink->height;
		glutInitWindowSize (w,h);
		glutSink->winHandle=glutCreateWindow(glutSink->getName());
		glutDisplayFunc(GLUTSink::mainDisplayFunc);
		glutIdleFunc(GLUTSink::idleFunc);
        ////////////
#ifdef WIN32
        glutSink->glContext=wglGetCurrentContext();
        glutSink->dc=wglGetCurrentDC();
#endif
#ifdef LINUX
        glutSink->dc=glXGetCurrentDrawable();
        glutSink->dsp=glXGetCurrentDisplay();
        glutSink->glContext=glXGetCurrentContext();
#endif

		//////////////////////////////////////////////
		//create texture
		glEnable(GL_TEXTURE_2D);
		long data_size = 4 * sizeof(GLubyte) * TEXTURE_WIDTH * TEXTURE_HEIGHT;
		GLubyte *data = (GLubyte*)malloc(data_size);
		memset(data, 0xFF, data_size);
		glGenTextures(1, &glutSink->video_texture_id);
		glBindTexture(GL_TEXTURE_2D, glutSink->video_texture_id);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glTexImage2D(GL_TEXTURE_2D, 0, glutSink->internalFormat, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0,
					glutSink->format, GL_UNSIGNED_BYTE, data);

		glDisable(GL_TEXTURE_2D);
		free(data);
		//////////////////////////////////////////////
    }
    //save context
    //if((int)(GLUTSink::glutSinks.size())>0)
    //    Manager::getInstance()->setGLContext(GLUTSink::glutSinks[0]->getGLContext(),GLUTSink::glutSinks[0]->getDeviceHandle());
    glutMainLoop();
    
    return 0;
}

void 
GLUTSink::init()
{
    logPrintS("Init GLUTSink '%s' \n",name.c_str());
	//glteximage2d supported formats
	//glteximage2d supported formats
	///original gl spec. 
	//gl_color_index 	each element is a single value, a color index. it is converted to fixed point (with an unspecified number of 0 bits to the right of the binary point), shifted left or right depending on the value and sign of gl_index_shift, and added to gl_index_offset (see glpixeltransfer). the resulting index is converted to a set of color components using the gl_pixel_map_i_to_r, gl_pixel_map_i_to_g, gl_pixel_map_i_to_b, and gl_pixel_map_i_to_a tables, and clamped to the range [0,1].
	//gl_red 	each element is a single red component. it is converted to floating point and assembled into an rgba element by attaching 0.0 for green and blue, and 1.0 for alpha. each component is then multiplied by the signed scale factor gl_c_scale, added to the signed bias gl_c_bias, and clamped to the range [0,1] (see glpixeltransfer).
	//gl_green 	each element is a single green component. it is converted to floating point and assembled into an rgba element by attaching 0.0 for red and blue, and 1.0 for alpha. each component is then multiplied by the signed scale factor gl_c_scale, added to the signed bias gl_c_bias, and clamped to the range [0,1] (see glpixeltransfer).
	//gl_blue 	each element is a single blue component. it is converted to floating point and assembled into an rgba element by attaching 0.0 for red and green, and 1.0 for alpha. each component is then multiplied by the signed scale factor gl_c_scale, added to the signed bias gl_c_bias, and clamped to the range [0,1] (see glpixeltransfer).
	//gl_alpha 	each element is a single red component. it is converted to floating point and assembled into an rgba element by attaching 0.0 for red, green, and blue. each component is then multiplied by the signed scale factor gl_c_scale, added to the signed bias gl_c_bias, and clamped to the range [0,1] (see glpixeltransfer).
	//gl_rgb 	each element is an rgb triple. it is converted to floating point and assembled into an rgba element by attaching 1.0 for alpha. each component is then multiplied by the signed scale factor gl_c_scale, added to the signed bias gl_c_bias, and clamped to the range [0,1] (see glpixeltransfer).
	//gl_rgba 	each element is a complete rgba element. it is converted to floating point. each component is then multiplied by the signed scale factor gl_c_scale, added to the signed bias gl_c_bias, and clamped to the range [0,1] (see glpixeltransfer).
	//gl_luminance 	each element is a single luminance value. it is converted to floating point, and then assembled into an rgba element by replicating the luminance value three times for red, green, and blue, and attaching 1.0 for alpha. each component is then multiplied by the signed scale factor gl_c_scale, added to the signed bias gl_c_bias, and clamped to the range [0,1] (see glpixeltransfer).
	//gl_luminance_alpha 	each element is a luminance/alpha pair. it is converted to floating point, and then assembled into an rgba element by replicating the luminance value three times for red, green, and blue. each component is then multiplied by the signed scale factor gl_c_scale, added to the signed bias gl_c_bias, and clamped to the range [0,1] (see glpixeltransfer).

	//windows extension
	//gl_bgr_ext 	each pixel is a group of three components in this order: blue, green, red. gl_bgr_ext provides a format that matches the memory layout of windows device-independent bitmaps (dibs). thus your applications can use the same data with win32 function calls and opengl pixel function calls.
	//gl_bgra_ext 	each pixel is a group of four components in this order: blue, green, red, alpha. gl_bgra_ext provides a format that matches the memory layout of windows device-independent bitmaps (dibs). thus your applications can use the same data with win32 function calls and opengl pixel function calls.

	//
	//enum pixel_format {
	//format_r8g8b8	= 0,
	//format_b8g8r8	= 1,
	//format_r8g8b8x8	= 2,
	//format_b8g8r8x8	= 3,
	//format_r5g6b5	= 4,
	//format_l8		= 5,

	//format_unknown	= 6
	//};
	
	// map ov to gl pixel format and set gl_texture_2d_sink's format
	switch(curPixelFormat)
	{
		case FORMAT_R8G8B8:
			format=GL_RGB;
			internalFormat=3;
			break;

		case FORMAT_B8G8R8:
			format=GL_BGR_EXT;
			internalFormat=3;
			break;
		
		case FORMAT_R8G8B8X8:
			format=GL_RGBA;
			internalFormat=4;
			break;

		case FORMAT_B8G8R8X8:
			format=GL_BGRA_EXT;
			internalFormat=4;
			break;


//		case FORMAT_R5G6B5:
//			format=;
//			internalFormat=;
//			break;

		case FORMAT_L8:
			format=GL_LUMINANCE;
			internalFormat=GL_LUMINANCE8;
			break;

		default:
			logPrintE("GL_TEXTURE_2D_Sink does not suppport the current pixel format %s\n",
				(PixelFormat::FormatToString(curPixelFormat)).c_str());
			exit(-1);

    }
	//get the first state 
	
	//states this sink 'lives' in
   	if(inputs.size()<=0)
		return;
    state=this->inputs[0]->getState();
    if(state)
    {
		this->width=state->width;
		this->height=state->height;

		flip_h = false; 
		flip_v = true;
	    
		float u_rt = (float)width / TEXTURE_WIDTH;
		float v_rt = (float)height / TEXTURE_HEIGHT;
	    
		t_u0 = (flip_h ? u_rt : 0   );
		t_u1 = (flip_h ?    0 : u_rt);
		t_v0 = (flip_v ? v_rt : 0   );
		t_v1 = (flip_v ?    0 : v_rt);
		//
		GLUTSink::glutSinks.push_back(this);
	}// if(state)
}

void
GLUTSink::start()
{
	logPrintS("Building GLUTSink\n");
	if(!GLUTSink::isGlutThread){
		//start glut in a new thread
		GLUTSink::isGlutThread=true;
		ACE_hthread_t* threadHandle = new ACE_hthread_t();
		if(ACE_Thread::spawn((ACE_THR_FUNC)GLUTSink::mainLoop,
				0, 	
				THR_NEW_LWP|THR_JOINABLE, 	
				0, 	
				threadHandle,
				0, 	
				0, 	
				0
		)==-1)
		{ 
			logPrintE("GLUTSink Error in spawning thread\n"); 
		}
	}
}


void
GLUTSink::process()
{
	if(!state)
		return;

	if(Buffer* buffer = state->getCurrentBuffer())
		bufferSychronizer.assign(buffer);
}


void 
GLUTSink::updateTexture()
{
	// bufferSychronizer does all the synchronization work...
	//
	if(Buffer* buffer = bufferSychronizer.getLocked())
	{
		// only update if this buffer contains new content!
		//
		if(updateCtr != buffer->getUpdateCounter())
		{
			updateCtr = buffer->getUpdateCounter();
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, video_texture_id);
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 
							width,height, this->format, GL_UNSIGNED_BYTE,
							(void*)buffer->getPixels());
			glDisable(GL_TEXTURE_2D);
		}
		else
		{
		}

		buffer->unlock();
	}
}


void 
GLUTSink::redraw()
{
    //preGLCalls()
    glPushMatrix();
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
    glDisable(GL_LIGHTING);
    glDisable(GL_ALPHA_TEST);
    glDisable(GL_BLEND);
    ///////////////////////    
    
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glEnable(GL_TEXTURE_2D);
    
    // redraw texture
    glBindTexture(GL_TEXTURE_2D, video_texture_id);
    glBegin(GL_QUADS);
    // video backplate
    glTexCoord2f(t_u0,t_v0); glVertex3f(-1.0f, -1.0f,  0.0f);
    glTexCoord2f(t_u1,t_v0); glVertex3f( 1.0f, -1.0f,  0.0f);
    glTexCoord2f(t_u1,t_v1); glVertex3f( 1.0f,  1.0f,  0.0f);
    glTexCoord2f(t_u0,t_v1); glVertex3f(-1.0f,  1.0f,  0.0f);
    glEnd();
    
    glDisable(GL_TEXTURE_2D);
	
    ///postGLCalls()
    glPopAttrib();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

void 
GLUTSink::idleFunc()
{
	glutPostRedisplay();
}

void 
GLUTSink::mainDisplayFunc()
{
	if(Manager::getInstance()->glContextChanged)
		//if(!wglGetCurrentContext())
	{
		logPrintS("GLUTSINK trying to set new gl context \n");
		if((int)(GLUTSink::glutSinks.size())>0)
		{
#ifdef WIN32
			bool hasGLContext=wglMakeCurrent(GLUTSink::glutSinks[0]->getDeviceHandle(),GLUTSink::glutSinks[0]->getGLContext());
#endif
#ifdef LINUX
			bool hasGLContext=glXMakeCurrent(GLUTSink::glutSinks[0]->getDisplay(),
							 GLUTSink::glutSinks[0]->getDeviceHandle(),
							 GLUTSink::glutSinks[0]->getGLContext());
#endif

			if(hasGLContext)
				logPrintI("Successfully set new glContext in GLUTSink thread\n") ;
			else{
				logPrintE("Couldn't set new glContext\n");
				return;
			}
		}
		Manager::getInstance()->glContextChanged=false;
	}

    int size=(int)GLUTSink::glutSinks.size();
    for (int i=0;i<size;i++)
    {	
		glutSetWindow(glutSinks[i]->winHandle);
		glutSinks[i]->updateTexture();
		glutSinks[i]->redraw();
    }

    glutSwapBuffers();
	//Sleep(10);
}

#endif //ENABLE_GLUTSINK
