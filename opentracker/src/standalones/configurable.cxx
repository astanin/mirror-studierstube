
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
/** source file containing the main function for standalone use.
 *
 * @author Eduardo E. Veas
 *
 * $Id: main.cxx 1164 2006-06-14 05:27:23Z bornik $
 * @file                                                                   */
/* ======================================================================= */

#include <ace/Thread.h>
#include <OpenTracker/OpenTracker.h>
#include <OpenTracker/core/Configurator.h>

#include <ace/Signal.h>

#include <fstream>

using namespace std;
using namespace ot;

extern "C" void
SIGINThandler (int signum, siginfo_t*, ucontext_t*)
{ 
    logPrintI ("Received SIGINT -> gracefully shutting down!\n");
    Configurator::instance()->getContext()->stopLoop();
}


#include <OpenTracker/misc/FileConfigurationThread.h>


/**
 * The main function for the standalone program. It expects a
 * filename as argument, tries to parse the configuration file
 * and starts the main loop, if successful
 */
int main(int argc, char **argv)
{
    const char * filename;
    if( argc != 2 )
    {
        cout << "Usage : " << argv[0] << " configfile" << endl;
        return 1;
    }
    if (argc > 2)
    {
        filename = argv[2];
    } else {
        filename = argv[1];
    }

    // important parts of the system
    // get a context, the default modules and factories are
    // added allready ( because of the parameter 1 )
    Context & context = (*(Configurator::instance() ->getContext()));

    cout << "Context established." << endl;
    
    //FileConfigurationThread*  ct= new FileConfigurationThread( "reconfig.xml");

    Configurator::instance() ->runConfigurationThread( filename );
	
    // parse the configuration file, builds the tracker tree
    context.parseConfiguration( argv[1] );

    cout << "Parsing complete." << endl << endl << "Starting mainloop !" << endl;

    //ct->start();
    // initializes the modules and starts the tracker main loop
    context.run();

    //ct->finish();
    //OSUtils::sleep(1000);
    //delete ct;


    return 0;
}

/* 
 * ------------------------------------------------------------
 *   End of configurable.cxx
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
