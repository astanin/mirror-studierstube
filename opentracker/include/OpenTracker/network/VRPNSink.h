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
/** header file for VRPNSink node.
 *
 * @author Gerhard Reitmayr
 *
 * $Id: VRPNSink.h 1846 2007-05-23 08:16:47Z jfn $
 * @file                                                                   */
/* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section vrpnsink VRPNSink
 *
 * The VRPNSink node acts as a single tracking server device using VRPN. It 
 * provides its data via a single connection setup by the @ref vrpnmodule.
 * Therefore a single VRPNSink node is identifyable by the @c name attribute
 * that specifies the device name of the node. The @c type field selects the type
 * of device to be, either a tracker for 6DOF information or a button with up to 
 * 8 buttons. The values correspond to the OpenTracker event type. See 
 * http://www.vrpn.org/ for details on VRPN.
 * 
 * It has the following attributes :
 * @li @c name the VRPN device name to distinguish it from other devices on the server
 * @li @type @c (tracker|button) to configure the type of device to be
 * @li @station the station number to report on for a tracker device
 *
 * An example element looks like this :
 * @verbatim<VRPNSink name="tracker3" type="tracker" station="2">
 <Any EventGenerator element type>
 </VRPNSink>@endverbatim
*/

#ifndef _VRPNSINK_H
#define _VRPNSINK_H

#include "../OpenTracker.h"

#ifdef USE_VRPN

class vrpn_BaseClass;
class vrpn_Connection;

namespace ot {

    /** 
     * The node representing a VRPN server device. 
     * @author Gerhard Reitmayr
     * @ingroup network
     */
    class OPENTRACKER_API VRPNSink : public Node
    {
        // Members
    public:
        /// name
        std::string name;
        /// station number
        int station;
        /// type of connection
        enum Type { TRACKER, BUTTON } type;    

        // Methods
    protected:
    
        /// the VRPN server object
        vrpn_BaseClass * server;

        /** constructor method */
        VRPNSink();

        /** 
         * create the required VRPN device server objects attached to the
         * local server connection
         */
        void start(vrpn_Connection * connection);

        /** 
         * call VRPN mainloop
         */
        void mainloop();

    public:       
    
        /// destructor
        virtual ~VRPNSink();

        /** tests for EventGenerator interface being present. Is overriden to
         * return 1 always.
         * @return always 1 */
        virtual int isEventGenerator()
        {
            return 1;
        }
    
        /**
         * this method notifies the object that a new event was generated.
         * It stores a copy of the received event and passes the event on
         * to its observers.
         * @param event reference to the new event. Do not change the
         *        event values, make a copy and change that !
         * @param generator reference to the EventGenerator object that
         *        notified the EventObserver.
         */
        virtual void onEventGenerated( Event& event, Node& generator);

        void pushEvent() {};
        void pullEvent() {};

        friend class VRPNModule;
    };

} // namespace ot

#endif

#endif

/* 
 * ------------------------------------------------------------
 *   End of VRPNSink.h
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
