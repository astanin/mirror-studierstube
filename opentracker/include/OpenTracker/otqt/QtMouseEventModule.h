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
 * @file   QtMouseEventModule.h
 * @author Christian Pirchheim
 *
 * @brief  Interface of class @c QtMouseEventModule
 *
 * $Id: OpenTracker.h 900 2006-01-19 16:47:43Z spiczak $
 */
/**
 * @page module_ref Module Reference
 * @section qtmouseeventmodule QtMouseEventModule
 *
 * Simulates the behaviour of a desktop mouse by generating Qt mouse events from incoming
 * tracking data. Posts these events to some target Qt application. Associated sink nodes
 * (see @ref sec_otqt_impl_sinks in the %OTQt manual) provide the tracking data. The
 * nested @ref qtappscreen node stores the spatial location and extent of the target
 * desktop screen.
 *
 * Besides manual editing, the XML elements @c QtMouseEventConfig, @c QtAppScreen and its
 * attribute values can be generated automatically by applying the OTQt configuration tool
 * (see @ref sec_otqt_configuration in the OTQt manual).
 *
 * Has the following XML attributes:
 *
 * @li @c TrackingSystemScaleOneMeter one real-world meter in tracking system scale
 * (optional; default value: "1.0")
 * @li @c ScreenDepthFrontInMeter lenght of depth vector (z-axis) perpendicular
 * on the screen plane, spanning the mouse sensitive volume (screen cuboid) in
 * the front direction of the target desktop screen plane (optional; default
 * value: "0.01" - is 1 cm)
 * @li @c ScreenDepthBackInMeter lenght of depth vector (z-axis) perpendicular
 * on the screen plane, spanning the mouse sensitive volume (screen cuboid) in
 * the back direction of the target desktop screen plane (optional; default
 * value: "0.005" (yes, also a positive value!) - is 5 mm)
 *
 * The nested @ref qtappscreen element is mandatory.
 *
 * An example element looks like this:
 *
@verbatim
<QtMouseEventConfig TrackingSystemScaleOneMeter="1.0" ScreenDepthInMeter="0.01" >
  <QtAppScreen
    ASHeightVec="0.0000 -0.9999 0.0000"
    ASWidthVec="0.9999 0.0000 0.0000"
    CSOrientationQuat="0.0000 0.0000 -0.7071 0.7071"
    CSRoot2ScreenRootVec="-0.9999 -0.9999 0.0000"
  />
</QtMouseEventConfig>@endverbatim
 */

#ifndef OTQT_QTMOUSEEVENTMODULE_H
#define OTQT_QTMOUSEEVENTMODULE_H
#include <OpenTracker/dllinclude.h>
#if USE_OTQT

#include <OpenTracker/otqt/QtMouseEventModuleBase.h>
#include <OpenTracker/otqt/QtAppScreen.h>
#ifdef USE_QT4
#include <Qt/qrect.h>
#include <Qt/qevent.h>
#include <Qt/qpoint.h>
#include <Qt/qdatetime.h>
#else
#include <qrect.h>
#include <qevent.h>
#include <qpoint.h>
#include <qdatetime.h>
#endif
#include <list>

namespace ot {

//--------------------------------------------------------------------------------
/**
 * @class QtMouseEventModule
 * @ingroup group_otqt_classes
 * @brief %Module (MEM) employed in the operation mode
 *
 * The @e operation @e mode is executed within the target application context where the
 * module actually generates mouse events from tracking data.
 *
 * The @e operation @e mode describes the actual mouse event generating procedure of
 * OTQt. It assumes, that the OTQt configuraton steps have been successfully performed. A
 * final XML configuration file was produced and a OTQt hook was implemented in the target
 * Qt application. With the start of the Qt application the OTQt module is initalized and
 * started.
 *
 * The initialization startup takes place in member methods of the OTQt class: the
 * OpenTracker context is initalized, the OTQt modules objects are created and added to
 * context factories, the provided XML configuration file is parsed, the MEM is
 * initialized which generates the appropriate sink objects.  Finally a timer thread using
 * the Qt class QTimer is initialized with a default timeout of 20 milliseconds calling
 * the OTQt::driveOT() method at the end of each timeout performing one OpenTracker cycle.
 *
 * The design principle concerning sinks and modules is the following: The sinks are kept
 * rather "stupid" as they basically store copies of incoming events.  Some sinks provide
 * simple but useful methods which indicate changes in tracking event data. Most of the
 * work is done in the module (MEM) itself: the application screen 3D location management,
 * the mouse cursor placement and the generation of button and wheel events. There main
 * reason for that design decision is simple: only in the module all information is
 * available to decide wheater certain Qt mouse events should be generated or not.
 *
 * The OTQt sinks store incoming events only if the pending events differ from the last
 * event which was processed in the MEM. Each sink employs a two element event queue
 * internally where current and previous events are stored.  If the pending event differs
 * from the current event, the event is copied and stored as new current event while the
 * old current event becomes the previous event. The original event is passed unchanged to
 * parent nodes of the sink. The intention of this early event filtering is the reduction
 * of computational load.  Only relevant events are processed afterwards in the
 * module. Each sink indicates the arrival of a new event by setting the @e pending
 * @e event dirty bit.
 *
 * The @e pending @e event dirty bit is the signal for the module that a sink provides a
 * new, unseen tracking event. The main processing method of the MEM is the pullState()
 * method which is triggered from within the OpenTracker cycle method OTQt::driveOT()
 * after all sinks received their events and could set their dirty bit flag.
 */
class OPENTRACKER_API QtMouseEventModule : public QtMouseEventModuleBase {
private:
  /**
   * @class ButtonEvent
   * @brief Describes a mouse button event
   */
  typedef struct {
    /// global desktop mouse cursor position
    QPoint global_pos;
    /// button event type (using native Qt type values)
    QEvent::Type type;
    /// true if current button event type differs from previous button event type
    bool type_changed;
    /// button id (using values declared in class MouseButtonSink)
    int button_id;
    /// event creation time
    QTime timestamp;
    /// Qt widget under global mouse cursor pos at the time of the event (use with care!)
    QWidget * target_widget;
  } ButtonEvent;
  /**
   * @class ButtonEventHistory
   * @brief Records a collection of special button events from the past
   */
  typedef struct {
    /// latest press event which was NOT identified as double click press (latest single
    /// press event)
    ButtonEvent last_init_press;
    /// latest release event
    ButtonEvent last_release;
    /// latest double click event
    ButtonEvent last_dblclick;
  } ButtonEventHistory;
  /// type defintion of a button event list
  typedef std::list<ButtonEvent> ButtonEventList;
  /// type defintion of a native Qt MouseEvent list
  typedef std::list<QMouseEvent *> QMouseEventList;
  /// type defintion of a native Qt WheelEvent list
  typedef std::list<QWheelEvent *> QWheelEventList;

public:
  /**
   * Calls base class default constructor.
   */
  QtMouseEventModule();
  ~QtMouseEventModule();

private:
  QtMouseEventModule(QtMouseEventModule const & ref);
  QtMouseEventModule  operator=(QtMouseEventModule const & ref);

public:

