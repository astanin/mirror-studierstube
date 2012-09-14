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
/** header file for ParButton module.
 *
 * @author Gerhard Reitmayr
 *
 * $Id: ParButtonModule.h 2103 2007-12-19 16:39:50Z bornik $
 * @file                                                                   */
/* ======================================================================= */

/**
 * @page module_ref Module Reference
 * @section parbuttonmodule ParButtonModule (Parallel Button Input)
 * 
 * This module implements reading button values from a parallel port. See the
 * @ref parbuttonsource node for a description of how to configure a source node.
 * This module has no attributes and need not be present
 * in the configuration section to operate. However, it reserves the name 
 * @c ParButtonConfig as configuration element.
 *
 * Some hints to get it working:
 * @li See the UserPort driver on http://www.studierstube.org/opentracker/libs.html
 *     to get it working under WindowsNT/2000/XP.
 * @li Set the type of parallel port to EPP in the bios. Not ECP or EPP/ECP etc.
 * @li Restart machine after attaching the plug for Windows OS.
 */

#ifndef _PARBUTTONMODULE_H
#define _PARBUTTONMODULE_H

#include "../OpenTracker.h"


#ifndef OT_NO_PARBUTTON_SUPPORT


namespace ot {
    /**
     * Implements a set of buttons triggered via the parallel port.
     *
     * @author Gerhard Reitmayr
     * @ingroup input
     */
    class OPENTRACKER_API ParButtonModule : public ThreadModule, public NodeFactory
    {
        // Members
    protected:
        /// maps devices to nodes
        std::map<std::string, std::pair<Node *, unsigned short> > nodes;
        /// stop flag
        bool stop;
        /**
	 * Mainloop 
         */
	void run();
    public:
        /** constructor method. */
        ParButtonModule(); 
        /** destructor method */
        ~ParButtonModule();
        /** This method is called to construct a new Node. It compares
         * name to the InterTraxSource element name, and if it matches
         * creates a new InterTraxSource node.
         * @param name reference to string containing element name
         * @attributes refenrence to StringTable containing attribute values
         * @return pointer to new Node or NULL. The new Node must be
         *         allocated with new ! */
        virtual Node * createNode( const std::string& name, const StringTable& attributes);    
        /**
         * opens the ports needed and starts the receive thread
         * It is called after initialisation is done.
         */
        virtual void start();

	/**
         * closes devices */
        virtual void close();
        /**
         * pushes events into the tracker tree. Tries to read data from the
         * opened parallel ports and passes the events on.
         */
        virtual void pushEvent();
    };
	OT_MODULE(ParButtonModule);

} // namespace ot


#endif // OT_NO_PARBUTTON_SUPPORT


#endif

/* 
 * ------------------------------------------------------------
 *   End of ParButtonModule.h
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
