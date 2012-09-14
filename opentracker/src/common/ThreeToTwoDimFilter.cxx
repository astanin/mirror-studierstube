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
/** header file for ThreeToTwoDimFilter Node.
*
* @author Markus Sareika
*
* $Id: ThreeToTwoDimFilter.h
* @file                                                                   */
/* ======================================================================= */

#include <OpenTracker/common/ThreeToTwoDimFilter.h>
#ifdef USE_THREETOTWODIMFILTER

#include <cmath>

namespace ot {

	void ThreeToTwoDimFilter::initCalibrationData(std::vector<float> aSWidthVec_, 
		std::vector<float> aSHeightVec_, std::vector<float> cSOrientationQuat_, 
		std::vector<float> cSRoot2ScreenRootVec_, float trackingSystemScaleOneMeter_, 
		float screenDepthFrontInMeter_, float screenDepthBackInMeter_, 
		float screenResWidth_, float screenResHeight_)
	{
		dimCalc->initCalibrationData(aSWidthVec_, aSHeightVec_, cSOrientationQuat_, 
			cSRoot2ScreenRootVec_, trackingSystemScaleOneMeter_, screenDepthFrontInMeter_, 
			screenDepthBackInMeter_, screenResWidth_, screenResHeight_);
	}
	
	// Keep Data received from incomming Events
	void ThreeToTwoDimFilter::onEventGenerated( Event& event, Node& generator) 
	{
		static int i=0;
		if (generator.getName().compare("AspdInput") == 0) {
			// store input event from sink	
			aspdInput=event;			
			//newInput=true;	
			return;
		}
		
		if (generator.getName().compare("PointingDeviceInput") == 0) {
			// store input event from sink		
			pointingDeviceInput=event;
			newInput=true;			
			return;
		}
	}

	void ThreeToTwoDimFilter::push()
	{
		dimCalc->updateASPD(aspdInput);
		dimCalc->updateMPD(pointingDeviceInput);
		if( dimCalc->isMPDInsideScreenCuboid() )
		{
			printf("**************************push event inside !!!!\n");
			event.getPosition()[0]= dimCalc->getMPD2DCoords().x;
			event.getPosition()[1]= dimCalc->getMPD2DCoords().y;
			//source->event.getPosition()[3]= mousewheel;
			event.getButton() = pointingDeviceInput.getButton();
			event.timeStamp();
			
		}
	}

void ThreeToTwoDimFilter::pushEvent()
	{
		lock();  
		if (newInput)
		{
			newInput = false;
			push();
			unlock();
			updateObservers( event );
		}
		else
		{
			unlock();
		}
		
	}
	
	ThreeToTwoDimFilter::~ThreeToTwoDimFilter()
	{
		// Destructor method
	}
	
}

#endif //USE_THREETOTWODIMFILTER
