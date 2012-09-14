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
/** header file for TCPSink Node.
 *
 * @author Gerhard Reitmayr
 *
 * $Id: TCPSink.h 1807 2007-04-12 10:35:50Z bornik $
 * @file                                                                   */
/* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section tcpsink TCPSink
 * The TCPSink node is a simple sink for the @ref tcpmodule module. It resembles 
 * a single station in the stream of data that is served to clients by the @ref
 * tcpmodule module. The node has the following elements :
 * @li @c station number of the station
 * @li @c position on|off flag to specify whether position information is to be send
 * @li @c orientation on|off flag to specify whether orientation information is to be send
 * @li @c button on|off flag to specify whether button information is to be send
 * @li @c time on|off flag to specify whether timestamp information is to be send
 *
 * An example element looks like this :
 * @verbatim
 <TCPSink station="0" position="on" rotation="off" >
 <Any EventGenerator element type>
 </TCPSink>@endverbatim
*/

#ifndef _TCPSINK_H
#define _TCPSINK_H

#include "../OpenTracker.h"


#ifndef OT_NO_NETWORK_SUPPORT


namespace ot {

    /** This class is a simple sink that stores a copy of all data passing through it
     * and marks itself as changed for the TCPModule to process the new data. It also
     * stores the protocol information necessary to send the right data.
     * @author Gerhard Reitmayr
     * @ingroup network
     */
    class OPENTRACKER_API TCPSink : public Node
    {
        // Members
    public:
        /// station number
        int station;
        /// flag whether it was modified since last turn
        int changed;
        /// the event that is stored
        Event event;
        /// flags whether what to output
        int posFlag, orientFlag, buttonFlag,timeFlag;

        // Methods
    protected:
        /** constructor method,sets members 
         * @param number_ the station number
         */
        TCPSink(  int number_, int pos_, int rot_, int but_, int time_ ) :
            Node(),
            station( number_ ),
            changed( 0 ),
            posFlag( pos_ ),
            orientFlag( rot_ ),
            buttonFlag( but_ ),
            timeFlag( time_ )
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
         * It stores a copy of the received event, marks itself as changed
         * and passes the event on to its observers.
         * @param event reference to the new event. Do not change the
         *        event values, make a copy and change that !
         * @param generator reference to the EventGenerator object that
         *        notified the EventObserver.
         */
        virtual void onEventGenerated( Event & e, Node & generator)
        {
            event = e;
            changed = 1;
            updateObservers( event );
        }

        void pushEvent() {};
        void pullEvent() {};

        friend class TCPModule;
    };

} // namespace ot


#endif //OT_NO_NETWORK_SUPPORT


#endif

/* 
 * ------------------------------------------------------------
 *   End of TCPSink.h
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
