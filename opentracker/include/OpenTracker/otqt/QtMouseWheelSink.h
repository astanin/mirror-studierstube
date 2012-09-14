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
 * @file   QtMouseWheelSink.h
 * @author Christian Pirchheim
 *
 * @brief  Interface of class @c QtMouseWheelSink
 *
 * $Id: OpenTracker.h 900 2006-01-19 16:47:43Z spiczak $
 */
/**
 * @page Nodes Node Reference
 * @section qtmousewheelsink QtMouseWheelSink
 *
 * Provides the @ref qtmouseeventmodule module with input device wheel states, used to
 * compute Qt application mouse wheel events. Designed to be linked with the so-called
 * Mouse Wheel Device (MWD).
 *
 * @copydoc ot::QtMouseWheelSink
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
 <QtMouseWheelSink consume-events="false" >
 <Any EventGenerator element type>
 </QtMouseWheelSink>@endverbatim
 *
 */

#ifndef OTQT_QTMOUSEWHEELSINK_H
#define OTQT_QTMOUSEWHEELSINK_H
#include <OpenTracker/dllinclude.h>
#if USE_OTQT

#include <OpenTracker/otqt/QtMouseEventSinkBase.h>

namespace ot {

    //--------------------------------------------------------------------------------
    /**
     * @class QtMouseWheelSink
     * @ingroup group_otqt_classes
     * @brief Destination sink of the MWD
     *
     * Provides wheel state values either @e enabled or @e disabled for wheel forward and
     * backward rotation.
     *
     * The MWS uses he @c button member of the OpenTracker @c State event class and maps slots
     * to wheel events as follows. A wheel move is indicated by a binary slot value of
     * @c 1 (@e true/enabled).
     @verbatim
     Slot           Wheel Event

     0              Wheel Forward Move
     1              Wheel Backward Move
     2-7            not used
     @endverbatim
     *
     * Provides methods indicating wheel forward (see wheelForwardMove()) and wheel backward
     * (see wheelBackwardMove()) moves.
     */
    class OPENTRACKER_API QtMouseWheelSink : public QtMouseEventSinkBase {
    public:
        /**
         * Calls base class constructor.
         * @see QtMouseEventSinkBase(StringTable&)
         */
        QtMouseWheelSink(StringTable & xml_attrib_table)
            : QtMouseEventSinkBase(xml_attrib_table) { };
        ~QtMouseWheelSink() { };

    public:

        ///// class Node interface

        virtual void onEventGenerated(Event & event, Node & generator);

        /**
         * Returns true if a forward move state bit transition disabled -> enabled is discovered
         * investigating the current and previous wheel events.
         *
         * @return true if wheel recently moved forward
         */
        bool wheelForwardMove() const;
        /**
         * Returns true if a backward move state bit transition disabled -> enabled is
         * discovered investigating the current and previous wheel events.
         *
         * @return true if wheel recently moved backward
         */
        bool wheelBackwardMove() const;

        void pushEvent();
        void pullEvent();

    };

} // namespace ot

#endif // USE_OTQT

#endif // OTQT_QTMOUSEWHEELSINK_H

/*
 * ------------------------------------------------------------
 *   End of QtMouseWheelSink.h
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
