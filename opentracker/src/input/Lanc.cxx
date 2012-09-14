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
/** source file for Lanc Node.
*
* @author Markus Sareika
*
* $Id: Lanc.cpp
* @file                                                                   */
/* ======================================================================= */

#ifdef WIN32
#pragma warning(disable:4244)
#endif

#include <ace/OS.h>
#ifdef WIN32
#include <windows.h>
#endif //WIN32
#include <iostream>

#include <OpenTracker/input/Lanc.h>

#ifdef USE_PANTILTUNIT
#include <OpenTracker/core/OSUtils.h>


namespace ot {
    
	//constructor
	Lanc::Lanc():zoomout(false),
		zoomin(false),
		offsetFactor(1.f),
		wideAngle(37.1f),
		startTime(0),
		maxTimePos(8000),
		initLanc(false)
	{
		timePos=maxTimePos;
		#ifdef WIN32
		//initialize lanc
		int id = FindLancUSBDevice();
		if (GetUSBID(0, usb_id))
		{
			// get handle
			int retCode = OpenLancDeviceEx(&lancDevice,  usb_id,  NULL, LANCCALLBACK_NULL);
			int messageId = GetRegisteredLancMessage(lancDevice);
			// zoom out to maximum
			SendContinuousLancCommand(lancDevice,  COMMANDTYPE_CAMERA, COMMAND_ZOOM_OUT_SPEED_8, NULL);
			OSUtils::sleep(3000);
			SendLancCommand(lancDevice,  0, 0);
			initLanc=true;
		}
		else
			std::cerr << "failed to get lanc usb id" << std::endl;
		#endif
	}
	
	Lanc::~Lanc()
	{
		//destructor
		#ifdef WIN32
		if (initLanc) CloseLancDevice(lancDevice);
		#endif
	}

	void Lanc::setTeleAngle(float tele)
	{
		teleAngle=tele;
	}

	void Lanc::setWideAngle(float wide)
	{
		wideAngle=wide;
	}

	float Lanc::getFieldOfView()
	{
		//float fieldOfView = teleAngle*(float)timePos*offsetFactor;
		//return fieldOfView;
		zoomFactor = (timePos/maxTimePos);
		float fieldOfView = wideAngle*(float)zoomFactor;
		return fieldOfView;
	}

	void Lanc::updateTimePos()
	{
		if( zoomin )
		{
			double time = OSUtils::currentTime();
			timePos -= (time-startTime);
			if (timePos < 200) timePos = 200;
			startTime = time;
		}
		if( zoomout )
		{
			double time = OSUtils::currentTime();
			timePos += (time-startTime);
			if (timePos > maxTimePos) timePos = maxTimePos;
			startTime = time;
		}
	}

	bool Lanc::isZooming()
	{
		if( zoomin || zoomout ) return true;
		return false;
	}

