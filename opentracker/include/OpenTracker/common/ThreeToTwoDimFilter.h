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

/**
 * @page Nodes Node Reference
 * @section ThreeToTwoDimFilter ThreeToTwoDimFilter
 * The ThreeToTwoDimFilter node is a filter that transforms the input data with 
 * respect to the configuration of the config node : 
   <QtMouseEventCalibConfig>
	 <QtAppScreen
		 ASHeightVec="0.0000 -0.9999 0.0000"
		 ASWidthVec="0.9999 0.0000 0.0000"
		 CSOrientationQuat="0.0000 0.0000 -0.7071 0.7071"
		 CSRoot2ScreenRootVec="-0.9999 -0.9999 0.0000"
	 />
	</QtMouseEventCalibConfig>
 * This strange construction is due to backward compatibility to the OTQT 
 * calibration mechanism which can also be used here.
 *
 * The ThreeToTwoDimFilters are driven by the @ref ThreeToTwoDimFilterModule 
 * which holds the singular config for the calibration. 
 *
 * An example element looks like this :
 * @verbatim
 <ThreeToTwoDimFilter>
	 <AspdInput>
		<Any6DOFsource/>
	 </AspdInput>
	 <PointingDeviceInput>
		<Any6DOFsource/>
	 </PointingDeviceInput>
 </ThreeToTwoDimFilter>@endverbatim
 */

#ifndef _THREETOTWODIMFILTER_H
#define _THREETOTWODIMFILTER_H

#include <stdio.h>


#include <OpenTracker/OpenTracker.h>
#include <OpenTracker/common/ConsoleSource.h>
#include <OpenTracker/common/ThreeToTwoDimCalc.h>

#ifdef USE_THREETOTWODIMFILTER



namespace ot {

/**
 * This class implements a source that sets its valid flag in
 * regular intervals and updates any EventObservers. 
 * @author Markus Sareika
 * @ingroup input
 */
class OPENTRACKER_API ThreeToTwoDimFilter : public Node
{
// Members
public: 
    /// the state that is posted to the EventObservers
    Event event;
        
    /** tests for EventGenerator interface being present. Is overriden to
     * return 1 always.
     * @return always 1 */
    virtual int isEventGenerator()
    {
        return 1;
    }

    /** pushes event down the line. */
    void push();

	virtual void pushEvent();

	/** simple constructor, sets members to initial values */
	ThreeToTwoDimFilter() : Node(), 
		newInput(false)
	{
		dimCalc = new ThreeToTwoDimCalc;
	}

	~ThreeToTwoDimFilter();

	void initCalibrationData(std::vector<float> aSWidthVec_, std::vector<float> aSHeightVec_, 
		std::vector<float> cSOrientationQuat_, std::vector<float> cSRoot2ScreenRootVec_,  
		float trackingSystemScaleOneMeter_, float screenDepthFrontInMeter_, 
		float screenDepthBackInMeter_, float screenResWidth_, float screenResHeight_);
	
	virtual void onEventGenerated( Event& event, Node& generator);

protected:

    bool newInput;
	Event aspdInput;
	Event pointingDeviceInput;

	friend class ThreeToTwoDimFilterModule;

private:

	ThreeToTwoDimCalc* dimCalc;
};

}  // namespace ot

#endif //USE_THREETOTWODIMFILTER

#endif //_THREETOTWODIMFILTER_H
