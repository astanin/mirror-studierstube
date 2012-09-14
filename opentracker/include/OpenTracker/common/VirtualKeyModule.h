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
 /** header file for VirtualKeyModule Node.
  *
  * @author Markus Sareika
  *
  * $Id: VirtualKeyModule.h
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page module_ref Module Reference
 * @section VirtualKeyModule VirtualKeyModule
 * The VirtualKeyModule manages the @ref SysMouseSinks nodes
 */


#ifndef _VIRTUALKEYMODULE_H
#define _VIRTUALKEYMODULE_H

#include <OpenTracker/OpenTracker.h>

#ifdef USE_VIRTUALKEYSOURCE

#include <ace/OS.h>
#include <ace/Time_Value.h>

#ifdef WIN32 
#include <Windows.h>
#include <Winuser.h>
#endif

namespace ot {

class OPENTRACKER_API VirtualKeyModule : public ThreadModule, public NodeFactory
{

protected:
    
    NodeVector nodes;

public:

    /** 
	 * Constructor method. 
     */
    VirtualKeyModule();

    /** 
	 * Destructor method, clears nodes member.
	 */
    virtual ~VirtualKeyModule();

    /** This method is called to construct a new Node. It compares
     * name to the VirtualKeySource element name, and if it matches
     * creates a new VirtualKeySource node.
     * @param name reference to string containing element name
     * @attributes refenrence to StringTable containing attribute values
     * @return pointer to new Node or NULL. The new Node must be
     * allocated with new ! 
	 */
    virtual Node * createNode( const std::string& name,  const StringTable& attributes);

	/**
	 *  stop the thread
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
	 *  checks all VirtualKeySources and pushes new events into the tracker tree
	 */
	virtual void pushEvent();

	
	/**
	 *  the working thread loop
	 */
	void processLoop();

	friend class VirtualKeySource;

};

OT_MODULE(VirtualKeyModule);
} // namespace ot

#else
#ifdef WIN32
#pragma message(">>> OT_NO_VIRTUALKEY_SUPPORT")
#endif
#endif	//USE_VIRTUALKEYSOURCE

#endif	//_VIRTUALKEYMODULE_H
