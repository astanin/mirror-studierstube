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
/** source file for Group Gate Node
 *
 * @author Michael Knapp
 *
 * $Id: GroupGateNode.cxx 1807 2007-04-12 10:35:50Z bornik $
 *
 * @file                                                                   */
/* ======================================================================= */

// this will remove the warning 4786
#include <OpenTracker/tool/disable4786.h>

#include <cstdlib>
#include <OpenTracker/common/GroupGateNode.h>

#include <cstdio>
#include <iostream>

#include <ace/Log_Msg.h>


#ifndef OT_NO_GROUPGATE_SUPPORT


namespace ot {

    // constructor method
    GroupGateNode::GroupGateNode(const char *name, GroupGateGroup *owner)
    {
        Number = -1;
        Name = name;
        Owner = owner;
        IsActive = false;
    } 

    GroupGateNode::~GroupGateNode()
    {
        Neighbors.clear();
    } 

    void
    GroupGateNode::setNumber(int num)
    {
        Number = num;
    }

    int
    GroupGateNode::getNumber()
    {
        return Number;
    }

    void 
    GroupGateNode::addNeighbor(const char *neighbor)
    {
        Neighbors.push_back(neighbor);
    }

    bool
    GroupGateNode::isActive()
    {
        return IsActive;
    }

    void
    GroupGateNode::activate()
    {
        IsActive = true;
    }

    void
    GroupGateNode::deactivate()
    {
        IsActive = false;
    }

    const char *
    GroupGateNode::getGroupGateName()
    {
        return (Name.c_str());
    }

    void 
    GroupGateNode::onEventGenerated(Event &event, Node &generator)
    {
        if (generator.isNodePort() == 1)
        {
            NodePort &wrap = (NodePort &)generator;
            if (wrap.getType().compare("Override") == 0)
            {
                Owner->deactivateAll();
                activate();
                Owner->setActiveGroupGate(this);
                Owner->notifyActiveGate();
                updateObservers(event);
                return;
            }
        }
        if (isActive())
        {
            activate();
            Owner->notifyActiveGate();
            updateObservers(event);
            return;
        }
        else
        {
            if (Neighbors.size() != NeighborPtrs.size())
            {
                for (NeighborsVector::iterator it = Neighbors.begin(); it != Neighbors.end(); it++)
                {
                    GroupGateNode *node = (GroupGateNode *)(Owner->getNode((* it).c_str()));
                    NeighborPtrs.push_back(node);
                }
                if (Neighbors.size() != NeighborPtrs.size())				
                    ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:ERROR: Problem with GroupGateNode\n")));
            }
            for (NeighborPtrsVector::iterator it = NeighborPtrs.begin(); it != NeighborPtrs.end(); it++)
            {
                GroupGateNode *node = (GroupGateNode *)(* it);
                if (node->isActive())
                {
                    node->deactivate();
                    activate();
                    Owner->setActiveGroupGate(this);
                    Owner->notifyActiveGate();
                    updateObservers(event);
                    return;
                }
            }
            /*
              for (NeighborsVector::iterator it = Neighbors.begin(); it != Neighbors.end(); it++)
              {
              GroupGateNode *node = (GroupGateNode *)(Owner->getNode((* it).c_str()));
              if (node->isActive())
              {
              node->deactivate();
              activate();
              Owner->setActiveGroupGate(this);
              Owner->notifyActiveGate();
              updateObservers(event);
              return;
              }
              }
            */
        }
    }

    void GroupGateNode::pushEvent()
    {
        // nothing to do
    }

    void GroupGateNode::pullEvent()
    {
        // nothing to do
    }

    void Override::pushEvent()
    {
        // nothing to do
    }

    void Override::pullEvent()
    {
        // nothing to do
    }

    
} // namespace ot {


#else
#pragma message(">>> OT_NO_GROUPGATE_SUPPORT")
#endif //OT_NO_GROUPGATE_SUPPORT

/* 
 * ------------------------------------------------------------
 *   End of GroupGateNode.cxx
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
