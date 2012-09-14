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
/**
 * @file   ThreeToTwoDimCalc.h
 * @author Christian Pirchheim
 *
 * @brief  Interface of class \c ThreeToTwoDimCalc
 *
 * 
 */
/**
 * @page Nodes Node Reference
 * @section ThreeToTwoDimCalc ThreeToTwoDimCalc
 *
 * This @e configuration @e node is associated with @ref qtmouseeventmodule. Consists of
 * XML attributes, which determine the spatial location and extent of the Qt target
 * desktop screen relative to the position and orientation of the ASPD (dedicated 6-DOF
 * tracked device mounted on the the desktop screen). The attribute values can be
 * generated with the %OTQt calibration tool (see @ref sec_otqt_configuration of the %OTQt
 * manual).
 *
 * Has the following XML attributes:
 *
 * @li @c CSOrientationQuat quaternion quadruple determining the orientation difference of
 * application screen coordinate system to the world coordination system (mandatory)
 * @li @c CSRoot2ScreenRootVec positional 3D vector directing from the application screen
 * coordinate system origin (location of ASPD) to the screen root position, per definition
 * the top left corner of the desktop screen (mandatory)
 * @li @c ASWidthVec the 3D width vector (x-axis) of the desktop screen plane (mandatory)
 * @li @c ASHeightVec the 3D height vector (y-axis) of the desktop screen plane
 * (mandatory)
 *
 * An example element looks like this:
 *
@verbatim
@endverbatim
 *
 * @see class @c ot::ThreeToTwoDimCalc description
 */

#ifndef _THREETOTWODIMCALC_H
#define _THREETOTWODIMCALC_H
#include <OpenTracker/dllinclude.h>
#if USE_THREETOTWODIMFILTER

#include <OpenTracker/otqt/OTQtMath.h>
#include <OpenTracker/otqt/OTQtLog.h>
#include <OpenTracker/core/ConfigNode.h>
#include <OpenTracker/core/Event.h>
//#include <QtCore/qpoint.h>
#include <string>


//--------------------------------------------------------------------------------
/**
 * @class ThreeToTwoDimCalc
 * @ingroup group_otqt_classes
 * @brief Updates of application screen location and mouse cursor coordinates
 *
 * Without accurate information about the location and extent of the desktop screen, the
 * next step, the computation of the mouse cursor coordinates would be impossible. This
 * class provides method updateASPD() in order to update the geometric information about
 * the location of the desktop screen in case the ASPD changed its position and/or
 * orientation.
 *
 * @image html OTQt-WorldCS2ScreenPlaneTrafo.jpg "Transformation from world CS to target application screen plane"
 *
 * Geometric transformations applied to compute the spatial desktop screen location
 * starting from the world (tracking) coordinate system origin and its correlations with
 * the data gained in the @e calibration @e routine from the user-marked screen corner
 * positions (1), (2), (3) and (4). @f$\phi@f$ determines the orientation difference of
 * the ASPD to the world coordination system. The positional vector @f$ w@f$ directs from
 * the application screen coordinate system origin to the screen root position, per
 * definition the top left corner of the screen. The vectors @f$ v_1@f$ and @f$ v_2@f$
 * span the extent of the screen plane.  During @e operation @e mode the location of the
 * screen plane must be recomputed if the position or orientation of the ASPD changes. If
 * the new orientation angle is @f$\phi'@f$, the recomputation is established by rotating
 * the position vectors @f$ w@f$ and @f$ v_1, v_2@f$ about the difference angle @f$\phi' -
 * \phi@f$. These operations match vector transformations from one coordinate system
 * (represented by @f$\phi@f$) to another coordinate system (represented by @f$\phi'@f$).
 *
 * In order to compute 2D desktop mouse cursor coordinates from tracking data (the MPD)
 * this class provides the updateMPD() method.  OTQt defines a certain 3D volume located
 * in front of the target desktop screen as mouse event sensible region. This region is
 * called @e screen @e cuboid} (SC).
 *
 * @image html OTQt-ScreenCuboidWithPen.jpg "Screen Cuboid with Mouse Position Device (MPD) displayed as pen"
 *
 * The @e screen @e cuboid is spanned by the screen width @f$v_1@f$, screen height
 * @f$v_2@f$ and screen depth @f$v_3@f$. @f$v_3@f$ is computed as dot product of @f$v_1
 * \times v_2@f$. The volume marks the mouse event sensitive region. That means: only
 * while the MPD (displayed here as pen) actually intersects the screen coboid volume, the
 * OTQt module will generate and post mouse events. In that case, the MPD position is
 * converted into corresponding desktop coordinates and OTQt will post a MouseMove event
 * to the Qt applicaton, whereupon the correct mouse cursor position is set.
 *
 * Provides several methods to query the current MPD status: spatial 3D position with
 * getMPD3DPosition(), desktop mouse coordinates with getMPD2DCoords(), and the location
 * of the MPD relative to the screen cuboid can be retrieved with getMPDLocation().
 *
 */
