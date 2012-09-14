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
/** header file for Group Gate Module
 *
 * @author Michael Knapp
 * 
 * $Id: GroupGateModule.h 2103 2007-12-19 16:39:50Z bornik $
 *
 * @file                                                                   */
/* ======================================================================= */
/**
 * @page module_ref Module Reference
 * @section groupgatemodule GroupGateModule 
 *
 * This module provides and drives @ref groupgate nodes that 
 * generate standard events. The configuration element is called @c GroupGateConfig 
 * and has no attributes yet.
 *
 * An example configuration element looks like this :
 * @verbatim
 <GroupGateConfig/>@endverbatim
 *
 */

#ifndef _GROUPGATEMODULE_H
#define _GROUPGATEMODULE_H

#include "../OpenTracker.h"

#ifndef SWIG
#include <string>
#include <vector>
#endif

#ifndef OT_NO_GROUPGATE_SUPPORT


namespace ot {

    class GroupGateModule;

    typedef std::vector<std::string> StringVec;
    typedef std::map<std::string, Node *> NodeMap;

    class OPENTRACKER_API GroupGateGroup
    {
    protected:
        std::string Name;
        NodeMap Gates;
        Node *ActiveGate;
        Node *ActiveGroupGate;

    public:
        GroupGateGroup(const char *name);
        ~GroupGateGroup();

        Node *addNode(const char *name);
        Node *getNode(const char *name);
        Node *setActiveGate(bool enable);
    
        void deactivateAll();
        void setActiveGroupGate(Node *activegroupgate);
        void notifyActiveGate();
    };

    typedef std::map<std::string, GroupGateGroup *> GroupMap;

    /**
     * The module and factory to drive the test source nodes. It constructs
     * GroupGate nodes via the NodeFactory interface and pushes events into
     * the tracker tree according to the nodes configuration.
     * @author Michael Knapp
     * @ingroup common
     */
    class OPENTRACKER_API GroupGateModule : public Module, public NodeFactory
    {
    protected:
        GroupMap Groups;
        StringVec NameList;

    public:
        GroupGateModule() : Module(), NodeFactory() { };
        virtual ~GroupGateModule();
        virtual Node *createNode(const std::string &name, 
                                 const StringTable &attributes);

        const char *getGroupGateName(int groupgatenum);
        void activateGroupGate(const char *groupname, const char *groupgatename);
    };
	OT_MODULE(GroupGateModule);

} // namespace ot {


#endif //OT_NO_GROUPGATE_SUPPORT


#endif

/* 
 * ------------------------------------------------------------
 *   End of GroupGateModule.h
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
