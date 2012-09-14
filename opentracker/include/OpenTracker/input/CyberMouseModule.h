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
/** header file for Cybermouse module.
 *
 * @author Ivan Viola, Matej Mlejnek
 *
 * $Id: CyberMouseModule.h 2103 2007-12-19 16:39:50Z bornik $
 * @file                                                                   */
/* ======================================================================= */

/**
 * @page module_ref Module Reference
 * @section cybermousemodule CyberMouseModule
 * The CyberMouseModule provides and drives @ref cybermousesource nodes that 
 * generate standard events in certain intervals. It does not use a
 * configuration element, but reserves the name 'CyberMouseConfig'.
 */

#ifndef _CYBERMOUSEMODULE_H
#define _CYBERMOUSEMODULE_H

#include "../OpenTracker.h"

#ifdef USE_CYBERMOUSE

/**
 * The module and factory to drive the CyberMouseSource nodes. It constructs
 * CyberMouseSource nodes via the NodeFactory interface and pushes events into
 * the tracker tree according to the nodes configuration.
 * @author Ivan Viola, Matej Mlejnek
 * @ingroup input
 */

namespace ot {

    class OPENTRACKER_API CyberMouseModule : public Module, public NodeFactory
    {
        // Members
    protected:
        /// list of CyberMouseSource nodes in the tree
        NodeVector nodes;

        // Methods
    public:
        /** constructor method. */
        CyberMouseModule() : Module(), NodeFactory()
        {};
        /** Destructor method, clears nodes member. */
        virtual ~CyberMouseModule();
        /** This method is called to ruct a new Node. It compares
         * name to the CyberMouseSource element name, and if it matches
         * creates a new CyberMouseSource node.
         * @param name reference to string containing element name
         * @attributes refenrence to StringTable containing attribute values
         * @return pointer to new Node or NULL. The new Node must be
         *         allocated with new ! */
        virtual Node * createNode( const std::string& name, const StringTable& attributes);
	/**
         * closes CyberMouse dynamic library */
        virtual void close();
        /**
         * opens CyberMouse dynamic library (FREEDDLL.DLL)
         */
        virtual void start();
        /**
         * pushes events into the tracker tree. Checks all CyberMouseSources and
         * pushes new events, if a CyberMouseSource fires. The events store
	 * structure with position and status of the buttons.
         */
        virtual void pushEvent();
    };

	OT_MODULE(CyberMouseModule);

} // namespace ot

#endif

#endif

/* 
 * ------------------------------------------------------------
 *   End of CyberMouseModule.h
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
