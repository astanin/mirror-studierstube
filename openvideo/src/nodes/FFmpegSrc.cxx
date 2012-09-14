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
* $Id: FFmpegSrc.cxx 25 2005-11-28 16:11:59Z breiting $
* @file                                                                   */
/* ======================================================================= */

#include <openvideo/nodes/FFmpegSrc.h>
#include <openvideo/Manager.h>
#include <openvideo/openVideo.h>
#include <openvideo/State.h>

#include <iostream>

#ifdef ENABLE_FFMPEGSRC

#include <ffmpeg/avformat.h>

BEGIN_NAMESPACE_OV

// FFmpegSrcBuffer gives FFmpegSrc full access to openvideo::Buffer
class FFmpegSrcBuffer : public Buffer
{
    friend class FFmpegSrc;
public:
    FFmpegSrcBuffer(unsigned char* pixbuffer)
    {
        buffer = pixbuffer;
    }
    
    ~FFmpegSrcBuffer()
    {
        delete [] buffer;
        buffer = NULL;
    }

    void incUpdateCounter()  {  updateCtr++;  }
};


//FFmpegSrcState gives FFmpegSrc full access to openvideo::State
class FFmpegSrcState : public State
{
public:
    BufferVector& getBuffers()  {  return buffers;  }
    void setCurrentBuffer(Buffer* buf)  {  currentBuffer = buf;  }
};

#define FFmpeg_State(_STATE)    reinterpret_cast<FFmpegSrcState*>(_STATE)


FFmpegSrc::FFmpegSrc() : videoStream(-1), iFrame(0)
{
    // Register all formats and codecs
    av_register_all();

    // allocate new state
    state = new FFmpegSrcState;
}

FFmpegSrc::~FFmpegSrc()
{
    Manager::getInstance()->getLogger()->log("FFmpegSrc: feeing all mpeg streams ...");
    // Free the RGB image
    av_free(frameRGB);
    
    // Free the YUV frame
    av_free(frame);
    
    // Close the codec
    avcodec_close(codecContext);

    // Close the video file
    av_close_input_file(formatContext);

    state->unlockAllBuffers();
    delete state;

    delete [] buffer;

}

void 
FFmpegSrc::initPixelFormats()
{
    pixelFormats.push_back(PIXEL_FORMAT(FORMAT_R8G8B8));
}

void
FFmpegSrc::init()
{
    Manager::getInstance()->getLogger()->log("OpenVideo: init FFmpegSrc");

    state->clear();

    state->format=pixelFormat;
    
    // Open video file
    if (av_open_input_file(&formatContext, fileName.c_str(), NULL, 0, NULL) != 0)
    {		
        Manager::getInstance()->getLogger()->log("OpenVideo: ERROR - Cannot open input file\n");
        return;
    }

    // Retrieve stream information
    if(av_find_stream_info(formatContext) < 0)
    {
        Manager::getInstance()->getLogger()->log("OpenVideo: ERROR - Cannot find stream information\n");
        return;
    }
    
    // Dump information about file onto standard error
    dump_format(formatContext, 0, fileName.c_str(), false);

    // Find the first video stream
    for (int i=0; i<formatContext->nb_streams; i++)
        if (formatContext->streams[i]->codec[0].codec_type == CODEC_TYPE_VIDEO)
        {
            videoStream=i;
            break;
        }
    if (videoStream==-1)        
    {
        Manager::getInstance()->getLogger()->log("OpenVideo: ERROR - Cannot find a video stream\n");
        return;
    }
    
    // Get a pointer to the codec context for the video stream
    // FIXME: breiting, only consider the first codec in use, don't
    // know if further are existing
    codecContext = &formatContext->streams[videoStream]->codec[0];
    
    // Find the decoder for the video stream
    codec = avcodec_find_decoder(codecContext->codec_id);
    if (codec == NULL)
    {
        Manager::getInstance()->getLogger()->log("OpenVideo: ERROR - Cannot find the required codec\n");
        return;
    }
    
    // Inform the codec that we can handle truncated bitstreams -- i.e.,
    // bitstreams where frame boundaries can fall in the middle of packets
    if (codec->capabilities & CODEC_CAP_TRUNCATED)
        codecContext->flags |= CODEC_FLAG_TRUNCATED;

    // Open codec
    if(avcodec_open(codecContext, codec) < 0)
    {
        Manager::getInstance()->getLogger()->log("OpenVideo: ERROR - Cannot open codec\n");
        return;
    }

    state->width = codecContext->width;
    state->height = codecContext->height;

    // make a double buffered state
    for(int i=0; i<2; i++)
    {
        unsigned char *pixels = (unsigned char*)malloc(codecContext->width*codecContext->height*3 /* RGB, 3 byte */);
        // clear buffer
        memset(pixels, 0, codecContext->width*codecContext->height*3);
        
        reinterpret_cast<FFmpegSrcState*>(state)->getBuffers().push_back(new FFmpegSrcBuffer(pixels));
    }

    // Allocate video frame
    frame = avcodec_alloc_frame();

    // Allocate an AVFrame structure for RGB output
    frameRGB = avcodec_alloc_frame();

    // Determine required buffer size and allocate buffer
    int numBytes = avpicture_get_size(PIX_FMT_RGB24, 
                                      codecContext->width,
                                      codecContext->height);

    // allocate a plane buffer for initialization
    buffer = new uint8_t[numBytes];

    // Assign appropriate parts of buffer to image planes in frameRGB
    avpicture_fill((AVPicture *)frameRGB, buffer, PIX_FMT_RGB24,
                   codecContext->width, codecContext->height);

    
    // done.
}

