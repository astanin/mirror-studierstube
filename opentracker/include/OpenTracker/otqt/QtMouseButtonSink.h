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
 * @file   QtMouseButtonSink.h
 * @author Christian Pirchheim
 *
 * @brief  Interface of class @c QtMouseButtonSink
 *
 * $Id: OpenTracker.h 900 2006-01-19 16:47:43Z spiczak $
 */
/**
 * @page Nodes Node Reference
 * @section qtmousebuttonsink QtMouseButtonSink
 *
 * Provides the @ref qtmouseeventmodule module with input device button states, used to
 * compute Qt application mouse button events. Designed to be linked with the so-called
 * Mouse Button Device (MBD).
 *
 * @copydoc ot::QtMouseButtonSink
 *
 * Has the following XML attributes:
 *
 * @li @c consume-events Either "true" (enabled) or "false" (disabled, default). If
 * enabled, tracking events are not forwarded to their parent nodes within the OT node
 * graph iff they are @e consumed within the %OTQt MEM, that is, if a specific tracking
 * event was used to generate Qt mouse events .. @ref anc_xmlattrib_consume_events "more"
 *
 * An example element looks like this:
 *
@verbatim
<QtMouseButtonSink consume-events="true">
  <Any EventGenerator element type>
</QtMouseButtonSink>@endverbatim
 *
 */

#ifndef OTQT_QTMOUSEBUTTONSINK_H
#define OTQT_QTMOUSEBUTTONSINK_H
#include <OpenTracker/dllinclude.h>
#if USE_OTQT

#include <OpenTracker/otqt/QtMouseEventSinkBase.h>

#ifdef USE_QT4
#include <QtCore/qnamespace.h>
#else
#include <qnamespace.h>
#endif

namespace ot {

//--------------------------------------------------------------------------------
/**
 * @class QtMouseButtonSink
 * @ingroup group_otqt_classes
 * @brief Destination sink of the MBD
 *
 * Provides button state values either @e enabled or @e disabled for each button at a
 * time, at least the left, right and middle mouse button (see ButtonId enum).
 *
 * The mapping between the event button string (see class member State::button) and Qt
 * buttons implemented in the MBS looks the following:
 *
@verbatim
Slot           Qt Button Id

0              Qt::LeftButton
1              Qt::RightButton
2              Qt::MidButton
3-7            not used
@endverbatim
 *
 * A button is considered as "pressed" if the corresponding slot bit is equal to
 * @c 1 (@e true/enabled), otherwise the button is considered as "released".
 *
 * @see class @c QtMouseEventSinkBase description
 */

#if QT_VERSION >= 0x040000
	typedef Qt::MouseButton QMouseButton_t;
#else
	typedef Qt::ButtonState QMouseButton_t;
#endif

class OPENTRACKER_API QtMouseButtonSink : public QtMouseEventSinkBase {
public:
  /// number of buttons supported
  static int const BUTTON_COUNT = 3;
  /// Tracking event (State::button) bit to button identifier mapping
  enum ButtonId {
    /// left mouse button
    LEFT_BUTTON = 0,
    /// right mouse button
    RIGHT_BUTTON = 1,
    /// middle mouse button
    MIDDLE_BUTTON = 2
  };

public: // static
  /**
   * Maps an internal button id to the native Qt button id.
   *
   * Example: @c LEFT_BUTTON is mapped to @c Qt::LeftButton.
   *
   * @param button_id mouse button identifier
   * @return appropriate Qt mouse button identifier
   */
  static QMouseButton_t getQButtonId(ButtonId button_id);

public:
  /**
   * Calls base class constructor.
   * @see QtMouseEventSinkBase(StringTable&)
   */
  QtMouseButtonSink(StringTable & xml_attrib_table)
    : QtMouseEventSinkBase(xml_attrib_table)
    { };
  ~QtMouseButtonSink() { };

public:

  ///// class Node interface

  virtual void onEventGenerated(Event & event, Node & generator);

  /**
   * Returns true if for the specified button a state transition disabled -> enabled is
   * detected investigating the current and previous button events.
   *
   * @param button_id mouse button identifier
   * @return true if mouse event button was recently pressed
   */
  bool buttonPressed(ButtonId button_id) const;
  /**
   * Returns true if for the specified button a state transition enabled -> disabled is
   * detected investigating the current and previous button events.
   *
   * @param button_id mouse button identifier
   * @return true if mouse button was recently released
   */
  bool buttonReleased(ButtonId button_id) const;
  /**
   * Returns true if the specified button state is currently enabled.
   *
   * @param button_id mouse button identifier
   * @return true if mouse button is currently pressed
   */
  bool buttonOn(ButtonId button_id) const;
  
  void pushEvent();
  void pullEvent();

};

} // namespace ot

#endif // USE_OTQT

#endif // OTQT_QTMOUSEBUTTONSINK_H


/*
 * ------------------------------------------------------------
 *   End of QtMouseButtonSink.h
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
