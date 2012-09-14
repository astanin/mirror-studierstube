#ifndef _PHANTOM_SINK_H
#define _PHANTOM_SINK_H

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
/** header file for PhantomModule.
 *
 * @author Christian Fast
 *
 * $Id$
 * @file                                                                   */
/* ======================================================================= */

#ifdef USE_PHANTOM

// Holds data retrieved from HDAPI. 

typedef struct {
    HDdouble force[3];
    HDErrorInfo error;
    HHD handle;
  } DeviceInputData;

namespace ot {

  class OPENTRACKER_API PhantomSink : public Node {
  public:

    /* Container for tracking data*/
    Event state;
    int changed;
    HHD deviceHandle;
    HDSchedulerHandle schedulerHandle;
    bool feedback;
    float ramprate;
    
    DeviceInputData *currentData;

    PhantomSink(): Node(), changed(1) {}
    ~PhantomSink(){} 
    int isEventGenerator() {
      return 1;
    }

    /* If something has happened, move Aesop. Who created the
     * event (the generator) is irrelevant for me now
     */
    virtual void onEventGenerated( Event& event, Node& generator)
      {
			state = event;
			changed = 1;
			updateObservers( event );
      }
    friend class PhantomModule;
  };

}

#endif // USE_PHANTOM
#endif // _PHANTOM_SOURCE_H

