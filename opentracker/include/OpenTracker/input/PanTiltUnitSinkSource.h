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
/** header file for PanTiltUnitSinkSource Node.
  *
  * @author Markus Sareika
  *
  * $Id: PanTiltUnitSinkSource.h
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section PanTiltUnitSinkSource PanTiltUnitSinkSource
 * The PanTiltUnitSinkSource node is a simple EventGenerator that outputs the
 * current position and button state of the SpaceMouse. It is driven by
 * the @ref PanTiltUnitModule. 
 *
 * An example element looks like this :
 * @verbatim
<PanTiltUnitSinkSource/>@endverbatim
 */

#ifndef _PANTILTUNITSINKSOURCE_H
#define _PANTILTUNITSINKSOURCE_H

#include <stdio.h>
#include <queue>


#include "../OpenTracker.h"
#include "../common/ConsoleSource.h"

#ifdef USE_PANTILTUNIT

#include <ptuDll.h>
#pragma comment(lib,"ptudlld")

#include "Lanc.h"
#include "PtuCallback.h"


namespace ot {

/**
 * This class implements a source that sets its valid flag in
 * regular intervals and updates any EventObservers. 
 * @author Markus Sareika
 * @ingroup input
 */
class OPENTRACKER_API PanTiltUnitSinkSource : public Node
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
	void pushEvent( );
    void pullEvent( );

	// The COM port where the PTU is connected
	bool initComPort(int comPort);

	void closeComPort();
	
	PtuCallback* ptuCallback;

	/** simple constructor, sets members to initial values */
	PanTiltUnitSinkSource() : Node(), 
		publishEvent(false),
		movingPan(false),
		movingTilt(false),
		process(false)
	{

		lanc = new Lanc;
		float initOri[4]={0.0f, 0.0f, 0.0f, 1.0f};
		float initPos[3]={0.0f, 0.0f, 0.0f};
		topOffset.setPosition(initPos);
		topOffset.setOrientation(initOri);
		ptuLocation.setPosition(initPos);
		ptuLocation.setOrientation(initOri);
		tiltAngle = 0.;
		panAngle = 0.;
		isInit = false;
	}

	~PanTiltUnitSinkSource();
	

virtual void onEventGenerated( Event& event, Node& generator);

protected:

	int frequency, delayEvent, delay;

    // flags to indicate whether relativeInput or ptuGoto was changed during processing
    bool publishEvent, movingPan, movingTilt, process;

	double getTiltAngle();

	double getPanAngle();
	

	friend class PanTiltUnitModule;

private:

	// This Event holds the desired ptu Location (not implemented yet)
	Event ptuGoto;
	// This Event defines the moving speed of the ptu axis
	Event relativeInput;
	// This Event holds the top offset
	Event topOffset;
	// This Event holds the physical ptu location
	Event ptuLocation;
	// This Event defines the absolute orientation of the PTU axis
	Event absoluteInput;
	Event pickMouse;

	portstream_fd COMstream;
	
	double panResolution, tiltResolution;

	Lanc* lanc;

	std::queue<Event> delayQueue;

	double tiltAngle, panAngle;
	bool isInit;
};

}  // namespace ot

#endif //USE_PANTILTUNIT

#endif //_PANTILTUNITSINKSOURCE_H
