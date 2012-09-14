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
/** header file for ConsoleSource Node.
 *
 * @author Gerhard Reitmayr
 *
 * $Id: ConsoleSource.h 1807 2007-04-12 10:35:50Z bornik $
 * @file                                                                   */
/* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section consolesource ConsoleSource
 * The ConsoleSource node is a simple EventGenerator that generates new
 * tracking events for one station based on keyboard input to the console.
 * Each ConsoleSource node has a station number that can be selected via
 * the keyboard. There can be only one node per station. It is also managed
 * and driven by the @ref consolemodule.
 *
 * It has the following attributes :
 * @li @c number a number ( 0 - 9 ) giving the station number.
 *
 * An example element looks like this :
 * @verbatim
 <ConsoleSource number="2"/>@endverbatim
*/

#ifndef _CONSOLESOURCE_H
#define _CONSOLESOURCE_H

#include "../OpenTracker.h"

/**
 * This class implements a simple node that generates tracking events
 * based on keyboard input.
 * @author Gerhard Reitmayr
 * @ingroup common
 */

namespace ot {

    class OPENTRACKER_API ConsoleSource : public Node
    {
        // Members
    public:
        /// station number
        int number;
        /// the event that is stored
        Event event;
        /// a flag to indicate whether it was changed during processing
        int changed;
		/// a flag indicating a special pen console
		int penConsole;

        // Methods
    protected:
        /** constructor method,sets commend member
         * @param comment_ the comment line to use */
		ConsoleSource( int number_, int penConsole_ ) :
            Node(),
            number( number_ ),
            changed( 0 ),
			penConsole( penConsole_ )
	{
	}

    public:
        /** tests for EventGenerator interface being present. Is overriden to
         * return 1 always.
         * @return always 1 */
        virtual int isEventGenerator()
	{
            return 1;
	}

        void pushEvent() {};
        void pullEvent() {};

        friend class ConsoleModule;
    };

} // namespace ot

#endif


/* 
 * ------------------------------------------------------------
 *   End of ConsoleSource.cxx
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
