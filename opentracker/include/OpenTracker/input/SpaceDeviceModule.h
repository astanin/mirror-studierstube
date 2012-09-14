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
/** header file for Spacemouse module.
  *
  * @author 
  *
  * $Id: SpaceDeviceModule.h 1260 2006-07-13 09:46:25Z sareika $
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page module_ref Module Reference
 * @section SpaceDeviceModule SpaceDeviceModule
 * The SpaceDeviceModule provides and drives @ref SpaceDeviceSource nodes that 
 * generate standard events in certain intervals. It does not use a
 * configuration element, but reserves the name 'SpaceMouseConfig'.
 */

/**
 * @page spacemouse Space Mouse Integration
 *
 * Instructions on including Modules for 3Dconnexion in OpenTracker:
 *
 * @par 1.Step: Install the 3Dxware sdk and set env var: 3DCONNEXIONROOT
 *
 *
 * @par 2.Step: Recompile
 */

#ifndef _SPACEDEVICEMODULE_H
#define _SPACEDEVICEMODULE_H

#include "../OpenTracker.h"

#ifdef USE_SPACEDEVICE
#include <Windows.h>

/**
 * The module and factory to drive the SpaceDeviceSource nodes. It constructs
 * SpaceDeviceSource nodes via the NodeFactory interface and pushes events into
 * the tracker tree according to the nodes configuration.
 */

namespace ot {

class OPENTRACKER_API SpaceDeviceModule : public ThreadModule, public NodeFactory
{

protected:
    // list of SpaceDeviceSource nodes in the tree
    NodeVector nodes;

public:
    /** constructor method. */
    SpaceDeviceModule() : ThreadModule(), NodeFactory(), stop(0)
    {};

    /** Destructor method, clears nodes member.
	 */
    virtual ~SpaceDeviceModule();

    /** This method is called to construct a new Node. It compares
     * name to the SpaceDeviceSource element name, and if it matches
     * creates a new SpaceDeviceSource node.
     * @param name reference to string containing element name
     * @attributes refenrence to StringTable containing attribute values
     * @return pointer to new Node or NULL. The new Node must be
     * allocated with new ! 
	 */
    virtual Node * createNode( const std::string& name,  const StringTable& attributes);

	/**
     * closes SpaceMouse dynamic library 
	 */
    virtual void close();

    /**
	 * opens SpaceMouse dynamic library (SIAPPDLL.DLL)
     */
    virtual void start();

	virtual void run();

    // flag to stop the thread
    int stop;

    /**
     * pushes events into the tracker tree. Checks all SpaceDeviceSources and
     * pushes new events, if a SpaceDeviceSource fires. The events store
	 * structure with position and status of the buttons.
     */
    virtual void pushEvent();

	static HWND	hWndSpaceDevice;

private:
	void processMessages();

};
OT_MODULE(SpaceDeviceModule);
} // namespace ot

#else
#ifdef WIN32
#pragma message(">>> OT_NO_SPACEDEVICE_SUPPORT")
#endif
#endif	//USE_SPACEDEVICE

#endif	//_SPACEDEVICEMODULE_H
