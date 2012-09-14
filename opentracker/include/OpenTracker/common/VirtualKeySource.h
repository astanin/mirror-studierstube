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
/** header file for VirtualKeySource Node.
  *
  * @author Markus Sareika
  *
  * $Id: VirtualKeySource.h
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section VirtualKeySource VirtualKeySource
 * The VirtualKeySource node is a sink which monitors a key of the keyboard and
 * generates a button event when the key state changes
 *
 * An example element looks like this :
 * @verbatim
<VirtualKeySource virtualKeyCode="integer of virtualkeycode"/>
 */

#ifndef _VIRTUALKEYSOURCE_H
#define _VIRTUALKEYSOURCE_H


#include "../OpenTracker.h"

#ifdef USE_VIRTUALKEYSOURCE
#include <OpenTracker/common/VirtualKeyModule.h>


namespace ot {

/**
 * This class implements a source that sets its valid flag in
 * regular intervals and updates any EventObservers. 
 * @author Markus Sareika
 * @ingroup input
 */
class OPENTRACKER_API VirtualKeySource : public Node
{

public:
    /// the event that is published
    Event event;
	Event outevent;

	bool process;
         
    /** tests for EventGenerator interface being present. Is overriden to
     * return 1 always.
     * @return always 1 */
    virtual int isEventGenerator()
    {
        return 1;
    }

	/** simple constructor, sets members to initial values */
	VirtualKeySource(VirtualKeyModule * ptr) : Node(),
		virtualKeyCode(33)
	{ 
		virtualKeyModule = ptr;
	}

	~VirtualKeySource(void)
	{
		//
	}

protected:
    

	virtual void push()
	{
		lock();
		outevent = event;
		unlock();

		updateObservers( event );
	}

	int virtualKeyCode;
	VirtualKeyModule * virtualKeyModule;
	friend class VirtualKeyModule;

};

}  // namespace ot

#endif //USE_VIRTUALKEYSOURCE

#endif //_VIRTUALKEYSOURCE_H
