 /* ========================================================================
  * Copyright (c) 2006,
  * Institute for Computer Graphics and Vision
  * Graz University of Technology
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without
  * modification, are permitted provided that the following conditions are
  * met:
  *
  * Redistributions of source code must retain the above copyright notice,
  * this list of conditions and the following disclaimer.
  *
  * Redistributions in binary form must reproduce the above copyright
  * notice, this list of conditions and the following disclaimer in the
  * documentation and/or other materials provided with the distribution.
  *
  * Neither the name of the Graz University of Technology nor the names of
  * its contributors may be used to endorse or promote products derived from
  * this software without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
  * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
  * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
  * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
  * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
  * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
  * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
  * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** header file for ARToolKit interface module.
  *
  * @author Gerhard Reitmayr
  *
  * $Id: ARToolKitModule.h 1547 2006-10-25 10:33:10Z veas $
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page module_ref Module Reference
 * @section artoolkitmodule ARToolKitModule
 * The ARToolKitModule interfaces to the ARToolKit library to track the 
 * position and orientation of visual markers with a video camera. It uses
 * the information provided by @ref artoolkitsource nodes and inserts the event
 * events through @ref artoolkitsource nodes into the tree. The configuration 
 * element is @c ARToolKitConfig and has the following attributes :
 * @li @c camera-parameter file containing camera calibration data
 * @li @c treshhold 100 bounding value for treshholding the greyscale video
 *        image
 * @li @c framerate 10 a desired maximum framerate, the module will not exceed it.
 * @li @c videomode a string selecting the videomode for ARToolKit. This depends on 
 *        video interface linked with ARToolKit.
 * @li @c videomode2 a second string to configure a second camera for stereo mode.
 *        This is used in Studierstube to provide stereo video background.
 * @li @c videolinuxmode a string specifically selecting the Linux videomode for ARToolKit.
 *        It overrides the videomode attribute under Linux if provided. This allows to specifiy ARToolKit
 *        videomode settings for Windows and Linux in one file as they currently differ.
 * @li @c videolinuxmode2 the same as videomode2 but like the videolinuxmode attribute above just for Linux.
 * @li @c pattern-dir an optional string that is prefixed to any pattern filename or
 *        the camera data filename. It tries to find the file under the original as
 *        well the prefixed name, in that order.
 *
 * See @ref artoolkitparams for possible video configuration parameter strings
 *
 * An example configuration element looks like this :
 * @verbatim
<ARToolKitConfig camera-parameter="../data/camera" treshhold="150" framerate="5" videomode="0,5,3" pattern-dir="mypatterns/"/>@endverbatim
 */

/**
 * @defgroup input Device Input Classes
 * This group contains modules and nodes implementing device drivers for
 * OpenTracker. It is expected to grow in the future to accomodate a
 * wide range of devices.
 */

