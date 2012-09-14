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
/** source file for NodeFactoryContainer class.
 *
 * @author Gerhard Reitmayr
 *
 * $Id: NodeFactoryContainer.cxx 2103 2007-12-19 16:39:50Z bornik $
 * @file                                                                   */
/* ======================================================================= */

#include <OpenTracker/tool/disable4786.h>
#include <OpenTracker/core/NodeFactoryContainer.h>

#include <algorithm>

//using namespace std;

// Destructor method.

namespace ot {

    NodeFactoryContainer::~NodeFactoryContainer()
    {
        factories.clear();
    }

    // adds a NodeFactory to the container.

    void NodeFactoryContainer::addFactory(NodeFactory& factory)
    {
        NodeFactoryVector::iterator result = std::find( factories.begin(), factories.end(), &factory );
        if( result == factories.end())
        {
            factories.push_back( &factory );
        }
    }

    // constructs a new Node.

    Node * NodeFactoryContainer::createNode( const std::string& name, const StringTable& attributes)
    {
        Node * value = NULL;
        NodeFactoryVector::iterator it = factories.begin();
        while( it != factories.end() && value == NULL )
        {
            value = (*it)->createNode( name, attributes );
            it++;
        }
        return value;
    }

#ifdef USE_LIVE
    std::pair<Node*, NodeFactory*> NodeFactoryContainer::createNodeUsingFactory( const std::string& name, const StringTable& attributes)
    {
        Node * value = NULL;
        NodeFactory * factory;
        NodeFactoryVector::iterator it = factories.begin();
        while( it != factories.end() && value == NULL )
        {
            value = (*it)->createNode( name, attributes );
            if (value != NULL) {
                factory = (*it);
            }
            it++;
        }
        return NodeFactoryPair(value, factory);
    }
#endif
    // removes a NodeFactory from the container.

    void NodeFactoryContainer::removeFactory(NodeFactory& factory)
    {
        NodeFactoryVector::iterator result = std::find( factories.begin(), factories.end(), &factory );
        if( result != factories.end())
        {
            factories.erase( result );
        }
    }

	// remove all factories
	void NodeFactoryContainer::removeAll(){
		factories.clear();
	}

	void NodeFactoryContainer::copyFrom(const NodeFactoryContainer & other){
		factories = other.factories;
	};

} // namespace ot

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
