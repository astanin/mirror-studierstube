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
/** header file for OpenVideo module.
 *
 * @author Daniel Wagner
 *
 * $Id: ARToolKitModule.h 900 2006-01-19 16:47:43Z spiczak $
 * @file                                                                   */
/* ======================================================================= */


#ifndef _OPENVIDEOMODULE_H
#define _OPENVIDEOMODULE_H

#include "../OpenTracker.h"

#ifdef USE_OPENVIDEO


namespace openvideo{
    class Manager;
    class Buffer;
}


namespace ot {


    class OpenTrackerVideoSinkSubscriber;


    class OPENTRACKER_API OpenVideoModule : public ThreadModule
    {
    protected:
        /** the work method for the module thread. This method grabs a frame
         * and computes any marker information from it. Then it updates the
         * source nodes accordingly. If the stop flag is set, it finishes.*/
        virtual void run(); 

	std::string configFile, ovSinkName;

	openvideo::Manager*		ovManager;
	OpenTrackerVideoSinkSubscriber*	videoSinkSubscriber;

	Context*				context;

    public:
        /// constructor method
        OpenVideoModule(Context* ctx);

        /// destructor method
        virtual ~OpenVideoModule();

        /**
         * This method is called after initialisation is finished and before the
         * main loop is started. It initializes the OpenVideo library */
        virtual void start();

        virtual void close();

        /**
         * initializes the OpenVideo module. 
         * @param attributes StringMap of elements attribute values. Should be
         *        possibly , but is not for convenience.
         * @param localTree pointer to root of configuration nodes tree
         */
        virtual void init(StringTable& attributes, ConfigNode * localTree);
    };
	OT_MODULE(OpenVideoModule);

} // namespace ot

#endif  // USE_OPENVIDEO

#endif  // _OPENVIDEOMODULE_H

/* 
 * ------------------------------------------------------------
 *   End of OpenVideoModule.h
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
