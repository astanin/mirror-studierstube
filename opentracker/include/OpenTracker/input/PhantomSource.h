#ifndef _PHANTOM_SOURCE_H
#define _PHANTOM_SOURCE_H

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

#define sign(a,b) a>b?1.0f:-1.0f
#define MAXLENGTH 1000


typedef struct {
    HDint buttonState;
    HDfloat position[3];
    HDfloat lastPosition[3];
    HDfloat velocity[3];
    HDfloat lastVelocity[3];
    HDfloat acceleration[3];
    HDfloat force[3];
    HDErrorInfo error;
    HDfloat transformation[16];
    HDfloat rotation[4];
    HHD handle;
    bool updated;
} DeviceOutputData;

namespace ot {

/*struct haptic_track_data {
  bool reliable;   
  float POSITION[3];
  float ORIENTATION[4];
  double TRANSFORMATION[4][4];
  long button;
};*/

//--------------------------------------------------------------------------------
// CLASS: PhantomSource
//
// This class creates a haptic device, and updates the tracker tree with
// information from the haptic device whenever it is called upon.
//--------------------------------------------------------------------------------
class OPENTRACKER_API PhantomSource : public Node {
public:

  /* Container for tracking data*/
  Event state;
  HHD deviceHandle;
  
  DeviceOutputData *currentData;

  PhantomSource(): Node(), deviceHandle(HD_INVALID_HANDLE) { }
  ~PhantomSource() {}

  int isEventGenerator() {
    return 1;
  }
  
protected:

private:

    
};

}

#endif // USE_PHANTOM
#endif // _PHANTOM_SOURCE_H
