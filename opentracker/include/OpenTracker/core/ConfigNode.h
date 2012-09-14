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
/** header file for the ConfigNode class.
 *
 * @author Gerhard Reitmayr
 *
 * $Id: ConfigNode.h 1807 2007-04-12 10:35:50Z bornik $
 * @file                                                                   */
/* ======================================================================= */

#ifndef _CONFIGNODE_H
#define _CONFIGNODE_H

#include "../dllinclude.h"

#include "Node.h"
#include "StringTable.h"

/**
 * This node implements a simple node containing the elements name and
 * a string map with the attributes. It is used in the node trees passed
 * to the init section of the Modules, to add any more complicated 
 * initialization.
 * @author Gerhard Reitmayr
 * @ingroup core
 */

namespace ot {

    class OPENTRACKER_API ConfigNode : public Node
    {
        // Members

    public:   

        /**
         * constructor method
         * @param name_ element name of the config node
         * @param map_ string table with attribute values 
         */

        ConfigNode( StringTable & map_)
            : Node()
        {
            KeyIterator keys(map_);
            while( keys.hasMoreKeys())
            {
                const std::string & key = keys.nextElement();
                Node::put( key, map_.get( key ) );
            }
        }

        /**
         * returns a reference to the attribute StringTable
         * @return reference to the attribute StringTable */
        StringTable & getAttributes()
	{
            return attributes;
	}

        /**
         * returns the number of children nodes. This lets
         * any module parse its configuration tree.
         * @return number of children */
	virtual unsigned int countChildren()
	{
            return Node::countChildren();
	}

        /**
         * returns a child by index.
         * @return pointer to the desired child or NULL */
	virtual Node * getChild( unsigned int index )
	{
            return Node::getChild( index );
	}

        void pushEvent() {};
        void pullEvent() {};
#ifdef USE_LIVE
        friend class LiveContext;
#endif
    };

} // namespace ot

#endif

/* 
 * ------------------------------------------------------------
 *   End of ConfigNode.h
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
