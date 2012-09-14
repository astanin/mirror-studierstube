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
/** header file for Merge Node.
 *
 * @author Gerhard Reitmayr, Jochen von Spiczak
 *
 * $Id: MergeNode.h 1971 2007-08-21 09:48:51Z bornik $
 * @file                                                                   */
/* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section merge Merge
 * A Merge node is an EventGenerator node that listens to several other
 * EventGenerator nodes and merges data from these. It has several inputs
 * that are marked with different wrapper tags. It stores an internal event
 * and updates it with parts of the data depending on the type of input. Then
 * it generates an event of its own. Timestamps are treated specially. If no
 * child node is connected to the MergeTime input, then the timestamp of the
 * new event equals the timestamp of the last received event. Otherwise it
 * behaves like other inputs. The following list shows the possible
 * inputs :
 *
 * @li @c MergeAttribute only attributes with the name defined by the attributeName tag
 *        is taken from events received from children of this wrapper element
 * @li @c MergeTime only the time stamp data is taken
 * @li @c MergeTrigger if this element is present, the local event will only be propageted
 *        to parent nodes after an event from the trigger was received
 * @li @c MergeDefault any data that is not set by a child of another
 *        wrapper element is used.
 *
 * The MergeNode also provides the possibility to adjust the confidence value of the
 * resulting event depending on how regularly events are coming in from the different
 * inputs (MergeAttribut elements). If an agingFactor is defined for the MergeNode, the
 * confidence of each input will be multiplied by this factor whenever a new event comes
 * in (except for the input that generated the new event of course). The overall confidence
 * of the resulting event is then calculated depending on the confidenceCalculation flag.
 * If this flag is 'min', the resulting confidence is the minimum of all confidences. If
 * the flag is 'max', it is the maximum. If the flag is set to 'multiply', the
 * overall confidence is calculated as the product of all inputs' confidence values. Thus,
 * the confidence of the resulting event will decrease, if events are just coming from one
 * sole generator or a subset of generators all the time.
 *
 * An example element using all wrapper elements looks like this :
 * @verbatim
 <Merge agingFactor="0.8" confidenceCalculation="multiply">
 <MergeDefault>
 <Exactly one EventGenerator element type>
 </MergeDefault>
 <MergeTime>
 <One or more of any EventGenerator element type>
 </MergeTime>
 <MergeTrigger>
 <One or more of any EventGenerator element type>
 </MergeTrigger>
 <MergeAttribute attributeName="someAttribute">
 <One or more of any EventGenerator element type>
 </MergeAttribute>
 </Merge>@endverbatim
*/

#ifndef _MERGENODE_H
#define _MERGENODE_H

#include "../OpenTracker.h"

#ifndef SWIG
#include <map>
#endif

/**
 * A MergeNode is an EventGenerator node that observes several marked input
 * nodes and merges the data provided by the input nodes. The mark decides
 * which part of the data is used in the merge. It is implemented using
 * wrapper nodes and apropriate DTD definitions.
 * @author Gerhard Reitmayr, Jochen von Spiczak
 * @ingroup common
 */


#ifndef OT_NO_MERGENODE_SUPPORT


namespace ot {

    class OPENTRACKER_API MergeNode : public Node
    {
    public:
        /// flags to define how to calculate the overall confidence of the resulting event
        enum CONF_CALCULATION { MIN = 0, MAX, MULTIPLY };

    protected:
        /// Event variable to put transformed event into
        Event localEvent;
        /// factor by that not updated inputs are aging whenever a new event is received from another input
        float agingFactor;
        /// flag to define how to calculate the overall confidence of the resulting event
        CONF_CALCULATION confCalculation;
        /// confidence map stores aging confidence of each port
        std::map<NodePort*, float> confidenceMap;

        /** constructor method
         */
        MergeNode(float agingFactor_, CONF_CALCULATION confCalculation_)
            : confCalculation(confCalculation_)
	{
            agingFactor = agingFactor_;
            if (agingFactor > 1)
                agingFactor = 1;
            else if (agingFactor < 0)
                agingFactor = 0;
	}

    public: 

        /** tests for EventGenerator interface being present and returns
         * 1, if present.
         * @return always 1 */
        virtual int isEventGenerator()
	{
            return 1;
	}

        /**
         * This method is called by any child node. It updates the local
         * event with the right part of the passed event and notifies
         * it's parent and references in turn.
         * @param event the event value passed
         * @param generator the node generating the event
         */
        virtual void onEventGenerated( Event& event, Node & generator);

        void pushEvent();
        void pullEvent();

        friend class CommonNodeFactory;
    };

} // namespace ot


#endif // OT_NO_MERGENODE_SUPPORT


#endif

/* 
 * ------------------------------------------------------------
 *   End of MergeNode.cxx
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