namespace ot {

class OPENTRACKER_API ThreeToTwoDimCalc {
public:
  /**
   * @class ASCorner
   *
   * @brief Container to store the position and associated local coordinate system of a
   * desktop screen corner
   */
  typedef struct {
    /// local coordinate system position and orientation (ASPD)
    Event local_cs_root;
    /// screen corner position (MPD)
    Event corner;
  } ASCorner;
  /**
   * @class CalibInputData
   * @brief Tracking data collected during the calibration procedure
   */
  typedef struct {
    /// application screen coordinate system (ASPD pos/orient)
    Event as_cs_root;
    /// top left screen corner
    ASCorner top_left;
    /// top right screen corner
    ASCorner top_right;
    /// bottom right screen corner
    ASCorner bottom_right;
    /// bottom left screen corner
    ASCorner bottom_left;
  } CalibInputData;
  /**
   * @class CalibOutputData
   * @brief Output data of calibration routine
   */
  class CalibOutputData
  {
  public:
      CalibOutputData()
          : as_cs_orient(4),
            as_cs_root_to_screen_root(3),
            as_width_vec(3),
            as_height_vec(3)
      {
          //OTQT_DEBUG("CalibOutputData(): as_cs_orient.size () = %i\n", as_cs_orient.size());
      };
      ~CalibOutputData()
      {};
  public:
    /// difference angle between application screen and world (tracking) coordinate system
    std::vector<float> as_cs_orient;
    /// difference vector from AS coordinate system origin to screen plane root (top left screen corner per definition)
    std::vector<float> as_cs_root_to_screen_root;
    /// width vector spanning the screen plane
    std::vector<float> as_width_vec;
    /// height vector spanning the screen plane
    std::vector<float> as_height_vec;
  };

  /**
   * Location states of the MPD relative to the SC.
   */
  enum MPDLocationState {
    /// MPD location unknown
    MPD_LOC_UNKNOWN = 0x00,
    /// MPD resides inside SC
    MPD_LOC_INSIDE_SC = 0x01,
    /// MPD resides outside SC
    MPD_LOC_OUTSIDE_SC = 0x02,
  };

private:
  /**
   * @class ASData
   * @brief Geometric 3D data specifying spatial location and extent of application
   * desktop screen
   */
  class ASData
  {
  public:
      ASData()
          : as_cs_root(),
            as_screen_root(),
            as_width_vec(3),
            as_height_vec(3),
            as_depth_scalar_front(0),
            as_depth_scalar_back(0)
      {};
      ~ASData()
      {};
  public:
    /// application screen coordinate system position and orientation (ASPD)
    Event as_cs_root;
    /// screen plane root position
    Event as_screen_root;
    /// positional width vector spanning the screen plane (relative to screen plane root)
    std::vector<float> as_width_vec;
    /// positional height vector spanning the screen plane (relative to screen plane root)
    std::vector<float> as_height_vec;
    /// positive scalar multiplier of screen plane depth unit vector (front of screen)
    float as_depth_scalar_front;
    /// negative scalar multiplier of screen plane depth unit vector (back of screen)
    float as_depth_scalar_back;
  };
  /**
   * @class MPData
   * @brief Data collection reflecting latest perceived (current) MPD tracking state
   */
  typedef struct {
	  float x;
	  float y;
  } Point;