  ///// class NodeFactory interface

//  virtual Node * createNode(std::string const & name,  StringTable& attributes);

  ///// class Module interface

  /**
   * Calls base class method. Iterates through the child nodes of the given config node
   * tree. Encountering a @c QtAppScreen XML element, creates such an object. Passing the
   * local XML attributes to the class @c QtAppScreen constructor. Enables the @c
   * CONFIGURED state.
   *
   * @param attributes XML attribute key/value map
   * @param localTree module configuration XML element inclusive nested elements
   *
   * @see QtMouseEventModuleBase::init()
   */
  virtual void init(StringTable& attributes,  ConfigNode * localTree);
  /**
   * Calls the base class method. Verifies whether all class members have been initialized
   * correctly during the XML configuration file parsing procedure, especially the @c
   * QtAppScreen object (see init() method). If verified positively, enables the @c
   * CONFIGURED state flag. Otherwise disables the @c CONFIGURED and enables the @c STOP
   * state flag. Prints error messages to stderr stream for each non-successful check.
   *
   * @see QtMouseEventModuleBase::start()
   */
  virtual void start();
  //virtual int stop();
  /**
   * Does nothing.
   */
  virtual void close();
  /**
   * The processing of pending events starts with the ASPS. The MEM employs a separate
   * class QtAppScreen which provides methods to update the geometric information about
   * the location of the desktop screen in case the ASPD changed its position and/or
   * orientation. Without accurate information about the location and extent of the
   * desktop screen, the second step, the computation of the mouse cursor coordinates
   * would be impossible. OTQt defines a certain 3D volume located in front of the target
   * desktop screen as mouse event sensible region. This region is called screen cuboid
   * (SC).
   *
   * The consequent flow of the procedure mainly depends on whether the MPD resides inside
   * or outside the SC. If it is outside, the module instructs the sinks to refuse
   * tracking events for the duration of the MPD residing outside the SC and quits. At the
   * time the MPD reenters the SC this blockade is annulated. If the MPD resides inside
   * the SC, the MPD position is used to update the desktop cursor coordinate. On a
   * desktop coordinate change a MouseMove Qt event is created.
   *
   * Next, regarding the MBS, the current and previous event of each mouse button
   * (typically left, right and middle button) are compared, searching for bit transitions
   * which indicate either MouseButtonPress or MouseButtonRelease events. To detect the
   * special case of a MouseDoubleClick event, which is generated if the event triple
   * press, release, press occurs within a certain time frame, the MEM records the latest
   * mouse press and release event together with their timestamps in a dedicated data
   * structure.
   *
   * The generation of mouse wheel events also bases on the comparion of current and
   * previous event within the MWS. Certain bit transitions indicate either a Wheel
   * Forward Move or Wheel Backward Move expressed by a positive or negative delta value
   * in the Qt mouse wheel event.
   *
   * At the end of the pullState() method the generated events collected in lists are
   * posted to the target Qt application using the non-blocking postEvent() method which
   * takes Qt event object pointers as argument. The deallocation of the heap objects is
   * done by Qt.// At any point the pullState() method exits, the pending event flags of all
   * sinks are reseted.//
   */
  virtual void pullEvent();

private:
  /// collection of target application desktop screen information
  QtAppScreen * as_;
  /// event history for each mouse button
  ButtonEventHistory button_event_history_[QtMouseButtonSink::BUTTON_COUNT];
};

} // namespace ot

#endif // USE_OTQT

#endif // OTQT_QTMOUSEEVENTMODULE_H

/*
 * ------------------------------------------------------------
 *   End of QtMouseEventModule.h
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
