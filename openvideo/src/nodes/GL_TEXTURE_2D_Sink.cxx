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
/** The source file for the GL_TEXTURE_2D_Sink class.
  *
  * @author Denis Kalkofen
  * 
  * $Id: GL_TEXTURE_2D_Sink.cxx 232 2007-03-13 11:04:16Z bornik $
  * @file                                                                   
  * ======================================================================= */
#include <openvideo/nodes/GL_TEXTURE_2D_Sink.h>
#include <openvideo/openVideo.h>

#include <ace/Mutex.h>

#include <GL/gl.h>
#include <GL/glu.h>

#ifdef  ENABLE_GL_TEXTURE_2D_SINK

#if defined(OV_IS_WINXP) && !defined(_IS_KLIMTES_)
#  pragma comment(lib,"opengl32.lib")
#  pragma comment(lib,"glu32.lib")
#  define WIN32_LEAN_AND_MEAN
#endif

#include <openvideo/State.h>
#include <openvideo/Manager.h>

namespace openvideo{

unsigned int 
GL_TEXTURE_2D_Sink::get_video_texture_id()
{

	return video_texture_id[0];

}

GL_TEXTURE_2D_Sink::GL_TEXTURE_2D_Sink()
{
	name = typeName = "GL_TEXTURE_2D_Sink";
	mutex = new ACE_Mutex();
	width=height=0;	
	isRunning=false;
	internalFormat=0;
	buffer=0;
	video_texture_id[0]=0;
}

void 
GL_TEXTURE_2D_Sink::initPixelFormats()
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

GL_TEXTURE_2D_Sink::~GL_TEXTURE_2D_Sink()
{
	delete mutex;
}

/**
*	aquires the mutex	
*/   
void 
GL_TEXTURE_2D_Sink::acquire()
{
	mutex->acquire();
}


/**
*	releases the mutex.	
*/   
void 
GL_TEXTURE_2D_Sink::release()
{
	mutex->release();
}

void
GL_TEXTURE_2D_Sink::init()
{
     isRunning=false;
    //check if a device context is present
#ifdef WIN32
    HGLRC curContext=NULL;
    curContext= wglGetCurrentContext();
    if(!curContext)
    {
        return;
    }
#endif
    logPrintS("Init GL_TEXTURE_2D_Sink '%s' \n",name.c_str());
	mutex->acquire();

	
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
			return;
    }
	//
    
    state=this->inputs[0]->getState();
    if(state)
    {
		this->width=state->width;
		this->height=state->height;
	}
    flip_h = false; 
    flip_v = true;
    float u_rt = (float)width / TEXTURE_WIDTH;
    float v_rt = (float)height / TEXTURE_HEIGHT;
    t_u0 = (flip_h ? u_rt : 0   );
    t_u1 = (flip_h ?    0 : u_rt);
    t_v0 = (flip_v ? v_rt : 0   );
    t_v1 = (flip_v ?    0 : v_rt);
    //
    //create texture
    glEnable(GL_TEXTURE_2D);
    long data_size = 4 * sizeof(GLubyte) * TEXTURE_WIDTH * TEXTURE_HEIGHT;
    GLubyte *data = (GLubyte*)malloc(data_size);
    memset(data, 0xFF, data_size);
    glGenTextures(1, &video_texture_id[0]);
    glBindTexture(GL_TEXTURE_2D, video_texture_id[0]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glTexImage2D(GL_TEXTURE_2D, 0, this->internalFormat, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0,
		 this->format, GL_UNSIGNED_BYTE, data);
    
    glDisable(GL_TEXTURE_2D);
    free(data);
    mutex->release();
    //check gl errors
    GLenum e;
    if ((e = glGetError ()) != GL_NO_ERROR)
    {	
	    logPrintE("GL error: %s\n", gluErrorString(e));
	    logPrintE("Unable to init GL_TEXTURE_2D_Sink -> check if an opengl context is set");
	    return ;
    }
    //////////////////////////////////////////////
    isRunning=true;

}



void
GL_TEXTURE_2D_Sink::process()
{
    if(!Manager::hasGLContext)
        return;
    if(!isRunning)
    {
        init();
		return;
    }

	if(Buffer* buffer = state->getCurrentBuffer())
	{
		buffer->lock();

		mutex->acquire();
		glEnable(GL_TEXTURE_2D);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, video_texture_id[0]);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width,
						height, this->format, GL_UNSIGNED_BYTE,
						(void*)buffer->getPixels());

		glDisable(GL_TEXTURE_2D);
        GLenum e;
        while ((e = glGetError ()) != GL_NO_ERROR)
        {
            logPrintE("GL Error: %s\n", gluErrorString(e));

        }
		mutex->release();

		buffer->unlock();
	}

}
}//namespace openvideo{
#endif  //ENABLE_GL_TEXTURE_2D_SINK
