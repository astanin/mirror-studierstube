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
 * @file   QtMouseEventSinkBase.h
 * @author Christian Pirchheim
 *
 * @brief  Interface of class @c QtMouseEventSinkBase
 *
 * $Id: OpenTracker.h 900 2006-01-19 16:47:43Z spiczak $
 */

#ifndef OTQT_QTMOUSEEVENTSINKBASE_H
#define OTQT_QTMOUSEEVENTSINKBASE_H
#include <OpenTracker/dllinclude.h>
#if USE_OTQT

#include <OpenTracker/otqt/OTQtMath.h>
#include <OpenTracker/core/Event.h>
#include <OpenTracker/core/Node.h>
#include <OpenTracker/core/StringTable.h>

namespace ot {

//--------------------------------------------------------------------------------
/**
 * @class QtMouseEventSinkBase
 * @ingroup group_otqt_classes
 * @brief Base class of MEMs sink nodes
 *
 * %OTQt sinks are implemented as @e event @e generator nodes. Each sink node receives
 * tracking events from its child nodes and passes the event to its parent nodes in the
 * same cycle.
 *
 * %OTQt sinks basically store copies of incoming events (see onEventGenerated() method)
 * and provide methods to retrieve them (see getCurrentEvent(),
 * getPreviousEvent()). Different sinks provide useful methods which indicate changes in
 * tracking event data. Most of the work is done in the module (MEM) itself: the
 * application screen 3D location management, the mouse cursor placement and the
 * generation of button and wheel events. There main reason for that design decision is
 * simple: only in the module all information is available to decide wheater certain Qt
 * mouse events should be generated or not.
 *
 * The %OTQt sinks store incoming events only if the pending event differs from the last
 * event which was processed in the MEM. Each sink employs a two element event queue
 * internally where @e current and @e previous events are stored.  If the pending event
 * differs from the current event, the event is copied and stored as new current event
 * while the old current event becomes the previous event. The original event is passed
 * unchanged to parent nodes of the sink. The intention of this early event filtering is
 * the reduction of computational load.  Only relevant events are processed afterwards in
 * the module. Each sink indicates the arrival of a new event by setting the @e pending @e
 * event flag, which can be retrieved with isEventPending() and reset with
 * resetPendingEventBit() methods. Each sink's event acquisition can be suspended by
 * enabling the @c EVENT_LOCK state flag. Provides the enableState() method to switch
 * states.
 *
 * @b Filters (experimental!)
 *
 * This class provides additional mechanisms such as position (see method
 * isInsidePosThreshSphere()) and orientation (see method isInsideOrientThreshCone())
 * threshold filter. Derived sinks apply these methods in the acquisition of incoming
 * tracking events where convenient. Desired position and orientation threshold values can
 * be assigned over the sink's XML attributes. Assigning non-default values, the state
 * flags @c POS_THRESH_FILTER and/or @c ORIENT_THRESH_FILTER are enabled automatically
 * (see QtMouseEventSinkBase(StringTable &).
 *
 * In order to finally ignore a tracking event, the event must be catched by ALL applied
 * filters. Otherwise,
 *
 * The corresponding XML attributes are:
 *
 * @anchor anc_position_filter
 *
 * @li @c PosThreshRadiusInMeter radius of threshold sphere, applied in position filter
 * (optional; default value: "0.0" -> position filter disabled)
 *
 * The threshold sphere is determined by the last acquired input device position (center)
 * and the specified radius. Consecutive 3D tracking events are ignored, if they are
 * located inside the threshold sphere.
 *
 * @anchor anc_orientation_filter
 *
 * @li @c OrientThreshAngle radian angle used equally for all XYZ axes to describe an
 * euler rotation, applied in orientation filter (optional; default value: "0.0" ->
 * orientation filter disabled)
 *
 * The last acquired tracking event is the reference for the orienation
 * comparison. Consecutive tracking events are ignored, if their orientation difference on
 * all axes is less or equal the specified angle.
 *
 * @anchor anc_xmlattrib_consume_events
 *
 * @li @c consume-events Either "true" (enabled) or "false" (disabled, default). If
 * enabled, tracking events are not forwarded to their parent nodes within the OT node
 * graph iff they are @e consumed within the %OTQt MEM, that is, if a specific tracking
 * event was used to generate Qt mouse events.
 *
 * Example: Within a hybrid 2D/3D setup, where the same tracking devices are used, the
 * feature can be applied to filter all tracking events which have already been used
 * (consumed) within one UI (eg the 2D UI) and prevent them to be forwarded to the second
 * UI (eg the 3D UI) and causing unwanted interactions.
 *
 * Configuration example (schema):
 *
@verbatim
<NetworkSink target="3D-UI">
  <QtMouseButtonSink consume-events="true">
    <NetworkSource source="Tracking" />
  </QtMouseButtonSink>
</NetworkSink> @endverbatim
 *
 * Tracking events are generated by some source (here "Tracking") and enter the node graph
 * in the @c NetworkSink node. The @c QtMouseButtonSink is is connected to some 2D UI and
 * configured to @e consume events. Forwarded tracking events leave the graph through a @c
 * NetworkSink which sends them to another host where the 3D UI application is assumed.
 *
 * Only those tracking event pass the entire hierarchy, which are not @e consumed by the
 * %OTQt MEM, thus they where used to generate some mouse (button) event on the 2D
 * application. Otherwise the tracking event was not destined for the 2D UI and is
 * forwarded to the 3D UI.
 */
class OPENTRACKER_API QtMouseEventSinkBase : public Node {
public:  // typedefs, constants

