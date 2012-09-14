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

#include <OpenTracker/input/PhantomModule.h>
//#include "time.h"

#ifdef USE_PHANTOM

// Namespaces
using namespace std;

namespace ot {

  OT_MODULE_REGISTER_FUNC(PhantomModule) {
    OT_MODULE_REGISTRATION_DEFAULT(PhantomModule, "PhantomConfig");
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

  static std::map<int, HHD> id2handle;
  static std::map<std::string, HHD> name2handle;

  // ----------------------------------------------------------------------
  
  HDCallbackCode HDCALLBACK updateCalibrationCallback(void *pUserData) {
    HDenum *calibrationStyle = (HDenum *) pUserData;
    if (hdCheckCalibration() == HD_CALIBRATION_NEEDS_UPDATE) {
      hdUpdateCalibration(HD_CALIBRATION_AUTO); //*calibrationStyle);
    }
    return HD_CALLBACK_DONE;
  }



  // -----------------------------------------------------------------------------
  PhantomModule::PhantomModule() : ThreadModule(), NodeFactory(), stop(0)  {
  }

  // -----------------------------------------------------------------------------
  PhantomModule::~PhantomModule() {

    // Cleanup, unschedule our callbacks and stop the servo loop.
    hdStopScheduler();  
    for (map<std::string, HHD>::iterator it = name2handle.begin(); 
	 it != name2handle.end(); 
	 it++) {
      hdDisableDevice(it->second);
    }
  }
  
  std::string PhantomModule::seralize(HDfloat *value, int size)
  {
    std::stringstream ss;
    ss << "[" << size << ":";
    int i;
    for (i=0; i<size-1; i++)
      ss << value[i] << ";";
    ss << value[i] << "]";
    return ss.str();
  }
  // -----------------------------------------------------------------------------
  Node *PhantomModule::createNode(const string &name, StringTable &attributes) {
    logPrintI( "ot:Try to built PhantomModule node (%s)\n", name.c_str());
    if (name.compare("PhantomModule") == 0) {
      if (attributes.get("schedule").compare("true") == 0) {
	      startScheduler();
	      logPrintD("ot:Built PhantomModule node\n");
      }
    }

    return NULL;
  }
  
  HHD PhantomModule::getHandleById(int id) {
    map<int, HHD>::iterator it = id2handle.find(id);
    if (it == id2handle.end())
      return HD_INVALID_HANDLE;
    return it->second;
  }
  
  HHD PhantomModule::getHandleByName(std::string name) {
    map<std::string, HHD>::iterator it = name2handle.find(name);
    if (it == name2handle.end())
      return HD_INVALID_HANDLE;
    return it->second;
  }
  
  bool PhantomModule::initDevice(StringTable& attributes) {
  		
    std::string deviceName = attributes.get("devicename");
    if (deviceName.compare("") == 0) {
      logPrintE( "No devicename specified!\n");
      return false;
    }
  		
    int id=0;
    std::stringstream(attributes.get("id")) >> id;
			
    logPrintI("Trying to init haptic device %s with id %d\n", deviceName.c_str(), id);

    HHD handle = getHandleByName(deviceName);
		
    if (handle == HD_INVALID_HANDLE) {
    	
      handle = hdInitDevice(deviceName.c_str()); // Handle to the haptic device
      
      logPrintW( "Got internal haptic device handle %d\n", handle);

      
       HDErrorInfo error = hdGetError();
			
//       // Check if device can be initialized
       if (HD_DEVICE_ERROR(error)) {
//         // The device can't be initialized
     	  hduPrintError(stderr, &error, "Failed to initialize the haptic device");
 	      return false;
       }
       else {
 	      hduPrintError(stderr, &error, "Haptic device initialized");
 	      id2handle[id] = handle;
 	      name2handle[deviceName] = handle;
      }
    }
    else {
      fprintf(stderr, "Device already initialized (handle=%d)\n",handle);
      id2handle[id] = handle;
    }
      
    hdMakeCurrentDevice(handle);
    return true;
  }
  
  void PhantomModule::startScheduler() {
    HDErrorInfo error;
      
    if (hdGetCurrentDevice()!=HD_INVALID_HANDLE) {

      hdStartScheduler();
      	
      HDenum calibrationType = HD_CALIBRATION_AUTO;
      hdScheduleSynchronous(updateCalibrationCallback, &calibrationType, HD_MIN_SCHEDULER_PRIORITY);


       HDErrorInfo error = hdGetError();

       if (HD_DEVICE_ERROR(error)) {

 	      // The sceduler can't be initialized
 	      hduPrintError(stderr, &error, "Failed to start the scheduler");
       }
       else
/	      hduPrintError(stderr, &error, "Scheduler started");

    }
    else 
      logPrintD("ot:PhantomModule has nothing to schedule\n");

    		
  }
      
 
  // -------------------------------------------------------------------------
  void PhantomModule::init(StringTable& attributes, ConfigNode *localTree) {
    ThreadModule::init( attributes, localTree );

    logPrintI("PhantomModule::init(...)\n");
//    name2handle["Default PHANToM"] = HHD(0);
    initDevice(attributes);

    initialized=true;
    
  } // init(...)
}

#endif // USE_PHANTOM
#endif // DARWIN
#endif // WIN32


