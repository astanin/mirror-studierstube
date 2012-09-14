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
 /** header file for PanTiltUnitSource Node.
  *
  * @author Markus Sareika
  *
  * $Id: PanTiltUnitModule.h
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page module_ref Module Reference
 * @section PanTiltUnitModule PanTiltUnitModule
 * The PanTiltUnitModule provides and drives @ref PanTiltUnitSource nodes that 
 * generate events in certain intervals.
 */

/**
 * @page pantiltunit Pan Tilt Unit Integration
 *
 * Instructions on including Modules for PTU in OpenTracker:
 *
 * @par 1.Step: Install the ptu sdk and set env var: PTUROOT
 *
 * @par 2.Step: Recompile and have fun
 
 */

#ifndef _PANTILTUNITMODULE_H
#define _PANTILTUNITMODULE_H

#include <OpenTracker/OpenTracker.h>

#ifdef USE_PANTILTUNIT

#include <Windows.h>

/**
 * The module and factory to drive the PanTiltUnitSource nodes. It constructs
 * PanTiltUnitSource nodes via the NodeFactory interface and pushes events into
 * the tracker tree according to the nodes configuration.
 */

namespace ot {

class OPENTRACKER_API PanTiltUnitModule : public ThreadModule, public NodeFactory
{

protected:
    // list of PanTiltUnitSource nodes in the tree
    NodeVector nodes;

public:
    /** constructor method. */
    PanTiltUnitModule() : ThreadModule(), NodeFactory(), stop(false)
    {};

    /** Destructor method, clears nodes member.
	 */
    virtual ~PanTiltUnitModule();

    /** This method is called to construct a new Node. It compares
     * name to the PanTiltUnitSource element name, and if it matches
     * creates a new PanTiltUnitSource node.
     * @param name reference to string containing element name
     * @attributes refenrence to StringTable containing attribute values
     * @return pointer to new Node or NULL. The new Node must be
     * allocated with new ! 
	 */
    virtual Node * createNode( const std::string& name,  const StringTable& attributes);

	/**
     * stop the ptu thread
	 */
    virtual void close();

    /**
	 *  starts the ptu thread
     */
    virtual void start();

	/**
	 *  the ptu thread
	 */
	virtual void run();

	/**
	 *  flag to stop the ptu thread
	 */
    bool stop;

    /**
     * pushes events into the tracker tree. Checks all PanTiltUnitSources and
     * pushes new events, if a PanTiltUnitSource fires. The events store
	 * structure with position and status of the buttons.
     */
    virtual void pushEvent();


private:

	// 
	void processLoop();

};
OT_MODULE(PanTiltUnitModule);
} // namespace ot

#else
#ifdef WIN32
#pragma message(">>> OT_NO_PTU_SUPPORT")
#endif
#endif	//USE_PANTILTUNIT

#endif	//_PANTILTUNITMODULE_H
