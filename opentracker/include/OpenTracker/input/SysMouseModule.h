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
 /** header file for SysMouseModule Node.
  *
  * @author Markus Sareika
  *
  * $Id: SysMouseModule.h
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page module_ref Module Reference
 * @section SysMouseModule SysMouseModule
 * The SysMouseModule manages the @ref SysMouseSinks nodes that 
 * receive events in certain intervals.
 */


#ifndef _SYSMOUSEMODULE_H
#define _SYSMOUSEMODULE_H

#include <OpenTracker/OpenTracker.h>

#ifdef USE_SYSMOUSE

#include <ace/OS.h>
#include <ace/Time_Value.h>

#ifdef WIN32 
#include <Windows.h>

#endif

/**
 * The module and factory to drive the SysMouseSinks nodes. It constructs
 * SysMouseSinks nodes via the NodeFactory interface and pushes events into
 * the tracker tree according to the nodes configuration.
 */

namespace ot {

class OPENTRACKER_API SysMouseModule : public ThreadModule, public NodeFactory
{

protected:
    // list of SysMouseSinks nodes in the tree
    NodeVector nodes;

public:



    /** 
	 * Constructor method. 
     */
    SysMouseModule();

    /** 
	 * Destructor method, clears nodes member.
	 */
    virtual ~SysMouseModule();

    /** This method is called to construct a new Node. It compares
     * name to the SysMouseSink element name, and if it matches
     * creates a new SysMouseSink node.
     * @param name reference to string containing element name
     * @attributes refenrence to StringTable containing attribute values
     * @return pointer to new Node or NULL. The new Node must be
     * allocated with new ! 
	 */
    virtual Node * createNode( const std::string& name,  const StringTable& attributes);

	/**
	 * stop the thread
	 */
	virtual void close();

	/**
	 *  starts the thread
	 */
	virtual void start();

	/**
	 *  the thread
	 */
	virtual void run();

	/**
	 *  flag to stop the thread
	 */
	bool stop;

	/**
	 *  the working thread loop
	 */
	void processLoop();

	friend class SysMouseSink;

private:

	unsigned short buttonInput, lastButtonInput;
	int mouseX, mouseY;
	bool resetAbs;

#ifdef WIN32 
	PINPUT inputPtr;
	PMOUSEINPUT mouseInputPtr;
	int buttonPressed;
	DWORD mouseFlags;
#endif	

};

OT_MODULE(SysMouseModule);
} // namespace ot

#else
#ifdef WIN32
#pragma message(">>> OT_NO_SYSMOUSE_SUPPORT")
#endif
#endif	//USE_SYSMOUSE

#endif	//_SYSMOUSEMODULE_H
