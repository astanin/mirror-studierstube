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
/** header file for DynamicVirtualTransformation Node.
 *
 * @author Markus Sareika
 *
 * @todo think about using other node types than only the EventGenerator
 * as the base child. What semantics do make sense ??
 *
 * $Id: DynamicVirtualTransformation.h 1547 2006-10-25 10:33:10Z veas $
 * @file                                                                   */
/* ======================================================================= */

/**
 * @page transform_nodes Transformation Node Reference
 * This page describes the nodes implementing geometric transformations 
 * available in OpenTracker. The currently implemented transformations are
 * a specific subset of all affine transformations possible.
 *
 * All transformations receive the event from a single child node, transform
 * it and pass it on. To work with all interface types each transformation has
 * three different configuration elements whose outputs and inputs are typed
 * with the same interface. The configuration elements are distinguished by
 * the following prefixes :
 * @li @c Event - the child node and the output interface are of type EventGenerator
 * @li @c Queue - the child node and the output interface are of type EventQueue
 * @li @c Time  - the child node and the output interface are of type Timedependend
 * The transformation nodes act transparently and actually exhibit the same type
 * as their child node.
 * 
 * The following sections describe the different transformation nodes in detail.
 * 
 * @section dynamictransform DynamicVirtualTransformation
 *
 * This section describes the DynamicVirtualTransformation node and configuration element. 
 * A DynamicVirtualTransformation transforms the data child's event value by the base
 * child's event value. The base childs position and orientation define
 * the affine base in which the data childs values are interpreted. Then the data childs values
 * are transformed to world coordinates. The basic name of the configuration element
 * is @c DynamicVirtualTransform , by prepending the prefixes above, one receives the
 * actual element names. The @c TransformBase child must always be an EventGenerator.
 *
 * The confidence value of the result event is the product of the confidence values
 * of the two input events. Button event is only passed from the child, never from the 
 * @c TransformBase child. If you want to merge the button event you must use an additional
 * @ref buttonopnode to do that. The timestamp is always of the last event that triggers an
 * output (see @c baseevent).
 *
 * The elements may have the following attribute.
 *
 * @li @c baseevent (true|false) flag to define whether a change in the base generates an event.
 *                  If set to true, a new event is also generated whenever the @c TransformBase 
 *                  child passes an event on.
 * 
 * An example element looks like this :
 * @verbatim
 <EventDynamicVirtualTransform baseevent="true">
 <Any EventGenerator element type>
 <TransformBase>
 <Any EventGenerator element type>
 </TransformBase>
 </EventDynamicVirtualTransform>@endverbatim
*/

#ifndef _DYNAMICVIRTUALTRANSFORMATION_H
#define _DYNAMICVIRTUALTRANSFORMATION_H

#include "VirtualTransformation.h"

/**
 * The DynamicVirtualTransformation class implements a transformation that puts
 * the data childs data into the affine space defined by the base childs
 * data. If no data was received by the child sofar, it uses the
 * identity transformation. It builds upon the StaticTransformation by
 * setting the members of the StaticTransformation to the base childs
 * data. 
 * The confidence value of the result event is the product of the confidence values
 * of the two input events.
 * @author Gerhard Reitmayr
 * @ingroup common
 */

namespace ot {

    class OPENTRACKER_API DynamicVirtualTransformation : public VirtualTransformation
    {
    protected:

        /** stores the original event data to compute new values
         * on base changes.*/
        Event store;
        /** flag, whether a change in the base should generate an event or not */
        int baseEvent;

        /** constructor method. It sets default values on the
         * underlying StaticTransformation. */
        DynamicVirtualTransformation( int baseEvent_ = 1, bool usePos_ = true, bool useOrient_ = true);

    public:
    
        /**
         * this method is called by the EventGenerator to update it's observers.
         * This class computes a transformed event, stores it in its local variable
         * and notifies its observers in turn, propagating the change.
         * If the EventGenerator equals the baseChild, its data is stored in the
         * underlying StaticTransformation fields, to change the transformation itself.
         */
        virtual void onEventGenerated( Event& event, Node& generator);

        friend class CommonNodeFactory;

    };

} // namespace ot

#endif


/* 
 * ------------------------------------------------------------
 *   End of DynamicVirtualTransformation.h
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
