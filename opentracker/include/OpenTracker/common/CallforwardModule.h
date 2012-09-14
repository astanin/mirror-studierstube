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
/** header file for Callforward module.
 *
 * @author Alexander Bornik
 *
 * $$
 * @file                                                                   */
/* ======================================================================= */

/**
 * @page module_ref Module Reference
 * @section callforwardmodule CallforwardModule
 * The Callforward module provides @ref callforward nodes that pick up data
 * sent to them by the application. Callforward nodes thereby
 * are event generators. registered 
 * The Callforward module does not have a 
 * configuration element but reserves the name 'CallforwardConfig' for it.
 */

#ifndef _CALLFORWARDMODULE_H
#define _CALLFORWARDMODULE_H

#include "../OpenTracker.h"
//#include "CallforwardNode.h"

#ifndef OT_NO_CALLFORWARDMODULE_SUPPORT

namespace ot {

    typedef std::map<std::string, Node*> NodeMap;
    /**
     * The module and factory to drive the callforward nodes. It constructs
     * Callforward nodes via the NodeFactory interface and registers
     * @author Alexander Bornik
     * @ingroup common
     */
    class OPENTRACKER_API CallforwardModule : public Module, public NodeFactory
    {
     
        // Members
    protected:
        /// map of strings onto nodes
        NodeMap nodes;
        
        // Methods
    public:
        /** constructor method. */
        CallforwardModule() : Module(), NodeFactory()
	{};
        /** Destructor method, clears nodes member. */
        virtual ~CallforwardModule(){};    
        /** This method is called to construct a new Node. It compares
         * name to the Callforward element name, and if it matches
         * creates a new Callforward node.         * @param name reference to string containing element name
         * @attributes reference to StringMap containing attribute values
         * @return pointer to new Node or NULL. The new Node must be
         *         allocated with new ! */
        virtual Node * createNode( const std::string& name,  const StringTable& attributes);
 

        /** Registers a user provided event to be transported through the graph.
         * This call is thread safe. The event can be produced outside
         * the OpenTracker loop thread.
         * @param name of the Callforward node to push the event in
         * @param reference to the user provided event
         */
        bool callForward(const std::string& name, Event &event); 

        /**
         * pushes events into the tracker tree. Determines, whether the user
         * has set an Event via the callForward method and passes the events 
         * on in that case.
         */
        virtual void pushEvent();
        
    };

    OT_MODULE(CallforwardModule);

} // namespace ot {


#endif //OT_NO_CALLFORWARDMODULE_SUPPORT


#endif

/* 
 * ------------------------------------------------------------
 *   End of CallforwardModule.h
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
