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
  * $Id: GoGoModule.h  sareika $
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page module_ref Module Reference
 * @section GoGoModule GoGoModule
 * The GoGoModule provides and drives @ref GoGoSinkSource nodes that 
 * generate standard events in certain intervals. It does not use a
 * configuration element, but reserves the name 'SpaceMouseConfig'.
 */

#ifndef _GOGOMODULE_H
#define _GOGOMODULE_H

#include "../OpenTracker.h"

#ifdef USE_GOGO

#include <Windows.h>

/**
 * The module and factory to drive the GoGoSinkSource nodes. It constructs
 * GoGoSinkSource nodes via the NodeFactory interface and pushes events into
 * the tracker tree according to the nodes configuration.
 */

namespace ot {

class OPENTRACKER_API GoGoModule : public ThreadModule, public NodeFactory
{

protected:
    // list of GoGoSinkSource nodes in the tree
    NodeVector nodes;

public:
    /** constructor method. */
    GoGoModule() : ThreadModule(), NodeFactory(), stop(0)
    {};

    /** Destructor method, clears nodes member.
	 */
    virtual ~GoGoModule();

    /** This method is called to construct a new Node. It compares
     * name to the GoGoSinkSource element name, and if it matches
     * creates a new GoGoSinkSource node.
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

    virtual void start();

	virtual void run();

    // flag to stop the thread
    int stop;

    /**
     * pushes events into the tracker tree. Checks all GoGoSinkSources and
     * pushes new events, if a GoGoSinkSource fires. The events store
	 * structure with position and status of the buttons.
     */
    virtual void pushEvent();


private:
	void processMessages();

};
OT_MODULE(GoGoModule);
} // namespace ot

#else
#ifdef WIN32
#pragma message(">>> OT_NO_GOGO_SUPPORT")
#endif
#endif	//USE_GOGO

#endif	//_GOGOMODULE_H