	// mapping: 1 > zoominspeed8-1 > 0.2 stop -0.2 > zoomoutspeed1-8 > -1
    void Lanc::Zoom( float speed )         
    {
		// todo: some focalDistance calculation based on timestamps etc.
		
		//speed *= 0.8f; // do some speed scaling
        #ifdef WIN32

		if ( (speed<0.2) && (speed>-0.2) )   // zoom stop
		{
			if(initLanc)
				if (!SendLancCommand(lancDevice,  0, 0)) 
					std::cerr << "LANC Device Not Responding" << std::endl;
			double time = OSUtils::currentTime();
			if (zoomin)
			{
				timePos -= (time-startTime);
				if (timePos < 200) timePos = 200;
				zoomin=false;
			}
			if (zoomout)
			{
				timePos += (time-startTime);
				if (timePos > maxTimePos) timePos = maxTimePos;
				zoomout=false;
			}
		}
		if (speed>=0.2)
		{
			if(initLanc)
				if (!SendContinuousLancCommand(lancDevice,  COMMANDTYPE_CAMERA, COMMAND_ZOOM_IN_SLOW, NULL)) 
					std::cerr << "LANC Device Not Responding" << std::endl;
			startTime = OSUtils::currentTime();
			zoomin=true;
		}
		if (speed<=-0.2)
		{
			if(initLanc)
				if (!SendContinuousLancCommand(lancDevice,  COMMANDTYPE_CAMERA, COMMAND_ZOOM_OUT_SLOW, NULL)) 
					std::cerr << "LANC Device Not Responding" << std::endl;
			startTime = OSUtils::currentTime();
			zoomout=true;
		}

		/////////////////////////// multiple speeds mapping +8/-8
		//if (speed>=0.9)				       // zoom in speed 8
		//{
		//	if (!SendContinuousLancCommand(lancDevice,  COMMANDTYPE_CAMERA, COMMAND_ZOOM_IN_SPEED_8, NULL)) 
		//		std::cerr << "LANC Device Not Responding" << std::endl;   
		//}
		//if ( (speed<0.9) && (speed>=0.8) )   // zoom in speed 7
		//{
		//	if (!SendContinuousLancCommand(lancDevice,  COMMANDTYPE_CAMERA, COMMAND_ZOOM_IN_SPEED_7, NULL)) 
		//		std::cerr << "LANC Device Not Responding" << std::endl;   
		//}
		//if ( (speed<0.8) && (speed>=0.7) )   // zoom in speed 6
		//{
		//	if (!SendContinuousLancCommand(lancDevice,  COMMANDTYPE_CAMERA, COMMAND_ZOOM_IN_SPEED_6, NULL)) 
		//		std::cerr << "LANC Device Not Responding" << std::endl;   
		//}
		//if ( (speed<0.7) && (speed>=0.6) )   // zoom in speed 5
		//{
		//	if (!SendContinuousLancCommand(lancDevice,  COMMANDTYPE_CAMERA, COMMAND_ZOOM_IN_SPEED_5, NULL)) 
		//		std::cerr << "LANC Device Not Responding" << std::endl;   
		//}		
		//if ( (speed<0.6) && (speed>=0.5) )   // zoom in speed 4
		//{
		//	if (!SendContinuousLancCommand(lancDevice,  COMMANDTYPE_CAMERA, COMMAND_ZOOM_IN_SPEED_4, NULL)) 
		//		std::cerr << "LANC Device Not Responding" << std::endl;   
		//}
		//if ( (speed<0.5) && (speed>=0.4) )   // zoom in speed 3
		//{
		//	if (!SendContinuousLancCommand(lancDevice,  COMMANDTYPE_CAMERA, COMMAND_ZOOM_IN_SPEED_3, NULL)) 
		//		std::cerr << "LANC Device Not Responding" << std::endl;   
		//}
		//if ( (speed<0.4) && (speed>=0.3) )   // zoom in speed 2
		//{
		//	if (!SendContinuousLancCommand(lancDevice,  COMMANDTYPE_CAMERA, COMMAND_ZOOM_IN_SPEED_2, NULL)) 
		//		std::cerr << "LANC Device Not Responding" << std::endl;   
		//}
		//
		//if ( (speed<0.3) && (speed>=0.2) )   // zoom in speed 1
		//{
		//	if (!SendContinuousLancCommand(lancDevice,  COMMANDTYPE_CAMERA, COMMAND_ZOOM_IN_SPEED_1, NULL)) 
		//		std::cerr << "LANC Device Not Responding" << std::endl;   
		//}
		//if ( (speed<0.2) && (speed>-0.2) )   // zoom stop
		//{
		//	if (!SendLancCommand(lancDevice,  0, 0)) 
		//		std::cerr << "LANC Device Not Responding" << std::endl;
		//}
		//if ( (speed<=-0.2) && (speed>-0.3) ) // zoom out speed 1
		//{
		//	if (!SendContinuousLancCommand(lancDevice,  COMMANDTYPE_CAMERA, COMMAND_ZOOM_OUT_SPEED_1, NULL)) 
		//		std::cerr << "LANC Device Not Responding" << std::endl;
		//}
		//if ( (speed<=-0.3) && (speed>-0.4) ) // zoom out speed 2
		//{
		//	if (!SendContinuousLancCommand(lancDevice,  COMMANDTYPE_CAMERA, COMMAND_ZOOM_OUT_SPEED_2, NULL)) 
		//		std::cerr << "LANC Device Not Responding" << std::endl;
		//}
		//if ( (speed<=-0.4) && (speed>-0.5) ) // zoom out speed 3
		//{
		//	if (!SendContinuousLancCommand(lancDevice,  COMMANDTYPE_CAMERA, COMMAND_ZOOM_OUT_SPEED_3, NULL)) 
		//		std::cerr << "LANC Device Not Responding" << std::endl;
		//}
		//if ( (speed<=-0.5) && (speed>-0.6) ) // zoom out speed 4
		//{
		//	if (!SendContinuousLancCommand(lancDevice,  COMMANDTYPE_CAMERA, COMMAND_ZOOM_OUT_SPEED_4, NULL)) 
		//		std::cerr << "LANC Device Not Responding" << std::endl;
		//}
		//if ( (speed<=-0.6) && (speed>-0.7) ) // zoom out speed 5
		//{
		//	if (!SendContinuousLancCommand(lancDevice,  COMMANDTYPE_CAMERA, COMMAND_ZOOM_OUT_SPEED_5, NULL)) 
		//		std::cerr << "LANC Device Not Responding" << std::endl;
		//}
		//if ( (speed<=-0.7) && (speed>-0.8) ) // zoom out speed 6
		//{
		//	if (!SendContinuousLancCommand(lancDevice,  COMMANDTYPE_CAMERA, COMMAND_ZOOM_OUT_SPEED_6, NULL)) 
		//		std::cerr << "LANC Device Not Responding" << std::endl;
		//}
		//if ( (speed<=-0.8) && (speed>-0.9) ) // zoom out speed 7
		//{
		//	if (!SendContinuousLancCommand(lancDevice,  COMMANDTYPE_CAMERA, COMMAND_ZOOM_OUT_SPEED_7, NULL)) 
		//		std::cerr << "LANC Device Not Responding" << std::endl;
		//}
		//if (speed<=-0.9)                     // zoom out speed 8
		//{
		//	if (!SendContinuousLancCommand(lancDevice,  COMMANDTYPE_CAMERA, COMMAND_ZOOM_OUT_SPEED_8, NULL)) 
		//		std::cerr << "LANC Device Not Responding" << std::endl;
		//}
		#endif
    }
}

#endif //USE_PANTILTUNIT
