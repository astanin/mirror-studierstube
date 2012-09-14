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
/** source file for PhantomModule.
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

#include <OpenTracker/input/PhantomSourceModule.h>
//#include "time.h"

#ifdef USE_PHANTOM

// Namespaces
using namespace std;

namespace ot {

  OT_MODULE_REGISTER_FUNC(PhantomSourceModule) {
    OT_MODULE_REGISTRATION_DEFAULT(PhantomSourceModule, "PhantomSourceModule");
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

  
  //static DeviceOutputData pServoDeviceData;
  
  // Position and rotation used for the clutch
  /*  static MathUtils::Vector3 clutchPos1;
      static MathUtils::Quaternion clutchRotation;
      static MathUtils::Vector3 clutchOffset;
      static MathUtils::Quaternion rotOffset;
      static MathUtils::Vector3 spikeInitOffset; */


  // ----------------------------------------------------------------------
  HDCallbackCode HDCALLBACK updateDeviceDataCallback(void *pUserData) { 
  
    DeviceOutputData * pServoDeviceOutputData = static_cast<DeviceOutputData *>(pUserData);
    
    hdMakeCurrentDevice(pServoDeviceOutputData->handle);

    // Retrieve the current button(s). 
    hdGetIntegerv(HD_CURRENT_BUTTONS, &pServoDeviceOutputData->buttonState);
  
    // Get the current forces
    hdGetFloatv(HD_CURRENT_FORCE, pServoDeviceOutputData->force);
  
    // Get the current location of the device (HD_CURRENT_POSITION)
    // We declare a vector of three doubles since hdGetDoublev returns 
    // the information in a vector of size 3.

    // Get the position
    hdGetFloatv(HD_CURRENT_POSITION, pServoDeviceOutputData->position);
    
    // Get the last position
    hdGetFloatv(HD_LAST_POSITION, pServoDeviceOutputData->lastPosition);
    
    // Get the velocity
    hdGetFloatv(HD_CURRENT_VELOCITY, pServoDeviceOutputData->velocity);

    // Get the last velocity
//    hdGetFloatv(HD_LAST_VELOCITY, pServoDeviceOutputData->lastVelocity);
    
    // Get rotation
    hdGetFloatv(HD_CURRENT_TRANSFORM, pServoDeviceOutputData->transformation);
    
    // Also check the error state of HDAPI
    pServoDeviceOutputData->error = hdGetError();
    
    // inform that values have changed
    pServoDeviceOutputData->updated = true;
      
    return HD_CALLBACK_DONE;
  } // End updateDeviceCallback

  // -----------------------------------------------------------------------------
  PhantomSourceModule::PhantomSourceModule() {

    /*    spikeInitOffset[0] = 0;
	  spikeInitOffset[1] = 40;
	  spikeInitOffset[2] = 0;*/

  }

  // -----------------------------------------------------------------------------
  PhantomSourceModule::~PhantomSourceModule() {
    sources.clear();
  }

  // -----------------------------------------------------------------------------
  Node *PhantomSourceModule::createNode(const string &name, StringTable &attributes) {

    // Check if createNode is meant for me
    if( name.compare("PhantomSource") == 0 ) {
      //std::cerr << "PhantomSourceModule::createNode(...)" << std::endl;

      int id=0;
      std::stringstream(attributes.get("id")) >> id;
    	
      HHD handle = getHandleById(id);
    	
      if (handle != HD_INVALID_HANDLE) {

	NodeVector::iterator it;
	for( it = sources.begin(); it != sources.end(); it++ ) {
	  PhantomSource * source = (PhantomSource*)((Node*)(*it));
	}
	if( it != sources.end()) {
	  ACE_DEBUG((LM_DEBUG, ACE_TEXT("ot:PhantomSource exists already\n")));
	  return NULL;
      	}
      	PhantomSource * source = new PhantomSource();
      	source->state.timeStamp();
      	source->deviceHandle = handle;
      	source->currentData = new DeviceOutputData();
      	sources.push_back( source );

      	ACE_DEBUG((LM_INFO, ACE_TEXT("ot:Built PhantomSource node\n")));
      	return source;
      }
      else {
      	ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:PhantomSource cannot retrieve device handle (id unknown)\n")));
	return NULL;
      }
    }

    return NULL;
  }

  //------------------------------------------------------------------------------
  void PhantomSourceModule::pushEvent() {
    
    for( NodeVector::iterator it = sources.begin(); it != sources.end(); it++ ) {
      PhantomSource *source = (PhantomSource *)((Node*)(*it));
      lock();
      fprintf(stderr, "PhantomSourceModule::pushEvent() handle = %d\n", source->deviceHandle);
      DeviceOutputData *currentData = source->currentData;
      currentData->handle = source->deviceHandle;
      
      currentData->lastVelocity[0] = currentData->velocity[0];
      currentData->lastVelocity[1] = currentData->velocity[1];
      currentData->lastVelocity[2] = currentData->velocity[2];
      
      getDeviceData(currentData);				
				
      source->state.setAttribute("vector<float>", "force", seralize(currentData->force,3));
      source->state.setAttribute("vector<float>", "position", seralize(currentData->position,3));
      source->state.setAttribute("vector<float>", "orientation", seralize(currentData->rotation,4));
	    
	    source->state.setAttribute("vector<float>", "velocity", seralize(currentData->velocity,3));
	     
      source->state.getButton() = currentData->buttonState;
   
      source->state.getConfidence() = 1.0f;
      
      double lasttime = source->state.time;
	      
      // Bring a timeStamp to source->state     
      source->state.timeStamp();
      double time = source->state.time;
      
      fprintf(stderr, "timediff %f %f %f\n", time, lasttime, time - lasttime);
      fprintf(stderr, "velo x %f %f %f\n", currentData->velocity[0], currentData->lastVelocity[0], currentData->velocity[0]-currentData->lastVelocity[0]);
       
      currentData->acceleration[0] = (currentData->velocity[0]-currentData->lastVelocity[0])/(time-lasttime);
      currentData->acceleration[1] = (currentData->velocity[1]-currentData->lastVelocity[1])/(time-lasttime);
      currentData->acceleration[2] = (currentData->velocity[2]-currentData->lastVelocity[2])/(time-lasttime);
      
      source->state.setAttribute("vector<float>", "acceleration", seralize(currentData->acceleration,3));
			
      source->updateObservers( source->state );
				
      unlock();
    }
  }

  // -------------------------------------------------------------------------
  void PhantomSourceModule::getDeviceData(DeviceOutputData *currentData) {

    int reliable;
    double rm[4][4]; // Rotation matrix
    float rmTransposed[3][3]; // Transposed of the rotational matrix
    float q[4];

    hdMakeCurrentDevice(currentData->handle);


    // Perform a synchronous call to copy the most current device state
    hdScheduleSynchronous(updateDeviceDataCallback, currentData, HD_MIN_SCHEDULER_PRIORITY);
    
    // Create the rotation matrix
    for(int j=0;j<4;j++) {
      for(int k=0;k<4;k++) {
	int idx = (4*j) + k;
	rm[j][k] = (float)currentData->transformation[idx];
	if(j<3 && k<3)
	  rmTransposed[k][j] = (float)currentData->transformation[idx];
      }
    }
  
    MathUtils::matrixToQuaternion(rmTransposed,(float *)currentData->rotation);
    
  } // getDeviceData(...)
}

