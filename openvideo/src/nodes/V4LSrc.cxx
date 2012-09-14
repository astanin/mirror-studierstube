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
/** The source file for the V4LSrc class.
  *
  * @author Petter Risholm
  *
  * $Id:
  * @file
  * ======================================================================= */

#include <openvideo/nodes/V4LSrc.h>

#include <openvideo/State.h>
#include <iostream>
#include <cstdlib>

#ifdef ENABLE_V4LSRC

// Check to see whether they are portable
#include <sys/mman.h>  /* mmap */
#include <fcntl.h>

namespace openvideo {

using namespace std;

// Initialize variable to default values.
V4LSrc::V4LSrc() : videoWidth(640), videoHeight(480),
		   videoMode(VIDEO_MODE_PAL), videoChannel(VideoCompositeSource),
		   pixelFormat(FORMAT_B8G8R8), videoPixelFormat(V4L_FORMAT_RGB24),
		   videoBuffer(NULL), videoFd(-1), videoFrame(0)
{
  strcpy(videoDevice, "/dev/video0");

  state=new State();
  state->clear();
  state->width=videoWidth;
  state->height=videoHeight;
  state->format=pixelFormat;
}

V4LSrc::~V4LSrc()
{
  if (videoFd >= 0) {
    close(videoFd);
  }

  if (videoBuffer != NULL) {
    munmap(videoBuffer, videoInfo.size);
  }

  delete state;
}

void
V4LSrc::start()
{
  state->clear();
  state->width=videoWidth;
  state->height=videoHeight;
  state->format=pixelFormat;

  if ((videoFd = open(videoDevice, O_RDWR)) < 0) {
    cerr << "Couldn't open " << videoDevice << endl;

    return;
  }

  if (V4LMGetMMInfo(videoFd, &videoInfo) < 0) {
    cerr << "Couldn't acquire info struct." << endl;

    close(videoFd);

    return;
  }

  cout << "v4lMMinfo" << endl
       << "\tframes: " << videoInfo.frames << endl
       << "\tsize: " << videoInfo.size << endl;

  if (videoInfo.frames < 2) {
    cerr << "Device doesn't support double buffering." << endl;

    close (videoFd);

    return;
  }

  if (V4LSetSource(videoFd, videoChannel, videoMode) <0){
    cerr << "Couldn't set requested source (channel, mode)." << endl;

    close(videoFd);

    return;
  }

  /// Min/max capture width/height
  V4LCaps videoCaps;

  if (V4LGetCaps(videoFd, &videoCaps) < 0) {
    cerr << "Couldn't get Caps structure." << endl;

    close(videoFd);

    return;
  }

  if (videoWidth < videoCaps.minwidth)
    videoWidth = videoCaps.minwidth;
  else if (videoWidth > videoCaps.maxwidth)
    videoWidth = videoCaps.maxwidth;

  if (videoHeight < videoCaps.minheight)
    videoHeight = videoCaps.minheight;
  else if (videoHeight > videoCaps.maxheight)
    videoHeight = videoCaps.maxheight;

  V4LFormat  v4lFormat;
  v4lFormat.width  = videoWidth;
  v4lFormat.height = videoHeight;
  v4lFormat.format = videoPixelFormat;

  V4LMSetFormat(&v4lFormat);

  // FIXME: Should check that we got the correct format.
  V4LMGetFormat(&v4lFormat);
  cout << "v4LFormat:" << endl
       << "\twidth: " << v4lFormat.width << endl
       << "\tvheight: " << v4lFormat.height << endl
       << "\tformat: " << v4lFormat.format << endl;

  // Initialize picture attributes to mid-range
  V4LSetBrightness(videoFd, 65535 / 2);
  V4LSetContrast(videoFd,   65535 / 2);
  V4LSetSaturation(videoFd, 65535 / 2);
  V4LSetHue(videoFd,        65535 / 2);

  if ((videoBuffer = (unsigned char *) mmap(0, videoInfo.size,
				   PROT_READ| PROT_WRITE,
				   MAP_SHARED , videoFd, 0))
      == MAP_FAILED) {
    cerr << "Failed to mmap buffer." << endl ;

    close(videoFd);
    return;
  }

  // Start capture for first frame.
  if (V4LMCapture(videoFd, videoFrame) < 0) {
    cerr << "Couldn't start of frame." << endl << flush;
    return;
  }
}

void
V4LSrc::process()
{
  timeval timestamp;

  if (videoFd < 0) {
    state->frame = NULL;
    return;
  }

  // Wait for this frame
  if (V4LMSync(videoFd, videoFrame) < 0) {
    cerr << "Couldn't sync with the device." << endl << flush;

    state->frame = NULL;

    return;
  }

  // start capture of next frame
  if (V4LMCapture(videoFd, 1-videoFrame) < 0) {
    cerr << "Couldn't capture frame." << endl << flush;

    state->frame = NULL;

    return;
  }

  assert(videoBuffer);

  // Get pointer to the captured frame
  state->frame = videoBuffer + videoInfo.offsets[videoFrame];

  // switch image buffer (capture is double buffered)
  videoFrame = 1 - videoFrame; // 0<->1
}

void
V4LSrc::postProcess()
{
  state->frame=NULL;
}

bool
V4LSrc::setParameter(string key, string value)
{
  cout << "V4LSrc::setParameter: " << key << " " << value << endl;
  if(Node::setParameter(key,value))
      return true;


  if(key=="width")
    {
      this->videoWidth=atoi(value.c_str());
      return true;
    }
  else if(key=="height")
    {
      this->videoHeight=atoi(value.c_str());
      return true;
    }
  else if (key=="device")
    {
      strcpy(this->videoDevice, value.c_str());
      return true;
    }
  else if (key=="format")
    {
      PIXEL_FORMAT pf = PixelFormat::StringToFormat(value);

      switch(pf) {
      case (FORMAT_B8G8R8):
	pixelFormat = pf;
	videoPixelFormat = V4L_FORMAT_RGB24;
	break;

      case (FORMAT_B8G8R8X8):
	pixelFormat = pf;
	videoPixelFormat = V4L_FORMAT_RGB32;
	break;

      case (FORMAT_R5G6B5):
	pixelFormat = pf;
	videoPixelFormat = V4L_FORMAT_RGB16;
	break;

      case (FORMAT_L8):
	pixelFormat = pf;
	videoPixelFormat = V4L_FORMAT_Y8;
	break;

      default:
	cout << "PixelFormat " << value << " is not supported. "
	     << "Using default value " << PixelFormat::FormatToString(pixelFormat) << endl;
      }

      return true;
    }
  else if (key=="videoMode")
    {
      if (value=="PAL")
	{
	  videoMode = VIDEO_MODE_PAL;
	  return true;
	}
      else if (value=="NTSC")
	{
	  videoMode = VIDEO_MODE_NTSC;

	  return true;
	}
      else {
	assert(false && "Wrong videoMode parameter.");
      }

      return false;
    }
  else if (key=="videoChannel")
    {
      if (value=="TV")
	{
	  videoChannel=VideoTVSource;
	}
      else if (value=="Composite")
	{

	  videoChannel=VideoCompositeSource;

	}
      else if (value=="SVideo")
	{
	  videoChannel=VideoSVideoSource;
	}
      else {
	assert(false && "Wrong videoChannel.");
      }

      return true;
    }
  else
    {
      cout << "No key: " << key << endl;
    }
  return false;
}

}; // namespace openvideo

#endif // ENABLE_V4LSRC
