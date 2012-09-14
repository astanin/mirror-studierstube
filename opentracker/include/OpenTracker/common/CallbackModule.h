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
/** header file for Callback module.
 *
 * @author Gerhard Reitmayr
 *
 * $Id: CallbackModule.h 2103 2007-12-19 16:39:50Z bornik $
 * @file                                                                   */
/* ======================================================================= */

/**
 * @page module_ref Module Reference
 * @section callbackmodule CallbackModule
 * The Callback module provides @ref callbacknode nodes that call registered 
 * callbacks functions for any event they receive. Each node can call only
 * one function. The nodes are addressed by unique names, set in the 
 * element of that node. After reading the initialization file an application
 * can register callback functions on the read nodes. It doesn't have a 
 * configuration element but reserves the name 'CallbackConfig' for it.
 */

#ifndef _CALLBACKMODULE_H
#define _CALLBACKMODULE_H

#include "../OpenTracker.h"



#ifndef OT_NO_CALLBACKMODULE_SUPPORT
namespace ot{

    class CallbackNode;
};
//#include "CallbackNode.h"

typedef void OTGlobalCallbackFunction(ot::CallbackNode *, ot::Event &, void *);
typedef void OTCallbackFunction (ot::CallbackNode *, ot::Event &, void *);

namespace ot {

    typedef std::map<std::string, Node*> NodeMap;
    typedef std::map<std::string, std::pair<OTCallbackFunction*, void*> > fctmap_type;
    /**
     * The module and factory to drive the callback nodes. It constructs
     * CallbackNode nodes via the NodeFactory interface and registers
     * callbacks with them after parsing
     * @author Gerhard Reitmayr
     * @ingroup common
     */
    class OPENTRACKER_API CallbackModule : public Module, public NodeFactory
    {
        // Members
    protected:
        /// map of strings onto nodes
        NodeMap nodes;

        /// global callback function pointer
        OTGlobalCallbackFunction *gcbfunction;
        /// data passed to global callback function
        void *gcbdata;
        /// map storing all local callbacks -> even if the nodes are not there
        /// Thus Callbacks are bound when the node is created later on
        fctmap_type lcbmap;

        // Methods
    public:
        /** constructor method. */
        CallbackModule() : Module(), NodeFactory(), 
            gcbfunction(NULL), gcbdata(NULL), lcbmap()
	{};
        /** Destructor method, clears nodes member. */
        virtual ~CallbackModule(){};    
        /** This method is called to construct a new Node. It compares
         * name to the TestSource element name, and if it matches
         * creates a new TestSource node.
         * @param name reference to string containing element name
         * @attributes refenrence to StringMap containing attribute values
         * @return pointer to new Node or NULL. The new Node must be
         *         allocated with new ! */
        virtual Node * createNode( const std::string& name, 
                                   const StringTable& attributes);
        /** This method can be used to setup a map of the existing per node
         * callback functions and data registered in all nodes of the module
         * regardless of their current existance.
         * This function is internally used to keep registered callback 
         * functions alive, when the context is newly set up or copied
         */
        void getLocalCallbackMap(fctmap_type &fmap);
        
        /** sets a callback on a certain node. The node is identified by its
         * unique name. Any present callback function in the node is overwritten
         * by the new one.
         * @param name the unique name of the callback node 
         * @param function the new callback function to set on the node
         * @param data pointer to data that is passed to the callback function
         */
        void setCallback( const std::string& name, OTCallbackFunction * function, void * data = NULL );

        /** sets a global callback. Any present global callback function in the
         * module is overwritten by the new one.
         * @param function the new callback function to set on the module
         * @param data pointer to data that is passed to the callback function
         */
        
        void setGlobalCallback( OTGlobalCallbackFunction * function, void * data = NULL );
        /** returns the global callback function
         */

        OTGlobalCallbackFunction * getGlobalCallbackFunction() const;
        /** returns the global callback function data
         */
        void * getGlobalCallbackData() const;
    };

    OT_MODULE(CallbackModule);

} // namespace ot {


#endif //OT_NO_CALLBACKMODULE_SUPPORT


#endif

/* 
 * ------------------------------------------------------------
 *   End of CallbackModule.h
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
