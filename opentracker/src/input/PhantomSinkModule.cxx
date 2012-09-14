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
/** source file for PahntomModule.
 *
 * @author Antonio Rella
 * @author Christian Fast
 *
 * $Id$
 * @file                                                                   */
/* ======================================================================= */

// this is a linux implementation
#ifndef WIN32
#ifndef DARWIN

#include <OpenTracker/input/PhantomSinkModule.h>

#ifdef USE_PHANTOM

// Namespaces
using namespace std;

namespace ot {

  OT_MODULE_REGISTER_FUNC(PhantomSinkModule) {
    OT_MODULE_REGISTRATION_DEFAULT(PhantomSinkModule, "PhantomSinkModule");
	}
  //------------------------------------------------------------------------------
  // FUNCTION: createNode
  // INPUT: 
  //   name       - reference to string containing element name
  //   attributes - refenrence to StringMap containing attribute values 
  // RETURNS: pointer to new Node or NULL. The new Node must be allocated with new
  //
  // This function is called by opentracker when a new source is declared in the
  // opentracker (xml) config file.
  //
  //------------------------------------------------------------------------------

  // ----------------------------------------------------------------------
  HDCallbackCode HDCALLBACK updateForcesCallback(void *pUserData) { 

    DeviceInputData * pServoDeviceInputData = static_cast<DeviceInputData *>(pUserData);

    hdBeginFrame(pServoDeviceInputData->handle);

    // Set the forces
    hdSetDoublev(HD_CURRENT_FORCE, pServoDeviceInputData->force);
        
    // Also check the error state of HDAPI
    pServoDeviceInputData->error = hdGetError();
      
    // Copy the position into our device_data tructure.
    hdEndFrame(pServoDeviceInputData->handle);

    return HD_CALLBACK_CONTINUE;
  } // End updateDeviceCallback
  
  // -----------------------------------------------------------------------------
  PhantomSinkModule::PhantomSinkModule() : PhantomModule() { 
    //ThreadModule(), NodeFactory(), stop(0) {
    std::cerr << "PhantomSinkModule::PhantomSinkModule()" << std::endl;
  }

  // -----------------------------------------------------------------------------
  PhantomSinkModule::~PhantomSinkModule() {
    NodeVector::iterator it;
    for( it = sinks.begin(); it != sinks.end(); it++ ) {
      hdUnschedule( ((PhantomSink*)((Node*)(*it)))->schedulerHandle );
    }
    sinks.clear();
  }

  // -----------------------------------------------------------------------------
  Node *PhantomSinkModule::createNode(const string &name, StringTable &attributes) {
   
    // Check if createNode is ment for me
    if (name.compare("PhantomSink") == 0) {
    	
      int id=0;
      std::stringstream(attributes.get("id")) >> id;
    	
      HHD handle = getHandleById(id);
    	
      if (handle != HD_INVALID_HANDLE) {

	hdMakeCurrentDevice(handle);
	NodeVector::iterator it;
      	for( it = sinks.begin(); it != sinks.end(); it++ ) {
	  PhantomSink * sink = (PhantomSink*)((Node*)(*it));
      	}
      	
      	PhantomSink* sink = new PhantomSink();
      	sink->deviceHandle = handle;
      	sink->currentData = new DeviceInputData;
      	sink->currentData->handle = handle;
      	
      	sink->feedback = 1;
      	std::stringstream(attributes.get("feedback")) >> sink->feedback;
      	
      	hdEnable(HD_FORCE_OUTPUT);
      	hdGetFloatv(HD_FORCE_RAMPING_RATE,&(sink->ramprate));
	//      	sink->ramprate = 0.5;
      	std::stringstream(attributes.get("ramprate")) >> sink->ramprate;
      	fprintf(stderr, "ramprate = %f\n", sink->ramprate);
      	hdSetFloatv(HD_FORCE_RAMPING_RATE,&(sink->ramprate));
	      sink->schedulerHandle = 
	        hdScheduleAsynchronous(updateForcesCallback, 
				  sink->currentData, 
				  HD_DEFAULT_SCHEDULER_PRIORITY);

      	sinks.push_back( sink );
      	// Get the feedback attribute
      	
      	ACE_DEBUG((LM_INFO, ACE_TEXT("ot:Built PhantomSink node\n")));
      	return sink;
      }
      else {
      	ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:PhantomSink cannot retrieve device handle (unkown id)\n")));
	return NULL;
      }
    }

    return NULL;
  }

  //------------------------------------------------------------------------------
  void PhantomSinkModule::pullEvent() {
    NodeVector::iterator it;
    for( it = sinks.begin(); it != sinks.end(); it++ ) {  
      PhantomSink * sink = (PhantomSink *)((Node*)(*it));
      	 
      fprintf(stderr, "PhantomSourceModule::pushEvent() handle = %d\n", sink->deviceHandle);
      
      if (sink->feedback) {
	sink->currentData->force[0] = sink->state.getPosition()[0];
	sink->currentData->force[1] = sink->state.getPosition()[1];
	sink->currentData->force[2] = sink->state.getPosition()[2];
      }
      else {
	sink->currentData->force[0] = 0;
	sink->currentData->force[1] = 0;
	sink->currentData->force[2] = 0;
      }				
    }
  } // pullState() 
}

#endif // USE_PHANTOM
#endif // DARWIN
#endif // WIN32


