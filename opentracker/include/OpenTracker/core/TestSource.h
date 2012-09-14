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
/** header file for TestSource Node.
 *
 * @author Gerhard Reitmayr
 *
 * $Id: TestSource.h 1974 2007-08-24 07:34:06Z bornik $
 * @file                                                                   */
/* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section testsource TestSource
 * The TestSource node is a simple EventGenerator that fires in fixed intervals
 * standard events. The events can be customized to have other then the default
 * values.
 *
 * It also supports simulation of noisy data. The parameter @c noise defines the
 * size of a uniform distribution used to perturb the given default position and
 * orientation. In addition to that it also defines the probability that
 * the orientation representation is using the negative representation.
 *
 * Moreover, the TestSource provides some multi-modal attributes of type int, char, double,
 * and float. These attributes can either be renamed by an EventUtilityNode and used as test
 * data for the implementation of new nodes, or you can add more or different attributes in the
 * same way to get test data for your implementation.
 *
 * The node has the following elements :
 * @
 * @li @c frequency every freq. cycle it fires
 * @li @c offset starting after offset cycles
 * @li @c position position value of the event to fire as 3 floats
 * @li @c orientation orientation value of the event to fire as 4 floats representing a quaternion
 * @li @c button a 16 bit integer value representing the button events
 * @li @c confidence a float value in [0,1] to represent the confidence value
 * @li @c noise a float value > 0, if present will output noisy data for simulations
 *
 * An example element looks like this :
 * @verbatim
 <TestSource frequency="10" offset="5" />@endverbatim
*/

#ifndef _TESTSOURCE_H
#define _TESTSOURCE_H

#include "../dllinclude.h"


#ifndef OT_NO_TESTMODULE_SUPPORT


#include "Node.h"

/**
 * This class implements a simple source that is fired by its module in
 * regular intervals and updates any EventObservers.
 * @ingroup core
 * @author Gerhard Reitmayr
 */

namespace ot {

    class OPENTRACKER_API TestSource : public Node
    {
        // Members
    protected:
        /// per node cycle counter
        unsigned long cycle;
        /// Time to sleep between executions
        ACE_Time_Value sleeptime;
        /// Time offset of executions
        ACE_Time_Value sleepoffset;
        /// use noise and noise level
        double noise;
        /// the original event
        Event event;
        /// the perturbed event 
        Event perturbed;
        /// the event posted to observers
        Event outevent;
        /// flag whether an event should be posted
        bool changed;
        
        // Methods
    protected:
        /** simple constructor, sets members to initial values
         * @param frequency_ initial value for member frequency
         * @param offset_ initial value for member offset */
        TestSource( double frequency_, int offset_ );
        
        virtual ~TestSource() 
        {
            std::cout << "TestSource destructor" << std::endl;
	}
    public:
        /** tests for EventGenerator interface being present. Is overriden to
         * return 1 always.
         * @return always 1 */
        int isEventGenerator()
        {
            return 1;
        }

        /** pushes event down the line. Needed to access protected
         * updateObservers method in EventGenerator. Note that the
         * implementation of this method is in the file TestModule.cxx !
         */
        void pushEvent();
        void pullEvent();

        inline void resetCycleCounter() { cycle = 1; };
        void calculateEvent();

        friend class TestModule;
    };

} // namespace ot


#endif //OT_NO_TESTMODULE_SUPPORT


#endif

/* 
 * ------------------------------------------------------------
 *   End of TestSource.h
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
