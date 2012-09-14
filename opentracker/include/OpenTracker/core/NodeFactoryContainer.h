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
/** header file for NodeFactoryContainer class.
 *
 * @author Gerhard Reitmayr
 *
 * $Id: NodeFactoryContainer.h 2110 2008-01-16 10:06:56Z bornik $
 * @file                                                                   */
/* ======================================================================= */

#ifndef _NODEFACTORYCONTAINER_H
#define _NODEFACTORYCONTAINER_H

#include "../dllinclude.h"

#include "NodeFactory.h"

namespace ot {

    /**
     * used to keep a list of NodeFactories.
     */
    typedef std::vector<NodeFactory*> NodeFactoryVector;
    typedef std::pair<Node*, NodeFactory*> NodeFactoryPair;

    /**
     * An implementation of NodeFactory that contains several NodeFactories and
     * acts like the one providing all node types of the different factories
     * together. When createNode is called it loops through the known
     * NodeFactories and calls createNode on them until it receives a non NULL
     * result. It then returns this node.
     * @author Gerhard Reitmayr
     * @ingroup core
     */

    class OPENTRACKER_API NodeFactoryContainer : public NodeFactory
    {

    protected:
        /// the list of known NodeFactories
        NodeFactoryVector factories;

    public:
        /** Destructor clears the factories list. */
        virtual ~NodeFactoryContainer();
        /**
         * adds a NodeFactory to the container making nodes provided by this
         * factory accessible.
         * @param factory reference to the NodeFactory object */
        void addFactory(NodeFactory& factory);
        /**
         * removes a NodeFactory from the container.
         * @param factory reference to the NodeFactory object */
        void removeFactory(NodeFactory& factory);
        /**
         * implements abstract method from NodeFactory. it loops through the known
         * NodeFactories and calls createNode on them until it receives a non NULL
         * result. It then returns this node.
         * @param name reference to string containing element name
         * @param attributes refenrence to StringTable containing attribute values
         * @return pointer to new Node or NULL. The new Node must be
         *         allocated with new ! */
        virtual Node * createNode( const std::string& name,  const StringTable& attributes);
#ifdef USE_LIVE
        NodeFactoryPair createNodeUsingFactory( const std::string& name,  const StringTable& attributes);
#endif
		void removeAll();
		void copyFrom(const NodeFactoryContainer & other);
    };

} // namespace ot

#endif

/* 
 * ------------------------------------------------------------
 *   End of NodeFactoryContainer.h
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
