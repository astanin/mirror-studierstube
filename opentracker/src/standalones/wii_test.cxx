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
 * @author Michele Fiorentino
 *
 * $Id: main.cxx
 * @file                                                                   */
/* ======================================================================= */

#include <OpenTracker/OpenTracker.h>
#include <OpenTracker/core/Configurator.h>
#include <ace/Signal.h>
#include <iostream>
#include <string>
#include <OpenTracker/core/ThreadContext.h>

#include <OpenTracker/common/CallforwardModule.h>
#include <OpenTracker/common/CallforwardNode.h>
#include <OpenTracker/input/WiiModule.h>

using namespace std;
using namespace ot;

extern "C" void
SIGINThandler (int signum, siginfo_t*, ucontext_t*)
{ 
   Configurator::instance(ot::THREAD)->getContext()->stopLoop();
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
	// added already ( because of the parameter 1 )
   ThreadContext & context = (*dynamic_cast<ThreadContext*>((Configurator::instance(ot::THREAD) ->getContext())));
	cout << "Context established." << endl;

   ACE_Sig_Action sa((ACE_SignalHandler)SIGINThandler, SIGINT);
	Configurator::instance(ot::THREAD) ->runConfigurationThread( filename );
	// parse the configuration file, builds the tracker tree
	context.parseConfiguration( argv[1] );

    printf("OT |SETUP : Parsing Complete\n");
    printf("OT |INFO : Starting Mainloop\n");

    // initializes the modules and starts the tracker main loop
    //context.run();
    context.runOnDemand(); // otherwise it stops
/*
    CallforwardModule * cfm = (CallforwardModule * )context.getModule("CallforwardConfig");
    WiiModule* wii = (WiiModule*) context.getModule("WiiConfig");
    if (wii) printf("OT |INFO : Wii founded\n");
*/

    while (1)
    {
       Sleep(2000);
    //kbhit() 
//     printf(">");
 /*
           int i = 0;
           Event ev;
           if (wii->wiimote->mLastButtonStatus.mA)//wii->wiimote->mLastButtonStatus.mA
            {
               printf("Pressed %d A\n",i);
               ev.setAttribute("int","VibroFreqency","100");
               ev.setAttribute("int","Led","2");
            }
            else if (i == 300)
            {
               printf("Pressed B\n");
               ev.setAttribute("int","VibroFreqency","0"); 
               ev.setAttribute("int","Led","2");
            } else ev.setAttribute("int","Led","255");
            cfm->callForward("cf1", ev);*/
      
   } 
    
    /*
    char command;
    while (1)
    {

       //
       printf(">>> ");
      cin >> command;
      cout << "command: " <<  command << endl; 
      if (command == '1') 
      {
         ev.setAttribute("int","Led","2");
         cfm->callForward("cf1", ev);
      }

      
      switch(command)
      {

      case ('1'):
         {
            printf(">>>pressed 1 ");
            //context.stopLoop();
            ev.setAttribute("int","Led","2");
            //ev.setAttribute("int","Vibro","50");
            //ev.setAttribute("int","VibroFreqency","50");
      break;   
      }

      case ('2'):
         {
   //         context.stopLoop();
            ev.setAttribute("int","Led","10");
            ev.setAttribute("int","VibroFreqency","100");
            ev.setAttribute("int","Vibro","100");
break;
         }

      default:
         {
            Event ev;
            ev.setAttribute("int","Led","4");

         }
      } // end switch


    }
    printf("OT |INFO : Context closed\n");
    */
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
