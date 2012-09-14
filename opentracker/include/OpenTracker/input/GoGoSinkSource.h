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
/** header file for GoGoSinkSource Node.
  *
  * @author Markus Sareika
  *
  * $Id: GoGoSinkSource.h  sareika $
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section GoGoSinkSource GoGoSinkSource
 * The GoGoSinkSource node is a EventGenerator that communicates with the 
 * SoGoGoKit and does the calculationes. They provide convenience 
 * functionalities in kontext of the realative input device.
 * 
 * see also the @ref gogomodule. 
 *
 * An example element looks like this :
 * @verbatim
 <ConsoleSink comment="GoGo" active="on">
 <EventSink tracking="gogo">
	 <GoGoSinkSource rotationWeight="0.1" translationWeight="1.0">
		 <RelativeInput>
		 <ConsoleSink comment="SpaceDevice" active="on">
			 <SpaceDeviceSource/>
		 </ConsoleSink>
		 </RelativeInput>
		 <ViewerLocation>
		 <ConsoleSink comment="ViewerLocation" active="on">
			 <EventKeyboardSource number="9"/>
		 </ConsoleSink>
		 </ViewerLocation>
		 <GoGoDeviceKit>
		 <ConsoleSink comment="GoGoDeviceKit" active="on">
			 <EventSource node="GOGODEVICEKIT" button="gogoButton"/>
		 </ConsoleSink>
		 </GoGoDeviceKit>
	 </GoGoSinkSource>
 </EventSink> 
 </ConsoleSink>
 
@endverbatim
 */

#ifndef _GOGOSINKSOURCE_H
#define _GOGOSINKSOURCE_H

#include "../OpenTracker.h"
#include "../common/ConsoleSource.h"

#ifdef USE_GOGO

// assign nodekit buttons
#define OTCOM_NONE				0
#define OTCOM_RESETPOSITION		1
#define OTCOM_RESETROTATION		2
#define OTCOM_TOGGLE_ROTATECAMERAAXIS  3




namespace ot {

/**
 * This class implements a simple source that sets its valid flag in
 * regular intervals and updates any EventObservers. 
 * @ingroup input
 */
class OPENTRACKER_API GoGoSinkSource : public Node
{
// Members
public: 
    /// the state that is posted to the EventObservers
    Event event;

// Methods
public:
    /** simple constructor, sets members to initial values */
    GoGoSinkSource() : Node()
    {
		for(int i(0);i<3;i++) 
		{
			viewerLocationPos[i] = 0.f;
			relativeInputPos[i] = 0.f;
			setAbsolutePos[i] = 0.f;
			tmpEventPos[i] = 0.f;
			viewerLocationOri[i] = 0.f;
			relativeInputOri[i] = 0.f;
			setAbsoluteOri [i] = 0.f;
			tmpEventOri[i] = 0.f;
		}
		viewerLocationOri[3] = 1.f;
		relativeInputOri[3] = 1.f;
		setAbsoluteOri [3] = 0.f;
		tmpEventOri[3] = 1.f;

		tmpEventCursorDistance=0.f;
		tmpEventScalingFactor=0.f;

		// default values
		tWeightDefault = 0.5f;
		rWeightDefault = 0.1f;

		tWeight = tWeightDefault;
		rWeight = rWeightDefault;

		nlDistance = 10;
		gogok = 1.0f;  //1.0f/6.0f;
		//scale = 1.0f;

		useAbsRotation = true;
		setAbsPos = false;
	};
        
    /** tests for EventGenerator interface being present. Is overriden to
     * return 1 always.
     * @return always 1 */
    virtual int isEventGenerator()
    {
        return 1;
    }

    /** pushes event down the line. */
    void push();

virtual void onEventGenerated( Event& event, Node& generator);

protected:
	// SpaceMouse Cursor-specific variables
	bool useAbsRotation;

	bool setAbsPos;

	// Incoming events providing data for calculations
	float viewerLocationPos[3];
	float relativeInputPos[3];
	float setAbsolutePos[3];
	float viewerLocationOri[4]; 
	float relativeInputOri[4];
	float setAbsoluteOri[4];
	unsigned short relativeInputBut, gogoDeviceKitBut, setAbsoluteBut;

	// This Event holds the result of the calculation of
	// current cursor position and is sent to the observers
    float tmpEventPos[3];
	float tmpEventOri[4];
	unsigned short tmpEventBut;
	float tmpEventCursorDistance, tmpEventScalingFactor;

	// A flag to indicate whether tmpEvent was changed during processing
    int changed;

	// old event type depricated???User Position
	//Event usrpos;
	
	// weights for translation and rotation
	float tWeightDefault, rWeightDefault;
	float tWeight, rWeight;

	// old depricated???
	bool extNode;

	// scaling factor
	//float scale;
	//float cursorDistance;

	// parameters for GoGo Interaction
	float nlDistance;			// distance where non-linear movement starts
    float gogok;
	// scaling factor for nonlinear movement acceleration

	bool newEvent, newKitEvent;

	void computeNewLocation();
	void computeGoGoNodeKitEvent();
	void setAbsoluteLocation();

	friend class GoGoModule;

private:

	// indicates that a new event has arrived
	

};

}  // namespace ot

#endif //USE_GOGO

#endif //_GOGOSINKSOURCE_H
