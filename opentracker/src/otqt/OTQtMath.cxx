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
 * @file   OTQtMath.cxx
 * @author Christian Pirchheim
 *
 * @brief  Implementation of class \c OTQtMath
 *
 * $Id: OpenTracker.h 900 2006-01-19 16:47:43Z spiczak $
 */

#include <OpenTracker/dllinclude.h>
#if USE_OTQT || USE_THREETOTWODIMFILTER

#include <OpenTracker/otqt/OTQtMath.h>
#include <OpenTracker/otqt/OTQtLog.h>

namespace ot {

//--------------------------------------------------------------------------------
std::vector<float> &
OTQtMath::rotateVectorFromCSToCS(Event const & cs_from, Event const & cs_to,
                                 std::vector<float> const & vec_local_from,
                                 std::vector<float> & vec_local_to)
{
  // Compute orig / target CS orientation difference
    std::vector<float> cs_from_orient_conj(4);
  MathUtils::invertQuaternion(cs_from.getOrientation(), cs_from_orient_conj);
  std::vector<float> cs_from_to_orient_diff(4);
  MathUtils::multiplyQuaternion(cs_to.getOrientation(), cs_from_orient_conj, cs_from_to_orient_diff);
  // Rotate vector
  MathUtils::rotateVector(cs_from_to_orient_diff, vec_local_from, vec_local_to);
  return vec_local_to;
}

//--------------------------------------------------------------------------------
Event &
OTQtMath::transformVectorFromCSToCS(Event const & cs_from, Event const & cs_to,
                                    Event const & vec_global, Event & vec_global_to)
{
  // Convert global vector to local vector relative to original CS
  std::vector<float> vec_local_from(3);
  for (int i = 0; i < 3; i++) {
    vec_local_from[i] = vec_global.getPosition()[i] - cs_from.getPosition()[i];
  }
  std::vector<float> vec_local_to(3);
  // Rotate local vector about the orientation (from - to) difference of given CS
  rotateVectorFromCSToCS(cs_from, cs_to, vec_local_from, vec_local_to);
  // Convert local vector to global vector relative to target CS
  for (int i = 0; i < 3; i++) {
    vec_global_to.getPosition()[i] = cs_to.getPosition()[i] + vec_local_to[i];
  }
  for (int i = 0; i < 4; i++) {
    vec_global_to.getOrientation()[i] = cs_to.getOrientation()[i];
  }
  return vec_global_to;
}

//--------------------------------------------------------------------------------
float
OTQtMath::distance(std::vector<float> const & v1, std::vector<float> const & v2)
{
  if (v1.size() != v2.size()) {
    throw Exception("OTQtMath::distance(): Argument vectors have different dimension.");
  }
#ifdef WIN32
  float tmp[3];
#else
  float tmp[v1.size()];
#endif
  RowVector vec1(v1.size());
  vec1 << ot::copyV2A<float>(v1, tmp);
  RowVector vec2(v2.size());
  vec2 << ot::copyV2A(v2, tmp);
  RowVector diff = vec1 - vec2;
  return (float)(diff.NormFrobenius());
}

//--------------------------------------------------------------------------------
RowVector
OTQtMath::crossProductR3(RowVector const & x, RowVector const & y)
{
  if (x.Ncols() != 3 || y.Ncols() != 3) {
    throw Exception("OTQtMath::crossProductR3(): One of given RowVectors is not a triple.");
  }

  RowVector z(3);
  z(1) = x(2)*y(3) - x(3)*y(2);
  z(2) = x(3)*y(1) - x(1)*y(3);
  z(3) = x(1)*y(2) - x(2)*y(1);

  return z;
}

//--------------------------------------------------------------------------------
bool
OTQtMath::isWithinPlaneBorders(RowVector const & mpos,
                               RowVector const & normal, RowVector const & point_on_plane,
                               RowVector & distances)
{
  //OTQT_DEBUG("OTQtMath::isWithinPlaneBorders(): *** START. normal = %f %f %f\n",
  //           normal(1), normal(2), normal(3));

  double normal_length = normal.NormFrobenius();
  double tmp2 = (normal * mpos.t()).AsScalar();
  double b;

  b = (normal * point_on_plane.t()).AsScalar();
  distances(1) = fabs(tmp2 - b) / normal_length ;

  b = (normal * (point_on_plane + normal).t()).AsScalar();
  distances(2) = fabs(tmp2 - b) / normal_length;

  // compare with max distance
  double max_distance = normal_length;
  //OTQT_DEBUG("OTQtMath::isWithinPlaneBorders(): max_distance = %f\n",
  //           max_distance);
  //OTQT_DEBUG("OTQtMath::isWithinPlaneBorders(): *** END. distances = %f %f\n",
  //           distances(1), distances(2));
  //printf("OTQtMath::isWithinPlaneBorders(): max_distance = %f\n", max_distance);
  //printf("OTQtMath::isWithinPlaneBorders(): *** END. distances = %f %f\n", distances(1), distances(2));
  return (distances(1) <= max_distance && distances(2) <= max_distance);
}

} // namespace ot

#endif // USE_OTQT


/*
 * ------------------------------------------------------------
 *   End of OTQtMath.cxx
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
