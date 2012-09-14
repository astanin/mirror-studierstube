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
 * @author Gerhard Reitmayr
 *
 * $Id: TestModule.h 2103 2007-12-19 16:39:50Z bornik $
 * @file                                                                   */
/* ======================================================================= */

/**
 * @page module_ref Module Reference
 * @section testmodule TestModule
 * The TestModule provides and drives @ref testsource nodes that generate standard
 * events in certain intervals. It doesn't have a configuration element
 * but reserves the name 'TestConfig' for it. It does something useful as soon as
 * at least one @ref testsource node is created.
 */

#ifndef _TESTMODULE_H
#define _TESTMODULE_H

#include "../dllinclude.h"


#ifndef OT_NO_TESTMODULE_SUPPORT

#ifndef SWIG
#include <vector>
#include <queue>
#endif

#include "Module.h"
#include "NodeFactory.h"

#ifndef SWIG
#include <ace/Time_Value.h>
#endif

namespace ot {

    typedef std::vector<Node::Ptr> NodeVector;
    typedef std::pair<ACE_Time_Value, Node::Ptr> PQEntry;

    class comptimepair
    {
    public:
        comptimepair() {};
        inline bool operator()(const PQEntry  &n1, const PQEntry &n2)
        {
            return (n1.first > n2.first);
        }
    };

    typedef std::priority_queue<PQEntry, std::vector<PQEntry >, comptimepair > PriorityQueue;
                                
    /**
     * The module and factory to drive the test source nodes. It constructs
     * TestSource nodes via the NodeFactory interface and pushes events into
     * the tracker tree according to the nodes configuration.
     * @author Gerhard Reitmayr
     * @ingroup core
     */

    class OPENTRACKER_API TestModule : public ThreadModule, public NodeFactory
    {
        // Members
    protected:
        /// list of TestSource nodes in the tree
        NodeVector nodes;
        /// stop flag 
	int stop;
        ACE_Time_Value starttime;
        PriorityQueue pqueue;

        // Methods
    public:
        /** constructor method. */
        TestModule() : ThreadModule(), NodeFactory()
        {
            stop = 0;
        };
        /** Destructor method, clears nodes member. */
        virtual ~TestModule();
        /** This method is called to ruct a new Node. It compares
         * name to the TestSource element name, and if it matches
         * creates a new TestSource node.
         * @param name reference to string containing element name
         * @attributes refenrence to StringMap containing attribute values
         * @return pointer to new Node or NULL. The new Node must be
         *         allocated with new ! */
        virtual Node * createNode( const std::string& name,  const StringTable& attributes);

	/**
	 * This method is called after initialisation is finished and before the
	 * main loop is started.*/
	virtual void start();
	/**
	 * Close */
	virtual void close();

        /**
         * removes Node from module but does not destroy it
         */
        virtual void removeNode(Node * node);

        /**
         * pushes events into the tracker tree. Checks all TestSources and
         * pushes new events, if a TestSource fires.
         */
        virtual void pushEvent();

    protected:
	/**
	 * Mainloop */
        void run();
    };

	OT_MODULE(TestModule);

} // namespace ot


#endif //OT_NO_TESTMODULE_SUPPORT


#endif

/* 
 * ------------------------------------------------------------
 *   End of TestModule.h
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
