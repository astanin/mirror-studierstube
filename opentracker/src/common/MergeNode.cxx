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
/** source file for Merge Node.
 *
 * @author Gerhard Reitmayr, Jochen von Spiczak
 *
 * $Id: MergeNode.cxx 1853 2007-05-24 17:13:25Z bornik $
 * @file                                                                   */
/* ======================================================================= */

// this will remove the warning 4786
#include <OpenTracker/tool/disable4786.h>

#include <OpenTracker/common/MergeNode.h>

#include <OpenTracker/core/OtLogger.h>

#ifndef OT_NO_MERGENODE_SUPPORT


namespace ot {

    // generates a new data item upon receiving an event

    void MergeNode::onEventGenerated( Event& event, Node & generatorNode)
    {
        //logPrintI("MergeNode::onEventGenerated\n");

        if (generatorNode.isNodePort() != 1){ // this should never happen
            logPrintE("Is generator happened\n");
            return;
        }
        NodePort &generator = (NodePort&)generatorNode;

        // merge attribute
        if (generator.getType().compare("MergeAttribute") == 0)
        {
            std::string attrName, typeName, value;
            try
            {
                attrName = generator.get("attributeName");
                typeName = event.getAttributeTypeName(attrName);
                value = event.getAttributeValueString(attrName);
                localEvent.setAttribute(typeName, attrName, value);
            }
            catch (std::invalid_argument)
            {
                // two attributes of the same name but of different types are present!
                //logPrintE("MergeNode::Caught invalid argument: %s, %s, %s \n",
                //          attrName.c_str(), typeName.c_str(), value.c_str());
                return;
            }
        }

        // merge default
        else if (generator.getType().compare("MergeDefault") == 0)
        {
            // copy all attributes but those handled by other MergeAttribute nodes
            Event temp = localEvent;
            localEvent = event;
            localEvent.time = temp.time;

            unsigned int i = 0;
            NodePort *port = getPort("MergeAttribute", i++);
            while (port != NULL)
            {
                std::string attrName = port->get("attributeName");
                try
                {
                    // copy back value of separately handeled attribute
                    std::string typeName = temp.getAttributeTypeName(attrName);
                    std::string value = temp.getAttributeValueString(attrName);
                    localEvent.setAttribute(typeName, attrName, value);
                }
                catch (std::invalid_argument)
                {
                    // attrName was not in localEvent before, thus delete it
                    localEvent.delAttribute(attrName);
                }
                port = getPort("MergeAttribute", i++);
            }
        }

        // merge time
        if (generator.getType().compare("MergeTime") == 0)
            localEvent.time = event.time;
        // if there is no special time node, always propagate time
        else if ((getPort("MergeTime") == NULL) || (getPort("MergeTime")->countChildren() == 0))
            localEvent.time = event.time;


        // age confidence of each input
        std::map<NodePort*, float>::iterator it;
        for (it = confidenceMap.begin(); it != confidenceMap.end(); it++)
            (*it).second *= agingFactor;
        // set confidence of generator
        confidenceMap[&generator] = event.getConfidence();
        // calculate average confidence depending on confCalculation flag
        float &localConf = localEvent.getConfidence();
        switch (confCalculation)
        {
            case MIN:
                localConf = 1;
                for (it = confidenceMap.begin(); it != confidenceMap.end(); it++)
                    localConf = (localConf < (*it).second) ? localConf : (*it).second;
                break;
            case MAX:
                localConf = 0;
                for (it = confidenceMap.begin(); it != confidenceMap.end(); it++)
                    localConf = (localConf > (*it).second) ? localConf : (*it).second;
                break;
            case MULTIPLY:
                localConf = 1;
                for (it = confidenceMap.begin(); it != confidenceMap.end(); it++)
                    localConf *= (*it).second;
                break;
        }


        // merge trigger
        if (generator.getType().compare("MergeTrigger") == 0)
        {
            updateObservers(localEvent);
        }
        // if there is no special trigger node, always propagate events
        else if ( (getPort("MergeTrigger") == NULL) || 
                  (getPort("MergeTrigger")->countChildren() == 0) )
        {
            updateObservers(localEvent);
        }
    }

    void MergeNode::pushEvent()
    {
        //logPrintI("MergeNode::pushEvent\n");
        // nothing to do
    }

    void MergeNode::pullEvent()
    {
        // nothing to do
    }

} // namespace ot


#else
#pragma message(">>> OT_NO_MERGENODE_SUPPORT")
#endif // OT_NO_MERGENODE_SUPPORT


/* 
 * ------------------------------------------------------------
 *   End of MergeNode.h
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
