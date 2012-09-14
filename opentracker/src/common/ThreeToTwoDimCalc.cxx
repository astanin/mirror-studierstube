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
 * @file   ThreeToTwoDimCalc.cxx
 * @author Christian Pirchheim
 *
 * @brief  Implementation of class \c ThreeToTwoDimCalc
 *
 *  *************** still depends on OTQTMath ******************
 */

#include <OpenTracker/dllinclude.h>
#ifdef USE_THREETOTWODIMFILTER

#include <OpenTracker/common/ThreeToTwoDimCalc.h>
//#include <Qt/qapplication.h>
//#include <Qt/qrect.h>
//#include <Qt/qdesktopwidget.h>

#include <ace/OS.h>
#include <cmath>

namespace ot {

//--------------------------------------------------------------------------------
ThreeToTwoDimCalc::ThreeToTwoDimCalc():screenResWidth(1280),
									   screenResHeight(1024)
{

}

//--------------------------------------------------------------------------------
void ThreeToTwoDimCalc::initCalibrationData(std::vector<float> aSWidthVec_, std::vector<float> aSHeightVec_, 
									 std::vector<float> cSOrientationQuat_, std::vector<float> cSRoot2ScreenRootVec_,  
									 float trackingSystemScaleOneMeter_, float screenDepthFrontInMeter_, 
									 float screenDepthBackInMeter_, float screenResWidth_, float screenResHeight_)
{
	calib_out_.as_width_vec = aSWidthVec_;
	calib_out_.as_height_vec = aSHeightVec_;
	calib_out_.as_cs_orient = cSOrientationQuat_;
	calib_out_.as_cs_root_to_screen_root = cSRoot2ScreenRootVec_;

	// CS root / screen root position
	for (int i = 0; i < 4; i++) {
		as_data_init_.as_cs_root.getOrientation()[i] = calib_out_.as_cs_orient[i];
		as_data_init_.as_screen_root.getOrientation()[i] = calib_out_.as_cs_orient[i];
	}
	// CS root / screen root position, screen plane span vectors
	for (int i = 0; i < 3; i++) {
		as_data_init_.as_cs_root.getPosition()[i] = 0;
		as_data_init_.as_screen_root.getPosition()[i] = calib_out_.as_cs_root_to_screen_root[i];
		as_data_init_.as_width_vec[i] = calib_out_.as_width_vec[i];
		as_data_init_.as_height_vec[i] = calib_out_.as_height_vec[i];
	}
	// compute screen depth scale
	float tracking_one_meter = trackingSystemScaleOneMeter_;
	float as_depth_front_in_meter = screenDepthFrontInMeter_;
	float as_depth_back_in_meter = screenDepthBackInMeter_;
	as_data_init_.as_depth_scalar_front = tracking_one_meter * as_depth_front_in_meter;
	as_data_init_.as_depth_scalar_back = tracking_one_meter * as_depth_back_in_meter;

	///// assign current data the initial values
	as_data_.as_cs_root = as_data_init_.as_cs_root;
	as_data_.as_screen_root = as_data_init_.as_screen_root;
	as_data_.as_depth_scalar_front = as_data_init_.as_depth_scalar_front;
	as_data_.as_depth_scalar_back = as_data_init_.as_depth_scalar_back;
	for (int i = 0; i < 3; i++) {
		as_data_.as_width_vec[i] = as_data_init_.as_width_vec[i];
		as_data_.as_height_vec[i] = as_data_init_.as_height_vec[i];
	}

	///// init screen res
	screenResWidth = screenResWidth_;
	screenResHeight = screenResHeight_;

	///// init mouse position object data
	updateMPD(Event::null);
}

//--------------------------------------------------------------------------------
void ThreeToTwoDimCalc::convert(CalibOutputData const & out, StringTable & table)
{
  // generate attributes from calibration output data
  float tmp3[3], tmp4[4];
  table.put("CSOrientationQuat", ot::copyV2A(out.as_cs_orient, tmp4), 4);
  table.put("CSRoot2ScreenRootVec", ot::copyV2A(out.as_cs_root_to_screen_root, tmp3), 3);
  table.put("ASWidthVec", ot::copyV2A(out.as_width_vec, tmp3), 3);
  table.put("ASHeightVec", ot::copyV2A(out.as_height_vec, tmp3), 3);
}


//--------------------------------------------------------------------------------
void ThreeToTwoDimCalc::convert(CalibInputData const & in, CalibOutputData & out)
{
  ///// transform input corners to common coordinate system

  CalibInputData in_final;
  in_final.as_cs_root = in.as_cs_root;

  // top / left
  OTQtMath::transformVectorFromCSToCS(in.top_left.local_cs_root, in_final.as_cs_root,
                                      in.top_left.corner, in_final.top_left.corner);
  in_final.top_left.local_cs_root = in_final.as_cs_root;
  // top / right
  OTQtMath::transformVectorFromCSToCS(in.top_right.local_cs_root, in_final.as_cs_root,
                                      in.top_right.corner, in_final.top_right.corner);
  in_final.top_right.local_cs_root = in_final.as_cs_root;
  // bottom / right
  OTQtMath::transformVectorFromCSToCS(in.bottom_right.local_cs_root, in_final.as_cs_root,
                                      in.bottom_right.corner, in_final.bottom_right.corner);
  in_final.bottom_right.local_cs_root = in_final.as_cs_root;
  // bottom / left
  OTQtMath::transformVectorFromCSToCS(in.bottom_left.local_cs_root, in_final.as_cs_root,
                                      in.bottom_left.corner, in_final.bottom_left.corner);
  in_final.bottom_left.local_cs_root = in_final.as_cs_root;


  ///// save screen coordinate system orientation

  //  out.as_cs_orient.resize(4);
  //OTQT_DEBUG("ThreeToTwoDimCalc::convert(): out.as_cs_orient.size() = %i\n", out.as_cs_orient.size());
  for (int i = 0; i < 4; i++) {
    out.as_cs_orient[i] = in_final.as_cs_root.getOrientation()[i];
  }

  ///// difference vector between screen coordinate system root and screen root

  //  out.as_cs_root_to_screen_root.resize(3);
  for (int i = 0; i < 3; i++) {
    out.as_cs_root_to_screen_root[i] =
      in_final.top_left.corner.getPosition()[i] - in_final.as_cs_root.getPosition()[i];
  }

  ///// compute screen base vectors


  // +++ Coordinate System
  //
  //   ASP (coordinate system root (pos/orient)
  //   /
  //  /
  // SR (screen root == top/left)
  //
  // +++ Screen plane and corners
  //
  // P1 (top/left) ----- P2 (top/right)
  // |                   |
  // |   screen plane    |
  // |                   |
  // P4 (bottom/left) -- P3 (bottom/right)
  //
  // +++ Base Vectors
  //
  // P1 -> P2, P4 -> P3: "x-axis" : v1 (vec_tl_tr)
  // P1 -> P4, P2 -> P3 : "y-axis" : v2 (vec_tl_tr)

  // corners of screen
  RowVector tl(3); // P1
  float tmp[3];
  tl << ot::copyV2A(in_final.top_left.corner.getPosition(), tmp);
  RowVector tr(3); // P2
  tr << ot::copyV2A(in_final.top_right.corner.getPosition(), tmp);
  RowVector br(3); // P3
  br << ot::copyV2A(in_final.bottom_right.corner.getPosition(), tmp);
  RowVector bl(3); // P4
  bl << ot::copyV2A(in_final.bottom_left.corner.getPosition(), tmp);

  // vectors which span screen plane (cuboid)
  RowVector vec_tl_tr(3); // x - direction, v1
  RowVector vec_tl_bl(3); // y - direction, v2

  // base vector v1: top/left -> top/right
  vec_tl_tr = tr - tl;

  // diagonal vector: top/left -> bottom/right
  RowVector diag_tl_br = br - tl;
  // length parameter for diagonal vector, resets bottom/right corner position, difference
  // vector between bottom/right and top/right is normal on v1
  float k = DotProduct(vec_tl_tr, vec_tl_tr) / DotProduct(vec_tl_tr, diag_tl_br);
  // base vector v2: top/right -> (transformed) bottom/right
  vec_tl_bl = k * diag_tl_br - vec_tl_tr;

  //  out.as_width_vec.resize(3);
  // out.as_height_vec.resize(3);
  for (int i = 0; i < 3; i++) {
    out.as_width_vec[i] = vec_tl_tr(i+1);
    out.as_height_vec[i] = vec_tl_bl(i+1);
  }
}

//--------------------------------------------------------------------------------
void ThreeToTwoDimCalc::updateASPD(Event const & as_cs_root_curr)
{
  ///// transform screen position

  Event as_screen_root_curr;
  OTQtMath::transformVectorFromCSToCS(as_data_init_.as_cs_root, as_cs_root_curr,
                                      as_data_init_.as_screen_root, as_screen_root_curr);
  as_data_.as_screen_root = as_screen_root_curr;

  ///// transform screen plane base vectors
  std::vector<float> as_width_vec_curr(3);
  OTQtMath::rotateVectorFromCSToCS(as_data_init_.as_cs_root, as_cs_root_curr,
                                   as_data_init_.as_width_vec, as_width_vec_curr);
  std::vector<float> as_height_vec_curr(3);
  OTQtMath::rotateVectorFromCSToCS(as_data_init_.as_cs_root, as_cs_root_curr,
                                   as_data_init_.as_height_vec, as_height_vec_curr);
  for (int i = 0; i < 3; i++) {
    as_data_.as_width_vec[i] = as_width_vec_curr[i];
    as_data_.as_height_vec[i] = as_height_vec_curr[i];
  }

  // set current CS root
  as_data_.as_cs_root = as_cs_root_curr;
}

//--------------------------------------------------------------------------------
void ThreeToTwoDimCalc::updateMPD(Event const & mpd_pos)
{

  // save current MPD pos
  mp_data_.position = mpd_pos;

  ///// init vectors

  // base vector "x-axis"
  RowVector width(3);
  float tmp3[3];
  width << ot::copyV2A(as_data_.as_width_vec, tmp3);
  // base vector "y-axis"
  RowVector height(3);
  height << ot::copyV2A(as_data_.as_height_vec, tmp3);

  // base vector "z-axis" as cross product width x height
  RowVector depth = - OTQtMath::crossProductR3(width, height);
#if 0
  printf("ThreeToTwoDimCalc::updateMPD(): depth = width x height = %f %f %f\n",
             depth(1), depth(2), depth(3));
#endif
  RowVector depth1 = depth / depth.NormFrobenius();
  // add negative depth (back side of screen depth)
  depth = depth1 * (as_data_.as_depth_scalar_front + as_data_.as_depth_scalar_back);

  // screen root
  RowVector screen_root(3);
  screen_root << ot::copyV2A(as_data_.as_screen_root.getPosition(), tmp3);
  RowVector depth_screen_root = screen_root - (depth1 * as_data_.as_depth_scalar_back);

  // given MPD position
  RowVector mpos(3);
  mpos << ot::copyV2A(mpd_pos.getPosition(), tmp3);
#if 0
  printf("ThreeToTwoDimCalc::updateMPD(): mpos = %f %f %f\n",
	  mpos(1), mpos(2), mpos(3));
#endif
  ///// check position within screen cuboid

  RowVector distances_width(2);
  RowVector distances_height(2);
  RowVector distances_depth(2);

  bool isInside = true;
  isInside &= OTQtMath::isWithinPlaneBorders(mpos, width, screen_root, distances_width);
  isInside &= OTQtMath::isWithinPlaneBorders(mpos, height, screen_root, distances_height);
  isInside &= OTQtMath::isWithinPlaneBorders(mpos, depth, depth_screen_root, distances_depth);

#if 0
  printf("ThreeToTwoDimCalc::updateMPD(): depth = %f %f %f\n",
	  depth(1), depth(2), depth(3));
  printf("ThreeToTwoDimCalc::updateMPD(): width = %f, distances_width() = %f %f\n",
	  width.NormFrobenius(), distances_width(1), distances_width(2));
  printf("ThreeToTwoDimCalc::updateMPD(): height = %f, distances_height() = %f %f\n",
	  height.NormFrobenius(), distances_height(1), distances_height(2));
  printf("ThreeToTwoDimCalc::updateMPD(): depth = %f, distances_depth() = %f %f\n",
	  depth.NormFrobenius(), distances_depth(1), distances_depth(2));
  ACE_OS::sleep(ACE_Time_Value(1, 0));
#endif


  if (!isInside)
  {
	  
    // transition inside -> outside: MPD location changed
    bool mpd_loc_changed_this_cycle = (mp_data_.mpd_loc_inside_screen_cuboid);
    // MPD is located outside SC
    mp_data_.mpd_loc_inside_screen_cuboid = false;


	//printf("ThreeToTwoDimCalc::updateMPD(): depth = %f %f %f\n",depth(1), depth(2), depth(3));
	printf("ThreeToTwoDimCalc::updateMPD(): width = %f, distances_width() = %f %f\n",
		width.NormFrobenius(), distances_width(1), distances_width(2));
	printf("ThreeToTwoDimCalc::updateMPD(): height = %f, distances_height() = %f %f\n",
		height.NormFrobenius(), distances_height(1), distances_height(2));
	printf("ThreeToTwoDimCalc::updateMPD(): depth = %f, distances_depth() = %f %f\n",
		depth.NormFrobenius(), distances_depth(1), distances_depth(2));
    return;
  }

  // NOTE: MPD position is within screen cuboid

  ///// convert mouse position to desktop coordinates

  RowVector mpos_screen_coords(2);
  mpos_screen_coords(1) = distances_width(1);
  mpos_screen_coords(2) = distances_height(1);
  //OTQT_DEBUG("ThreeToTwoDimCalc::isMPDWithinScreenCuboid(): mpos_screen_coords = %f %f\n",
  //           mpos_screen_coords(1), mpos_screen_coords(2));
  // convert to desktop coordinates
  Point desktop_coords_new;


  // optimize this for clarity ... 
  desktop_coords_new.x = mpos_screen_coords(1) / width.NormFrobenius();
  desktop_coords_new.y = mpos_screen_coords(2) / height.NormFrobenius();

  /////  save MPD data

  // transition outside -> inside: MPD location changed
  bool mpd_loc_changed_this_cycle = !(mp_data_.mpd_loc_inside_screen_cuboid);
  // MPD is located INSIDE SC
  mp_data_.mpd_loc_inside_screen_cuboid = true;
  // desktop mouse cursor coordinates
  mp_data_.desktop_coords = desktop_coords_new;

  return;
}

//--------------------------------------------------------------------------------
ThreeToTwoDimCalc::MPDLocationState
ThreeToTwoDimCalc::getMPDLocation() const
{
  MPDLocationState ret = MPD_LOC_UNKNOWN;

  if (mp_data_.mpd_loc_inside_screen_cuboid)
    ret = MPD_LOC_INSIDE_SC;
  else
    ret = MPD_LOC_OUTSIDE_SC;

  return ret;
}

} // namespace ot

#endif // USE_THREETOTWODIMFILTER


/*
 * ------------------------------------------------------------
 *   End of ThreeToTwoDimCalc.cxx
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
