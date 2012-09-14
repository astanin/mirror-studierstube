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
/** header file for SysMouseSink Node.
  *
  * @author Markus Sareika
  *
  * $Id: SysMouseSink.h
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section SysMouseSink SysMouseSink
 * The SysMouseSink node is a sink which forwards the events to the 
 * systems mouse.
 * the @ref SysMouseModule. 
 *
 * An example element looks like this :
 * @verbatim
<SysMouseSink>
	<AbsoluteInput>               
		<Any EventGenerator element type normalized to 65535>      
	</AbsoluteInput>
	<RelativeInput>               
		<Any EventGenerator element type>          
	</RelativeInput>
</SysMouseSink>@endverbatim
 */

#ifndef _SYSMOUSESINK_H
#define _SYSMOUSESINK_H


#include "../OpenTracker.h"

#ifdef USE_SYSMOUSE

#include <OpenTracker/input/SysMouseModule.h>


namespace ot {

/**
 * This class implements a source that sets its valid flag in
 * regular intervals and updates any EventObservers. 
 * @author Markus Sareika
 * @ingroup input
 */
class OPENTRACKER_API SysMouseSink : public Node
{

public:
    /// the event that is stored
    Event relativeEvent, absoluteEvent;
    /// flag whether event was changed since last display
    int changedAbsolute, changedRelative;
	/// offsets for various screen adaptions
	int xOffset, yOffset;
	float xFactor, yFactor;
         
    /** tests for EventGenerator interface being present. Is overriden to
     * return 1 always.
     * @return always 1 */
    virtual int isEventGenerator()
    {
        return 1;
    }

	/** simple constructor, sets members to initial values */
	SysMouseSink( SysMouseModule * ptr) : Node(),
		changedAbsolute(0),
		changedRelative(1),
		xOffset(0),
		yOffset(0),
		xFactor(1),
		yFactor(1)
	{ 
		sysMouseModule = ptr;
	}

	~SysMouseSink(void)
	{}

protected:
    
    /**
     * this method notifies the object that a new event was generated.
     * It stores a copy of the received event and passes the event on
     * to its observers.
     * @param event reference to the new event. Do not change the
     *        event values, make a copy and change that !
     * @param generator reference to the EventGenerator object that
     *        notified the EventObserver.
     */
    virtual void onEventGenerated( Event& e, Node& generator)
	{
		if (generator.getName().compare("AbsoluteInput") == 0) 
		{
			sysMouseModule->lockLoop();
			absoluteEvent = e;
			changedAbsolute = 1;
			sysMouseModule->unlockLoop();
		}
		if (generator.getName().compare("RelativeInput") == 0) 
		{
			sysMouseModule->lockLoop();
			relativeEvent = e;
			changedRelative = 1;
			sysMouseModule->unlockLoop();
		}
        updateObservers( e );
	}
	SysMouseModule * sysMouseModule;
	friend class SysMouseModule;

};

}  // namespace ot

#endif //USE_SYSMOUSE

#endif //_SYSMOUSESINK_H
