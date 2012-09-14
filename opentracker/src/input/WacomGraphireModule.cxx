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
/** source file for WacomGraphireModule module.
 *
 * @author Ivan Viola, Matej Mlejnek
 *
 * $Id: WacomGraphireModule.cxx 1626 2006-11-14 16:27:12Z mendez $
 * @file                                                                   */
/* ======================================================================= */

// this will remove the warning 4786
#include <OpenTracker/tool/disable4786.h>

#include <OpenTracker/input/WacomGraphireSource.h>
#include <OpenTracker/input/WacomGraphireModule.h>

#ifdef USE_WACOMGRAPHIRE

#include <ace/Log_Msg.h>

#include <cstdio>
#ifdef WIN32
#include <iostream>    // VisualC++ uses STL based IOStream lib
#else
#include <iostream>
#endif

#pragma comment(lib,"wntab32x")
#pragma comment(linker, "/NODEFAULTLIB:libc")

#include "wintab.h"
/* What Wintab packet data items we want.  PK_CURSOR identifies which cursor 
   (0-5) generated the packet.  PACKETDATA must be defined before including
   pktdef.h 
*/

#define	PACKETDATA	( PK_X | PK_Y | PK_Z | PK_CURSOR | PK_BUTTONS | PK_NORMAL_PRESSURE )
#include "pktdef.h"

//using namespace std;

namespace ot {

	OT_MODULE_REGISTER_FUNC(WacomGraphireModule){
			OT_MODULE_REGISTRATION_DEFAULT(WacomGraphireModule, "WacomGraphireModule" );
	}

    HANDLE    hInst;   // Handle for instance
    PACKET    pkt;    // Packet
    HCTX    hTab;    // Handle for Tablet Context
    POINT    ptNew;    // XY value storage
    UINT    prsNew;    // Pressure value storage
    UINT    curNew;    // Cursor number storage
    UINT    butNew;    // Tilt value storage
    HWND    hWndWacom;

    // Destructor method
    WacomGraphireModule::~WacomGraphireModule()
    {
        nodes.clear();
    }

    // This method is called to construct a new Node.
    Node * WacomGraphireModule::createNode( const string& name, StringTable& attributes)
    {
        if( name.compare("WacomGraphireSource") == 0 )
        {       
            int device;
            int num = sscanf(attributes.get("device").c_str(), " %i", &device );
            if (num == 0) device = 1;
            WacomGraphireSource * source = new WacomGraphireSource(device);
            nodes.push_back( source );        
            logPrintI("Build WacomGraphireSource node\n");
            initialized = 1;
            return source;
        }
        return NULL;
    }

    LRESULT FAR PASCAL RuleAppWndProc (HWND hWndWacom, UINT wMsg, WPARAM wParam, LPARAM lParam)
    {
        return DefWindowProc(hWndWacom, wMsg, wParam, lParam);
    }


