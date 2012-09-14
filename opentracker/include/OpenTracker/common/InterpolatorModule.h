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
/** header file for TestModule module.
 *
 * @author Flo Ledermann
 *
 * $Id: InterpolatorModule.h 2103 2007-12-19 16:39:50Z bornik $
 * @file                                                                   */
/* ======================================================================= */

/**
 * @page module_ref Module Reference
 * @section interpolatormodule InterpolatorModule
 * Module for filters that interpolate tracking data between updates and therefore need
 * a module to drive the updates. Currently, only the @ref ElasticFilterNode is in this module.
 * It doesn't have a configuration element
 * but reserves the name 'InterpolatorConfig' for it.
 */

#ifndef _INTERPOLATORMODULE_H
#define _INTERPOLATORMODULE_H


#include "../OpenTracker.h"


#ifndef OT_NO_INTERPORLATORMODULE_SUPPORT


#include "../dllinclude.h"

#ifndef SWIG
#include <vector>
#endif

#include "../core/Module.h"
#include "../core/NodeFactory.h"


namespace ot {

    typedef std::vector<Node::Ptr> NodeVector;

    /**
     * Module for filters that interpolate tracking data between updates and therefore need
     * a module to drive the updates. Currently, only the @ref ElasticFilterNode is in this module.
     * @author Flo Ledermann
     * @ingroup common
     */
    class OPENTRACKER_API InterpolatorModule : public Module, public NodeFactory
    {
        // Members
    protected:
        /// list of interpolator nodes in the tree
        NodeVector nodes;
        /// current cycle count.
        int cycle;

        // Methods
    public:
        /** constructor method. */
        InterpolatorModule() : Module(), NodeFactory()
	{
            cycle = 0;
	};
        /** Destructor method, clears nodes member. */
        virtual ~InterpolatorModule();
        /** This method is called to build a new Node. 
         * @param name reference to string containing element name
         * @attributes refenrence to StringMap containing attribute values
         * @return pointer to new Node or NULL. The new Node must be
         *         allocated with new ! */
        virtual Node * createNode( const std::string& name,  
                                   const StringTable& attributes);
        /**
         * pushes events into the tracker tree. Checks all sources and
         * pushes new events.
         */
        virtual void pushEvent();
    };
	OT_MODULE(InterpolatorModule);

} // namespace ot


#endif // OT_NO_INTERPORLATORMODULE_SUPPORT

#endif //_INTERPOLATORMODULE_H

/* 
 * ------------------------------------------------------------
 *   End of InterpolatorModule.h
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
