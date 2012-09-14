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
/** header file for TimestampGeneratorModule module.
 *
 * @author Mathis Csisinko
 *
 * $Id: TimestampGeneratorModule.h 2103 2007-12-19 16:39:50Z bornik $
 * @file                                                                   */
/* ======================================================================= */

/**
 * @page module_ref Module Reference
 * @section timestampgeneratormodule TimestampGeneratorModule
 * TimestampGeneratorModule drives @ref timestampgenerator nodes, which are
 * able to regenerate events periodically after a configurable timeout to
 * perform time-driven operations even with push-driven event generator
 * nodes. It doesn't have a configuration element but reserves the @c
 * TimestampGeneratorConfig for it.
 */

#ifndef _TIMESTAMPGENERATORMODULE_H
#define _TIMESTAMPGENERATORMODULE_H

#include <OpenTracker/OpenTracker.h>


#ifndef OT_NO_TIMESTAMPGENERATOR_SUPPORT

namespace ot {

    typedef std::vector<Node::Ptr> NodeVector;

	/**
	 * TimestampGeneratorModule is the factory for TimestampGeneratorNode nodes.
	 * 
	 * @ingroup common
	 */

    class OPENTRACKER_API TimestampGeneratorModule: public Module,public NodeFactory
    {
        // Methods
    public:
        /** constructor method. */
        TimestampGeneratorModule(): Module(),NodeFactory()
        {}
        /** This method is called to construct a new Node. It compares
         * name to the TimestampGenerator element name, and if it matches
         * creates a new TimestampGeneratorNode node.
         * @param name reference to string containing element name
         * @attributes refenrence to StringMap containing attribute values
         * @return pointer to new Node or NULL. The new Node must be
         *         allocated with new ! */
        virtual Node* createNode(const std::string& name, 
				 const StringTable& attributes);
    };
              
	OT_MODULE(TimestampGeneratorModule);

} // namespace ot


#endif //OT_NO_TIMESTAMPGENERATOR_SUPPORT


#endif

