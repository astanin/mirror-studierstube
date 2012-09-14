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
/** The header file for the V4L2Src class.
 *
 * @author Bernhard Reitinger
 *
 * $Id:
 * @file                                                                   */
/* ======================================================================= */

#ifndef _V4L2SRC_H
#define _V4L2SRC_H

#include <openvideo/openVideo.h>
#include <openvideo/Node.h>
#include <openvideo/PixelFormat.h>
#include <openvideo/Converter.h>
#include <string>

#ifdef ENABLE_V4L2SRC

// taken from pwc-ioctl.h
#define V4L2_FPS_SHIFT		16
#define V4L2_FPS_MASK		0x00FF0000
#define V4L2_FPS_FRMASK		0x003F0000
#define V4L2_FPS_SNAPSHOT	0x00400000



namespace openvideo {

    /**
     *   @ingroup nodes
     *   A source for grabbing video with v4l2 driver.
     *
     *   This node can grab video from V4L2 cards (bt878 capture cards).
     *   The node takes the following parameters (default values in parentheses):
     *   width:        Width of capture frame (640).
     *   height:       Height of capture frame (480).
     *   device:       The device to capture from (/dev/video0)
     *   format:       The pixel format to pass on to downstream nodes (R8G8B8X8).
     *
     */
    class OPENVIDEO_API V4L2Src :
        public Node
    {
    public:

        /**
         * Specify the capture mode
         *
         *
         */
        enum IOMode {
            IO_METHOD_READ,
            IO_METHOD_MMAP,
            IO_METHOD_USERPTR
        };

        /// A buffer for storing the video data
        struct buffer {
            void   *start;
            size_t length;
        };


        /**
         *
         */
        V4L2Src();

        /**
         *
         */
        ~V4L2Src();

        /**
         * return value = the number of supported input formats. the formats are stored in 'pixelFormats'.
         */
        virtual void initPixelFormats();

        /**
         *	Sets all relevant parameters.
         */
        virtual bool setParameter(std::string key, std::string value);

	virtual void init();

        /**
         *	This function calls intializes the videowrapper lib. it then opens and starts the video stream.
         *	Finally it creates a new context where it puts the video specific data on.
         */
        virtual void start();

        /**
         *	Updates the video frame at its context object.
         */
        virtual void process();

        /**
         *	releases the videoframe (which was previously locked by the process fucntion.
         */
        virtual void postProcess();

    protected:

        /// Video device.
        char videoDevice[40];

        /// Width of video frame.
        int videoWidth;

        /// Height of video frame.
        int videoHeight;

        /// Frames per second (requires v4l
        int fps;

        /// Holds video mode identifier
        std::string videoModeString;
        unsigned long videoModePixelFormat;

    private:

        bool openDevice();

        bool initDevice();

        /// initialization for normal i/o reading
        bool initIORead(unsigned int bufferSize);

        /// initialization for mem-maping
        bool initIOMMap();

        /// initialization for user pointer i/o
        bool initIOUserp(unsigned int bufferSize);

        void closeDevice();

        void processImage(const void* p);

        void stop();

        /// Wrapper for making system ioctl call
        int xioctl(int fd, int request, void *arg);

        /// Allow setting the frame rate (only possible with V4L
        void setFrameRate(int fps);

        /// Video stream descriptor
        int videoFd;

        /// I/O mode
        int ioMode;

        /// Buffers for storing the video data
        struct buffer* buffers;

        /// Counter for buffers
        unsigned int nBuffers;

        /// Indicates if the driver can be initialized
        bool initialized;

        /// Image converter between YUV420 -> RGB32, by Daniel Wagner
        Converter * converter;
        //ConverterYV12 converter;
    };

} // namespace

#endif // ENABLE_V4L2SRC
#endif // __V4L2SRC_H

//========================================================================
// End of $FILENAME$
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
