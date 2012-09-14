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
  * ======================================================================== 
  *
  * @author Markus Sareika
  * 
  * $Id: ThreeToTwoDimFilterModule.h
  * @file                                                                   */
 /* ======================================================================= */


#include <OpenTracker/tool/disable4786.h>

#include <OpenTracker/common/ThreeToTwoDimFilter.h>
#include <OpenTracker/common/ThreeToTwoDimFilterModule.h>

#ifdef USE_THREETOTWODIMFILTER

#include <cstdio>
#include <iostream>

#if defined (WIN32)
#include <Windows.h>
#endif

#include <math.h>
#include <OpenTracker\tool\OT_ACE_Log.h>


namespace ot {
	OT_MODULE_REGISTER_FUNC(ThreeToTwoDimFilterModule){
		OT_MODULE_REGISTRATION_DEFAULT(ThreeToTwoDimFilterModule, "ThreeToTwoDimFilterConfig");
	}
	
	// Destructor method
	ThreeToTwoDimFilterModule::~ThreeToTwoDimFilterModule()
	{
		nodes.clear();
	}

	// This method is called to construct a new Node.
	Node * ThreeToTwoDimFilterModule::createNode( const std::string& name, const StringTable& attributes)
	{
		if( name.compare("ThreeToTwoDimFilter") == 0 )
		{       
			ThreeToTwoDimFilter * source = new ThreeToTwoDimFilter;
			source->event.setConfidence( 1.0f );
			//int comPort = (int)atof(attributes.get("comPort").c_str());

			std::vector<float> cSOrientationQuat_, cSRoot2ScreenRootVec_, aSWidthVec_,aSHeightVec_;
			float trackingSystemScaleOneMeter_, screenDepthFrontInMeter_, screenDepthBackInMeter_,
				screenResWidth_, screenResHeight_;
			attributes.get("CSOrientationQuat", cSOrientationQuat_, 4);
			attributes.get("CSRoot2ScreenRootVec", cSRoot2ScreenRootVec_, 3);
			attributes.get("ASWidthVec", aSWidthVec_, 3);
			attributes.get("ASHeightVec", aSHeightVec_, 3);
			attributes.get("TrackingSystemScaleOneMeter", &trackingSystemScaleOneMeter_);
			attributes.get("ScreenDepthFrontInMeter", &screenDepthFrontInMeter_);
			attributes.get("ScreenDepthBackInMeter", &screenDepthBackInMeter_);
			attributes.get("ScreenResWidth", &screenResWidth_);
			attributes.get("ScreenResHeight", &screenResHeight_);
			source->initCalibrationData( aSWidthVec_, aSHeightVec_, cSOrientationQuat_, 
				cSRoot2ScreenRootVec_, trackingSystemScaleOneMeter_, screenDepthFrontInMeter_, 
				screenDepthBackInMeter_, screenResWidth_, screenResHeight_ );
			
			nodes.push_back( source );
			logPrintS("Built ThreeToTwoDimFilter node\n");
			initialized = 1;
			return source;
		}
		if(  (name.compare("AspdInput") == 0) || (name.compare("PointingDeviceInput") == 0)  ) 
		{
			// create just a pass-through node
			NodePort *np = new NodePort();
			np->name = name;
			logPrintS("Built ThreeToTwoDimFilter Input node\n");
			return np;
		}
		// no node configured 
		return NULL;
	}


	void ThreeToTwoDimFilterModule::pushEvent()
	{
		
	}
} // namespace ot

#endif //USE_THREETOTWODIMFILTER
