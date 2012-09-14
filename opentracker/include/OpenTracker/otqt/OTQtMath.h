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
 * @file   OTQtMath.h
 * @author Christian Pirchheim
 *
 * @brief  Interface of class \c OTQtMath
 *
 * $Id: OpenTracker.h 900 2006-01-19 16:47:43Z spiczak $
 */

#ifndef OTQT_OTQTMATH_H
#define OTQT_OTQTMATH_H
#include <OpenTracker/dllinclude.h>
#if USE_OTQT || USE_THREETOTWODIMFILTER

#define WANT_STREAM
#include <newmat10/include.h>
#include <newmat10/newmat.h>
#include <newmat10/newmatio.h>
#include <cmath>
#include <OpenTracker/core/MathUtils.h>
#include <OpenTracker/core/Event.h>

namespace ot {

using namespace NEWMAT;

//--------------------------------------------------------------------------------
/**
 * @class OTQtMath
 * @ingroup group_otqt_classes
 * @brief Mathematical toolbox
 */
class OPENTRACKER_API OTQtMath : public MathUtils {
public:

  /// vector in R3
  typedef float Vector3[3];

private: // Ctor/Dtor/operator=()
  OTQtMath();
  ~OTQtMath();
  OTQtMath(OTQtMath const & ref);
  OTQtMath & operator = (OTQtMath const & ref);

public: // static

  /**
   * Transforms the given positional R3 vector from the one (source) local coordinate
   * system into another (target) coordinate system. Computes the difference angle between
   * the two coordinate systems and rotates the positional vector around that
   * angle. Returns the resulting rotated positional vector and additionally passes it to
   * the specified vector array.
   *
   * @param cs_from source CS position/orientation
   * @param cs_to target CS position/orientation
   * @param vec_local_from positional vector (oriented at source CS)
   * @param vec_local_to transformed positional vector (oriented at target CS)
   *
   * @return transformed positional vector (oriented at target coordinate system)
   */
   static std::vector<float> & rotateVectorFromCSToCS(Event const & cs_from, Event const & cs_to,
                                                      std::vector<float> const & vec_local_from,
                                                      std::vector<float> & vec_local_to);

  /**
   * Transforms given R3 vector from one (source) local coordinate system to another
   * (target) local coordinate system.
   *
   * @param cs_from source CS position/orientation
   * @param cs_to target CS position/orientation
   * @param vec_global vector in world coordinates
   * @param vec_global_to transformed vector in world coordinates
   *
   * @return transformed vector in world coordinates
   */
  static Event & transformVectorFromCSToCS(Event const & cs_from, Event const & cs_to,
                                           Event const & vec_global, Event & vec_global_to);

  /**
   * Returns the distance (frobenius norm of the difference) between given vectors (equal
   * dimensions!).
   *
   * @param v1 vector
   * @param v2 vector
   *
   * @return distance (frobenius norm)
   */
   static float distance(std::vector<float> const & v1, std::vector<float> const & v2);

  /**
   * Returns cross product in of given R3 vectors according to the "right-hand rule".
   *
   * @param x "middlefinger"
   * @param y "forefinger"
   *
   * @return "thumb" normal vector
   */
  static RowVector crossProductR3(RowVector const & x, RowVector const & y);

  /**
   * Return true if the given position resides within the volume limited by the following
   * planes:
   *
   * @li the plane defined by the normal and point
   * @li the plane defined by the normal and (point + normal)
   *
   * Computes the distances of the given position to both planes. Compares the resulting
   * values to the length of the normal vector. Returns true if both distance values are
   * less or equal the length of the normal vector. Passes the distance values to the
   *
   * @param mpos point in R3
   * @param normal positional vector in R3
   * @param point_on_plane point in R3
   * @param distances two-element vector; placeholder for the computed distance values
   *
   * @return true if position resides inside the volume between planes defined by normal
   * and point
   */
  static bool isWithinPlaneBorders(RowVector const & mpos,
                                   RowVector const & normal, RowVector const & point_on_plane,
                                   RowVector & distances);
  /**
   * Returns the signum of given float number.
   *
   * @param x float number
   * @return -1 if less zero, 0 if equal zero, 1 if greater zero
   */
  static int fsignum(float x) {
    return (x < 0) ? -1 : (x > 0);
  };

};

} // namespace ot

#endif // USE_OTQT

#endif // OTQT_OTQTMATH_H


/*
 * ------------------------------------------------------------
 *   End of OTQtMath.h
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
