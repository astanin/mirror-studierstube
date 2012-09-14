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
/** source file for CyberMouseModule module.
 *
 * @author Ivan Viola, Matej Mlejnek
 *
 * $Id: CyberMouseModule.cxx 1626 2006-11-14 16:27:12Z mendez $
 * @file                                                                   */
/* ======================================================================= */

// this will remove the warning 4786
#include <OpenTracker/tool/disable4786.h>

#include <OpenTracker/input/CyberMouseSource.h>
#include <OpenTracker/input/CyberMouseModule.h>

#ifdef USE_CYBERMOUSE

#include <cstdio>
#ifdef WIN32
#include <iostream>    // VisualC++ uses STL based IOStream lib
#else
#include <iostream>
#endif

#include "freeddll.h"

#include <ace/Log_Msg.h>

//using namespace std;

#define SZ_Get_FREED_Info   "Get_FREED_Info"
#define SZ_OpenFREED        "OpenFREED"
#define SZ_CloseFREED       "CloseFREED"

namespace ot {

	OT_MODULE_REGISTER_FUNC(CyberMouseModule){		
			OT_MODULE_REGISTRATION_DEFAULT(CyberMouseModule, "CyberMouseModule" );
	}

    typedef ULONG (FAR PASCAL *PFNGet_FREED_Info)(FREED_info *Info);
    extern PFNGet_FREED_Info lpfnGet_FREED_Info;

    typedef ULONG (FAR PASCAL *PFNOpenFREED)(DWORD,HWND);
    extern PFNOpenFREED lpfnOpenFREED;

    typedef ULONG (FAR PASCAL *PFNCloseFREED)();
    extern PFNCloseFREED lpfnCloseFREED;

    PFNGet_FREED_Info lpfnGet_FREED_Info;
    PFNOpenFREED lpfnOpenFREED;
    PFNCloseFREED lpfnCloseFREED;
    FREED_info FREED_Info;
    WORD FREED_Opened;

    HANDLE hLibrary;
    HWND hWndCyber;


    long FAR PASCAL WndProc (HWND hWndCyber, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        return DefWindowProc(hWndCyber, uMsg, wParam, lParam);
    }

    // Destructor method
    CyberMouseModule::~CyberMouseModule()
    {
        nodes.clear();
    }

    // This method is called to construct a new Node.
    Node * CyberMouseModule::createNode( const string& name, StringTable& attributes)
    {
        if( name.compare("CyberMouseSource") == 0 )
        {       
            CyberMouseSource * source = new CyberMouseSource;
            source->event.getConfidence() = 1.0f;
            nodes.push_back( source );
            logPrintI("Build CyberMouseSource node\n");
            initialized = 1;
            return source;
        }
        return NULL;
    }

    // opens CyberMouse library
    void CyberMouseModule::start()
    {
        if( isInitialized() == 1 )
        {
            WNDCLASS  wndclass ;

            // register FREED window class
            wndclass.style =         0 ;
            wndclass.lpfnWndProc =   WndProc ;
            wndclass.cbClsExtra =    0 ;
            wndclass.cbWndExtra =    0; 
            wndclass.hInstance =     NULL ;
            wndclass.hIcon =         NULL;
            wndclass.hCursor =       NULL;
            wndclass.hbrBackground = 0;
            wndclass.lpszMenuName =  NULL ;
            wndclass.lpszClassName = "dummyClassCyberMouseModule";

            RegisterClass(&wndclass) ;

            hWndCyber = CreateWindow(
                                     "dummyClassCyberMouseModule",
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

            if (!hWndCyber)
            {
                ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Could not create message-only window.\nFatal Error\n")));
                exit(1);
            }

            if ((hLibrary = LoadLibrary("FREEDDLL.DLL")) == NULL) {
                ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Error Loading FREEDDLL.DLL.\nFatal Error\n")));
                exit(1);
            }

            if ((lpfnGet_FREED_Info = (PFNGet_FREED_Info)GetProcAddress((HMODULE)hLibrary, SZ_Get_FREED_Info)) == NULL)
                exit(1);
            if ((lpfnOpenFREED = (PFNOpenFREED)GetProcAddress((HMODULE)hLibrary,SZ_OpenFREED)) == NULL)
                exit(1);
            if ((lpfnCloseFREED = (PFNCloseFREED)GetProcAddress((HMODULE)hLibrary,SZ_CloseFREED)) == NULL)
                exit(1);

            if (lpfnOpenFREED != NULL) {
                lpfnOpenFREED(0,hWndCyber);
                FREED_Opened = TRUE;
            }

            // get initial FREED information
            if (lpfnGet_FREED_Info != NULL) lpfnGet_FREED_Info(&FREED_Info);
        }
    }

    // closes CyberMouse library
    void CyberMouseModule::close()
    {
        if( isInitialized() == 1 && lpfnCloseFREED != NULL) {
            FREED_Opened = FALSE;
            lpfnCloseFREED();
        }
        if (hLibrary != NULL) FreeLibrary((HMODULE)hLibrary);
        PostQuitMessage(0);
    }

    // pushes events into the tracker tree.
    void CyberMouseModule::pushEvent()
    {
        if( isInitialized() == 1 )
        {
            lpfnGet_FREED_Info(&FREED_Info);
            if( ! (FREED_Info.Status & (NewPosition | NewButton_Status)))
                return;
        }
        for( NodeVector::iterator it = nodes.begin(); it != nodes.end(); it++ )
        {
            CyberMouseSource * source = (CyberMouseSource *)(*it);
            source->event.getPosition()[0] = FREED_Info.AbsX;
            source->event.getPosition()[1] = FREED_Info.AbsY;
            source->event.getPosition()[2] = FREED_Info.AbsZ;
            //middle button = 1, second button = 2, both = 3
            source->event.getButton() = (FREED_Info.Status & 0x3);
            source->event.timeStamp();
            source->updateObservers( source->event );
        }
    }

} // namespace ot


#else
#ifdef WIN32
#pragma message(">>> no CyberMouse support")
#endif
#endif

/* 
 * ------------------------------------------------------------
 *   End of CyberMouseModule.cxx
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
