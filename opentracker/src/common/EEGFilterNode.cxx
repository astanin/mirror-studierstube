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
/** source file for EEGFilter node.
 *
 * @author Alexander Bornik
 *
 * $Id $
 * @file                                                                   */
/* ======================================================================= */

// this will remove the warning 4786
#include <OpenTracker/tool/disable4786.h>

#include <map>
#include <cmath>
#include <fstream>

#include <OpenTracker/common/EEGFilterNode.h>

namespace ot {

    // constructor method
    EEGFilterNode::EEGFilterNode( const std::vector<double> &ifreqs, 
                                  const unsigned int icodelength,
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
                                  const std::string logfile)
        : frequencies(ifreqs),
          codelength(icodelength),
          samplerate(isamplerate),
          harmonics(iharmonics),
          prepause(iprepause),
          postpause(ipostpause),
          interpause(iinterpause),
          codeduration(icodeduration),
          threshold(ithreshold),
          hitpercentage(ihitpercentage),
          inatt(iinatt),
          outatt(ioutatt),
          triggeratt(itriggeratt),
          consume(iconsume),
          logfile(logfile),
          evmode(CALIB),
          calibcount(0),
          eegoutval(0),
          lasteegoutval(0)
    {
        using namespace std;
        eegsubs.clear();
        std::vector<double>::const_iterator it;

        logPrintI("EEGFilter frequencies: %u codelength %u\n",frequencies.size(), codelength);
        for (it = frequencies.begin(); it != frequencies.end(); it++)
        {
            eegsubs.push_back(make_pair(EEGSub(*it, harmonics, samplerate), 1.0));
        }

        evbuffertargetsize = 
            static_cast<unsigned int>((prepause + 
                                       codelength*(codeduration+interpause) 
                                       - interpause + postpause) 
                                      * samplerate * 0.001);
        evbuffercodestart = 
            static_cast<unsigned int>(prepause * samplerate * 0.001);
        evbufferpoststart = 
            static_cast<unsigned int>(
                                (prepause + 
                                 codelength*(codeduration+interpause) 
                                 - interpause) * samplerate * 0.001);
        
        logPrintI("ebts: %lu, ebcs: %lu, ebps: %lu\n inattname: %s\n outattname: %s\n triggerattname %s\n", 
                  evbuffertargetsize, evbuffercodestart, evbufferpoststart,
                  inatt.c_str(), outatt.c_str(), triggeratt.c_str());
    } 

