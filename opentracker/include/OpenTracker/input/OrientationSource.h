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
/** source file for OrientationModule.
 *
 * @author Gerhard Schall
 *
 * $Id$
 * @file                                                                   */
/* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section OrientationSource OrientationSource
 * The OrientationSource node is a simple EventGenerator that outputs orientation data
 * from a single Orientation tracker provided by FTW Vienna. See the @ref OrientationModule for more information.
 * 
 * In general the sensor module behaves like a Bluetooth GPS device. After the device discovery phase and 
 * connection phase the module opens a serial port profile service and starts sending data continually to 
 * the serial port. The data format is in the form: nnn.n,nn.n,nn.n
 * The first group shows the direction of the magnetic compass, then follows the tilt angle in x and y direction. 
 * All data are in decimal degrees. Tilt angles can also be negative.
 *
 * The device has 2 switches. The BT switch is for the Bluetooth module to start and stop. The HW module is for
 * the sensor module to start and stop. In the first step only the BT module must be 1, the HW switch has to be turned off.
 * Else the BT module doesnt go to data mode. After having established a connection, the HW switch can be turne on.
 
 * An example element looks like this :
 * @verbatim
 <OrientationSource/>@endverbatim
 *
 */

#ifndef _ORIENTSOURCE_H
#define _ORIENTSOURCE_H

#include "../OpenTracker.h"


namespace ot {


    class OrientationSource;
    typedef std::vector<OrientationSource*> OrientationSourceVector;

    /**
     * The OrientationSource reads and propagates data from an Orientation sensor delivered by FTW Vienna.
     * @author Gerhard Schall
     * @ingroup input
     */

    class OPENTRACKER_API OrientationSource : public Node 
    {
    public:        

	/// the new event
        Event event;
        bool modified;

    
        /** tests for EventGenerator interface being present. Is overriden to
         * return 1 always.
         * @return always 1 */
        virtual int isEventGenerator()
        {
            return 1;
        }
    
        void pushEvent();
        void pullEvent();

    protected:
	/// protected constructor so it is only accessible by the module
	OrientationSource() : Node()
            { modified = true; }


        friend class OrientationModule;
    };


}  // namespace ot


#endif // !defined(_OrientSource_H)

/* ===========================================================================
   End of OrientationSource.h
   ===========================================================================
   Automatic Emacs configuration follows.
   Local Variables:
   mode:c++
   c-basic-offset: 4
   eval: (c-set-offset 'subeventment-open 0)
   eval: (c-set-offset 'case-label '+)
   eval: (c-set-offset 'eventment 'c-lineup-runin-eventments)
   eval: (setq indent-tabs-mode nil)
   End:
   =========================================================================== */
