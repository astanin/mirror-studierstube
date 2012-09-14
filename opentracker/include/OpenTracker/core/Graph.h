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
/** The header file for the basic Graph class.
 *
 * @author Eduardo Veas
 *
 *
 * @file                                                                   */
/* ======================================================================= */

#ifndef OT_CORE_GRAPH_HH_INCLUDED
#define OT_CORE_GRAPH_HH_INCLUDED

#include "../dllinclude.h"

#ifndef SWIG
#include <vector>
#include <map>
#include <string>
#endif
#include <OpenTracker/misc/Cptr.h>
#include "IRefCounted.h"

namespace ot{
    class Node;

    class OPENTRACKER_API Graph{    
    protected:
        //  typedef std::map<std::string, Node::Ptr> NodeVector;
        typedef std::vector<Node::Ptr> NodeVector;

        NodeVector nodes;
#ifndef USE_LIVE
        OT_DECLARE_IREFCOUNTED;
        typedef Cptr<Graph> Ptr;
#endif
        bool changed;

    public:
        Graph();
        virtual ~Graph();
  
        void addNode(Node *);
        void remNode(Node *);
  
        void connectNodes(Node * parent, Node * child);
        void disconnectNodes(Node * parent, Node * child);
        void touch();
        
        unsigned int countNodes();
        Node * getNode(unsigned int idx);

        Node * findNode(const std::string & key, const std::string & val);

        void pushEvents();
        void pullEvents();        

        void writeGraph(const char * filename);
    };

    OPENTRACKER_API bool traversalCompare(const Node::Ptr &ptr1, const Node::Ptr &ptr2);

};

#endif // OT_CORE_GRAPH_HH_INCLUDED

/* 
 * ------------------------------------------------------------
 *   End of Graph.h
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
