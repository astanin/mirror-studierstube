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
/** header file for FileSink Node.
 *
 * @author Gerhard Reitmayr
 *
 * $Id: FileSink.h 2123 2008-02-15 11:32:21Z bornik $
 * @file                                                                   */
/* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section filesink FileSink 
 * The FileSink node writes incoming event data to an output file. It is created
 * and controlled by the @ref filemodule. It associates its output stream with 
 * a station number, that is written into the file to multiplex different sinks
 * into one file. There may be more then one FileSink using the same station number
 * and / or output file. However the file may only be used for output.
 * It has the following attributes :
 * @li @c file the file name to use
 * @li @c station the station number to record
 *
 * An example element looks like this :
 * @verbatim
 <FileSink file="test.out" station="0">
 <Any EventGenerator element type>
 </FileSink>@endverbatim
*/

#ifndef _FILESINK_H
#define _FILESINK_H

#include "../OpenTracker.h"
#ifndef SWIG
#include "File.h"
#include <ace/Thread_Mutex.h>
#include <ace/Guard_T.h>
#endif

/**
 * This class implements a simple EventGenerator that writes any incoming events
 * to the associated file.
 * @author Gerhard Reitmayr
 * @ingroup common
 */

namespace ot {

    class OPENTRACKER_API FileSink : public Node
    {
        // Members
    public:
        /// Output File object associated with this node.
        File & file;
        /// station number
        int station;	

        // Methods
    protected:

        /** constructor method,sets commend member
         * @param file_ the File object to write to
         * @param station_ the station number to use */
        FileSink( File & file_, int station_ = 0 ) :
            Node(), 
            file( file_ ),
            station( station_ )
            { type="FileSink"; }

    public:
        /** tests for EventGenerator interface being present. Is overriden to
         * return 1 always.
         * @return always 1 */
        virtual int isEventGenerator()
	{
            return 1;
	}
    
        /**
         * this method notifies the object that a new event was generated.
         * It writes the event value out to the file and passes it
         * on to its parent.
         * @param event reference to the new event. Do not change the
         *        event values, make a copy and change that !
         * @param generator reference to the EventGenerator object that
         *        notified the EventObserver.
         */
        virtual void onEventGenerated( Event& event, Node& generator)
	{
            // file.write( event, station );
            //logPrintI("FileSink::onEventGenerated\n");
            updateObservers( event );
            ACE_Guard<ACE_Thread_Mutex> mutexlock(*mutex);
	    file.write(event, station);
	}

        void pushEvent() {};
	void pullEvent() {};

        friend class FileModule;
    };

}

#endif


/* 
 * ------------------------------------------------------------
 *   End of FileSink.h
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
