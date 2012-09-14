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
/** header file for EEGFilter Node.
 *
 * @author Alexander Bornik
 *
 * $Id$
 * @file                                                                   */
/* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section despfilter EEGFilter
 * EEGFilter implements a filtering of EEG samples to distinguish between
 * a given number of states based on dominant frequencies in EEG data
 * sampled at a given rate. the consume parameter allows to control
 * whether the raw data should be passed on.
 

 Example usage/defaults:

 @verbatim
 \<DESPFilter frequencies="6.25 7.25 8.00 13.00" inattname="eeg" outattname="eegout" samplerate="256" harmonics="3" consume="true">
 ... (any Event Source)
 \</DESPFilter>
 @endverbatim
*/

#ifndef _EEGFILTERNODE_H
#define _EEGPFILTERNODE_H

#include <OpenTracker/OpenTracker.h>

#ifndef SWIG
#include <deque>
#endif

#include <OpenTracker/misc/EEGSub.h>

/**
 * @author Alexander Bornik
 * @ingroup common
 */

namespace ot {

    class OPENTRACKER_API EEGFilterNode : public Node   
    {

        enum evmodetype { CALIB =0, NORMAL=1 };
    protected:
        bool init;
        std::vector<double> frequencies;
        unsigned int codelength;
        double samplerate;
        unsigned int harmonics;
        double prepause, postpause, interpause;
        double codeduration;
        double threshold;
        double hitpercentage;
        std::string inatt;
        std::string outatt;
        std::string triggeratt;
        bool consume;
        std::string logfile;
        std::vector<std::pair<EEGSub, double> > eegsubs;
        std::deque<int> evbuffer;
        unsigned int evbuffertargetsize;
        unsigned int evbuffercodestart;
        unsigned int evbufferpoststart;
        
        // internal variables
        evmodetype evmode;
        unsigned long calibcount;
        std::vector<double> calibvec;

        Event currentEvent;  ///< The event we're currently working with
        Event targetEvent;   ///< The one we will be forwarding
        int eegoutval, lasteegoutval;

    public:
        /** constructor method. It sets initial values for the treshhold and type */
        EEGFilterNode( const std::vector<double> &ifreqs, 
                       unsigned int icodelength,
                       const double &isamplerate,
                       const unsigned int iharmonics,
                       const double &iprepause,
                       const double &ipostpause,
                       const double &iinterpause,
                       const double &icodeduration,
                       const double &ithreshold,
                       const double &ihitpercentage,
                       const std::string &iinatt,
                       const std::string &ioutatt,
                       const std::string &itriggeratt,
                       bool iconsume,
                       std::string logfile);

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

#endif //_DESPFILTERNODE_H

/* 
 * ------------------------------------------------------------
 *   End of EEGFilterNode.h
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
