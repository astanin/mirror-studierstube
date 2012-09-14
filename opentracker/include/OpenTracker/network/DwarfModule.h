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
/** header file for DWARFMODULE module.
 *
 * @author Gerhard Reitmayr, Christian Sandor, Martin Bauer
 *
 * $Id: DwarfModule.h 1547 2006-10-25 10:33:10Z veas $
 * @file                                                                   */
/* ======================================================================= */

/**
 * @page module_ref Module Reference
 * @section dwarfmodule DwarfModule
 * The DwarfModule 
 */

#ifndef _DWARFMODULE_H
#define _DWARFMODULE_H

#include "../OpenTracker.h"

#ifdef USE_DWARF

#include <vector>
#include <map>

class CorbaInit;

namespace DWARF {
    class PoseSenderService;
}

namespace ot {

    class DwarfSink;
    class DwarfSource;

    /**
     * The module and factory to drive the test source nodes. It constructs
     * TestSource nodes via the NodeFactory interface and pushes events into
     * the tracker tree according to the nodes configuration.
     * @author Gerhard Reitmayr, Christian Sandor, Martin Bauer
     * @ingroup network
     */
    class OPENTRACKER_API DwarfModule : public Module, public NodeFactory
    {
        // Members
    protected:
        /// list of DwarfSinks in the OpenTracker graph
        std::vector<DwarfSink *> sinks;
        /// list of DwarfSources in the OpenTracker graph
        std::vector<DwarfSource *> sources;

        /** maps ability name to ThingId to store information.
         * Will be obsolete, if ThingId becomes an attribute. 
         */
        std::map<std::string, std::string> thingIdMap;

        /** maps ability name to ThingType to store information.
         * Will be obsolete, if ThingType becomes an attribute. 
         */
        std::map<std::string, std::string> thingTypeMap;

        /// encapsulates all PoseData abilities of the service
        DWARF::PoseSenderService * service;

        /// corba initialization, should be a singleton
        CorbaInit * myOrbConnection;

        // Methods
    public:
        /** constructor method. */
        DwarfModule() : Module(), NodeFactory()
        {};
        /** Destructor method, clears nodes member. */
        virtual ~DwarfModule();

        virtual void init(StringTable & attributes, ConfigNode * localTree);

        /** This method is called to ruct a new Node. It compares
         * name to the TestSource element name, and if it matches
         * creates a new TestSource node.
         * @param name reference to string containing element name
         * @attributes refenrence to StringMap containing attribute values
         * @return pointer to new Node or NULL. The new Node must be
         *         allocated with new ! */
        virtual Node * createNode( const std::string& name,  StringTable& attributes);
        /**
         * pushes events into the tracker tree. Checks all TestSources and
         * pushes new events, if a TestSource fires.
         */
        virtual void pushEvent();

        virtual void pullEvent();
    };
	OT_MODULE(DwarfModule);
	

} // namespace ot

#endif // USE_DWARF

#endif

/* 
 * ------------------------------------------------------------
 *   End of DwarfModule.h
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