  /// State flags
  enum StateFlag {
    /// if enabled, indicates the acquisition of a new @e current (pending) tracking event
    EVENT_PENDING_SIGNAL = 0x01,
    /// while enabled, the sink will not acquire tracking events (disabled by default)
    EVENT_ACQUIRE_LOCK = 0x02,
    /// enable/disable the position threshold filter applied in the acquisition of
    /// tracking event (disabled by default)
    POS_THRESH_FILTER = 0x04,
    /// enable/disable the orientation threshold filter applied in the acquisition of
    /// tracking event (disabled by default)
    ORIENT_THRESH_FILTER = 0x08,
    /// enable/disable event consumption; if enabled, the sink does not forward events to
    /// its parent nodes while the @c EVENT_CONSUME_SIGNAL is enabled (disabled by
    /// default)
    CONSUME_EVENTS = 0x10,
    /// enabled, the sink will not forward tracking events to its parents (disabled by
    /// default); this flag is used in conjuction with the @c CONSUME_EVENTS flag (and is
    /// thus overruled by that flag)
    EVENT_CONSUME_SIGNAL = 0x20
  };

protected:
  /// object state encoding type
  typedef unsigned short state_t;

public: // Ctor etc
  /**
   * Disables all state flags by default. Sets current and previous event to
   * State::null. Stores specified XML attribute map. Computes position and orientation
   * threshold values from XML attributes, enabling the corresponding @c POS_THRESH_FILTER
   * and @c ORIENT_THRESH_FILTER flags if appropriate.
   *
   * @param xml_attrib_table XML attribute key/value map
   */
  QtMouseEventSinkBase(StringTable & xml_attrib_table);
  virtual ~QtMouseEventSinkBase() { };

private:
  QtMouseEventSinkBase();
  QtMouseEventSinkBase(QtMouseEventSinkBase const & ref);
  QtMouseEventSinkBase & operator = (QtMouseEventSinkBase const & ref);

public:

  ///// class Node interface

  /**
   * OpenTracker tracking event push pattern callback method. Basically stores a copy of
   * the incoming event as new current event in the internal event queue and forwards the
   * original event to parent nodes.
   *
   * Applies a series of elimination checks on the incoming event. Ignores the event if
   * any of these checks fail. Otherwise acquires the event, thus the incoming event
   * becomes the new current event and the old current event becomes the new previous
   * event. Enables the @c EVENT_PENDING_SIGNAL flag to indicate an acquisition.
   *
   * Applied checks in detail: Ignores the incoming tracking event if ..
   *
   * @li the @c EVENT_LOCK flag is enabled (currently only implemented in MBS!).
   *
   * @li it equals the @e current event (only modified events pass). Different relevant
   * tracking data portions (position, orientation, button state) are compared, depending
   * on the actual sink.
   *
   * @li position threshold check fails (optional, depending on actual sink, @c
   * POS_THRESH_FILTER flag enabled).
   *
   * @li orientation threshold check fails (optional, depending on actual sink, @c
   * ORIENT_THRESH_FILTER flag enabled).
   *
   * Forwards the tracking event if ..
   *
   * @li either @c CONSUME_EVENTS or @c EVENT_CONSUME_SIGNAL are
   * disabled. If both are true the event is said to be locally @e
   * consumed and thus NOT forwarded.
   *
   * @param event tracking event
   * @param generator event generating child node
   */
  virtual void onEventGenerated(Event & event, Node & generator) = 0;
  /**
   * Returns true if node implements the OpenTracker @c event edge type interface: the
   * node pushes incoming events to its parent nodes.
   * @return true if node forwards incoming events to its parent nodes
   */
  inline virtual int isEventGenerator() { return true; }

