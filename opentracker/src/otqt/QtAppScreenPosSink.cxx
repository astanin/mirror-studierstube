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
 * @file   QtAppScreenPosSink.cxx
 * @author Christian Pirchheim
 *
 * @brief  Implementation of class @c QtAppScreenPosSink
 *
 * $Id: OpenTracker.h 900 2006-01-19 16:47:43Z spiczak $
 */

#include <OpenTracker/dllinclude.h>
#if USE_OTQT

#include <OpenTracker/otqt/QtAppScreenPosSink.h>

namespace ot {

    //--------------------------------------------------------------------------------
    void QtAppScreenPosSink::onEventGenerated(Event & event, Node & generator) {

        // switch 'acquire pending event' (default: yes)
        bool acquire = true;

        // basic check: position or orientation unchanged
        acquire = false;
        for (int i = 0; i < 3; i++) {
            if (event.getPosition()[i] != curr_event_.getPosition()[i] ||
                event.getOrientation()[i] != curr_event_.getOrientation()[i]) {
                acquire = true;
                break;
            }
        }
        if (acquire) {

            // at least one filter check must be performed
            bool one_true = false;
            // all performed filter checks must apply
            bool all_true = true;

            // within position threshold sphere ?!
            if (all_true && (state_ & POS_THRESH_FILTER)) {
                one_true = true;
                all_true &= isInsidePosThreshSphere(event);
            }
            // within orientation thresh cone ?!
            if (all_true && (state_ & ORIENT_THRESH_FILTER)) {
                one_true = true;
                all_true &= isInsideOrientThreshCone(event);
            }

            // do NOT filter per default
            bool discard = (one_true && all_true);

            // do NOT acquire only if ALL filters apply
            acquire = !discard;
        }
        // acquire tracking event
        if (acquire)
            acquireEvent(event);

        // pass original event to parent nodes
        forwardEvent(event);
    }

    void QtAppScreenPosSink::pushEvent()
    {
        // nothing to do
    }

    void QtAppScreenPosSink::pullEvent()
    {
        // nothing to do
    }
} // namespace ot

#endif // USE_OTQT


/*
 * ------------------------------------------------------------
 *   End of QtAppScreenPosSink.cxx
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
