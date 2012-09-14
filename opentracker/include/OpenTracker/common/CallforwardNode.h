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
/** header file for Callforward node.
 *
 * @author Alexander Bornik
 *
 * $Id$
 * @file                                                                   */
/* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section callforwardnode Callforward Node
 * The Callforward node is a simple EventGenerator
 * It has the following attributes :
 * @li @c name a unique name to identify it among all Callforward nodes.
 *
 * An example element looks like this :
 * @verbatim
 <Callforward name="cf1">
 @endverbatim
*/

#ifndef _CALLFORWARDNODE_H
#define _CALLFORWARDNODE_H

#include "../OpenTracker.h"


#ifndef OT_NO_CALLFORWARDMODULE_SUPPORT

#ifndef SWIG
#include <ace/Synch.h>
#endif

namespace ot {

    typedef ACE_Mutex mutex_type;
    class CallforwardModule;
    //class CallforwardNode;

    /**
     * This class implements a simple node that stores an event and passes
     * an eventually pending event, each time it is traversed.
     * @author Alexander Bornik
     * @ingroup common
     */
    class OPENTRACKER_API CallforwardNode : public Node
    {
        // Members
    public:
    protected:
        /** name of the CallforwardNode for retrieving it from the module.
         * Note that this is not the name returned by getName(), rather the 
         * value set by the attribute name.
         */
        std::string name;

        /// flag, whether a new event has been set
        bool pendingevent;

        /// the event set by the uses
        Event event, outevent;

        /// pointer to creating module CallforwardModule
        CallforwardModule * cfmodule;

    public:

        // Methods
    protected:
        /** constructor method,sets commend member
         * @param name_ the name of the Callforward node */
        CallforwardNode( const std::string & name_ ) :
            Node(),
            name( name_ ),
            pendingevent(false),
            cfmodule( NULL )
	{}

    public:
        /** tests for EventGenerator interface being present. Is overriden to
         * return 1 always.
         * @return always 1 */
        virtual int isEventGenerator()
	{
            return 1;
	}
        
        /**
         * This method returns the value set by the name attribute of the 
         * Callforward node.
         * This is a different value then the one returned by getName(),
         * which is the
         * value set by the attribute DEF.
         * @return reference to the name string.
         */
        const std::string & getCallforwardName(void) const
	{
            return CallforwardNode::name;
	};

        /** 
         * method to set the event to be pushed into the graph
         * the lock ensures threadsafe event setting 
         */

        void setEvent(const Event& inevent)
        {
            lock();

            event = inevent;
            pendingevent = true;

            unlock();
        }

        void pushEvent();
        void pullEvent();

        friend class CallforwardModule;
    };

} // namespace ot


#endif //OT_NO_CALLFORWARDMODULE_SUPPORT


#endif

/* 
 * ------------------------------------------------------------
 *   End of CallforwardNode.h
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