/**
 * @page artoolkitparams ARToolkitModule Video Configuration
 *
 * This page describes various configuration parameters for the ARToolkit module
 * embedded in OpenTracker. These configuration strings need to be specified in
 * the @c videomode and @c videomode2 attributes of the @ref artoolkitmodule 
 * configuration element.
 *
 * @section win Windows DirectShow driver
 *
 *  parameter format is either NULL or a list of tokens, separated by commas ","
 *
 *  @subsection binary Binary Tokens
 *
 *  @li @c flipH flip image horizontally (WARNING: non-optimal performance)
 *  @li @c flipV flip image vertically (WARNING: non-optimal performance)
 *  @li @c showDlg displays either WDM capture filter's property page or
 *                 MSDV decoder format dialog (depending on source media type).
 *                 only applies to WDM_CAP, will be ignored for AVI_FILE
 *  @li @c loopAVI continuously loops through an AVI file (applies only to AVI_FILE)
 *  @li @c noClock does not use a Reference Clock to synchronize samples;
 *                 use this flag for offline post-processing (applies only to AVI_FILE)
 *  @li @c renderSec render secondary streams (applies only to AVI_FILE)
 *                   An AVI file can contain an arbitrary number of multiplexed A/V substreams,
 *                   usually there are at most 2 substreams (1st: video, 2nd: audio).
 *                   the AVI_FILE input module will only try to render stream 0x00 (assuming that
 *                   it's video) and ignore the remaning substreams.
 *                   Use this flag to force IGraphBuilder->Render(IPin*) calls on substreams 1..n
 *                   DO NOT SET this flag if your AVI file contains more than one video stream
 *
 *  @subsection param Parametrized Tokens
 *
 *   @li @c inputDevice=? supported parameters: <br>
 *                   @c "WDM_CAP"  (WDM_VIDEO_CAPTURE_FILTER) use the DirectShow WDM wrapper
 *                              to obtain live video input from a streaming capture device
 *                              such as a IEEE 1394 DV camera or USB webcam.<br>
 *                              OPTIONAL: set deviceName=? and/or ieee1394id=? for better
 *                                        control over the choice of suitable WDM drivers
 *                   @c "AVI_FILE" (ASYNC_FILE_INPUT_FILTER) use an Asynchronous File Input
 *                              Filter to read & decode AVI video data<br>
 *                              NOTE: be sure to specify the input file name by pointing
 *                                    fileName=? to a valid AVI file.<br>                  
 *                   EXAMPLE: "inputDevive=WDM_CAP", or "inputDevice=AVI_FILE"<br>
 *                   DEFAULT: "WDM_CAP" will be selected if you omit this token
 *
 *   @li @c videoWidth=? preferred video width, EXAMPLE: "videoWidth=720"
 *                   only applies to WDM_CAP, will be ignored for AVI_FILE
 *   @li @c videoHeight=? preferred video height, EXAMPLE: "videoHeight=576"
 *                   only applies to WDM_CAP, will be ignored for AVI_FILE
 *
 *   @li @c pixelFormat=? internal pixel format (see PixelFormat.h for supported types)<br>
 *                   PERFORMANCE WARNING: Be sure to match your IDirect3DTexture/OpenGL texture
 *                                        formats to whatever you specify here, i.e. use
 *                                        PXtoOpenGL(format), PXtoD3D(format) for creating your
 *                                        textures! (see PixelFormat.h for details)<br>
 *                   EXAMPLE: "pixelFormat=PIXELFORMAT_RGB32"<br>
 *					 NOTE: if you omit the pixelFormat=? token, the global
 *                         constant (default_PIXELFORMAT, usually PIXELFORMAT_RGB32)
 *                         will be selected.
 *
 *   @li @c friendlyName=?  only applies to WDM_CAP, will be IGNORED if "inputDevice=WDM_CAP" is not set.)
 *                    preferred WDM device (WARNING: WDM "friendly names" are locale-dependent),
 *                    i.e. try to match substring <?> with the "friendly names" of
 *                    enumerated DirectShow WDM wrappers (ksproxy.ax).
 *                    WILL BE IGNORED IF deviceName=? IS SET.
 *                    EXAMPLE: "deviceName=Microsoft DV Camera" for IEEE1394 DV devices
 *                             "deviceName=QuickCam" for Logitech QuickCam.
 *
 *   @li @c deviceName=?  only applies to WDM_CAP, will be IGNORED if "inputDevice=WDM_CAP" is not set.)
 *                   preferred WDM device, i.e. try to match substring <?> with the
 *                   "device names" of enumerated DirectShow WDM wrappers (ksproxy.ax).
 *                   Device names look like: "@device:*:{860BB310-5D01-11d0-BD3B-00A0C911CE86}"
 *                   Use GraphEdit to figure out your camera's device name.
 *                   deviceName=? WILL ALWAYS WILL OVERRIDE friendlyName=?
 *                   EXAMPLE: "1394#unibrain&fire-i_1.2#4510000061431408"
 *
 *   @li @c fileName=?  only applies to AVI_FILE, will be IGNORED if "inputDevice=AVI_FILE" is not set.
 *                   input file name, if just use a file's name (without its full path), the WIN32
 *                   API fuction SearchPath() (Winbase.h) will be used to locate the file.<br>
 *                   EXAMPLE: "fileName=C:\Some Directory\Another Directory\Video.AVI"<br>
 *                            "fileName=video.AVI" (will succeed if C:\Some Directory\Another Directory\
 *                                                  is:  * the application's startup directory
 *                                                       * the current directory
 *                                                       * listed in the PATH environment variable)
 *
 *  @li @c ieee1394id=?  only applies to WDM_CAP, will be IGNORED if "inputDevice=WDM_CAP" is not set.
 *                   Unique 64-bit device identifier, as defined by IEEE 1394.
 *					 Hexadecimal value expected, i.e. "ieee1394id=437d3b0201460008"
 *                   Use /bin/IEEE394_id.exe to determine your camera's ID.
 *
 * @li @c deinterlaceEvent=? supported parameters (see VFX_STATE_names[])<br>
 *                           "off"  : disable deinterlacing (DEFAULT)<br>
 *                           "on"   : force deinterlacing (even for progressive frames)<br>
 *                           "auto" : enable deinterlacing only if
 *                                    (VIDEOINFOHEADER.dwInterlaceFlags & AMINTERLACE_IsInterlaced)<br>
 *                         WARNING: EXPERIMENTAL FEATURE!
 *
 * @li @c deinterlaceMethod=? deinterlacing method (see VFxDeinterlaceParam.h for supported modes)
 *                         supported parameters (see DEINTERLACE_METHOD_names[]):<br>
 *                           "blend"      : blend fields (best quality)<br>
 *                           "duplicate1" : duplicate first field<br>
 *                           "duplicate2" : duplicate second field<br>
 *                         NOTE: omitting this token results in default mode (DEINTERLACE_BLEND) being used.<br>
 *                         WARNING: EXPERIMENTAL FEATURE!
 *
 *  @subsection examples Examples
 *@verbatim
   videomode="inputDevice=WDM_CAP,showDlg"
   videomode="flipH,flipV,showDlg"
   videomode="pixelFormat=PIXELFORMAT_RGB24,showDlg"
   videomode="showDlg,deinterlaceEvent=on,deinterlaceMethod=duplicate1"
   videomode="videoWidth=640,flipH,videoHeight=480,showDlg,deinterlaceEvent=auto"
   videomode="inputDevice=WDM_CAP,friendlyName=Microsoft DV Camera,videoWidth=720,videoHeight=480"
   videomode="inputDevice=WDM_CAP,friendlyName=Logitech,videoWidth=320,videoHeight=240,flipV"
   videomode="inputDevice=WDM_CAP,frendlyName=Microsoft DV Camera,ieee1394id=437d3b0201460008"
   videomode="inputDevive=WDM_CAP,deviceName=1394#unibrain&fire-i_1.2#4510000061431408"
   videomode="inputDevice=AVI_FILE,fileName=C:\Some Directory\Another Directory\Video.AVI"
   videomode="inputDevice=AVI_FILE,fileName=Video.AVI,pixelFormat=PIXELFORMAT_RGB24"
@endverbatim
 *
 *****************************************************************************/

