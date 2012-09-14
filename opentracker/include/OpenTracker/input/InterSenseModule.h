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
/** header file for InterSenseModule module.
 *
 * @author Ivan Viola, Matej Mlejnek, Gerhard Reitmayr
 *
 * $Id: InterSenseModule.h 2103 2007-12-19 16:39:50Z bornik $
 * @file                                                                   */
/* ======================================================================= */

/**
 * @page module_ref Module Reference
 * @section intersensemodule InterSenseModule
 * The InterTraxModule provides and drives @ref intersensesource nodes that 
 * generate standard events in certain intervals. It uses a configuration 
 * element called @c InterSenseConfig, which in turn contains one element
 * called @c ISTracker per configured tracker. Only trackers that are
 * configured will be used. The @c ISTracker element has the following 
 * attributes :
 * @li @c id a unique id to identify the tracker, this the same as the id 
 *           attribute of the @ref intersensesource nodes.
 * @li @c comport the serial port the tracker is connected to. If 0 then
 *        the first one is used or an InterTrax2 connected to the USB port.
 *
 * An example configuration element looks like this :
 * @verbatim
 <InterSenseConfig>
 <ISTracker comport="0" id="InterTrax"/>
 </InterSenseConfig>@endverbatim
 *
 * @note If an InterTrax2 device is used the orientation data will be transformed
 * into a standard OpenGL coordinate system where +Y points upwards, +X to the right 
 * and the default view is down -Z. Therefore yaw is a rotation around +Y, pitch a
 * rotation around +X and roll a rotation around +Z.
 */

#ifndef _INTERSENSEMODULE_H
#define _INTERSENSEMODULE_H

#include "../OpenTracker.h"


#ifndef OT_NO_INTERSENSE_SUPPORT


namespace ot {

    struct ISTracker;

    typedef std::vector<ISTracker *> ISTrackerVector;

    /**
     * The module and factory to drive the InterSenseSource nodes. It constructs
     * InterSenseSource nodes via the NodeFactory interface and pushes events into
     * the tracker tree according to the nodes configuration.
     *
     * On Windows it relies on the isense.dll and itrax2.dll distributed with 
     * OpenTracker in the bin directory. These include support for USB based
     * InterTrax2 devices. The Unix version relies distributed code from InterSense.
     *
     * @author Ivan Viola, Matej Mlejnek, Gerhard Reitmayr
     * @ingroup input
     */
    class OPENTRACKER_API InterSenseModule : public Module, public NodeFactory
    {
        // Members
    protected:
        /// stores information about the configured trackers and the
        /// sources.
        ISTrackerVector trackers;

        /// Indicates if we should reset the heading of the trackers upon start
        bool resetheading;

        /// Indicates whether the tracker is used for head tracking
        bool headtracking;

        // Methods
    public:
        /** constructor method. */
        InterSenseModule() : 
            Module(), 
            NodeFactory(),            
            resetheading(true),
            headtracking(false)
        { 
        };
        /** Destructor method, clears nodes member. */
        virtual ~InterSenseModule();
        /**
         * initializes the tracker module. For each configured tracker it
         * allocates an ISTracker structure and initializes the tracker.
         * @param attributes StringTable of elements attribute values. Should be
         *        possibly , but is not for convenience.
         * @param localTree pointer to root of configuration nodes tree
         */
        virtual void init(StringTable& attributes,  ConfigNode * localTree);
        /** This method is called to construct a new Node. It compares
         * name to the InterTraxSource element name, and if it matches
         * creates a new InterTraxSource node.
         * @param name reference to string containing element name
         * @attributes reference to StringTable containing attribute values
         * @return pointer to new Node or NULL. The new Node must be
         *         allocated with new ! */
        virtual Node * createNode( const std::string& name, const StringTable& attributes);

        /**
         * starts the module. Here we will reset the heading of any pure orientation
         * tracker. */
        virtual void start();

	/**
         * closes InterSense library */
        virtual void close();
        /**
         * pushes events into the tracker tree. Checks all trackers for new data
         * and fires the InterSenseSources, if new data is present.
         */
        virtual void pushEvent();
    };

	OT_MODULE(InterSenseModule);
} // namespace ot


#endif // OT_NO_INTERSENSE_SUPPORT


#endif

/* 
 * ------------------------------------------------------------
 *   End of InterSenseModule.h
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
