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
/** header file for JoystickSource Node.
 *
 * @author Rainer Splechtna
 *
 * $Id: JoystickSource.h 1547 2006-10-25 10:33:10Z veas $
 * @file                                                                   */
/* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section joysticksource JoystickSource 
 *
 * The JoystickSource node is a simple EventGenerator that inserts events generated from
 * the joystick input data into the tracker tree. The JoystcikSource element has the 
 * following attributes :
 * @li @c id the ID of the joystick the source is associated with
 *
 * An example element looks like this :
 * @verbatim
 <JoystickSource id="1"/>@endverbatim
*/
 
#ifndef _JOYSTICKSOURCE_H
#define _JOYSTICKSOURCE_H

#include "../OpenTracker.h"

#ifdef USE_JOYSTICK

#include "../dllinclude.h"

/**
 * This class implements a simple EventGenerator. It is updated by the
 * JoystickModule.
 * @ingroup input
 * @author Rainer Splechtna
 */

namespace ot {

	class OPENTRACKER_API JoystickSource : public Node
	{
		// Members
	public:
		/// stores joystick ID
		int id;
		/// the event that is posted to the EventObservers
		Event event;

		// Methods
	protected:
		/** simple constructor, sets members to initial values
		 * @param id_ joystick ID */
		JoystickSource( int id_ ) : 
			Node(),
			id( id_ )
		{}

		/// the event that is updated by the @see JostickModule polling thread.
		Event tmpEvent;
		/// a flag to indicate whether tmpEvent was changed during processing
		int changed;

	public:            
		/** tests for EventGenerator interface being present. Is overriden to
		 * return 1 always.
		 * @return always 1 */
		int isEventGenerator()
		{
			return 1;
		}
		/** pushes event down the line. */
		void push()
		{
			event.timeStamp();
			updateObservers( event );
		}

		friend class JoystickModule;
	};
}

#endif
#endif

/* 
 * ------------------------------------------------------------
 *   End of JoystickSource.h
 * ------------------------------------------------------------
 *   Automatic Emacs configuration follows.
 *   Local Variables:
 *   mode:c++
 *   c-basic-offset: 4
 *   eval: (c-set-offset 'substatement-open 0)
 *   eval: (c-set-offset 'case-label '+)
 *   eval: (c-set-offset 'statement 'c-lineup-runin-statements)
 *   eval: (setq indent-tabs-mode nil)
 *   End:
 * ------------------------------------------------------------ 
 */
