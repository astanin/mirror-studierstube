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
 * @file   QtMouseButtonSink.cxx
 * @author Christian Pirchheim
 *
 * @brief  Implementation of class @c QtMouseButtonSink
 *
 * $Id: OpenTracker.h 900 2006-01-19 16:47:43Z spiczak $
 */

#include <OpenTracker/dllinclude.h>
#if USE_OTQT

#include <OpenTracker/otqt/QtMouseButtonSink.h>
#include <OpenTracker/otqt/OTQtLog.h>

namespace ot {

//--------------------------------------------------------------------------------
void QtMouseButtonSink::onEventGenerated(Event & event, Node & generator) {
  OTQT_DEBUG("QtMouseButtonSink::onEventGenerated(): --- START: event.getButton() = %hx\n", event.getButton() );

  if (!(state_ & EVENT_ACQUIRE_LOCK)) {
    if (event.getButton() != curr_event_.getButton()) {
      // acquire tracking event
      acquireEvent(event);
    }
  }
  // pass original event to parent nodes
  forwardEvent(event);
}


    void QtMouseButtonSink::pushEvent()
    {
        // nothing to do
    }

    void QtMouseButtonSink::pullEvent()
    {
        // nothing to do
    }

//--------------------------------------------------------------------------------
bool QtMouseButtonSink::buttonPressed(ButtonId button_id) const {
  if (button_id < 0 || button_id >= BUTTON_COUNT)
    return false;

  if (((prev_event_.getButton() >> button_id) & 0x0001) == 0 &&
      ((curr_event_.getButton() >> button_id) & 0x0001) == 1) {
    return true;
  }
  return false;
}

//--------------------------------------------------------------------------------
bool QtMouseButtonSink::buttonReleased(ButtonId button_id) const {
  if (button_id < 0 || button_id >= BUTTON_COUNT)
    return false;

  if (((prev_event_.getButton() >> button_id) & 0x0001) == 1 &&
      ((curr_event_.getButton() >> button_id) & 0x0001) == 0) {
    return true;
  }
  return false;
}

//--------------------------------------------------------------------------------
bool QtMouseButtonSink::buttonOn(ButtonId button_id) const
{
  if (button_id < 0 || button_id >= BUTTON_COUNT)
    return false;
  return (((curr_event_.getButton() >> button_id) & 0x0001) == 1);
}

//--------------------------------------------------------------------------------
QMouseButton_t QtMouseButtonSink::getQButtonId(ButtonId button_id)
{
  QMouseButton_t ret = Qt::NoButton;

  switch (button_id) {

  case LEFT_BUTTON:
    ret = Qt::LeftButton;
    break;
  case RIGHT_BUTTON:
    ret =  Qt::RightButton;
    break;
  case MIDDLE_BUTTON:
    ret = Qt::MidButton;
    break;
  }

  return ret;
}

} // namespace ot

#endif // USE_OTQT


/*
 * ------------------------------------------------------------
 *   End of QtMouseButtonSink.cxx
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