    // opens WacomGraphire library
    void WacomGraphireModule::start()
    {
        LOGCONTEXT  lcMine; // The context of the tablet 
        AXIS    TabletX, TabletY;   // The maximum tablet size
        char    WName[50];  // String to hold window name 
        WNDCLASS    wc;
	
        if( isInitialized() == 1 )
        {
            // check if WinTab available.
            if (!WTInfo(0, 0, NULL)) 
            {
                ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:WinTab Services Not Available.\nFatal Error\n")));
                return;
	    }

	    // check if WACOM available.
            WTInfo(WTI_DEVICES, DVC_NAME, WName);
            if (strncmp(WName,"WACOM",5)) 
            {
                ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Wacom Tablet Not Installed.\nFatal Error\n")));
                return;
            }
	
	    // Fill in window class structure with parameters that describe the message-only window.
            wc.style = 0;
            wc.lpfnWndProc = RuleAppWndProc;
            wc.cbClsExtra = 0;
            wc.cbWndExtra = 0;
            wc.hInstance = NULL;
            wc.hIcon = NULL;
            wc.hCursor = NULL;
            wc.hbrBackground = 0;
            wc.lpszMenuName =  NULL;
            wc.lpszClassName = "dummyClassWacomGraphireModule";

            //Register the window class and return success failure code.
            RegisterClass(&wc);

            hWndWacom = CreateWindow(
                                     "dummyClassWacomGraphireModule",
                                     NULL,
                                     NULL,
                                     0,
                                     0,
                                     CW_USEDEFAULT,
                                     CW_USEDEFAULT,
                                     NULL,
                                     NULL,
                                     NULL,
                                     NULL
                                     );

            // If window could not be created, return "failure" 
            if (!hWndWacom) 
            {
                ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Could not create message-only window\nFatal Error\n")));
                exit(1);
            }

            // get default region */
            WTInfo(WTI_DEFCONTEXT, 0, &lcMine);

            // modify the digitizing region 
            lcMine.lcOptions |= CXO_MESSAGES;
            lcMine.lcPktData = PACKETDATA;
            lcMine.lcPktMode = 0;
            lcMine.lcMoveMask = PACKETDATA;
            lcMine.lcBtnUpMask = lcMine.lcBtnDnMask;

	    // Set the entire tablet as active
            WTInfo(WTI_DEVICES,DVC_X,&TabletX);
            WTInfo(WTI_DEVICES,DVC_Y,&TabletY);
            lcMine.lcInOrgX = 0;
            lcMine.lcInOrgY = 0;
            lcMine.lcInExtX = TabletX.axMax;
            lcMine.lcInExtY = TabletY.axMax;

            // output the data in screen coords
            lcMine.lcOutOrgX = lcMine.lcOutOrgY = 0;
            lcMine.lcOutExtX = (LONG)(127.6*40);//GetSystemMetrics(SM_CXSCREEN); //127.6
            // move origin to upper left 
            lcMine.lcOutExtY = (LONG)(-92.8*40);//-GetSystemMetrics(SM_CYSCREEN); //92.8

            // open the region 
            if ((hTab = WTOpen(hWndWacom, &lcMine, TRUE)) == NULL)
            {            
                ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Failed to open WacomGraphire library !\n")));
                initialized = 0;
            }
        }
    }

    // closes WacomGraphire library
    void WacomGraphireModule::close()
    {
        if( isInitialized() == 1 && hTab)
        {
            WTClose(hTab);
        }
    }

    // pushes events into the tracker tree.
    void WacomGraphireModule::pushEvent()
    {
        if( isInitialized() == 1 )
        {
            unsigned int newest, oldest;
            if (!WTQueuePacketsEx(hTab, &oldest, &newest)) return;
            if (WTPacket(hTab, newest, &pkt))
            {
                // save new co-ordinates
                ptNew.x = (UINT)pkt.pkX;
                ptNew.y = (UINT)pkt.pkY;
                curNew = pkt.pkCursor; // penTip = 1, eraser = 2, puck = 0
                butNew = (UINT)pkt.pkButtons; // pen & eraser = 1, hi = 4, low= 2; 
                prsNew = pkt.pkNormalPressure;	// pressure					
            }
            for( NodeVector::iterator it = nodes.begin(); it != nodes.end(); it++ )
            {	
                WacomGraphireSource * source = (WacomGraphireSource *)(*it);
                if (curNew==source->device)
                {
                    source->event.getPosition()[0] = (float)(ptNew.x/40.0/1000);
                    source->event.getPosition()[1] = (float)(ptNew.y/40.0/1000);
                    source->event.getPosition()[2] = (float)(prsNew); 
                    source->event.getButton() = butNew;
                    source->event.timeStamp();
                    source->updateObservers( source->event );
                }
            }
        }
    }

} // namespace ot


#else
#ifdef WIN32
#pragma message(">>> no wacom support")
#endif
#endif

/* 
 * ------------------------------------------------------------
 *   End of WacomGraphireModule.cxx
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