  /**
   * Returns true if a new tracking event was acquired (available as current event)
   * indicated by the @c EVENT_PENDING_SIGNAL state flag.
   *
   * @return true if new current event available
   */
  inline bool isEventPending() const { return (state_ & EVENT_PENDING_SIGNAL); };
  /**
   * Disables the @c EVENT_PENDING_SIGNAL flag.
   */
  inline void resetPendingEventBit() { state_ &= ~EVENT_PENDING_SIGNAL; };
  /**
   * Returns the latest acquired tracking event (tracking node current event).
   * @return current tracking event of node
   */
  inline Event const & getCurrentEvent() const { return curr_event_; };
  /**
   * Returns the predecessor of the latest acquired tracking event (tracking node previous
   * event).
   * @return previous tracking event of node
   */
  inline Event const & getPreviousEvent() const { return prev_event_; };
  /**
   * Enables or disables the given state flag depending on the binary argument. Enables
   * flag if true (default), otherwise disables flag. Returns the state flag value after
   * the update was performed.
   *
   * @param flag state flag
   * @param enable set true if state flag should be enabled (default), otherwise disables
   *
   * @return enable/disable (true/false) value of the state flag after update
   */
  bool enableState(StateFlag flag, bool enable = true);
  /**
   * Forwards the given tracking event to its parents if either @c CONSUME_EVENTS or @c
   * EVENT_CONSUME_SIGNAL are disabled. If both are true the event is said to be locally
   * @e consumed and thus NOT forwarded.
   *
   * @param event tracking event to forward
   */
  void forwardEvent(Event & event);

protected:

  /**
   * Returns true if 3D position difference of given tracking event and current event does
   * not exceed pre-defined threshold distance (see POS_THRESH_RADIUS).
   *
   * @param event tracking event
   * @return true if positional distance less or equal than threshold value
   */
  bool isInsidePosThreshSphere(Event const & event) const;
  /**
   * Performs a trivial orientation comparison test. Computes the difference quaternion
   * angle between the given tracking event and the current event. Verifies the resulting
   * quaternion against some pre-defined threshold value (see ORIENT_THRESH_RADIUS) by
   * comparing the quaternion vector elements. Returns true if each difference quaternion
   * vector element absolute value is less or equal the corresponding absolute value of
   * the threshold quaternion vector.
   *
   * @param event tracking event
   * @return true if orientation distance less or equal than threshold value
   */
  bool isInsideOrientThreshCone(Event const & event) const;
  /**
   * Acquires the given tracking event as new current event. Resets the old current event
   * as new previous event. Discards the the old previous element. Enables the @c
   * EVENT_PENDING_SIGNAL state flag.
   *
   * @param event pending tracking event
   */
  void acquireEvent(Event const & event);

protected:
  /// state bit string
  state_t state_;
  /// current tracking event of node
  Event curr_event_;
  /// predecessor of current (previous) tracking event of node
  Event prev_event_;
  /// attributes of the node (and module) XML elements
  StringTable xml_attrib_table_;
  /// vector distance threshold value (derived from XML attribute values - pseudo
  /// constant)
  float POS_THRESH_RADIUS;
  /// orientation threshold value (derived from XML attribute values - pseudo constant)
  float ORIENT_THRESH_QUAT[4];

public:
  /// direct access on protected and private members granted
  friend class QtMouseEventModule;
};

} // namespace ot

#endif // USE_OTQT

#endif // OTQT_QTMOUSEEVENTSINKBASE_H

/*
 * ------------------------------------------------------------
 *   End of QtMouseEventSinkBase.h
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
