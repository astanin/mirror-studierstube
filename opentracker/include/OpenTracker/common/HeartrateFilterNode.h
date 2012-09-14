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
/** header file for HeartrateFilter Node.
 *
 * @author Alexander Bornik
 *
 * $Id?$
 * @file                                                                   */
/* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section heartratefilter HeartrateFilter
 * HeartrateFilter implements a filter that can be used to extract the
   heart rate and provide a trigger for the starting heartbeat

 Example usage/defaults:

 @verbatim
 \<HeartrateFilter threshold onDuration="4" offDuration="100" tooMuch="0.62" samplefactor="4" datafield="ecgdata" consume="true">
 ... (any Event Source)
 \</HeartrateFilter>
 @endverbatim
*/

#ifndef _HEARTRATEFILTERNODE_H
#define _HEARTRATEFILTERNODE_H

#include "../OpenTracker.h"

/**
 * @author Alexander Bornik
 * @ingroup common
 */


namespace ot {

    class OPENTRACKER_API HeartrateFilterNode : public Node   
    {

    protected:
        bool init;

        // internal variables
        Event currentEvent;  ///< The event we're currently working with
        Event targetEvent;   ///< The one we will be forwarding
        Event lastEvent;     ///< The last one we worked with

        /// the event attribute to use
        std::string attrname;
        /// characteristica describing a beat
        double threshold;
        double toomuch;
        unsigned int onDuration;    
        unsigned int offDuration;   
        /// sample rate conversion factor for other data, if the rate 
        /// delivered by the device is too high
        unsigned int samplefactor;  
        /// whether or not to forward the other attributes
        bool consume; 

        /// internal variables
        int over;
        int downcnt;
        unsigned long samplecount;
        double heartrate[3];
        double outheartrate;
        int actheartrate;
        bool hadstrangerate;

    public:
        /** constructor method. It sets initial values for the treshhold and type */
        HeartrateFilterNode(std::string attrname_,
                            double threshold_ = 0.03, 
                            double toomuch_ = 0.2 , 
                            unsigned int onDuration_ = 4, 
                            unsigned int offDuration_ = 100, 
                            unsigned int samplefactor_ = 1,
                            bool consume_ = false);

        /** tests for EventGenerator interface being present and returns
         * 1, if present.
         * @return always 1 */
        virtual int isEventGenerator()
        {
            return 1;
        }

        /**
         * This method is called by any child node. 
	     * @param event the event value passed
	     * @param generator the node generating the event
         */
        virtual void onEventGenerated( Event& event, Node & generator);                                
        void pushEvent();
        void pullEvent();
    };

} // namespace ot



#endif //_HEARTRATEFILTERNODE_H

/* 
 * ------------------------------------------------------------
 *   End of HeartrateFilterNode.h
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
