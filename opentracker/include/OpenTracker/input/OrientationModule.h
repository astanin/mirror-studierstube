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
 * @Page module_ref Module Reference
 * @section OrientationModule OrientationModule 
 *
 * This module implements the device driver for Orientation inertial trackers from FTW.
 * The corresponding source node is @ref OrientationSource.
 * 
 * An example configuration element looks like this:
 * @verbatim
 <OrinentationConfig device="com1"/>
 @endverbatim
*/

#ifndef _ORIENTMODULE_H
#define _ORIENTMODULE_H

#include "../OpenTracker.h"
#ifndef OT_NO_ORIENTATION_SUPPORT
#include "OrientationSource.h"
#include "../misc/serialcomm.h"




namespace ot {

    /// maximum number of targets that can be processed
    const int ORIENTATION_MAX_TARGETS = 8;
    /// maximum length of conversion packet buffer
    const int ORIENTATION_PACKET_MAX_LENGTH = 8;
    /// maximum size of serial i/o buffer
    const int ORIENTATION_COMM_BUFFER_SIZE = 4096;


    //class OrientationSource;


    /**
     * This module simply supports the OrientationSource. 
     *
     * @ingroup input
     * @author Gerhard Schall
     */

    class OPENTRACKER_API OrientationModule : public ThreadModule, public NodeFactory
    {
    protected:
      
        /// vector containing all targets, which have an associated sourceNode
        OrientationSourceVector sources;
      
      
        /// flag to stop the thread
        bool stop;
    
        /// is TRUE if the serial port was opened
        bool serialportIsOpen;

        /// port structure for the serial port data
        SerialPort port;

        // methods
    protected:
        /// this method is executed in its own thread and reads data from the Orientation tracker.
        virtual void run();

    public:    
        /** basic constructor */
        OrientationModule();

        /** destructor */
        virtual ~OrientationModule();

        /**
         * initializes the OrientationModule module. 
         * @param attributes refenrence to StringTable containing attribute values
         * @param localTree pointer to root of configuration nodes tree
         */
        virtual void init(StringTable& attributes, ConfigNode * localTree);

        /** This method is called to construct a new Node. It compares
         * name to the OrientationSource element name, and if it matches
         * creates a new OrientationSource node.
         * @param name reference to string containing element name
         * @param attributes reference to StringTable containing attribute values
         * @return pointer to new Node or NULL. The new Node must be
         *         allocated with new!
         */
        virtual Node * createNode( const std::string& name, const StringTable& attributes);  
     
        /**
         * starts the processing thread. It is called after initialisation is done.
         */
        virtual void start();

        /**
         * closes the module and closes any communication and stops thread.
         */
        virtual void close();
    
        /**
         * pushes event information into the tree. It checks whether there is new
         * data form the OrientationSource node, copies it into the nodes and calls
         * push on them.    
         */
        virtual void pushEvent();   
      
    };

	OT_MODULE(OrientationModule);

}  // namespace ot


#endif // !defined(_ORIENTMODULE_H)
#endif

/* ===========================================================================
   End of OrientationModule.h
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
