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
/** header file for NetworkSource Node.
  *
  * @author Valerie Maquil
  *
  * $Id: NetworkSource.h 900 2006-01-19 16:47:43Z spiczak $
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section networksource NetworkSource
 * The NetworkSource node is a simple EventGenerator that inserts events from
 * the network into the tracker tree. The NetworkSource element has the 
 * following attributes :
 * @li @c number the stations number, between 0 and any positive number
 * @li @c multicast-address the multicast group to send to
 * @li @c port port to send to
 *  
 * An example element looks like this :
 * @verbatim
<NetworkSource number="1" multicast-address="224.0.0.10" port="12345"/>@endverbatim
 */

#ifndef _OLDOTNETWORKSOURCE_H
#define _OLDOTNETWORKSOURCE_H

#include "../OpenTracker.h"

#ifdef OT_OLD_NETWORK_SUPPORT

namespace ot {

/**
 * This class implements a simple node that inserts new events from the network
 * into the tracker tree.
 * @author Gerhard Reitmayr
 * @ingroup network
 */
class OPENTRACKER_API OldOTNetworkSource : public Node
{
// Members
public:
    /// the event that is posted to the EventObservers
    Event event;
      
// Methods
protected:
    /** constructor */
    OldOTNetworkSource() : Node()
    {}

public:                
    /** tests for EventGenerator interface being present. Is overriden to
     * return 1 always.
     * @return always 1 */
    virtual int isEventGenerator()
    {
        return 1;
    }
  
    void pushEvent() {};
    void pullEvent() {};
    
    friend class OldOTNetworkSourceModule;
};

} // namespace ot

#endif
#endif
