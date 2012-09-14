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
/** header file for FileSource Node.
 *
 * @author Gerhard Reitmayr
 *
 * $Id: FileSource.h 1971 2007-08-21 09:48:51Z bornik $
 * @file                                                                   */
/* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section filesource FileSource
 * The FileSource node reads event data from an input file and passes new
 * events into the graph. It is created and controlled by the @ref filemodule.
 * It associates its input stream with
 * a station number, that is contained in the input file to demultiplex different input
 * streams from one file. There may be more then one FileSource using the same input file,
 * however they need to use different station numbers. Moreover the file may
 * only be used for input.
 *
 * The event data is processed in the order appearing in the file. During each cycle,
 * each source fires at most once. Data that has a station number which is not used
 * by any FileSource is dropped.
 *
 * A FileSource has the following attributes :
 * @li @c file the file name to use
 * @li @c station the station number to record
 * @li @c localtime false (true|false) whether to use the local time as time stamp or
 *                  the time recorded in the file
 *
 * An example element looks like this :
 * @verbatim
 <FileSource file="name" station="0" localtime="false"/>@endverbatim
*/

#ifndef _FILESOURCE_H
#define _FILESOURCE_H

#include "../OpenTracker.h"

#ifndef SWIG
#include "File.h"
#endif
/**
 * This class implements a simple EventGenerator source node. It is driven by the
 * FileModule class which reads the data from the file and passes it to the
 * right FileSource object.
 * @author Gerhard Reitmayr
 * @ingroup common
 */
namespace ot {

    class FileModule;
    typedef std::map<double, Event*> EventMap;

    class OPENTRACKER_API FileSource : public Node
    {
        // Members
    public:
        /// station number
        int station;
        /// flag whether timestamp should be taken from the file or generated locally
        bool localTime;
        /// flag whether event was changed this round
        bool changed;

    protected:
        /// map matching events to timestamps for realtime playback
        std::map<double, Event*> eventMap;
        /// iterator pointing to current event in eventMap
        std::map<double, Event*>::iterator currentEvent;

        FileModule *module;

        // Methods
    protected:
        /** constructor method,sets the station number and other default values.
         * @param station the station number to use.
         */
        FileSource( int station_ = 0, bool localTime_ = false,
                    FileModule *module_ = NULL ) :
            Node(),
            station( station_ ),
            localTime( localTime_ ),
            changed( 0 ),
            module(module_)
	{}

    public:
        /** tests for EventGenerator interface being present. Is overriden to
         * return 1 always.
         * @return always 1 */
        virtual int isEventGenerator()
	{
            return 1;
	}

        void pushEvent();
        void pullEvent();

        friend class FileModule;
    };

} // namespace ot

#endif


/*
 * ------------------------------------------------------------
 *   End of FileSource.h
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
