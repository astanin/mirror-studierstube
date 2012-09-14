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
 * @file   QtMouseEventModule.cxx
 * @author Christian Pirchheim
 *
 * @brief  Implementation of class @c QtMouseEventModule
 *
 * $Id: OpenTracker.h 900 2006-01-19 16:47:43Z spiczak $
 */

#include <OpenTracker/dllinclude.h>
#if USE_OTQT

#include <OpenTracker/otqt/QtMouseEventModule.h>
#include <OpenTracker/otqt/OTQtLog.h>
#ifdef USE_QT4
#include <Qt/qapplication.h>
#include <Qt/qcursor.h>
#include <Qt/qwidget.h>
#else
#include <qapplication.h>
#include <qcursor.h>
#include <qwidget.h>
#endif

namespace ot {

//--------------------------------------------------------------------------------
QtMouseEventModule::QtMouseEventModule()
  : QtMouseEventModuleBase(),
    as_(NULL)
{
}

//--------------------------------------------------------------------------------
QtMouseEventModule::~QtMouseEventModule()
{
  if (as_ != NULL) delete as_;
}


//--------------------------------------------------------------------------------
void QtMouseEventModule::init(StringTable & attributes, ConfigNode * localTree)
{
  // set state
  state_ |= CONFIGURED;

  ///// call base class method

  QtMouseEventModuleBase::init(attributes, localTree);

  ///// Run through <ConfigNodes />

  if (localTree != NULL) {

    ConfigNode * root = localTree;
    for (unsigned int i = 0; i < root->countChildren(); i++) {

      ConfigNode * child = dynamic_cast<ConfigNode *>(root->getChild(i));
      OTQT_DEBUG("QtMouseEventModule(): child.getType() = %s\n",
                 child->getType().c_str());

      // find <QtAppScreen> nodes
      if (child->getType().compare("QtAppScreen") == 0) {

        OTQT_DEBUG("QtMouseEventModule(): creating QtAppScreen object\n");

        // currently only single QtAppScreen is supported
        if (as_ != NULL) {
          OTQT_WARNING("QtMouseEventModule::init(): QtAppScreen object already created. "
                       "Another ConfigNode element 'QtAppScreen' found");
          continue;
        }

        // add values to attribute table
        StringTable & attrib_table = child->getAttributes();
        // add all attributes of the module
        KeyIterator it(xml_attrib_table_);
        while (it.hasMoreKeys()) {
          std::string key = it.nextElement();
          std::string value = xml_attrib_table_.get(key);
          attrib_table.put(key, value);
        }

        // create config node
        as_ = new QtAppScreen(attrib_table);

      }
    }
  }
}

//--------------------------------------------------------------------------------
void QtMouseEventModule::start()
{
  OTQT_DEBUG("QtMouseEventModule()::start(): *** START with state = %d\n", state_);
  // do nothing if module was not configured
  if (!(state_ & CONFIGURED))
    return;

  // set state
  state_ |= INITIALIZED;

  ///// call base method
  QtMouseEventModuleBase::start();

  ///// check configuration conditions
  bool ok = true;
  if (as_ == NULL) {
    OTQT_ERROR("Missing <QtAppScreen /> Node in OT configuration.");
    ok = false;
  }

  if (!ok) {
    state_ |= MEM_STOP;
    state_ &= ~INITIALIZED;
  }

  OTQT_DEBUG("QtMouseEventModule()::start(): *** FINISHED with state = %d\n", state_);
}

//--------------------------------------------------------------------------------
void QtMouseEventModule::close()
{
  OTQT_INFO("QtMouseEventModule closed.\n");
}


// //--------------------------------------------------------------------------------
// Node * QtMouseEventModule::createNode(std::string const & name, StringTable & attributes)
// {
//   OTQT_DEBUG("QtMouseEventModule()::createNode(): *** START with name = %s\n", name.c_str());
//   return QtMouseEventModuleBase::createNode(name, attributes);
// }


//--------------------------------------------------------------------------------
void QtMouseEventModule::pullEvent()
{
  OTQT_DEBUG("QtMouseEventModule()::pullState(): *** START with state = %d\n", state_);

  // return immediately if module is not correctly set up
  if (!(state_ & INITIALIZED))
    return;

  //////////// ASPS ////////////

  if (asp_sink_->isEventPending()) {
    OTQT_DEBUG("QtMouseEventModule()::pullState(): --- ASP is pending\n");
    // update AS data with new ASPD position/orientation
    as_->updateASPD(asp_sink_->getCurrentEvent());
  }

  //////////// MPS ////////////

  // MPD location relative SC changed from inside <-> outside in current cycle (default:
  // does not change)
  bool mpd_loc_changed_this_cycle = false;

  if (mp_sink_ != NULL && mp_sink_->isEventPending()) {

    OTQT_DEBUG("QtMouseEventModule()::pullState(): --- MP is pending\n");

    // MPD location relative SC before update
    bool mpd_loc_inside_sc_pre_update = as_->isMPDInsideScreenCuboid();
    // update MPD position data
    as_->updateMPD(mp_sink_->getCurrentEvent());
    // MPD location relative SC after update
    bool mpd_loc_inside_sc_post_update = as_->isMPDInsideScreenCuboid();
    // update MPD location changed flag: true if (0,1) (1,0), false otherwise
    mpd_loc_changed_this_cycle = (mpd_loc_inside_sc_pre_update ^ mpd_loc_inside_sc_post_update);
  }

  ///// check if latest known MPD position is within the 3D screen cuboid

  // new MPD position within screen cuboid -> new desktop coordinates
  bool update_desktop_mouse_pos = false;

  // get the desktop coordinates of the latest announced MPD position
  QPoint MPOS_DESKTOP_GLOBAL;

  // MPD is inside of screen cuboid
  if (as_->isMPDInsideScreenCuboid()) {
    OTQT_DEBUG("QtMouseEventModule()::pullState(): -> MPD inside\n");

    // MPD is inside SC (FIRST CYCLE)
    if (mpd_loc_changed_this_cycle) {

      OTQT_DEBUG("QtMouseEventModule()::pullState(): --> MPD inside first cycle\n");

      // enable the consume event signal
      enableEventConsumeSignalAllSinks(true);

      // unlock MBS event generation
      if (mb_sink_ != NULL) {
        mb_sink_->enableState(QtMouseButtonSink::EVENT_ACQUIRE_LOCK, false);
      }
    }
    // get desktop mouse coordinates
    MPOS_DESKTOP_GLOBAL = as_->getMPD2DCoords();
    update_desktop_mouse_pos = true;
  }

  // MPD is outside of screen cuboid
  else {

    // SPECIAL CASE: MPD is outside of screen cuboid (FIRST CYCLE)
    if  (mpd_loc_changed_this_cycle) {
      OTQT_DEBUG("QtMouseEventModule()::pullState(): -> MPD outside first cycle\n");

      // disable the consume event signal
      enableEventConsumeSignalAllSinks(false);

      if (mb_sink_ != NULL) {
        // reset all mouse buttons to released state no matter if event is currently
        // pending or not
        mb_sink_->acquireEvent(Event::null);
        // lock MBS event acquisition
        mb_sink_->enableState(QtMouseButtonSink::EVENT_ACQUIRE_LOCK, true);
      }

      // get latest valid desktop mouse coordinates
      MPOS_DESKTOP_GLOBAL = as_->getMPD2DCoords();
    }

    // MPD is outside of screen cuboid for >= 2 cycles
    else {
      OTQT_DEBUG("QtMouseEventModule()::pullState(): -> MPD outside >= 2 cylces\n");
      return;
    }
  }

  OTQT_DEBUG("QtMouseEventModule()::pullState(): MPOS_DESKTOP_GLOBAL = %d, %d\n",
             MPOS_DESKTOP_GLOBAL.x(), MPOS_DESKTOP_GLOBAL.y());

  ///// .. NOW ITS GETTING INTERESTING

  // VERY RARE :: mouse cursor was moved by some other device (while MPD within screen
  // cuboid) -> quit because subsequent MouseEvents relied on this position
  if (!update_desktop_mouse_pos && (QCursor::pos() != MPOS_DESKTOP_GLOBAL)) {
    QPoint mpos_desktop_global_curr = QCursor::pos();
    OTQT_DEBUG("QtMouseEventModule()::pullState(): Cursor::pos() = %d, %d\n",
               mpos_desktop_global_curr.x(), mpos_desktop_global_curr.y());
    return;
  }

  // set desktop mouse cursor position
  if (update_desktop_mouse_pos) {
    QCursor::setPos(MPOS_DESKTOP_GLOBAL.x(), MPOS_DESKTOP_GLOBAL.y());
    QPoint mpos_desktop_global_curr = QCursor::pos();
    OTQT_DEBUG("QtMouseEventModule()::pullState(): Cursor::pos() = %d, %d\n",
               mpos_desktop_global_curr.x(), mpos_desktop_global_curr.y());
  }

  // get current widget under global mouse position
#if QT_VERSION >= 0x040000
  QWidget * widget = QApplication::widgetAt(MPOS_DESKTOP_GLOBAL);
#else
  QWidget * widget = QApplication::widgetAt(MPOS_DESKTOP_GLOBAL, true);
#endif
  OTQT_DEBUG("QtMouseEventModule()::pullState(): widget pointer = %x (NULL?)\n",
             (unsigned int)widget);

  ///// Quit if no Qt widget under current desktop mouse position

  if (widget == NULL) {
    return;
  }

  // init QEvent lists
  QMouseEventList qt_mouse_events;
  QWheelEventList qt_wheel_events;

  // generate MouseMove event
  if (update_desktop_mouse_pos) {

    QMouseButton_t q_state = Qt::NoButton;

    // take first currently pressed button as value of 'state' variable
    if (mb_sink_ != NULL) {
      for (int button_id = 0; button_id < QtMouseButtonSink::BUTTON_COUNT; button_id++) {
        if (mb_sink_->buttonOn((QtMouseButtonSink::ButtonId)button_id)) {
          q_state = QtMouseButtonSink::getQButtonId((QtMouseButtonSink::ButtonId)button_id);
          break;
        }
      }
    }

    QMouseEvent * move = new QMouseEvent(QEvent::MouseMove,
                                         widget->mapFromGlobal(MPOS_DESKTOP_GLOBAL),
                                         MPOS_DESKTOP_GLOBAL,
                                         Qt::NoButton,
                                         q_state
#if QT_VERSION >= 0x040000
                                         , Qt::NoModifier
#endif
        );

    OTQT_DEBUG("QtMouseEventModule::pullState(): (EVENT) MouseMove: state Qt::Button = %d\n", q_state);
    qt_mouse_events.push_back(move);
  }


  //////////// MBS ////////////

  if (mb_sink_ != NULL) {
    // && mb_sink_->isStatePending()) { // commented out because of (currently disabled!)
    // button ON test (drag-and-drop issue)

    OTQT_DEBUG("QtMouseEventModule()::pullState(): --- MBS pending\n");

    ///// for each mouse button

    for (int button_id = 0; button_id < QtMouseButtonSink::BUTTON_COUNT; button_id++) {

      OTQT_DEBUG("QtMouseEventModule()::pullState(): +++ button id = %d\n", button_id);

      // get event history for the current button
      ButtonEventHistory * button_event_history = &button_event_history_[button_id];
      // init event container
      ButtonEventList button_event_list;

      // create event and init with default values
      ButtonEvent event;
      event.button_id = button_id;
      event.global_pos = MPOS_DESKTOP_GLOBAL;
      event.timestamp = QTime::currentTime();
      event.type_changed = true;
      event.target_widget = widget;
      // NOTE: event type is missing

      ///// button pressed / double click event

      if (mb_sink_->isEventPending() && mb_sink_->buttonPressed((QtMouseButtonSink::ButtonId)button_id)) {

        OTQT_DEBUG("QtMouseEventModule()::pullState(): -> buttonPressed.\n");

        ///// Mouse Double Click ?!

        bool is_double_click = false;

        int dbl_click_offset_msec = QApplication::doubleClickInterval();
        QTime now = QTime::currentTime();
        QTime dbl_click_thresh = now.addMSecs( - dbl_click_offset_msec);

        // get latest double click event
        ButtonEvent dblclick = button_event_history->last_dblclick;
        // get latest (initial) press event
        ButtonEvent press = button_event_history->last_init_press;

        if (press.timestamp > dblclick.timestamp &&
            press.button_id == button_id &&
            press.type == QEvent::MouseButtonPress &&
            press.timestamp >= dbl_click_thresh &&
            press.target_widget == widget)
          {
                // get latest release event
                ButtonEvent release = button_event_history->last_release;

                OTQT_DEBUG("QtMouseEventModule()::pullState(): release.button_id = %d\n", release.button_id);

                if (release.button_id == button_id &&
                    release.type == QEvent::MouseButtonRelease &&
                    release.timestamp >= press.timestamp &&
                    release.timestamp >= dbl_click_thresh &&
                    release.target_widget == widget)
                  {
                        // YES, it is a double click
                        is_double_click = true;
                  }
          }

        OTQT_DEBUG("QtMouseEventModule()::pullState(): is_double_click = %d\n", is_double_click);

        ///// Decision: press / dblclick

        if (is_double_click) {
          event.type = QEvent::MouseButtonDblClick;
          // save in history
          button_event_history->last_dblclick = event;
          OTQT_DEBUG("QtMouseEventModule::pullState(): (EVENT) QMouseButtonDblClick: button_id = %d\n",
                     button_id);
        }
        else {
          event.type = QEvent::MouseButtonPress;
          // save in history
          button_event_history->last_init_press = event;
          OTQT_DEBUG("QtMouseEventModule::pullState(): (EVENT) QMouseButtonPress: button_id = %d\n",
                     button_id);
        }

        // add to list
        button_event_list.push_back(event);
      }

      ///// button released event

      else if (mb_sink_->isEventPending() && mb_sink_->buttonReleased((QtMouseButtonSink::ButtonId)button_id)) {

        OTQT_DEBUG("QtMouseEventModule()::pullState(): -> buttonReleased.\n");

        // fill event data
        event.type = QEvent::MouseButtonRelease;

        OTQT_DEBUG("QtMouseEventModule::pullState(): (EVENT) QMouseButtonRelease: button_id = %d\n",
                   button_id);
        // save in history
        button_event_history->last_release = event;
        // add to list
        button_event_list.push_back(event);
      }

      ///// button ON

      else if (false && mb_sink_->buttonOn((QtMouseButtonSink::ButtonId)button_id)) {

        OTQT_DEBUG("QtMouseEventModule()::pullState(): -> button ON.\n");

        // fill event data
        event.type = QEvent::MouseButtonPress;
        event.type_changed = false;

        OTQT_DEBUG("QtMouseEventModule::pullState(): (EVENT) QMouseButtonPressON: button_id = %d\n",
                   button_id);
        // add to list
        button_event_list.push_back(event);
      }
      ///// no event

      else {

        OTQT_DEBUG("QtMouseEventModule()::pullState(): -> no Event.\n");
        continue;

      }

      ///// convert ButtonEvents -> QMouseEvents

      OTQT_DEBUG("QtMouseEventModule()::pullState(): -> convert ButtonEvents to QMouseEvents.\n");

      ButtonEventList::const_iterator it = button_event_list.begin();
      for ( ; it != button_event_list.end(); it++) {

        ButtonEvent event = (*it);

        QMouseButton_t q_button = QtMouseButtonSink::getQButtonId((QtMouseButtonSink::ButtonId)event.button_id);
        QMouseButton_t q_state = Qt::NoButton;
        if (event.type == QEvent::MouseButtonRelease
            /*|| (event.type == QEvent::QMouseButtonPress && !event.type_changed)*/) {
          q_state = q_button;
        }

        QMouseEvent * mouse = new QMouseEvent(event.type,
                                              widget->mapFromGlobal(MPOS_DESKTOP_GLOBAL),
                                              MPOS_DESKTOP_GLOBAL,
                                              q_button,
                                              q_state
#if QT_VERSION >= 0x040000
                                              , Qt::NoModifier
#endif
            );

        // add to event list
        qt_mouse_events.push_back(mouse);
      }

      // clear list
      button_event_list.clear();

    } // for each button

  } // MBS != NULL


  ////////// MWS ////////////

  if (mw_sink_ != NULL &&
      mw_sink_->isEventPending() &&
      (mw_sink_->wheelForwardMove() || mw_sink_->wheelBackwardMove())) {

    OTQT_DEBUG("QtMouseEventModule()::pullState(): --- MWS is pending.\n");

    // compute the delta value indicating space to jump
    int const DEFAULT_DELTA = 120;
    int delta = ((mw_sink_->wheelForwardMove()) ? DEFAULT_DELTA  : -DEFAULT_DELTA);
    delta *= QApplication::wheelScrollLines();
    OTQT_DEBUG("QtMouseEventModule()::pullState(): delta = %d\n", delta);

    QWheelEvent * wheel = new QWheelEvent(MPOS_DESKTOP_GLOBAL,
                                          widget->mapFromGlobal(MPOS_DESKTOP_GLOBAL),
                                          delta,
                                          Qt::NoButton,
#if QT_VERSION >= 0x040000
                                          Qt::NoModifier,
#endif
                                          Qt::Vertical);
    // add to event list
    qt_wheel_events.push_back(wheel);

  }

  ///// distribute Q[Mouse|Wheel]Events /////

  if (state_ & DISPATCH_QT_EVENTS) {
    OTQT_DEBUG("QtMouseEventModule()::dispatchQtEvents(): mouse_events.size() = %d\n", qt_mouse_events.size());
    // Mouse Events
    QMouseEventList::const_iterator it_m = qt_mouse_events.begin();
    for ( ; it_m != qt_mouse_events.end(); it_m++) {
      QApplication::postEvent(widget, (*it_m));
    }
    OTQT_DEBUG("QtMouseEventModule()::dispatchQtEvents(): wheel_events.size() = %d\n", qt_wheel_events.size());
    // Wheel Events
    QWheelEventList::const_iterator it_w = qt_wheel_events.begin();
    for ( ; it_w != qt_wheel_events.end(); it_w++) {
      QApplication::postEvent(widget, (*it_w));
    }
  }
  qt_mouse_events.clear();
  qt_wheel_events.clear();

  OTQT_DEBUG("QtMouseEventModule()::pullState(): *** FINISHED.\n");
}

} // namespace ot

#endif // USE_OTQT


/*
 * ------------------------------------------------------------
 *   End of QtMouseEventModule.cxx
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
