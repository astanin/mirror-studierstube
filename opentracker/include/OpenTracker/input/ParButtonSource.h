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
/** header file for ParButtonSource Node.
 *
 * @author Gerhard Reitmayr
 *
 * $Id: ParButtonSource.h 1953 2007-08-09 15:27:30Z sareika $
 * @file                                                                   */
/* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section parbuttonsource ParButtonSource 
 * This element reads button values from the parallel port. It will only generate
 * events, when the status changes. It works only on some hardware that has
 * a bidirectional parallel port. This includes Indigo2, O2, and most
 * modern PCs. There may be only one source per parallel port. The port itself
 * is defined by the device name or the base port address, depending on the
 * operating system. Both types of parameters are written into the @c dev attribute.
 *
 * An example element looks like this :
 * @verbatim
 <ParButtonSource dev="0x378"/>@endverbatim
 *
 * See the @ref parbuttonmodule for information on how to get it to work on different
 * operating systems.
 */

#ifndef _PARBUTTONSOURCE_H
#define _PARBUTTONSOURCE_H

#include "../OpenTracker.h"

namespace ot {

    /**
     * This class implements a simple EventGenerator. It is updated by the
     * ParButtonModule.
     * @author Gerhard Reitmayr
     * @ingroup input
     */
    class OPENTRACKER_API ParButtonSource : public Node
    {
        // Members
    public: 
        /// the event that is posted to the EventObservers
        Event event;
        
        bool changed;
        
        /// device handle or base address
        unsigned int handle;

        // Methods
    protected:
        /** simple constructor, sets members to initial values */
        ParButtonSource( unsigned int handle_ ) : Node(), handle( handle_ )
        {
            changed = 1;
        }

    public:            
        /** tests for EventGenerator interface being present. Is overriden to
         * return 1 always.
         * @return always 1 */
        virtual int isEventGenerator()
        {
            return 1;
        }

        
		void pushEvent()
		{            
			lock();
			if( changed == 1 )
			{			
				updateObservers( event );
				changed = 0;
			}
			unlock();
		}

		void pullEvent()
		{
			// nothing to do
		}    

        friend class ParButtonModule;
    };

} // namespace ot

#endif

/* 
 * ------------------------------------------------------------
 *   End of ParButtonSource.h
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
