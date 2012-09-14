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
/** header file for Module class.
 *
 * @author Gerhard Reitmayr
 *
 * $Id: Module.h 2114 2008-02-04 15:25:12Z bornik $
 * @file                                                                   */
/* ======================================================================= */

#ifndef _MODULE_H
#define _MODULE_H

#include "../dllinclude.h"

#include "ConfigNode.h"
#include "StringTable.h"
#include <OpenTracker/core/IRefCounted.h>
#include <OpenTracker/misc/Cptr.h>

/**
 * Module is an abstract super class for all OpenTracker modules. A module
 * is associated with a configuration element. If the configuration element
 * is present, it is initialized with init. Then it is started. During the
 * main loop in Context, pushEvent and pullEvent, and stop are periodically
 * called. When the Context exits its main loop it calls close on all methods.
 *
 * This class provides empty implementations for all its methods instead of
 * pure virtual functions, so that classes inheriting from it don't have to
 * implement all methods in a trivial way.
 * @author Gerhard Reitmayr
 * @ingroup core
 */

namespace ot {

    class OPENTRACKER_API Module
    {
        //members
    protected:
        /// check if this module appeared in the configuration section
        int initialized;
        /** pointer to the context this module is working in. It will be set
         * by the initialize method.
         */
        Context * context;
        
        OT_DECLARE_IREFCOUNTED;
        typedef Cptr<Module> Ptr;
    public:
        
     //   static Context * contextx;
        
        //methods
    public:
        /// constructor method
        Module(): initialized(0), context(NULL) { OT_INITIALIZE_IREFCOUNTED; };

	/// virtual destructor (as it befits any true class hierarchy)
        virtual ~Module();
        virtual void setContext(Context * c){  context = c; };
        Context * getContext() const {return context;}; 
        /**
         * closes the module. A place for cleanup code etc.
         * This class provides an empty implementation for subclasses not doing
         * anything here. */
        virtual void close() {};

        /**
         * initializes the tracker module. This class provides an implementation
         * that sets the initialization flag to true. Subclasses should call this
         * method, if they override it, before doing anything else. It takes the attributes of the
         * element configuring this module and a local tree consisting of the
         * children of the element. This tree must be build of Nodes.
         * @param attributes StringTable of elements attribute values. Should be
         *        possibly , but is not for convenience.
         * @param localTree pointer to root of configuration nodes tree
         */
        virtual void init(StringTable& attributes,  ConfigNode * localTree)
        {
            initialized = 1;
        };

        /**
         * pulls event information out of the tracker tree. It enables the module
         * to query any EventQueue or TimeDependend node in the shared memory. It
         * is called after pushEvent was executed on each module.
         */
        virtual void pullEvent() {};

        /**
         * pushes event information into the tracker tree. It enables the module
         * to push new data into the tree by updating EventGenerator nodes and
         * thereby triggering an event.
         */
        virtual void pushEvent() {};

        /**
         * This method is called after initialisation is finished and before the
         * main loop is started. It allows the module to finish any setups that
         * need to be done before entering the main loop. */
        virtual void start() {};

        /**
         * tests whether the module wants the tracker main loop to stop.
         * @return 1 if main loop should stop, 0 otherwise. */
        virtual int stop()
        {
            return 0;
        };

		/**
		 * This method should be called by the Node's factory, in order to register the Node with its Module.
		 * It should be overloaded by descendants that need to keep track of their Nodes.
		 */
		virtual void addNode(const Node * ){};
		/**
		 * This method deregisters a node from the Module. It is called when removing a node from the graph.
		 * It should be overloaded by descendants that need to keep track of their Nodes.
		 */
		virtual void removeNode(Node *){};

        /**
         * tests whether the module was initialized or not.
         * @return 1 of the module was initialized, 0 otherwise. */
        int isInitialized()
        {
            return initialized;
        };

        friend class Context;
    };



/*
#define OT_MODULE(MODNAME) OPENTRACKER_API static void registerModule (Context * context, void * data)

#define OT_MODULE_REGISTER_FUNC(MODNAME) \
	OPENTRACKER_API void MODNAME::registerModule( Context * context , void * data) 

#define OT_MODULE_REGISTRATION_DEFAULT(MODNAME, REGISTRATIONSTRING) \
		MODNAME * mod = new MODNAME();\
		context->addFactory (*mod);\
		context->addModule (REGISTRATIONSTRING, *mod)

#define OT_REGISTER_MODULE (MODNAME, VOIDPARAMETER) \
	Configurator::addModuleInit(#MODNAME, MODNAME::registerModule, VOIDPARAMETER)

*/


class Context;


#define OT_MODULE(MODNAME) OPENTRACKER_API void registerModule##MODNAME (Context * context, void * data)

#define OT_MODULE_REGISTER_FUNC(MODNAME) \
	OPENTRACKER_API void registerModule##MODNAME( Context * context , void * data) 

#define OT_MODULE_REGISTRATION_DEFAULT(MODNAME, REGISTRATIONSTRING) \
		MODNAME * mod = new MODNAME();\
		mod->setContext(context);\
		context->addFactory (*mod);\
		context->addModule (REGISTRATIONSTRING, *mod)

#define OT_REGISTER_MODULE(MODNAME, VOIDPARAMETER) \
	Configurator::addModuleInit(#MODNAME, registerModule##MODNAME, VOIDPARAMETER)



} // namespace ot

#endif

/* 
 * ------------------------------------------------------------
 *   End of Module.h
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
