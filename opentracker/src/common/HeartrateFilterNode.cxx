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
/** source file for HeartrateFilter node.
 *
 * @author Alexander Bornik
 *
 * $Id?$
 * @file                                                                   */
/* ======================================================================= */

// this will remove the warning 4786
#include <OpenTracker/tool/disable4786.h>

#include <OpenTracker/common/HeartrateFilterNode.h>


namespace ot {

    // constructor method
    HeartrateFilterNode::HeartrateFilterNode(std::string attrname_,
                                             double threshold_, 
                                             double toomuch_, 
                                             unsigned int onDuration_, 
                                             unsigned int offDuration_, 
                                             unsigned int samplefactor_,
                                             bool consume_)
    {
        attrname = attrname_;
        threshold = threshold_;
        toomuch = toomuch_;
        onDuration = onDuration_;
        offDuration = offDuration_;
        samplefactor = samplefactor_;
        consume = consume_;

        logPrintI("HeartrateFilterNode: ths: %lf, tmch: %lf, onD: %u, offD: %u, sf: %u, cs: %d\n", 
                  threshold, toomuch, onDuration, offDuration, 
                  samplefactor, consume);

        over = 0;
        downcnt = 0;
        samplecount = 0;
        actheartrate = 0;
        heartrate [0] = heartrate[1] = heartrate[2] = 0.0;
        outheartrate = 0.0;
        hadstrangerate = false;

        init = false;
    } 

    // Applies the filter to incoming events
    void HeartrateFilterNode::onEventGenerated( Event& event, Node & generator )
    {
        if (!init)
        {
            // Makes an initial copy
            //lastEvent.getPosition()=event.getPosition();
            //lastEvent.getOrientation()=event.getOrientation();
            //lastEvent.getConfidence()=event.getConfidence();
            //lastEvent.time=event.time;

            // Sets the given filtering val
            //poseDesp.setAlpha(filterPos,filterOri);
            init = true;
        }

        if (consume)
        {            
            targetEvent = Event();
            targetEvent.time = event.time;
            targetEvent.getConfidence() = event.getConfidence();
        }
        else
        {
            /// copy old event
            targetEvent = event;
        }

        bool beat = false;

        if (event.hasAttribute(attrname))
        {

            double attval=0.0;
            if (event.getAttributeTypeName(attrname) == "double")
                attval = event.getAttribute(attrname, (double)0);
            else if (event.getAttributeTypeName(attrname) == "float")
                attval = event.getAttribute(attrname, (float)0);
            else if (event.getAttributeTypeName(attrname) == "short")
                attval = event.getAttribute(attrname, (short)0);



            if (downcnt == 0)
            {
                if ((attval > threshold) && (attval < toomuch))
                {
                    
                    ++over;
                }
                else
                {
                    over = 0;
                }
                if (over >= (int) onDuration)
                {
                    downcnt = offDuration;
                    beat = true;
                    over = 0;

                    // stabilize heart rate
                    heartrate[(++actheartrate)%3] = OSUtils::currentTime();
                    if ( heartrate[actheartrate%3] - 
                         heartrate[(actheartrate+2)%3] < 6000.0)
                    {
                        double hd1 = heartrate[actheartrate%3] - 
                            heartrate[(actheartrate+2)%3] ;
                        double hd2 = heartrate[(actheartrate+2)%3] - 
                            heartrate[(actheartrate+1)%3] ;
                        
                        if ((hd1 > 2.0*hd2 || hd1 < 0.5*hd2) && !hadstrangerate)
                        {
                            outheartrate = 60000.0/hd2;
                            hadstrangerate=true;
                        }   
                        else
                        {
                            hadstrangerate = false;
                            outheartrate = 60000.0/hd1;
                        }
                    }
                    logPrintI("%lf - %d\n", outheartrate,over);
                }                
            }
            else
            {
                // show beat for half the off time
                if (downcnt > (int) offDuration/2) beat = true;
                --downcnt;                
            }
        }

        targetEvent.setAttribute<float>("heartrate", (float)outheartrate);
        targetEvent.setAttribute<bool>("trigger", beat);

        if (samplecount%samplefactor == 0)
        {
            updateObservers( targetEvent );
        }
        samplecount++;        
    }

    void HeartrateFilterNode::pushEvent()
    {
        // nothing to do
    }

    void HeartrateFilterNode::pullEvent()
    {
        // nothing to do
    }
} // namespace ot

/* 
 * ------------------------------------------------------------
 *   End of HeartrateFilterNode.cxx
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
