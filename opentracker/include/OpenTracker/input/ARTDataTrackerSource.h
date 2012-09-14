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
/** header file for ARTDataTrackerSource Node.
 *
 * @author Christopher Schmidt
 *
 * $Id: ARTDataTrackerSource.h 1812 2007-05-04 16:36:12Z sareika $
 * @file                                                                   */
/* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section artdatatrackersource ARTDataTrackerSource
 * The ARTDataTrackerSource node is a simple EventGenerator that inputs the data 
 * from the ART Data Tracker.
 * @li @c number the body number of the specific source, starting with 0
 *
 * An example element looks like this :
 * @verbatim
 <ARTDataTrackerSource number="0"/>@endverbatim
*/

#ifndef _ARTDATATRACKERSOURCE_H
#define _ARTDATATRACKERSOURCE_H

#include "../dllinclude.h"

#include "../OpenTracker.h"


#ifndef OT_NO_ARTDATATRACKER_SUPPORT


/**
 * This class implements a simple source that inputs the data from the ART Data Tracker
 * and updates any EventObservers.
 * @ingroup input
 * @author Christopher Schmidt
 */

namespace ot {

    class OPENTRACKER_API ARTDataTrackerSource : public Node
    {
	// Members
    public:
	/// body number
	int number;
	/// the event that is stored
	Event event;
	/// a flag to indicate whether it was changed during processing
	int changed;

	// Methods
    public:
	/** simple constructor, sets members to initial values
	 * @param number_ the body number of the specific source */
	ARTDataTrackerSource( int number_ ) : Node(), number( number_ ), changed( 1 )
	{
	};
	  
	/** tests for EventGenerator interface being present. Is overriden to
	 * return 1 always.
	 * @return always 1 */
	int isEventGenerator();

	void pushEvent();
	void pullEvent();
    };

} // namespace ot


#endif //OT_NO_ARTDATATRACKER_SUPPORT


#endif

/* ===========================================================================
   End of ARTDataTrackerSource.h
   ===========================================================================
   Automatic Emacs configuration follows.
   Local Variables:
   mode:c++
   c-basic-offset: 4
   eval: (c-set-offset 'subeventment-open 0)
   eval: (c-set-offset 'case-label '+)
   eval: (c-set-offset 'eventment 'c-lineup-runin-eventments)
   eval: (setq indent-tabs-mode nil)
   End:
   =========================================================================== */
