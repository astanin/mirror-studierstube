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
/** source file containing the main function and test code 
 * for the test program use.
 *
 * @author Alexander Bornik
 *
 * $Id$
 * @file                                                                   */
/* ======================================================================= */

#include <OpenTracker/OpenTracker.h>

#include <iostream>

using namespace std;

#include <OpenTracker/common/CallbackModule.h>
#include <OpenTracker/common/CallbackNode.h>
#include <OpenTracker/common/CallforwardModule.h>

using namespace ot;


void clientACB( ot::CallbackNode * node,  Event & event, void * data ){
    double diff = (OSUtils::currentTime() - event.time ) / 1000;
    cout << node->getName() << " time diff " << diff << endl;

}

void clientBCB( ot::CallbackNode * node,  Event & event, void * data ){
    double diff = (OSUtils::currentTime() - event.time ) / 1000;
    cout << node->getName() << " time diff " << diff << endl;

}

void globalClientCB( CallbackNode * node,  Event & event, void * data ){
    using namespace std;

    cout << "This is the global callback function." << endl;
}

/**
 * The main function for the test program. It expects a
 * filename as argument, tries to parse the configuration file
 * and starts the main loop, if successful
 */
int main(int argc, char **argv)
{
    if( argc != 2 )
    {
        cout << "Usage : " << argv[0] << " configfile" << endl;
        return 1;
    }

    cout << "OpenTracker Callback/Callforward test program ...\n";

    // context initialization
    Context context( 1 );
    
    cout << "Context established." << endl;

    // gain access to the interface modules for reading and writing
    // events from/to the OpenTracker Graph
    CallbackModule * cbModule = 
      dynamic_cast<CallbackModule*>(context.getModule("CallbackConfig"));
    CallforwardModule * cfModule = 
      dynamic_cast<CallforwardModule*>(context.getModule("CallforwardConfig"));

    // make sure that the needed modules are in OpenTracker
    if (cbModule == NULL)
    {
        cout << "Your OpenTracker does no contain the CallbackModule!" << endl;
        exit(1);
    }
    if (cbModule == NULL)
    {
        cout << "Your OpenTracker does no contain the CallbackModule!" << endl;
        exit(1);
    }

    // parse the configuration file, builds the tracker tree
    context.parseConfiguration( argv[1] );


    cbModule->setCallback( "clientA", clientACB );
    cbModule->setCallback( "clientB", clientBCB );
    cbModule->setGlobalCallback( globalClientCB);
    
    context.run();
    return 0;
}

/* 
 * ------------------------------------------------------------
 *   End of cbcfexample.cxx
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
