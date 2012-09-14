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
/** header file for XSensModule
 *
 * @author Gerhard Reitmayr
 * 
 * $Id: XSensModule.h 717 2004-07-27 11:54:49Z reitmayr $
 *
 * @file                                                                   */
/* ======================================================================= */
/**
 * @page module_ref Module Reference
 * @section XSensModule XSensModule 
 *
 * This module implements the device driver for XSens MT9-B inertial trackers.
 * The corresponding source node is @ref XSensSource.
 * Currently only a single MT9-B attached a comport is supported. It has no
 * configuration element but reserves the name @c XSensConfig for it.
 *
 * @note The implementation for Linux is missing. To run it on Windows, one needs
 * to install the MT9 SDK Software from Xsens. It will not work without it.
 */

#ifndef _XSENSMODULE_H
#define _XSENSMODULE_H

#include "../OpenTracker.h"


#ifndef OT_NO_XSENS_SUPPORT


namespace ot {


    class XSensSource;


    /**
     * This module simply supports the XSensSource. It uses the MTObj.dll or respective mtobj.so from the
     * bin directory.
     *
     * @ingroup input
     * @author Gerhard Reitmayr
     */
    class OPENTRACKER_API XSensModule : public Module, public NodeFactory
    {
    public:
	XSensModule();
	virtual ~XSensModule();

	virtual Node * createNode( const std::string & name, const StringTable & attributes );
	
	virtual void pushEvent();

	virtual void start();

	virtual void close();

    protected:
        XSensSource * source;
    };
	OT_MODULE(XSensModule);


}  // namespace ot


#endif // OT_NO_XSENS_SUPPORT

#endif // !defined(_XSENSMODULE_H)

/* 
 * ------------------------------------------------------------
 *   End of XSensModule.h
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
