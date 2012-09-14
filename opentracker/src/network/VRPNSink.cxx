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
/** source file for VRPNSink node.
 *
 * @author Gerhard Reitmayr
 *
 * $Id: VRPNSink.cxx 1846 2007-05-23 08:16:47Z jfn $
 * @file                                                                   */
/* ======================================================================= */

#include <OpenTracker/network/VRPNSink.h>

#ifdef USE_VRPN

#ifndef FALSE
#define FALSE 0
#endif


#include <cassert>

using namespace ot;

#include <vrpn_Tracker.h>
#include <vrpn_Button.h>


VRPNSink::VRPNSink():
    Node(), 
    name( ""),
    type( TRACKER ),
    station( 0 ),
    server( NULL )
{
}

VRPNSink::~VRPNSink()
{
    if( server != NULL )
    {
        delete server;
        server = NULL;
    }        
}

void VRPNSink::start(vrpn_Connection * connection)
{
    switch(type){
        case TRACKER:
            server = new vrpn_Tracker_Server( name.c_str(), connection, station + 1);
            break;
        case BUTTON:
            server = new vrpn_Button_Server( name.c_str(), connection, 8 );
            break;
        default:
            assert(FALSE && "VRPNSink undefined type!");
    }
}

void VRPNSink::mainloop()
{
    server->mainloop();
}

void VRPNSink::onEventGenerated( Event& event, Node& generator)
{
    switch(type){
        case TRACKER:
            {
                vrpn_float64 position[3] = 
                    {
                        event.getPosition()[0],
                        event.getPosition()[1],
                        event.getPosition()[2]
                    };
                vrpn_float64 orientation[4] = 
                    {
                        event.getOrientation()[0],
                        event.getOrientation()[1],
                        event.getOrientation()[2],
                        event.getOrientation()[3]
                    };

                timeval time;
                time.tv_sec = event.time / 1000.0;
                time.tv_usec = ( event.time - time.tv_sec * 1000.0 ) * 1000;

                ((vrpn_Tracker_Server *)server)->report_pose( station, time, position, orientation );
            }
            break;
        case BUTTON:
            {
                int i;
                for( i = 0; i < 8; i++ )
                {
                    ((vrpn_Button_Server *)server)->set_button(i, (event.getButton() & (1 << i)));
                }
            }
            break;
        default:
            assert(FALSE && "VRPNSink undefined type!");
    }
    updateObservers( event );
}


#else
#ifdef WIN32
#pragma message(">>> no VRPN support")
#endif
#endif

/* 
 * ------------------------------------------------------------
 *   End of VRPNSink.cxx
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
