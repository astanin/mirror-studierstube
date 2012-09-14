#ifndef _PHANTOM_SOURCE_MODULE_H
#define _PHANTOM_SOURCE_MODULE_H

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

// this is a linux implementation !!!
#ifndef WIN32
#ifndef DARWIN

#ifdef USE_PHANTOM

// Include the haptic libs
#include <HD/hd.h>
#include <HDU/hduError.h>
#include <HDU/hduVector.h>
#include <HL/hl.h>
#include <HLU/hlu.h>

// OpenTracker includes
#include <OpenTracker/dllinclude.h>
#include <OpenTracker/OpenTracker.h>
#include <OpenTracker/core/Module.h>
#include <OpenTracker/core/NodeFactory.h>
#include <OpenTracker/core/MathUtils.h>

// Standard includes
#include <vector>
#include <cstdio>
#include <math.h>
#include <cmath>
#include <assert.h>
#include <sched.h>
#include <fstream>
#include <ace/Log_Msg.h>
#include <iostream>

#include <termios.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#if defined(WIN32) || defined(linux)
#include <GL/glut.h>
#elif defined(__APPLE__)
#include <GLUT/glut.h>
#endif

#include "PhantomSource.h"
#include "PhantomModule.h"

namespace ot {

  // Holds data retrieved from HDAPI.
  


  //typedef std::vector<Node*> NodeVector;

  //--------------------------------------------------------------------------------
  // CLASS: PhantomSourceModule
  //
  // This class manages creation of new haptic sources, and updates them
  // when necessary
  //--------------------------------------------------------------------------------

  class OPENTRACKER_API PhantomSourceModule : public PhantomModule { 
    // Members
  private: 

    void getDeviceData(DeviceOutputData *currentData);

  
  protected:

    NodeVector sources;
    int stop;
  
    // Methods
  public:

    // Constructor
    PhantomSourceModule();

    // Destructor
    ~PhantomSourceModule();
 
    virtual Node *createNode(const std::string &name, StringTable &attributes);
    
    virtual void pushEvent();

  };
  
  OT_MODULE(PhantomSourceModule);
  
  

}
#endif // USE_PHANTOM
#endif // DARWIN
#endif // WIN32
#endif // _PHANTOM_MODULE_H
