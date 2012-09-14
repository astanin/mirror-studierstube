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
/** header file for the raw input module.
 *
 * @author Mathis Csisinko
 *
 * $Id: RawInputModule.h 1507 2006-10-11 08:22:20Z bornik $
 * @file                                                                   */
/* ======================================================================= */

/**
 * @page module_ref Module Reference
 * @section rawinputmodule RawInputModule
 * The RawInputModule interfaces to the raw input Windows API (available since
 * Windows XP). Raw input supports keyboards, mice and other human interface
 * devices. This is the only way to distinguish between input coming from
 * multiple devices on Windows XP, since DirectInput cannot distinguish them
 * on that platform.
 * The input device is specified by creating a @ref rawinputsource node in
 * order to insert event data into tree. Currently, only mice are supported by
 * RawInputModule.
 * When obtaining mouse input, button events are blocked from reaching the
 * system. [F12] can be used to toggle between using mouse button input
 * exclusively in tracking and Windows.
 * The configuration element is @c RawInputConfig and has to be present to
 * initialize the device list. On success, a list of device names is outputted.
 *
 * An example configuration element looks like this :
 * @verbatim
 <RawInputConfig/>@endverbatim
*/

#ifndef _RAWINPUTMODULE_H
#define _RAWINPUTMODULE_H

#include <OpenTracker/OpenTracker.h>

#ifdef USE_RAWINPUT
#ifdef WIN32

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#endif

#include <windows.h>

namespace ot {

    /**
     * A raw input tracking source module using the raw input Windows API. It
     * acts as a NodeFactory for RawInputSource nodes. In order to obtain input,
     * it runs a thread. Additionally, it keeps a vector of created nodes.
     *
     * @ingroup input
     */

    class OPENTRACKER_API RawInputModule: public ThreadModule,public NodeFactory
    {
        // methods
    public:
        /// constructor method
        RawInputModule();

        /// destructor method
        virtual ~RawInputModule();

        /** This method is called to construct a new Node. It compares
         * name to the RawInputSource element name, and if it matches
         * creates a new RawInputSource node.
         * @param name reference to string containing element name
         * @attributes refenrence to StringMap containing attribute values
         * @return pointer to new Node or NULL.
         */
        virtual Node* createNode(const std::string &name,const StringTable &attributes);

        /**
         * closes the module.
         */
        virtual void close();

        /**
         * pushes events into the tracker tree. Checks all source nodes for
         * new events and pushes them into the tracker tree.
         */
        virtual void pushEvent();

        /**
         * initializes the RawInputModule. 
         * @param attributes StringMap of elements attribute values.
         * @param pLocalTree pointer to root of configuration nodes tree
         */
        virtual void init(StringTable &attributes,ConfigNode* pLocalTree);

        // members
    protected:
        virtual void run();

        /// stores the sources
        NodeVector pSources;

    private:
        static LRESULT CALLBACK LowLevelMouseHook(int,WPARAM,LPARAM);
        static LRESULT CALLBACK LowLevelKeyboardHook(int,WPARAM,LPARAM);
        static LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
        void processRawInput(HWND,WPARAM,HRAWINPUT);

        UINT numDevices;
        char** deviceNames;
        RAWINPUTDEVICELIST* rawInputDeviceList;
        static bool filterButtons;
        static RawInputModule* pRawInputModule;
		DWORD threadId;
    };

	OT_MODULE(RawInputModule);
} // namespace ot

#endif
#endif  // USE_RAWINPUT

#endif

/* ===========================================================================
   End of RawInputModule.h
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
