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
/** header file for Mobilab_Handler
 *
 * @author Alexander Bornik
 * 
 * $Id$
 *
 * @file                                                                   */
/* ======================================================================= */

#ifndef _MOBILAB_HANDLER_H
#define _MOBILAB_HANDLER_H

#include <ace/Version.h>
#include <ace/Connector.h>
#include <ace/TTY_IO.h>
#include <ace/DEV_Connector.h>
#include <ace/Svc_Handler.h>
#include "../OpenTracker.h"

#ifndef EXCLUDE_MOBILAB_SUPPORT

namespace ot {


    class MobilabDriver;

    /**
     * This class retrieves data from the g.Mobilab device via the serial port.
     * It forwards the data to the MobilabDriver.
     * @ingroup input
     * @author Alexander Bornik
     */

    class Mobilab_Handler : public  ACE_Svc_Handler<ACE_TTY_IO, ACE_MT_SYNCH>
    {
    public:
	/// default constructor for ace framework. Do not use !
	Mobilab_Handler() {};

	Mobilab_Handler( MobilabDriver * parent_ );
	virtual ~Mobilab_Handler();
	
	virtual int open( void * factory );
	virtual int handle_input(ACE_HANDLE fd=ACE_INVALID_HANDLE);
	virtual int handle_signal( int, siginfo_t *, ucontext_t * );

    protected:     
	MobilabDriver * parent;
        // buffer for storing complete channel set data
	short mobilabbuf[16];
        // buffer fill counter
        int mobilabind;
    };

    typedef ACE_Connector<Mobilab_Handler, ACE_DEV_CONNECTOR> Mobilab_Connector;

}  // namespace ot



#endif //#ifdef EXCLUDE_MOBILAB_SUPPORT
#endif // !defined(_MOBILAB_HANDLER_H)

/* 
 * ------------------------------------------------------------
 *   End of Mobilab_Handler.h
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
