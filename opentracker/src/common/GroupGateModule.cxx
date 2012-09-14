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
/** source file for Group Gate Module
 *
 * @author Michael Knapp
 *
 * $Id: GroupGateModule.cxx 2103 2007-12-19 16:39:50Z bornik $
 *
 * @file                                                                    */
/* ======================================================================= */

#include <cstdlib>
#include <OpenTracker/common/GroupGateModule.h>
#include <OpenTracker/common/GroupGateNode.h>

#include <cstdio>
#include <iostream>
#include <cassert>

#ifndef _WIN32_WCE
#include <sstream>
#endif //_WIN32_WCE

#include <ace/Log_Msg.h>
#include <OpenTracker/tool/OT_ACE_Log.h>


#ifndef OT_NO_GROUPGATE_SUPPORT


namespace ot {

	OT_MODULE_REGISTER_FUNC(GroupGateModule){		
		OT_MODULE_REGISTRATION_DEFAULT(GroupGateModule, "GroupGateConfig" ); 
	}

    GroupGateGroup::GroupGateGroup(const char *name)
    {
        Name = name;
        ActiveGate = 0;
        ActiveGroupGate = 0;
    }

    GroupGateGroup::~GroupGateGroup()
    {
        Gates.clear();
    }

    Node *
    GroupGateGroup::addNode(const char *name)
    {
        GroupGateNode *node = new GroupGateNode(name, this);
        Gates.insert(NodeMap::value_type(name, node));
        return node;
    }

    Node *
    GroupGateGroup::getNode(const char *name)
    {
        NodeMap::iterator find = Gates.find(name);
        if (find != Gates.end()) return (GroupGateNode *)((* find).second);
        return 0;
    }

    Node * 
    GroupGateGroup::setActiveGate(bool enable)
    {
        if (ActiveGate != 0) delete ActiveGate;
        ActiveGate = 0;
        if (enable) ActiveGate = new ActiveGateNode();
        return ActiveGate;
    }

    void 
    GroupGateGroup::deactivateAll()
    {
        for (NodeMap::iterator it = Gates.begin(); it != Gates.end(); it++)
        {
            ((GroupGateNode *)((* it).second))->deactivate();
        }
    }

    void 
    GroupGateGroup::setActiveGroupGate(Node *activegroupgate)
    {
        ActiveGroupGate = activegroupgate;
    }

    void 
    GroupGateGroup::notifyActiveGate()
    {
        if ((ActiveGate != 0) && (ActiveGroupGate != 0))
        {
            GroupGateNode *node = (GroupGateNode *)ActiveGroupGate;
            ((ActiveGateNode *)ActiveGate)->pushEvent(node->getNumber());
        }
    }

    //-----------------------------------------------------------------------------

    // Destructor method
    GroupGateModule::~GroupGateModule()
    {
        Groups.clear();
        NameList.clear();
    }

    // This method is called to construct a new Node.
    Node *
    GroupGateModule::createNode(const std::string &name, const StringTable &attributes)
    {
        if(name.compare("GroupGate") == 0)
        {
            const char *cname = attributes.get("DEF").c_str();
            const char *cgroup = attributes.get("group").c_str();
            const char *cneighbors = attributes.get("neighbors").c_str();

            GroupGateGroup *group = 0;
            GroupMap::iterator find = Groups.find(cgroup);
            if (find == Groups.end()) 
            {
                group = new GroupGateGroup(cgroup);
                Groups.insert(GroupMap::value_type(cgroup, group));
            }
            else
            {
                group = (* find).second;
            }
            GroupGateNode *node = (GroupGateNode *)(group->addNode(cname));    
            node->setNumber(NameList.size());
            NameList.push_back(cname);

#ifdef _WIN32_WCE
            // groupgate node not yet supported under wince
            assert(false);
#else
#ifndef WIN32
            std::stringstream neighborstream;
            neighborstream << cneighbors;
            while (true)
            {
                std::string neighbor;
                neighborstream >> neighbor;
                if (neighbor == "") break;
                node->addNeighbor(neighbor.c_str());
            }
#else
            //workaround for broken VS8 stringstream implementation
            std::ostringstream oneighborstream;
            oneighborstream << cneighbors;
            std::istringstream ineighborstream(oneighborstream.str());

            while (true)
            {
                std::string neighbor;
                ineighborstream >> neighbor;
                if (neighbor == "") break;
                node->addNeighbor(neighbor.c_str());
            }

#endif
            return node;
#endif //_WIN32_WCE
        }
        else if (name.compare("ActiveGate") == 0)
        {
            Node *node = 0;
            const char *cgroup = attributes.get("group").c_str();
            GroupMap::iterator find = Groups.find(cgroup);
            if (find != Groups.end()) 
            {
                logPrintE("*** ActiveGroup: %s\n", cgroup);
                node =  (* find).second->setActiveGate(true);
            }
            return node;
        }
        else if (name.compare("Override") == 0)
        {
            Override *node = new Override();
            return node;
        }
        return 0;
    }

    const char *
    GroupGateModule::getGroupGateName(int groupgatenum)
    {
        return (NameList[groupgatenum].c_str());
    }

    void 
    GroupGateModule::activateGroupGate(const char *groupname, const char *groupgatename)
    {
        GroupMap::iterator find = Groups.find(groupname);
        if (find == Groups.end()) return;
        GroupGateGroup *group = (*find).second;
        if (group == 0) return;
        GroupGateNode *node = (GroupGateNode *)(group->getNode(groupgatename));
        if (node == 0) return;
        group->deactivateAll();
        node->activate();
    }

} // namespace ot


#else
#pragma message(">>> OT_NO_GROUPGATE_SUPPORT")
#endif // OT_NO_GROUPGATE_SUPPORT


/* 
 * ------------------------------------------------------------
 *   End of GroupGateModule.cxx
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