#ifndef _ARTOOLKITMODULE_H
#define _ARTOOLKITMODULE_H

#include "../OpenTracker.h"

#ifdef USE_ARTOOLKIT

#ifdef WIN32
//#ifndef __MEMORY_BUFFER_HANDLE__
//#define __MEMORY_BUFFER_HANDLE__

namespace ot {

	struct MemoryBufferHandle
	{
		unsigned long  n; // sample number
		__int64 t;		  // timestamp
	};

} // namespace ot

//#endif
#else
    typedef int MemoryBufferHandle;
#endif

#define STEREO_L 0
#define STEREO_R 1

/**
 * A video tracking source module using the ARToolKit library to track a 
 * number of markers in a video image. It sets up the video library and acts
 * as a NodeFactory for ARToolKitSource nodes. It also keeps a vetor of 
 * the created nodes to update them. It is implemented as a threaded module
 * doing the grabbing and video processing in a separate thread.
 *
 * Moreover it provides a programmatic interface to get the current video image.
 * It returns information about the used format and size and a pointer to
 * the image data.
 * @ingroup input
 */

namespace ot {

class OPENTRACKER_API ARToolKitModule : public ThreadModule, public NodeFactory
{
// members
protected:
    /// stores the sources
    NodeVector sources;
    int did;
    /// treshhold value to use in image processing
    int treshhold;
    /// flag to stop image processing thread
    int stop;
    /// file name of cameradata file
    std::string cameradata;
    /// desired maximal framerate
    double rate;
    /// videomode string
    std::string videomode;
    /// videomode string for second camera
    std::string videomode2;
    /// flag to denote if two cameras are configured
    bool stereo;

