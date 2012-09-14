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
/** header file for VRPNSource node.
 *
 * @author Gerhard Reitmayr
 *
 * $Id: VRPNSource.h 1547 2006-10-25 10:33:10Z veas $
 * @file                                                                   */
/* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section vrpnsource VRPNSource
 *
 * The VRPNSource node acts as a client for a VRPN server. It supports receiving 
 * either a single tracker station or a button server with up to 8 buttons. The
 * attribute @c type selects between the two modes. The @c station attribute selects
 * the tracker station to report on. The most important attribute is @name which
 * specifies the server device to connect to in the usual VRPN format. See 
 * http://www.vrpn.org/ for details on VRPN. The @ref vrpnmodule manages this node.
 *
 * It has the following attributes :
 * @li @c name the VRPN server device name, something like "tracker0@somehost:12345"
 * @li @c type (tracker|button) decides whether to connect using the tracker or the button interface
 * @li @c station to select the tracker station to report on. starts with 0.
 *
 * An example element looks like this :
 * @verbatim<VRPNSource name="tracker0@somehost:12345" type="tracker" station="0"/>@endverbatim
 */

#ifndef _VRPNSOURCE_H
#define _VRPNSOURCE_H

#include "../OpenTracker.h"

#ifdef USE_VRPN

class vrpn_BaseClass;

namespace ot {

    /**
     * VRPN client interface node. Connects to a VRPN server and reports incoming 
     * tracking data.
     * @author Gerhard Reitmayr
     * @ingroup network
     */
    class OPENTRACKER_API VRPNSource : public Node
    {
    public:
        /// name
        std::string name;
        /// type of connection
        enum Type { TRACKER, BUTTON } type;
        /// station number of station to report
        int station;
        /// event object for data flow
        Event event;
    
    protected:

        /// data pointer to underlying vrpn object
        vrpn_BaseClass * trackerObj;

        /** constructor method,sets commend member */
        VRPNSource();

        /** Opens connection to the VRPN server. Only for internal use
         * by the associated module. 
         */
        void start();

        /** Executes the vrpn object's mainloop. Only for internal use
         * by the associated module. 
         */
        void mainloop();    

    public:
        /** tests for EventGenerator interface being present. Is overriden to
         * return 1 always.
         * @return always 1 */
        virtual int isEventGenerator()
        {
            return 1;
        }
    
        /// destructor
        virtual ~VRPNSource();

        friend class VRPNModule;
    };

} // namespace ot

#endif

#endif

/* 
 * ------------------------------------------------------------
 *   End of VRPNSource.h
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