bool
FFmpegSrc::getFrame()
{
    static AVPacket packet;
    static int      bytesRemaining = 0;
    uint8_t         *rawData;
    int             bytesDecoded;
    int             frameFinished;
    static bool     fFirstTime = true;

    if(fFirstTime)
    {
        fFirstTime=false;
        packet.data=NULL;
    }
    
    // Decode packets until we have decoded a complete frame
    while (true)
    {
        // Work on the current packet until we have decoded all of it
        while (bytesRemaining > 0)
        {
            // Decode the next chunk of data
            bytesDecoded = avcodec_decode_video(codecContext, frame,
                                                &frameFinished, rawData, bytesRemaining);
            
            // Was there an error?
            if (bytesDecoded < 0)
            {
                Manager::getInstance()->getLogger()->log("OpenVideo: ERROR while decoding frame\n");
                return false;
            }
            
            bytesRemaining -= bytesDecoded;
            rawData += bytesDecoded;
            
            // Did we finish the current frame? Then we can return
            if (frameFinished)
            {
                return true;
            }
        }
        
        // Read the next packet, skipping all packets that aren't for this
        // stream
        do
        {
            // Free old packet
            if (packet.data != NULL)
                av_free_packet(&packet);
            
            // Read new packet
            if (av_read_packet(formatContext, &packet) < 0)
                goto loop_exit;
        } while (packet.stream_index != videoStream);

        bytesRemaining = packet.size;
        rawData = packet.data;
    }
    
 loop_exit:
    
    // Decode the rest of the last frame
    bytesDecoded = avcodec_decode_video(codecContext, frame, &frameFinished, 
                                        rawData, bytesRemaining);
    
    // Free last packet
    if (packet.data != NULL)
        av_free_packet(&packet);
 
    return frameFinished!=0;
}

void
FFmpegSrc::process()
{ 
    if (getFrame())
    {
        img_convert((AVPicture *)frameRGB, PIX_FMT_RGB24, (AVPicture*)frame, 
                    codecContext->pix_fmt, codecContext->width, codecContext->height);
        
        // send data to the OV State
        processImage();
        // saveImage();
    }
}

void FFmpegSrc::saveImage()
{
    using namespace std;
    cerr << ".";

    FILE *pFile;
    char szFilename[32];
    int  y;

    // Open file
    sprintf(szFilename, "frame%d.ppm", iFrame++);
    pFile=fopen(szFilename, "wb");
    if(pFile==NULL)
        return;

    // Write header
    fprintf(pFile, "P6\n%d %d\n255\n", codecContext->width, codecContext->height);
    
    // Write pixel data
    for(y=0; y<codecContext->height; y++)
        fwrite(frameRGB->data[0]+y*frameRGB->linesize[0], 1, codecContext->width*3, pFile);

    // Close file
    fclose(pFile);
}

void
FFmpegSrc::processImage()
{
     using namespace std;
     cerr << "processimage ...\n";

    FFmpegSrcBuffer* buffer = reinterpret_cast<FFmpegSrcBuffer*>(state->findFreeBuffer());
    
    if(!buffer)
    {
        Manager::getInstance()->getLogger()->log("OpenVideo::FFmpegSrc all buffers locked, can not read a new camera image!\n");
        return;
    }
    unsigned char* img = const_cast<unsigned char*>(buffer->getPixels());

    memcpy(img, frameRGB->data[0], codecContext->height*codecContext->width*3);

    FFmpeg_State(state)->setCurrentBuffer(buffer);
    buffer->incUpdateCounter();
}

bool 
FFmpegSrc::setParameter(std::string key, std::string value)
{
    using namespace std;

    Manager::getInstance()->getLogger()->logEx("FFmpegSrc::setParameter: %s %s\n", key.c_str(), value.c_str());
    if(Node::setParameter(key,value))
        return true;
    
    if (key=="file")
    {
        fileName = value.c_str();
        return true;
    }
    else if (key=="format")
    {
        PIXEL_FORMAT pf = PixelFormat::StringToFormat(value);
        
        switch(pf) {
            case (FORMAT_R8G8B8X8):
                pixelFormat = pf;
                break;
            default:
                cout << "PixelFormat " << value << " is not supported. "
                     << "Using default value " << PixelFormat::FormatToString(pixelFormat) << endl;
        }
        
        return true;
    }
    else
    {
        cout << "No key: " << key << endl;
    }
    return false;
}

END_NAMESPACE_OV

#endif // ENABLE

//========================================================================
// End of FFmpegSrc.cxx
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