#endif // USE_PHANTOM
#endif // DARWIN
#endif // WIN32

// Rotate vector around quaternion

/*	  		float scale = 0.297/287; 	// calculate postion in meters
	  
q1[0]=temp[bodyID].ORIENTATION[0];
q1[1]=temp[bodyID].ORIENTATION[1];
q1[2]=temp[bodyID].ORIENTATION[2];
q1[3]=temp[bodyID].ORIENTATION[3];

v1[0]=spikeInitOffset[0];
v1[1]=spikeInitOffset[1];
v1[2]=spikeInitOffset[2];

// calculate norm (should be 1)

float norm = q1[0]*q1[0] + q1[1]*q1[1] + q1[2]*q1[2] + q1[3]*q1[3];

q2[0]= v1[0] * q1[3] + v1[2] * q1[1] - v1[1] * q1[2];
q2[1]= v1[1] * q1[3] + v1[0] * q1[2] - v1[2] * q1[0];
q2[2]= v1[2] * q1[3] + v1[1] * q1[0] - v1[0] * q1[1];
q2[3]= v1[0] * q1[0] + v1[1] * q1[1] + v1[2] * q1[2];

spikeOffset[0] = (q1[3] * q2[0] + q1[0] * q2[3] + q1[1] * q2[2] - q1[2] * q2[1])/norm;
spikeOffset[1] = (q1[3] * q2[1] + q1[1] * q2[3] + q1[2] * q2[0] - q1[0] * q2[2])/norm;
spikeOffset[2] = (q1[3] * q2[2] + q1[2] * q2[3] + q1[0] * q2[1] - q1[1] * q2[0])/norm;
	  
// Set position and orientation

(temp[bodyID].POSITION[0]+=spikeOffset[0])*=scale;
(temp[bodyID].POSITION[1]+=spikeOffset[2])*=scale;
(temp[bodyID].POSITION[2]+=spikeOffset[1])*=scale;*/

