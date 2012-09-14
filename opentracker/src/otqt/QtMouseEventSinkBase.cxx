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
 * @file   QtMouseEventSinkBase.cxx
 * @author Christian Pirchheim
 *
 * @brief  Implementation of class @c QtMouseEventSinkBase
 *
 * $Id: OpenTracker.h 900 2006-01-19 16:47:43Z spiczak $
 */

#include <OpenTracker/dllinclude.h>
#if USE_OTQT

#include <OpenTracker/otqt/QtMouseEventSinkBase.h>
#include <OpenTracker/otqt/OTQtLog.h>

namespace ot {

//--------------------------------------------------------------------------------
QtMouseEventSinkBase::QtMouseEventSinkBase(StringTable & xml_attrib_table)
  : state_(0),
    curr_event_(Event::null),
    prev_event_(Event::null),
    xml_attrib_table_(xml_attrib_table)
{

  ///// compute thresholds

  float one_meter = 0.0;
  xml_attrib_table.get("TrackingSystemScaleOneMeter", &one_meter);

  // position threshold filter radius
  float radius = 0.0;
  xml_attrib_table.get("PosThreshRadiusInMeter", &radius);
  if (radius > 0) {
    POS_THRESH_RADIUS = one_meter * radius;
    enableState(POS_THRESH_FILTER, true);
  }
  OTQT_DEBUG("QtMouseEventSinkBase::QtMouseEventSinkBase(): POS_THRESH_RADIUS = %f\n",
             POS_THRESH_RADIUS);

  /// schrott (!!!)

  // orientation threshold filter angle
  float angle = 0.0;
  xml_attrib_table.get("OrientThreshAngle", &angle);
  if (angle != 0) {
    MathUtils::eulerToQuaternion(angle, angle, angle, MathUtils::XYZ, ORIENT_THRESH_QUAT);
    enableState(ORIENT_THRESH_FILTER, true);
  }
  OTQT_DEBUG("QtMouseEventSinkBase::QtMouseEventSinkBase(): ORIENT_THRESH_QUAT = %f %f %f %f\n",
             ORIENT_THRESH_QUAT[0], ORIENT_THRESH_QUAT[1], ORIENT_THRESH_QUAT[2], ORIENT_THRESH_QUAT[3]);

  // consume event aka global forward lock switch
  std::string consume_events_response = xml_attrib_table.get("consume-events");
  enableState(CONSUME_EVENTS, ((consume_events_response == "true") ? true : false));
  OTQT_DEBUG("QtMouseEventSinkBase::QtMouseEventSinkBase(): StateFlag::CONSUME_EVENTS = %if\n",
             (state_ & CONSUME_EVENTS));
}

//--------------------------------------------------------------------------------
bool QtMouseEventSinkBase::enableState(StateFlag flag, bool enable)
{
  if (enable) { state_ |= flag; }
  else { state_ &= ~flag; }
  return (state_ & flag);
}

//--------------------------------------------------------------------------------
bool
QtMouseEventSinkBase::isInsidePosThreshSphere(Event const & event) const
{
  return (OTQtMath::distance(event.getPosition(), curr_event_.getPosition()) <= POS_THRESH_RADIUS);
}

//--------------------------------------------------------------------------------
bool
QtMouseEventSinkBase::isInsideOrientThreshCone(Event const & event) const
{
  std::vector<float> orient_conj(4);
  MathUtils::invertQuaternion(curr_event_.getOrientation(), orient_conj);
  std::vector<float> orient_diff(4);
  MathUtils::multiplyQuaternion(event.getOrientation(), orient_conj, orient_diff);

  for (int i = 0; i < 4; i++) {
    if (fabsf(orient_diff[i]) > fabsf(ORIENT_THRESH_QUAT[i]))
      return false;
  }
  return true;
}

//--------------------------------------------------------------------------------
void
QtMouseEventSinkBase::acquireEvent(Event const & event)
{
  // acquire tracking event
  prev_event_ = curr_event_;
  curr_event_ = event;
  // set pending flag
  state_ |= EVENT_PENDING_SIGNAL;
}

//--------------------------------------------------------------------------------
void QtMouseEventSinkBase::forwardEvent(Event & event)
{
    // consume the event
    if ((state_ & CONSUME_EVENTS) && (state_ & EVENT_CONSUME_SIGNAL))
        return;
    // forward the event: pass it to parent nodes
    updateObservers(event);
}

} // namespace ot

#endif // USE_OTQT


/*
 * ------------------------------------------------------------
 *   End of QtMouseEventSinkBase.cxx
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
