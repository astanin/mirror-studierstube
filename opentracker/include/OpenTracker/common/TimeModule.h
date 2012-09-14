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
/** header file for Time Module.
 *
 * @author Gerhard Reitmayr
 *
 * $Id: TimeModule.h 1547 2006-10-25 10:33:10Z veas $
 * @file                                                                   */
/* ======================================================================= */

/**
 * @page module_ref Module Reference
 * @section timemodule TimeModule
 * The TimeModule controls the timing of the OpenTracker main loop. It does 
 * not provide any functionality related to nodes and events. There are two
 * operating modes. In @e Sleep mode the TimeModule sleeps after each turn
 * of the main loop a specified amount of time. In @e Framerate mode it tries
 * to achieve a certain framerate of loops per second. Both modes are limited
 * by the underlying implementation of OSUtils. That means that sleeps shorter
 * then 1/100 of a second are not possible, which obvious implications for the
 * @e Sleep mode. The @e Framerate mode works by inserting short sleeps after
 * a number of loops to achieve the desired overall framerate ! 
 *
 * The configuration element is called @c TimeConfig and has the following attributes :
 * @li @c rate float giving the desired frames per second (i.e. 100 )
 * @li @c sleep number of milliseconds to sleep each loop
 * @li @c display false either true or false, displays the framerate at the end.
 * If both @c rate and @c sleep are specified the @c rate attribute takes precedence and the 
 * @e Framerate mode is used. If no configuration element is present, the module
 * is not activated and the main loop runs at full speed.
 *
 * An example configuration element looks like this :
 * @verbatim
 <TimeConfig rate="187"/>@endverbatim
*/

#ifndef _TIMEMODULE_H
#define _TIMEMODULE_H

#include "../OpenTracker.h"

/**
 * This class implements the TimeModule functionality. It sleeps the required time
 * in the stop() method, which is executed after a loop to check for stopping
 * conditions. At this point it should not interfere with any event processing.
 * @author Gerhard Reitmayr
 * @ingroup common
 */


#ifndef OT_NO_TIMEMODULE_SUPPORT


namespace ot {

    class OPENTRACKER_API TimeModule : public Module
    {
        // Members
    protected:
        // number of milliseconds to sleep in @e Sleep mode
        int sleep, 
            // flag for displaying of frame rate
            display;
        // start time to calculate the frame rate in @e Framerate mode
        double startTime, 
            // the desired frame rate in frames / millisecond for computational purposes
            rate, 
            // counting frames
            count;

        // Methods
    public:
        /** constructor method. */
        TimeModule() : Module(), sleep(0), display(0), rate(0)
	{};
  
        /**
         * initializes the tracker module. Parses any @c sleep or @c rate attributes
         * @param attributes StringTable of elements attribute values. Should be
         *        possibly , but is not for convenience.
         * @param localTree pointer to root of configuration nodes tree
         */
        void init( StringTable & attributes,  ConfigNode * localTree);
  
        /** starts the module, stores startTime if in @i Framerate mode. */
        void start();
    
        /** test for stopping the main loop. Here the actual sleeping is done
         * if necessary.
         * @return always 0*/
        int stop();
    
        /** closes the module, prints out the framerate, if display is 1. */
        void close();
    };

	OT_MODULE(TimeModule);
} // namespace ot

	

#endif //OT_NO_TIMEMODULE_SUPPORT

#endif


/* 
 * ------------------------------------------------------------
 *   End of TimeModule.h
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
