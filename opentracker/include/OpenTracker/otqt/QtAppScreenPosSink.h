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
 * @file   QtAppScreenPosSink.h
 * @author Christian Pirchheim
 *
 * @brief  Interface of class @c QtAppScreenPosSink
 *
 * $Id: OpenTracker.h 900 2006-01-19 16:47:43Z spiczak $
 */
/**
 * @page Nodes Node Reference
 * @section qtappscreenpossink QtAppScreenPosSink
 *
 * Provides the @ref qtmouseeventmodule module with position and orientation data, used to
 * calculate the spatial location of the target desktop screen (display of the target Qt
 * application). Designed to be linked with a 6-DOF tracked input device, the so-called
 * Application Screen Position Device (ASPD).
 *
 * Has the following XML attributes:
 *
 * @li @c PosThreshRadiusInMeter radius of threshold sphere, applied in position filter
 * (optional; default value: "0.0" -> position filter disabled) .. @ref
 * anc_position_filter "more"
 * @li @c OrientThreshAngle radian angle used equally for all XYZ axes to describe an
 * euler rotation, applied in orientation filter (optional; default value: "0.0" ->
 * orientation filter disabled) .. @ref anc_orientation_filter "more"
 * @li @c consume-events Either "true" (enabled) or "false" (disabled, default). If
 * enabled, tracking events are not forwarded to their parent nodes within the OT node
 * graph iff they are @e consumed within the %OTQt MEM, that is, if a specific tracking
 * event was used to generate Qt mouse events .. @ref anc_xmlattrib_consume_events "more"
 *
 * An example element looks like this:
 *
@verbatim
<QtAppScreenPosSink PosThreshRadiusInMeter="0.001" OrientThreshAngle="0.1" consume-events="false" >
  <Any EventGenerator element type>
</QtAppScreenPosSink>@endverbatim
 *
 * @see class ot::QtAppScreenPosSink description
 */

#ifndef OTQT_QTAPPSCREENPOSSINK_H
#define OTQT_QTAPPSCREENPOSSINK_H
#include <OpenTracker/dllinclude.h>
#if USE_OTQT

#include <OpenTracker/otqt/QtMouseEventSinkBase.h>

namespace ot {

//--------------------------------------------------------------------------------
/**
 * @class QtAppScreenPosSink
 * @ingroup group_otqt_classes
 * @brief Destination sink of the ASPD
 *
 * Provides 3D position and orientation of the target applicaton desktop screen (6-DOF
 * device).
 *
 * @see class @c QtMouseEventSinkBase description
 */
class OPENTRACKER_API QtAppScreenPosSink : public QtMouseEventSinkBase {

public:
  /**
   * Calls base class constructor.
   * @see QtMouseEventSinkBase(StringTable&)
   */
  QtAppScreenPosSink(StringTable & xml_attrib_table)
    : QtMouseEventSinkBase(xml_attrib_table) { };
  ~QtAppScreenPosSink() { };

  void pushEvent();
  void pullEvent();

public:

  ///// class Node interface

  virtual void onEventGenerated(Event & event, Node & generator);
};

} // namespace ot

#endif // USE_OTQT

#endif // OTQT_QTAPPSCREENPOSSINK_H


/*
 * ------------------------------------------------------------
 *   End of QtAppScreenPosSink.h
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
