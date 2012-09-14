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
/** source file containing the main function for standalone use using
 *  a ubitrack server.
 *
 * @author Alexander Bornik
 *
 * $Id$
 * @file                                                                   */
/* ======================================================================= */

#include <OpenTracker/OpenTracker.h>
#include <OpenTracker/ubitrack/UbitrackClient.h>
#include <ace/Signal.h>
#include <iostream>

using namespace std;
using namespace ot;

extern "C" void
SIGINThandler (int signum, siginfo_t*, ucontext_t*)
{ 
    UbitrackClient::instance()->getContext()->stopLoop();

    logPrintI("Received SIGINT -> gracefully shutting down!\n");
}


/**
 * The main function for the standalone program. It expects a
 * filename as argument, tries to parse the configuration file
 * and starts the main loop, if successful
 */
int main(int argc, char **argv)
{
    const char * filename;
    const char * servername;
    if( argc != 3 )
    {
        cout << "Usage : " << argv[0] << " server configfile" << endl;
        return 1;
    }
    servername = argv[1];
    filename  = argv[2];

    // important parts of the system
    // get a context, the default modules and factories are
    // added allready ( because of the parameter 1 )
    Context & context = (*(UbitrackClient::
                           instance(ACE_INET_Addr(3000,servername))
                           ->getContext()));

    cout << "Context established." << endl;

    // signal handling for CTRL-C
    ACE_Sig_Action sa((ACE_SignalHandler)SIGINThandler, SIGINT);

    // parse the configuration file, builds the tracker tree
    UbitrackClient::instance()->sendUTQLFile(filename);
  
    printf("OT |SETUP : Parsing Complete\n");
    printf("OT |INFO : Starting Mainloop\n");

    // initializes the modules and starts the tracker main loop
    context.runOnDemand();
    printf("OT |INFO : Context closed\n");
    OSUtils::sleep(1000);
    return 0;
}

/* 
 * ------------------------------------------------------------
 *   End of main.cxx
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