  typedef struct {
    /// latest perceived (current) 3D position of MPD
    Event position;
    /// true if MPD currently resides inside SC
    bool mpd_loc_inside_screen_cuboid;
    /// latest valid 2D desktop coordinates (computed from MPD position)
    Point desktop_coords;
  } MPData;

public:
  /**
   * Converts raw tracking data acquired during the calibration routine into final
   * geometric data that can be stored in as XML attributes in an OpenTracker
   * configuration file.
   *
   * The given input data collection contains the position of the four application desktop
   * screen corners. The method derives the geometric relation (angle and distance)
   * between the local application screen coordinate system (as defined by the APSD) and
   * the screen plane (represented by screen root position) as well as the screen plane
   * extent (width/height spanning vectors computed from the four screen corners).
   *
   * Passes the computed data to the provided output object.
   *
   * @param in tracking data collected during calibration routine
   * @param out geometric data specifying the application screen plane location and extent
   */
  static void convert(CalibInputData const & in, CalibOutputData & out);
  /**
   * Wraps the geometric data elements from in the specified container into XML attribute
   * strings. Stores each attribute string in the provided key/value map.
   *
   * @param out final geometric data from the calibration routine
   * @param table XML attribute key/value map containing the geometric values
   */
	static void convert(CalibOutputData const & out, StringTable & table);

public:
  /**
   * Unwraps application screen geometric values (calibration data) from XML attributes
   * stored in provided key/value map. Computes initial application screen location from
   * input calibration data. Initializes mouse position data structure (default MPD
   * position set to (0,0,0)).
   *
   * @param table XML attribute key/value map
   */
	ThreeToTwoDimCalc();
	~ThreeToTwoDimCalc() { };

private:

public: // methods
  /**
   * Recalculates the application screen spatial 3D location based on the specified ASPD
   * position/orientation which defines an updated application screen coordinate
   * system. Transforms the screen root position and the screen plane itself from the
   * prior to the new local coordinate system.
   *
   * @param as_cs_root ASPD position/orientation (new local application screen coordinate
   * system)
   */
  void updateASPD(Event const & as_cs_root);
  /**
   * Recomputes the 2D desktop mouse cursor position on based on specified MPD 3D
   * position. Stores derived MPD state internally.
   *
   * Intersects the given MPD position with the screen cuboid. Returns immediately if the
   * MPD resides outside the screen cuboid. Otherwise computes new desktop mouse cursor
   * coordinates.
   *
   * @param mpd_pos MPD position
   */
  void updateMPD(Event const & mpd_pos);
  /**
   * Returns the current location of the MPD relative to the screen cuboid.
   * @return MPD location relative SC
   * @see MPDLocationState enumeration for possible return values
   */
  MPDLocationState getMPDLocation() const;
  /**
   * Return true if MPD resides inside the screen cuboid.
   * @return true if inside SC, otherwise false
   */
  inline bool isMPDInsideScreenCuboid() const { return mp_data_.mpd_loc_inside_screen_cuboid; };
  /**
   * Returns the desktop mouse cursor (x,y) coordinate pair derived from latest perceived
   * MPD position.
   * @return desktop mouse coordinates
   */
  inline Point getMPD2DCoords() const { return mp_data_.desktop_coords; };
  /**
   * Returns the latest perceived spatial 3D MPD position.
   * @return MPD 3D position
   */
  inline Event getMPD3DPosition() const { return mp_data_.position; };

  /**
   * Initializes all static calibration data.
   * 
   */
  void initCalibrationData(std::vector<float> aSWidthVec_, std::vector<float> aSHeightVec_, 
	  std::vector<float> cSOrientationQuat_, std::vector<float> cSRoot2ScreenRootVec_,  
	  float trackingSystemScaleOneMeter_, float screenDepthFrontInMeter_, 
	  float screenDepthBackInMeter_, float screenResWidth_, float screenResHeight_);


private: // members
  /// calibration geometrical data from the XML configuration file
  CalibOutputData calib_out_;
  /// initial application screen coordinate system and screen plane location (remains constant after init)
  ASData as_data_init_;
  /// current application screen coordinate system and screen plane location
  ASData as_data_;
  /// MPD position, state (concerning location relative to SC) and mouse cursor coordinates
  MPData mp_data_;

  float screenResWidth, screenResHeight;
};

} // namespace ot

#endif // USE_THREETOTWODIMFILTER

#endif // _THREETOTWODIMCALC_H


/*
 * ------------------------------------------------------------
 *   End of ThreeToTwoDimCalc.h
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
