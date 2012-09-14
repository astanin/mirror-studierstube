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
/** The header file for the V4LSrc class.
 *
 * @author Petter Risholm
 *
 * $Id:
 * @file                                                                   */
/* ======================================================================= */
#ifndef _V4LSRC_H
#define _V4LSRC_H

#include <openvideo/openVideo.h>
#include <openvideo/Node.h>
#include <openvideo/PixelFormat.h>
#include <string>

#ifdef ENABLE_V4LSRC

#include <openvideo/nodes/v4l.h>

namespace openvideo {

/**
 *   @ingroup nodes
 *   A source for grabbing video from V4L cards.
 *
 *   This node can grab video from V4L cards (bt878 capture cards).
 *   The node takes the following parameters (default values in parentheses):
 *   width:        Width of capture frame (640).
 *   height:       Height of capture frame (480).
 *   device:       The device to capture from (/dev/video0)
 *   format:       The pixel format to pass on to downstream nodes (B8G8R8).
 *   videoMode:    The video mode to use for grabbing (PAL).
 *   videoChannel: Channel to capture from (Composite).
 *
 */
class OPENVIDEO_API V4LSrc
    : public Node
{
 public:

  /**
   *  Defines which channel to grab video from.
   */
  enum VideoChannel {
    VideoTVSource = 0,
    VideoCompositeSource = 1,
    VideoSVideoSource = 2
  };

  /**
   *
   */
  V4LSrc();

  /**
   *
   */
  virtual ~V4LSrc();

  /**
   *	Sets all relevant parameters.
   */
  virtual bool setParameter(std::string key, std::string value);

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

  /// Defines video mode. Can take values (PAL | NTSC). Default is PAL.
  int videoMode;

  /// Defines video channel. (Composite|SVideo|TV). Default is Composite.
  VideoChannel videoChannel;

  /// Holds the pixel format used.
  PIXEL_FORMAT pixelFormat;

 private:

  /// Defines the v4l pixel format (Used internally).
  int videoPixelFormat;

  /// Video info.
  V4LMMInfo videoInfo;

  /// Capture buffer.
  unsigned char *videoBuffer;

  /// Video stream descriptor
  int videoFd;

  /// Switches between the frame buffers (2 are currently used)
  int videoFrame;
};

}; // namespace openvideo

#endif // ENABLE_V4LSRC
#endif // __V4LSRC_H
