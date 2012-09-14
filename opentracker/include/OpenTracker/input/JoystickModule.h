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
/** header file for JoystickModule module.
 *
 * @author Rainer Splechtna
 *
 * $Id: JoystickModule.h 2108 2008-01-02 19:44:00Z samset $
 * @file                                                                   */
/* ======================================================================= */

/**
 * @page module_ref Module Reference
 * @section JoystickModule JoystickModule
 * The JoystickModule provides and drives @ref JoystickSource nodes that 
 * generate events. The member position of the event gives the 
 * current normalized position of the joystick. The x,y and z components 
 * of position reflect the current position of the joystick's x,y and z-axes
 * (ranging from -1 to +1, e.g. -1,-1,z means that the joystick is in the upper
 * left corner). The member orientation is not used. The member button is used
 * as usual (note that right now only the first four buttons of a joystick are 
 * used).
 *
 * It doesn't have a configuration element
 * but reserves the name 'JoystickConfig' for it. It does something useful as soon as
 * at least one @ref JoystickSource node is created.
 */

#ifndef _JOYSTICKMODULE_H
#define _JOYSTICKMODULE_H

#include "../OpenTracker.h"

#ifdef USE_JOYSTICK

#include <windows.h>
#include <mmsystem.h>

#include "../dllinclude.h"

#include <vector>

namespace ot {

    typedef std::vector<Node::Ptr> NodeVector;

    /**
     * The module and factory to drive the JoystickSource nodes. It constructs
     * JoystickSource nodes via the NodeFactory interface and pushes events into
     * the tracker tree according to the nodes configuration. The implementation uses the 
     * winmm-library and therfore supports only two joysticks. 
     * @author Rainer Splechtna
     * @ingroup input
     */
    class OPENTRACKER_API JoystickModule : public ThreadModule, public NodeFactory
    {
        // Members
    protected:
        /// stores which joysticks are available
        int joyPresent[2];
        /// stores capabilities of joysticks
        JOYCAPS   joyCaps[2];

        /// stores x-axis range of joysticks
        int xRange[2];
        /// stores y-axis range of joysticks
        int yRange[2];
        /// stores z-axis range of joysticks
        int zRange[2];

        /** Updates member joyCaps for specified joystick ID.
         *  @param id joystick ID */
        void getCaps(int id);
        /// list of JoystickNode nodes in the tree
        NodeVector nodes;

        /** This method is the code executed in its own thread. It polls all
         * present joysticks and updates the tmpEvent of the associated 
         * @ref JoystickSource-nodes. */
        virtual void run();

        /// flag to stop the thread
        int stop;

        // Methods
    public:
        /** constructor method. */
        JoystickModule();

        /** Destructor method, clears nodes member. */
        virtual ~JoystickModule();

        /** This method is called to construct a new Node. It compares
         * name to the JoystickNode element name, and if it matches
         * creates a new JoystickNode node.
         * @param name reference to string containing element name
         * @attributes refenrence to StringMap containing attribute values
         * @return pointer to new Node or NULL. The new Node must be
         *         allocated with new ! */
        virtual Node * createNode( const std::string& name,  const StringTable& attributes);
        /**
         * Pushes events into the tracker tree. Checks all JoystickSource nodes and
         * pushes new events, if a JoystickSource node fires.
         */
        virtual void pushEvent();

        /**
         * Tests for available joysticks and starts the polling-thread.
         */
        virtual void start();

        /// Only sets the stop flag to 1.
        virtual void close();

    private:

        /// This method polls all present joysticks.
        void pollJoysticks();
    };

	OT_MODULE(JoystickModule);
} // namespace ot

#endif
#endif

/* 
 * ------------------------------------------------------------
 *   End of JoystickModule.h
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
