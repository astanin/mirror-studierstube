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
/** header file for MobilabSource Node.
 *
 * @author Alexander Bornik
 *
 * $Id$
 * @file                                                                   */
/* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section mobilabsource MobilabSource
 * The Mobilab Source node is a simple EventGenerator that outputs data received
 * on a particular channel of the g.Mobilab device.
 * It is managed by the @ref mobilabmodule, see there for more information on 
 * how to configure Mobilab support.
 * The MobilabSource element has the following attributes:
 * @li @c channel the channel number of the g.Mobilab device to use, 
 * between 0 and any positive number
 * @li @c type the data type to be used - possible values are 
 * unsigned short (default), float and double. Floating point types range from
 * 0 to 1.
 * An example element looks like this :
 * @verbatim
 <MobilabSource channel="0" type="double" />@endverbatim
*/

#ifndef _MOBILABSOURCE_H
#define _MOBILABSOURCE_H

#include <cassert>
#include <deque>

#include "../OpenTracker.h"

#ifndef EXCLUDE_MOBILAB_SUPPORT

#include "MobilabDriver.h"
#include "MobilabModule.h"

/**
 * A very simple EventGenerator node for outputing g.Mobilab channel data.
 * @author Alexander Bornik
 * @ingroup input
 */

namespace ot {

    enum datatypes {
        FLOAT_TYPE,
        DOUBLE_TYPE,
        USHORT_TYPE
    };

    class OPENTRACKER_API MobilabSource : public Node, public MobilabListener  
    {
    public:

        /// the event that is posted to the EventObservers
        std::deque<Event> eventqo;
        /// the  eventq queuefor data from the g.Mobilab device
        std::deque<Event> eventq;
        unsigned int channel;
        int datatype;
        std::string attname;
        bool fullfreq;
        bool changed;
    
	/** tests for EventGenerator interface being present. Is overriden to
         * return 1 always.
         * @return always 1 */
        virtual int isEventGenerator()
        {
            return 1;
        }

        virtual void newData( short sampleValue, double timev);

    protected:
        double lastpushtime;
        int pushcount;
        double rlastpushtime;
        int rpushcount;
        double lastndtime;
        int ndcounter;

	/// protected constructor so it is only accessible by the module
	MobilabSource() 
        { 
            channel = 0; 
            datatype = USHORT_TYPE; 
            attname="bcidata"; 
            fullfreq=true;
            lastpushtime = 0.0;
            pushcount = 0;
            rpushcount = 0;
            ndcounter = 0;
            changed = false;
        };

        void pushEvent()
        {
            //logPrintI("MobilabSource::pushEvent\n");
            double pushtime = OSUtils::currentTime();
	    if (pushcount == 0)
	    {
	        lastpushtime = pushtime;
	        pushcount = 1;
	    }
	    else if (pushtime - lastpushtime > 10000.0)
	    {
	        pushcount++;
                if (!fullfreq)
                {
                    logPrintI("MobilabSource push rate (%d): %f Hz\n", 
                              channel,
                              pushcount/10.0);
                }
	        pushcount = 0;
	  
	    }
	    else
	    {
	        pushcount++;
	    }

            lock();
            /*
            if (channel == 0 && buffer.hasAttribute(attname))
            {
                std::cout << "PositioX = " << buffer.getAttributeValueString(attname)
                          << std::endl;
            }
            */
            if( changed || fullfreq)
            {
                double rpushtime = OSUtils::currentTime();                
                if (rpushcount == 0)
                {
                    rlastpushtime = rpushtime;
                    rpushcount += eventq.size();
                }
                else if (rpushtime - rlastpushtime > 10000.0)
                {
                    rpushcount += eventq.size();
                    if (!fullfreq)
                    {
                        logPrintI("MobilabSource real push rate (%d): %f Hz\n", 
                                  channel,
                                  rpushcount/10.0);
                    }
                    rpushcount = 0;
	  
                }
                else
                {
                    rpushcount += eventq.size();
                }


                eventqo = eventq;
                eventq.clear();
                changed = false;
                
                unlock();
                while (!eventqo.empty())
                {
                    updateObservers( eventqo.front());
                    eventqo.pop_front();
                }
            }
            else
            {
                unlock();
            }
        }

        void pullEvent() {};

	friend class MobilabModule;
    };

    inline void MobilabSource::newData( short sampleValue, double timev)
    {
        //logPrintI("MobilabSource(%x)::newData %hu\n",this,sampleValue);
        /*
        double ndtime = OSUtils::currentTime();
        if (ndcounter == 0)
        {
            lastndtime = ndtime;
            ndcounter = 1;
        }
        else if (ndtime - lastndtime > 10000.0)
        {
            ndcounter++;
            logPrintI("Mobilab newData rate: %f Hz\n", ndcounter/10.0);
            ndcounter = 0;
            
        }
        else
        {
            ndcounter++;
        }
        */
        lock();
        eventq.push_back(Event());
        Event &backelem = eventq.back();
        backelem.time = timev;
        
        backelem.getConfidence() = 1.0;

        switch (datatype)
        {
            case  FLOAT_TYPE :
                {
                    float fltval = sampleValue;
                    fltval /= 32768.0f;
                    backelem.setAttribute<float>(attname, fltval); 
                }
                break;
            case DOUBLE_TYPE:
                {
                    double dblval = sampleValue;                    
                    dblval /= 32768.0F;
                    backelem.setAttribute<double>(attname, dblval); 
                }
                break;
            case USHORT_TYPE:
                backelem.setAttribute<unsigned short>(attname, sampleValue); 
                break;
            default:                
                backelem.setAttribute<unsigned short>(attname, 0); 
                break;
        }
        changed = true;

        unlock();
        /*
        if (channel == 0)
        {
            std::cout << "Position = " << buffer.getAttributeValueString(attname)
                      << std::endl;
        }
        */
    }
    
} // namespace ot

#endif // !ifndef(EXCLUDE_MOBILAB_SUPPORT)

#endif // !defined(_MOBILABSOURCE_H)


/* 
 * ------------------------------------------------------------
 *   End of MobilabSource.h
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