    /// pointer to the buffer map
    unsigned char * frame;
    /// size of the image in pixels
    int sizeX, sizeY;
	/// an optional prefix for pattern filenames
	std::string patternDirectory;

// methods
protected:
    /** the work method for the module thread. This method grabs a frame
     * and computes any marker information from it. Then it updates the
     * source nodes accordingly. If the stop flag is set, it finishes.*/
    virtual void run(); 

    /**
     * grabs a frame and processes the data */
    void grab();

public:
    /// constructor method
    ARToolKitModule();

    /// destructor method
    virtual ~ARToolKitModule();

    /** This method is called to construct a new Node. It compares
     * name to the ARToolKitSource element name, and if it matches
     * creates a new ARToolKitSource node.
     * @param name reference to string containing element name
     * @attributes refenrence to StringMap containing attribute values
     * @return pointer to new Node or NULL. The new Node must be
     *         allocated with new ! */
    virtual Node * createNode( const std::string& name, StringTable& attributes);

    /**
     * This method is called after initialisation is finished and before the
     * main loop is started. It opens the artoolkit library and video system.*/
    virtual void start();

    /** closes the artoolkit library */
    virtual void close();

    /**
     * pushes events into the tracker tree. Checks all source nodes for
     * new events and pushes them into the tracker tree.
     */
    virtual void pushEvent();

    /**
     * initializes the ARToolKit module. 
     * @param attributes StringMap of elements attribute values. Should be
     *        possibly , but is not for convenience.
     * @param localTree pointer to root of configuration nodes tree
     */
    virtual void init(StringTable& attributes, ConfigNode * localTree);

    /** returns whether two cameras are configured */
    bool isStereo();

    /** returns the width of the grabbed image in pixels */
    int getSizeX(int stereo_buffer = STEREO_L);

    /** returns the height of the grabbed image in pixels */
    int getSizeY(int stereo_buffer = STEREO_L);

    /** returns whether the grabbed image is flipped horizontally
	  * or vertically */
    void getFlipping(bool* isFlippedH, bool* isFlippedV, int stereo_buffer = STEREO_L);

    /** returns a pointer to the grabbed image. The image format
     * is depending on the pixel format and typically RGB or RGBA 
     * times X times Y bytes. 
     * @return pointer to image buffer */
    unsigned char * lockFrame(MemoryBufferHandle* pHandle, int stereo_buffer = STEREO_L);
	// formerly getFrame()

    /** releases the pointer to the grabbed image.
     * @release frame pointer */
    void unlockFrame(MemoryBufferHandle Handle, int stereo_buffer = STEREO_L);

    /** 
     * returns the OpenGL flag that is used by ARToolkit to describe
     * the pixel format in the frame buffer. This is a simple way to 
     * pass the necessary information to use the image data in GL calls. */
    int getImageFormat(int stereo_buffer = STEREO_L);
};

	OT_MODULE(ARToolKitModule);

} // namespace ot

#endif

#endif

/* 
 * ------------------------------------------------------------
 *   End of ARToolKitModule.h
 * ------------------------------------------------------------
 *   Automatic Emacs configuration follows.
 *   Local Variables:
 *   mode:c++
 *   c-basic-offset: 4
 *   eval: (c-set-offset 'substatement-open 0)
 *   eval: (c-set-offset 'case-label '+)
 *   eval: (c-set-offset 'statement 'c-lineup-runin-statements)
 *   eval: (setq indent-tabs-mode nil)
 *   End:
 * ------------------------------------------------------------ 
 */
