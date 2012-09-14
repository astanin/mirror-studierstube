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
/** header file for ARToolKitPlusModule module.
 *
 * @author Daniel Wagner
 * @author Erick Mendez
 *
 * $Header$
 * @file                                                                   */
/* ======================================================================= */

/**
 * @page module_ref Module Reference
 * @section artoolkitplusmodule ARToolKitPlusModule
 * The ARToolKitModule interfaces to the ARToolKit library to track the
 * position and orientation of visual markers with a video camera. It uses
 * the information provided by @ref artoolkitplussource nodes and inserts the state
 * events through @ref artoolkitplussource nodes into the tree. The configuration
 * element is @c ARToolKitConfig and has the following attributes :
 * @li @c camera-parameter: file containing camera calibration data
 * @li @c treshold: either a numerical value (0-255) or 'auto' for automatic thresholding. Default is '100'
 * @li @c undist-mode: undistortion mode; one of 'none', 'lut' or 'std' (default)
 * @li @c detect-mode: marker detection mode (marker history); either 'lite' or 'std' (default)
 * @li @c pose-estimator: pose estimator selection; one of the following 'std' (default), 'cont' (better version of std) or 'rpp' (robust pose estimator)
 * @li @c marker-mode: one of the following: 'template' (default), 'idbased' or 'bch'
 * @li @c border-width: width of the border; default value is '0.250'
 * @li @c pattern-dir an optional string that is prefixed to any pattern filename or
 *        the camera data filename. It tries to find the file under the original as
 *        well the prefixed name, in that order.
 *
 * An example configuration element looks like this :
 * @verbatim
 <ARToolKitPlusConfig camera-parameter="quickcampro400.dat" marker-mode="idbased" border-width="0.125" treshold="auto" pose-estimator="cont" /> @endverbatim
*/


#ifndef _ARTOOLKITMODULEPLUS_H
#define _ARTOOLKITMODULEPLUS_H

#include "../dllinclude.h"

#include <vector>
#include <map>

#include "../OpenTracker.h"
#include "../core/VideoUser.h"


#ifdef USE_ARTOOLKITPLUS


namespace ARToolKitPlus
{
  class TrackerSingleMarker;
  class Logger;
}

class ARToolKitPlusModuleLogger;



namespace ot
{

  class ARToolKitPlusModule;
  typedef std::vector<Node::Ptr> NodeVector;
  typedef std::map<int,Node::Ptr> MarkerIdMap;


  /**
   * The module and factory to drive the test source nodes. It constructs
   * TestSource nodes via the NodeFactory interface and pushes events into
   * the tracker tree according to the nodes configuration.
   * @author Gerhard Reitmayr
   * @ingroup core
   */
  class OPENTRACKER_API ARToolKitPlusModule : public ThreadModule, public NodeFactory, public VideoUser
    {
      // Members
    protected:
      /// map to find nodes that contain markers
      MarkerIdMap sourcesMap;

      /// list of TestSource nodes in the tree
      NodeVector sources;

      /// stores the list of all markers that were visible during the last update
      NodeVector visibleMarkers;

      /// stores a list of the best marker confidences found in an image
      float	*bestCFs;
      int		maxMarkerId;

      /// file name of cameradata file
      std::string cameradata;

      /// size of the image in pixels
      int sizeX, sizeY;

      /// an optional prefix for pattern filenames
      std::string patternDirectory;

      /// an optional camera device name
      std::string cameraDeviceHint;

      /// if true ot will use arDetectMarkerLite instead of arDetectMarker
      bool useMarkerDetectLite;

      /// flag to stop image processing thread
      int stop;

      void init(StringTable& attributes, ConfigNode * localTree);

      void updateSingleMarkerSource(Node *source, float cf, float matrix[3][4]);
      void updateMultiMarkerSource(Node *source, float cf, float matrix[3][4]);
      void updateEvent(Event &event, float matrix[3][4]);

      float trackerNear,trackerFar;
      ARToolKitPlus::TrackerSingleMarker* tracker;
      ARToolKitPlus::Logger* logger;

      // Methods
    public:
      enum {
	MAX_MARKERID = 511
      };

      /** constructor method. */
      ARToolKitPlusModule();

      /** Destructor method, clears nodes member. */
      virtual ~ARToolKitPlusModule();

      /** This method is called to ruct a new Node. It compares
       * name to the TestSource element name, and if it matches
       * creates a new TestSource node.
       * @param name reference to string containing element name
       * @attributes refenrence to StringMap containing attribute values
       * @return pointer to new Node or NULL. The new Node must be
       *         allocated with new ! */
      virtual Node * createNode( const std::string& name, const StringTable& attributes);
      /**
       * pushes events into the tracker tree. Checks all TestSources and
       * pushes new events, if a TestSource fires.
       */
      virtual void pushEvent();

      /**
       * queries the camera and pushes data into the tree
       */
      void update();

      /** returns the width of the grabbed image in pixels */
      int getSizeX() const  {  return sizeX;  }

      /** returns the height of the grabbed image in pixels */
      int getSizeY() const  {  return sizeY;  }

      ARToolKitPlus::TrackerSingleMarker* getARToolKitPlus()  {  return tracker;  }

      const char* getARToolKitPlusDescription() const;

      /// Sets a camera device name
      /**
       *  This device name can be used to select between several
       *  available camera files. This must be done before the
       *  ARToolKitPlus::init() is called.
       *  If no camera hint is set or the config file does not contain
       *  any <CameraHint> sections, the standard 'camera-parameter' value
       *  is used.
       */
      void setCameraDeviceHint(const char* nDeviceName)  {  cameraDeviceHint = nDeviceName;  }

      NodeVector& getVisibleMarkers()  {  return visibleMarkers;  }

      void newVideoFrame(const unsigned char* image, int width, int height, PIXEL_FORMAT format);

      bool doBench;
    };

	OT_MODULE(ARToolKitPlusModule);

} // namespace ot


#endif //#ifdef USE_ARTOOLKITPLUS

#endif // _ARTOOLKITMODULEPLUS_H

/*
 * ------------------------------------------------------------
 *   End of ARToolkitPlusModule.h
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
