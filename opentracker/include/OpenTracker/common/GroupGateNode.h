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
/** header file for Group Gate Node
 *
 * @author Michael Knapp
 *
 * $Id: GroupGateNode.h 1971 2007-08-21 09:48:51Z bornik $
 *
 * @file                                                                   */
/* ======================================================================= */
/**
 * @page Nodes Node Reference
 * @section groupgate GroupGate
 * 
 * The GroupGate element is a node within a adjacency graph of other nodes
 * within a group. Per default all nodes are deactived. The Override element
 * has to be used to activate at least one node. This node is managed by a 
 * dedicated @ref groupgatemodule.
 *
 * A GroupGate only generates an event if the node itself or its neighbors
 * are active.
 *
 * The @c GroupGate element has the following attributes :
 *
 * @li @c DEF defines the name of the node. This name must not be used more 
 *        than once within a group
 * @li @c group defines the group to which the node is added
 *        if the group does not exist, a new one is created
 * @li @c neighbors defines the neighbors of the node within a group
 * 
 * The @c Override element is used to activate a node within a group. See
 * also the @ref activegate element that outputs the currently active
 * GroupGate of a given group. 
 *
 * example: elements for a cyclic three noded graph looks like this 
 * @verbatim
 <GroupGate DEF="Node1" group="CyclicGraph" neighbors="Node3">
 <Override>
 <...incomming events...>
 </Override>
 <...incomming events...>
 </GroupGate>
 <GroupGate DEF="Node2" group="CyclicGraph" neighbors="Node1">
 <...incomming events...>
 </GroupGate>
 <GroupGate DEF="Node3" group="CyclicGraph" neighbors="Node2">
 <...incomming events...>
 </GroupGate>
 <ActiveGate group="CyclicGraph"/>@endverbatim
 *
 *
 * @section activegate ActiveGate
 *
 * The @c ActiveGate node is used to determine the currently active @ref groupgate
 * of a group.
 * It returns a numeric value in the button field of the event structure
 * The value is used to call the getGroupGateName() method of @ref groupgatemodule
 * to return the name of the @ref groupgate. See the @ref groupgate element for an
 * example of the ActiveGate element.
 * It has the following attributes :
 *
 * @li @c group defines the group of which the active node should be traced.
 */

#ifndef _GROUPGATENODE_H
#define _GROUPGATENODE_H

#include "../OpenTracker.h"
#include "GroupGateModule.h"

#ifndef SWIG
#include <string>
#include <vector>
#endif


#ifndef OT_NO_GROUPGATE_SUPPORT


namespace ot {

    typedef std::vector<std::string> NeighborsVector;
    typedef std::vector<Node *> NeighborPtrsVector;

    /**
     * A GroupGateNode is an EventGenerator node that ... TO DO
     * @author Michael Knapp
     * @ingroup common
     */
    class OPENTRACKER_API GroupGateNode : public Node   
    {
    private:
        int Number;
        GroupGateGroup *Owner;
        std::string Name;
        NeighborsVector Neighbors;
        NeighborPtrsVector NeighborPtrs;
        bool IsActive;

    protected:
   
        /** constructor method. It sets initial values for the timeout. */
        GroupGateNode(const char *name, GroupGateGroup *owner);
        ~GroupGateNode();

        void setNumber(int num);
        int getNumber();
        void addNeighbor(const char *neighbor);
        bool isActive();
        void activate();
        void deactivate();
        const char *getGroupGateName();

    public:
        virtual int isEventGenerator()
	{
            return 1;
	}

        virtual void onEventGenerated(Event &event, Node &generator);                                
        void pushEvent();
        void pullEvent();

        friend class GroupGateModule;
        friend class GroupGateGroup;
    };


    class OPENTRACKER_API Override : public Node
    {
    protected:
        Override() : Node() 
	{ 
	}

    public:

        virtual int isNodePort()
	{
            return 1;
	}

        virtual void onEventGenerated(Event &event, Node &generator)
	{
            updateObservers(event);
	}

        void pushEvent();
        void pullEvent();

        friend class GroupGateModule;
    };

    /**
     * An ActiveGateNode is an EventGenerator node that ... TO DO
     * @author Michael Knapp
     * @ingroup common
     */
    class OPENTRACKER_API ActiveGateNode : public Node   
    {
    private:
        Event event;

    protected:
   
        ActiveGateNode() : Node()
	{
	}
  
    public:

        virtual int isEventGenerator()
	{
            return 1;
	}

        void pushEvent(unsigned short groupgatenum)
	{
            event.getButton() = groupgatenum;
            event.timeStamp();
            updateObservers(event);
	}

        void pushEvent() { };
        void pullEvent() { };

        friend class GroupGateModule;
        friend class GroupGateGroup;
    };

} // namespace ot


#endif // OT_NO_GROUPGATE_SUPPORT

#endif


/* 
 * ------------------------------------------------------------
 *   End of GroupGateNode.h
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
