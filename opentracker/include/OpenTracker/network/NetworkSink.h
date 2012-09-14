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
/** header file for NetworkSink Node.
 *
 * @author Gerhard Reitmayr, Mathis Csisinko
 *
 * $Id: NetworkSink.h 1807 2007-04-12 10:35:50Z bornik $
 * @file                                                                   */
/* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section networksink NetworkSink
 * The NetworkSink node is a simple EventObserver / EventGenerator that passes
 * events on and stores a copy of the last one. The @ref networksinkmodule checks on
 * each pass, whether a NetworkSink node stored a new event and sends that data
 * as the specified station to the network. The NetworkSink element has the
 * following attributes :
 * @li @c mode (unicast|multicast) chooses between multicast and unicast mode
 * @li @c name the stations name
 * @li @c number the (nonnegative) stations number
 * @li @c multicast-address the multicast group to send to in multicast mode
 * @li @c port port to send to in multicast mode or to listen for in unicast mode
 * @li @c interface ip address of the interface to work on 
 *  
 * An example element looks like this :
 * @verbatim
 <NetworkSink mode="unicast" name="station name" number="0" port="54321">
 <Any EventGenerator element type>
 </NetworkSink>
 <NetworkSink mode="multicast" name="station name" number="1" multicast-address="224.0.0.10" port="12345" interface="192.168.2.100">
 <Any EventGenerator element type>
 </NetworkSink>@endverbatim
*/

#ifndef _NETWORKSINK_H
#define _NETWORKSINK_H

#include "../OpenTracker.h"


#ifndef OT_NO_NETWORK_SUPPORT


#include "Network.h"

namespace ot {

    struct NetworkSender;

    /**
     * This class implements a simple node that stores a copy of the last
     * event it received and passed on for output to the console. The
     * associated NetworkSinkModule checks for changes and generates a
     * new network package if necessary.
     * @author Gerhard Reitmayr
     * @ingroup network
     */
    class OPENTRACKER_API NetworkSink : public Node
    {
        // Members
    public:
        /// station name
        std::string stationName;
        /// station number
        short int stationNumber;
        /// network sender pointer
        NetworkSender * networkSender;
        /// flag whether it was modified since last turn
        int modified;
        /// the event that is stored
        Event event;

        // Methods
    protected:
        /** constructor method,sets members
         * @param name_ the station name
         * @param number_ the station number
         * @param networkSender_ pointer to the network sender this station belongs to
         */
        NetworkSink( std::string & name_,short int number_, NetworkSender * networkSender_ ) :
            Node(),     
            stationName( name_ ),
            stationNumber( number_ ),
            networkSender( networkSender_ ),
            modified( 0 )
            {}

    public:
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
        virtual void onEventGenerated( Event & e, Node & generator)
        {
            event = e;
            modified = 1;
            updateObservers( event );
        }

        void pushEvent() {};
        void pullEvent() {};

        friend class NetworkSinkModule;
    };

} // namespace ot


#endif //OT_NO_NETWORK_SUPPORT

#endif

/* 
 * ------------------------------------------------------------
 *   End of NetworkSink.h
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
