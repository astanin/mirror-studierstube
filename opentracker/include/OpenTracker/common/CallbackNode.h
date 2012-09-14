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
/** header file for Callback node.
 *
 * @author Gerhard Reitmayr
 *
 * $Id: CallbackNode.h 1970 2007-08-20 11:12:16Z bornik $
 * @file                                                                   */
/* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section callbacknode Callback Node
 * The Callback node is a simple EventObserver / EventGenerator that passes
 * events on and calls a registered callback function everytime it
 * receives a new event with the passed event.
 * It has the following attributes :
 * @li @c name a unique name to identify it among all Callback nodes.
 *
 * An example element looks like this :
 * @verbatim
 <Callback name="cb1">
 <Any EventGenerator element type>
 </Callback>@endverbatim
*/

#ifndef _CALLBACKNODE_H
#define _CALLBACKNODE_H

#include "../OpenTracker.h"

#ifndef SWIG
#include <ace/Synch.h>
#endif

#ifndef OT_NO_CALLBACKMODULE_SUPPORT

#include <OpenTracker/common/CallbackModule.h>
namespace ot {
class CallbackNode;
}



namespace ot {

    class CallbackModule;
    class CallbackNode;

    /**
     * This class implements a simple node that stores a function pointer
     * and calls it every time an event it received. The event passed to the
     * can be changed by the function and the changes will propagate to the parent node.
     * Furthermore it passes the event on to its single child.
     * @author Gerhard Reitmayr
     * @ingroup common
     */
    class OPENTRACKER_API CallbackNode : public Node
    {
        // Members
    public:
    protected:
        typedef ACE_Mutex mutex_type;
        /** name of the CallbackNode for retrieving it from the module.
         * Note that this is not the name returned by getName(), rather the value
         * set by the attribute name.
         */
        std::string name;
        /// callback function
        OTCallbackFunction * function;
        /// data pointer
        void * data;
        /// the event passed to the function and the parent
        Event event;

        /// mutex to protect the event
        mutex_type eventmutex;

        /// pointer to creating module CallbackModule
        CallbackModule * cbmodule;

        // Methods
    protected:
        
        /** constructor method,sets commend member
         * @param name_ the name of the Callback node */
        CallbackNode( const std::string & name_ ) :
            Node(),
            name( name_ ),
            function( NULL ),
            data( NULL ),
            cbmodule( NULL )
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
         * This method notifies the object that a new event was generated.
         * @param event reference to the new event.
         * @param generator reference to the EventGenerator object that
         *        notified the EventObserver.
         */
        virtual void onEventGenerated( Event& event, Node& generator);

        /**
         * This method returns the value set by the name attribute of the CallbackNode.
         * This is a different value then the one returned by getName() which is the
         * value set by the attribute DEF.
         * @return reference to the name string.
         */
        const std::string & getCallbackName(void) const
	{
            return CallbackNode::name;
	};

        void pushEvent();
        void pullEvent();

        friend class CallbackModule;
    };

} // namespace ot


#endif //OT_NO_CALLBACKMODULE_SUPPORT


#endif

/* 
 * ------------------------------------------------------------
 *   End of CallbackNode.h
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
