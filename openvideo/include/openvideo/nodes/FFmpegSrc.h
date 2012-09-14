/* ======================================================================== 
* Copyright (C) 2006  Graz University of Technology  
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
* PROJECT: OpenVideo  
* ======================================================================== */  
/** The header file for the FFmpegSrc class.  
*  
* @author Bernhard Reitinger
*  
* $Id: FFmpegSrc.h 25 2005-11-28 16:11:59Z breiting $  
* @file                                                                   */  
/* ======================================================================= */  

#ifndef _FFMPEGSRC_H_
#define _FFMPEGSRC_H_

#ifdef ENABLE_FFMPEGSRC

#include <ffmpeg/avformat.h>

#include <openvideo/openVideo.h>
#include <openvideo/Node.h>

#include <string>


BEGIN_NAMESPACE_OV

/**
*	
*/
class FFmpegSrc : 
	public openvideo::Node
{
public:
    /**
    *     The Constructor	
    */
    FFmpegSrc();

    /**
    *     The destructor.
    */
    virtual ~FFmpegSrc();

    /**
    *	Sets all relevat parameters. 
    */
    virtual bool setParameter(std::string key, std::string value);

    /**
     * 	This function calls intializes the videowrapper lib. it then opens and starts the video stream.
     *	Finally it creates a new context where it puts the video specific data on.
     */
    virtual void init();

    /**
     *	Updates the node.
     */
    virtual void process();

    virtual void initPixelFormats();
protected:	

    bool getFrame();

    void processImage();
    void saveImage();

    /// Holds the pixel format used.
    PIXEL_FORMAT pixelFormat;
    
private:
    /// AV context for format
    AVFormatContext  *formatContext;
    
    /// AV context for codec
    AVCodecContext  *codecContext;

    /// Video stream identifier
    int videoStream;

    /// The actual codec for the stream    
    AVCodec         *codec;

    /// Holds the actual frame in the video stream
    AVFrame         *frame; 

    /// Holds the converted frame of the current video stream
    AVFrame         *frameRGB;

    /// Input filename to open
    std::string fileName;
    
    // buffer used for storing the frames
    uint8_t *buffer;

    // dummy counter for saving images
    int iFrame;
	
};// class 

END_NAMESPACE_OV

#endif // ENABLE

#endif //_FFMPEGSRC_H_

//========================================================================
// End of FFmpegSrc.h 
//========================================================================
// Local Variables:
// mode: c++
// c-basic-offset: 4
// eval: (c-set-offset 'substatement-open 0)
// eval: (c-set-offset 'case-label '+)
// eval: (c-set-offset 'statement 'c-lineup-runin-statements)
// eval: (setq indent-tabs-mode nil)
// End:
//========================================================================