    // Applies the filter to incoming events
    void EEGFilterNode::onEventGenerated( Event& event, Node & generator )
    {
        
        // The current event we're working on
        currentEvent = event;
        
        // The event we will be passing
        targetEvent = event;

        lasteegoutval = eegoutval;
        eegoutval = -1;

        if (currentEvent.hasAttribute(triggeratt)
            && currentEvent.getAttributeValueString(triggeratt) == "1")
        {
            if (evmode != CALIB)
            {
                //logPrintI("EEGFilter: restarting calibration\n");
                evmode = CALIB;
                calibcount = 0;
            }
        }

        //logPrintI("EEGFilter ...\n");
        //event.printout();

        // do we need to process the event?
        if (currentEvent.hasAttribute(inatt) && 
            currentEvent.getAttributeTypeName(inatt) == "double")
        {
            if (evmode != CALIB)
            {                
                //logPrintI("EEGFilter normal operation ...\n");
                std::vector<std::pair<EEGSub, double> >::iterator it;
                double actval;
                double maxval = 0.0;
				double maxval2 = 0.0;
                int i;
                int maxi = -1;
				int maxi2 = -1;
                
                // new value into filter and calculate maximum response frequency
                // index
                
                for (it=eegsubs.begin(),i=0; it != eegsubs.end(); it++, i++)
                {
                    actval = it->first.filter(currentEvent.getAttribute(inatt,(double)0));
                    //printf("EEGSubs %i: %lf\n", i, actval);
                    actval *= it->second;
                    
                    if ( actval > maxval)
                    {
                        maxval2 = maxval;
                        maxi2 = maxi;
                        maxval = actval;
                        maxi = i;
                    }
                    else if (actval > maxval2)
                    {
                        maxval2 = actval;
                        maxi2 = i;
                    }
                }
				
                if (maxval <= maxval2*threshold)
                {
                    maxi = -1;
                    maxval = 0;
                }
                //printf(" maxval(%d): %lf  maxval(%d): %lf \n", maxi, maxval, maxi2, maxval2);
				
                // update event buffer
                
                if (evbuffer.size() < evbuffertargetsize)
                {
                    evbuffer.push_front(maxi);                
                }
                else
                {
                    if (evbuffer.size() > evbuffertargetsize)
                        logPrintI("EEGFilter: reached buffer size exceeded\n");
                
                    evbuffer.pop_back();
                    evbuffer.push_front(maxi);
                
                    // calculate whether the code could be successfully recognized
                    // first analyze event buffer and setup voting structure
                    unsigned int precount = 0;
                    unsigned int postcount = 0;
#ifndef WIN32
                    std::map<int, int> digitmap[codelength];
#else
                    codelength = 2;
                    std::map<int, int> digitmap[2];
#endif
                    unsigned int i;
                    for (i=0; i<evbuffertargetsize; i++)
                    {
                        if (i<evbuffercodestart)
                        {
                            if (evbuffer[i] == -1) ++precount;
                        }
                        else if (i<evbufferpoststart)
                        {
                            int dmindex = 
                                (i-evbuffercodestart)/
                                ((evbufferpoststart-evbuffercodestart)/codelength);
                            //printf("dmindex: %d\n", dmindex);
                            digitmap[dmindex][evbuffer[i]]++;
                        }
                        else
                        {
                            if (evbuffer[i] == -1) ++postcount;
                        }
                        
                    }
                
                    ///evaluate voting results
                    int tempoutval = 0;
                    for (i=0; i<codelength;  i++)
                    {
                        int totalsamples =0;
                        std::map<int, int>::const_iterator it;
                        int actval,actcount;
                        int maxval=-1;
                        int maxcount=0;

                        /// find the dominant frequency for a code sequence 
                        /// part
                        for (it=digitmap[i].begin(); it != digitmap[i].end(); it++)
                        {
                            actval = it->first;
                            actcount = it->second;
                            totalsamples += actcount;
                            if (actcount>maxcount)
                            {
                                maxcount = actcount;
                                maxval = actval;
                            }
                        }       
                        /// only, if the predominant vote is for one of 
                        /// the target frequencies, we continue
                        if (maxval != -1)
                        {
                            /// it has to meet the hit percentage requirement also
                            if (maxcount >= ((evbufferpoststart-evbuffercodestart)/codelength)*hitpercentage/100.0)
                            {
                                /// calculate output value in a 
                                /// frequencies.size() - ary number system
                                tempoutval *= frequencies.size();
                                tempoutval += maxval;
                                //printf("+percentage: %d/%d\n",maxcount, (int)(((evbufferpoststart-evbuffercodestart)/codelength)*hitpercentage/100.0));
                            }
                            else
                            {
                                //printf("-percentage: %d/%d\n",maxcount, (int)(((evbufferpoststart-evbuffercodestart)/codelength)*hitpercentage/100.0));
                                /// don't continue if sequence part does
                                /// not meet hit percentage
                                tempoutval = -1;
                                break;
                            }
                        }
                        else
                        {
                            //printf("no response\n");
                            /// if one code sequence part is not responding to 
                            /// a certain frequency we may skip the other parts
                            /// as well
                            tempoutval = -1;
                            break;
                        }
                    
                    }
                    // should only arrive here, if the sequence is detected
                    // clear buffer
                    if (tempoutval != -1)
                        evbuffer.clear();
                    eegoutval = tempoutval;

                }
                
            }
            else // perform factor calibration
            {            
                //logPrintI("EEGFilter calibrating ...\n");
                std::vector<std::pair<EEGSub, double> >::iterator it;
                unsigned int i;
                if (calibcount >= 30*samplerate)
                {
                    double maxval = 0.0;
                    // calculate average response on a specific frequency
                    // and maximum of averages
                    for (i=0; i < calibvec.size(); i++)
                    {
                        calibvec[i] /= static_cast<double>(calibvec.size());
                        if (calibvec[i] > maxval)
                        {
                            maxval = calibvec[i];
                        } 
                    }
                    
                    logPrintI("EEGFilter: calibration completed. factors: ");
                    
                    // calculate and set influence factors 
                    for (it=eegsubs.begin(),i=0; it != eegsubs.end(); it++, i++)
                    {
                        it->second = maxval / calibvec[i];
                        std::cerr << it->second << " ";
                    }
                    std::cerr << std::endl;
                                        
                    // resume to normal operation
                    evmode = NORMAL;
                    evbuffer.clear();
                }
                else if (calibcount == 0)
                {
                    logPrintI("EEGFilter: starting calibration\n");

                    calibvec.clear();
                    
                    for (it=eegsubs.begin(); it != eegsubs.end(); it++)
                    {
                        calibvec.push_back(0.0);
                    }
                    
                }              
                else
                {
                    // fill calibration vector with values
                    for (it=eegsubs.begin(),i=0; it != eegsubs.end(); it++, i++)
                    {
                        calibvec[i] += it->
                            first.filter(currentEvent.getAttribute(inatt,
                                                               (double)0));
                    }
                }
                calibcount++;
            }
            // remove source event if requested

            if (consume) targetEvent.delAttribute(inatt);
        }

        // add/set output value to event

        targetEvent.setAttribute<int>(outatt, eegoutval);
        //targetEvent.printout();
        
        if (lasteegoutval != eegoutval)
        {
            // write to logfile
            if (logfile != "")
            {
                std::ofstream ofile;
                ofile.open(logfile.c_str(), 
                    std::ios::out | std::ios::app);
                ofile << targetEvent.getPrintOut() << std::endl;
                ofile.close();
             }
        }

        // Update the observers
        if (!consume || lasteegoutval != eegoutval)
        {
            updateObservers( targetEvent );
        }
    }

    void EEGFilterNode::pushEvent()
    {
        // nothing to do
    }

    void EEGFilterNode::pullEvent()
    {
        // nothing to do
    }
} // namespace ot


/* 
 * ------------------------------------------------------------
 *   End of EEGFilterNode.cxx
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
