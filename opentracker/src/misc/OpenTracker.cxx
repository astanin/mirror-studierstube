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
/** Source file for OpenTracker. It contains global static definitions and
 * global functions etc. 
 *
 * @author Gerhard Reitmayr
 *
 * $Id: OpenTracker.cxx 2133 2008-03-11 17:06:47Z mendez $
 * @file                                                                   */
/* ======================================================================= */

// prevent ACE from defining _WIN32_WINNT to 0x0400...
#ifdef WIN32 
#define _WIN32_WINNT 0x0501
#endif

#ifdef WIN32
// a trick to avoid warnings when ace includes the STL headers
#pragma warning(disable:4786)
#endif

#include <OpenTracker/OpenTracker.h>

#include <stdlib.h>
#include <string>

#include <ace/ACE.h>


#include <OpenTracker/dllinclude.h>
#include <OpenTracker/input/RawInputModule.h>
#include <OpenTracker/core/Event.h>
#include <OpenTracker/core/Context.h>
#include <OpenTracker/common/CommonNodeFactory.h>
#include <OpenTracker/core/TestModule.h>
#include <OpenTracker/core/Configurator.h>

// the following modules are not supported under Windows CE
#ifndef _WIN32_WCE
#include <OpenTracker/common/ConsoleModule.h>
#ifndef DARWIN
#include <OpenTracker/input/InterSenseModule.h>
#include <OpenTracker/input/ParButtonModule.h>
#endif //DARWIN
#include <OpenTracker/input/XSensModule.h>
#include <OpenTracker/input/DynaSightModule.h>
#include <OpenTracker/input/FastTrakModule.h>
#include <OpenTracker/input/FOBModule.h>
#include <OpenTracker/input/ARTDataTrackerModule.h>
#endif //_WIN32_WCE


#include <OpenTracker/input/OrientationModule.h>
#include <OpenTracker/network/NetworkSinkModule.h>
#include <OpenTracker/network/NetworkSourceModule.h>
#include <OpenTracker/common/TimeModule.h>
#include <OpenTracker/common/FileModule.h>
#include <OpenTracker/network/TCPModule.h>

#include <OpenTracker/input/UltraTrakModule.h>
#include <OpenTracker/common/GroupGateModule.h>
#include <OpenTracker/common/InterpolatorModule.h>
#include <OpenTracker/common/ButtonHoldFilterModule.h>
#include <OpenTracker/input/GPSModule.h>
#include <OpenTracker/input/MobilabModule.h>
#include <OpenTracker/input/MagicYModule.h>
#include <OpenTracker/common/CallbackModule.h>
#include <OpenTracker/common/CallforwardModule.h>
#include <OpenTracker/common/LogModule.h>

// these modules depend on compile options
//#include <OpenTracker/input/StbTrackerModule.h>

#include <OpenTracker/input/ARToolKitModule.h>
#include <OpenTracker/input/ARToolKitPlusModule.h>
#include <OpenTracker/input/CyberMouseModule.h>
#ifndef __APPLE__
#include <OpenTracker/input/LinmouseModule.h>
#include <OpenTracker/input/MidiModule.h>
#include <OpenTracker/input/ICubeXModule.h>
#endif
#include <OpenTracker/input/WacomGraphireModule.h>
#include <OpenTracker/input/WiiModule.h>
#include <OpenTracker/input/JoystickModule.h>
#include <OpenTracker/input/SpaceDeviceModule.h>
#include <OpenTracker/input/GoGoModule.h>
#include <OpenTracker/input/PanTiltUnitModule.h>
#include <OpenTracker/input/SysMouseModule.h>
#include <OpenTracker/common/ThreeToTwoDimFilterModule.h>
#include <OpenTracker/common/VirtualKeyModule.h>
//#include <OpenTracker/input/SpeechModule.h>               // DEPRECATED Mendez 20070606
#include <OpenTracker/input/P5GloveModule.h>
#include <OpenTracker/input/MulticastInputModule.h>
#include <OpenTracker/input/UbisenseModule.h>
#include <OpenTracker/input/OpenVideoModule.h>
#include <OpenTracker/network/DwarfModule.h>
#include <OpenTracker/network/VRPNModule.h>
#include <OpenTracker/network/CORBAModule.h>
#include <OpenTracker/network/PhantomMiddlewareModule.h>
#include <OpenTracker/common/TimestampGeneratorModule.h>
#include <OpenTracker/network/OldOTNetworkSourceModule.h>
#include <OpenTracker/otpy/PythonModule.h>

#include <OpenTracker/input/PhantomModule.h>
#include <OpenTracker/input/PhantomSourceModule.h>
#include <OpenTracker/input/PhantomSinkModule.h>

// DLL main function

#ifdef WIN32

#ifndef OPENTRACKER_STATIC
BOOL APIENTRY DllMain( HANDLE hModule, 
		       DWORD  ul_reason_for_call, 
		       LPVOID lpReserved
		       )
{
    switch( ul_reason_for_call )
    {
        case DLL_PROCESS_ATTACH:		
            ACE::init();
            break;
        case DLL_THREAD_ATTACH:
            break;
        case DLL_THREAD_DETACH:		
            break;
        case DLL_PROCESS_DETACH:		
            ACE::fini();
            break;	
    }
    return TRUE;
}
#endif //OPENTRACKER_STATIC

#endif //WIN32

// initializes context

namespace ot {


    extern "C" {
        OT_MODULE_REGISTER_FUNC(OpenTracker){

            // Instance the default modules and add to factory and parser
            CommonNodeFactory * common = new CommonNodeFactory;
            context->addFactory( *common );

            LogModule * log = new LogModule;
            context->addModule( "LogConfig", *log );
        }

    }


    // Register all known module initialization functions.

    void OPENTRACKER_API initializeOpenTracker(){
        // one time initializations
        // register the creator functions and generic names of all known types
        logPrintI("Initializing Opentracker\n");
        Event::registerAllKnownTypes();

        //register Modules and Factories
        OT_REGISTER_MODULE(OpenTracker, NULL);

#ifndef OT_NO_TESTMODULE_SUPPORT
        OT_REGISTER_MODULE(TestModule, NULL);
#endif

#ifndef OT_NO_ORIENTATION_SUPPORT		
        OT_REGISTER_MODULE(OrientationModule, NULL);
#endif

#ifndef OT_NO_NETWORK_SUPPORT
        OT_REGISTER_MODULE(NetworkSinkModule, NULL);
#endif


#ifndef _WIN32_WCE
#ifndef OT_NO_CONSOLE_SUPPORT
        OT_REGISTER_MODULE(ConsoleModule, NULL);
#endif

#ifndef OT_NO_NETWORK_SUPPORT
        OT_REGISTER_MODULE(NetworkSourceModule, NULL);
#endif


#endif //_WIN32_WCE

#ifndef __APPLE__
#ifdef USE_MIDI
        OT_REGISTER_MODULE(MidiModule, NULL);
#endif

#ifdef USE_ICUBEX
        OT_REGISTER_MODULE(ICubeXModule, NULL);
#endif

#endif

#ifdef USE_ARTOOLKIT
        logPrintW("Registering ARToolkit Module\n");
        OT_REGISTER_MODULE(ARToolKitModule, NULL);
#endif

#ifdef USE_ARTOOLKITPLUS
        OT_REGISTER_MODULE(ARToolKitPlusModule, NULL);
#endif

#ifdef USE_STBTRACKER
        OT_REGISTER_MODULE(StbTrackerModule, NULL);
#endif

#ifdef OT_OLD_NETWORK_SUPPORT
        OT_REGISTER_MODULE(OldOTNetworkSourceModule, NULL );
#endif

#ifdef USE_OPENVIDEO
        OT_REGISTER_MODULE(OpenVideoModule, NULL);
#endif

#ifdef USE_WACOMGRAPHIRE
        OT_REGISTER_MODULE(WacomGraphireModule, NULL);
#endif

#ifdef USE_CYBERMOUSE
        OT_REGISTER_MODULE(CyberMouseModule, NULL);
#endif	

#ifdef USE_WII_SUPPORT
        OT_REGISTER_MODULE(WiiModule, NULL);
#endif

//#ifdef USE_LINMOUSEMODULE
#ifndef WIN32
#ifndef __APPLE__
	OT_REGISTER_MODULE(LinmouseModule, NULL);
#endif
#endif	

#ifdef USE_JOYSTICK
        OT_REGISTER_MODULE(JoystickModule, NULL);
#endif

#ifdef USE_SPACEDEVICE
        OT_REGISTER_MODULE(SpaceDeviceModule, NULL);
#endif	

#ifdef USE_GOGO
		OT_REGISTER_MODULE(GoGoModule, NULL);
#endif	

#ifdef USE_PANTILTUNIT
		OT_REGISTER_MODULE(PanTiltUnitModule, NULL);
#endif	

#ifdef USE_SYSMOUSE
		OT_REGISTER_MODULE(SysMouseModule, NULL);
#endif

#ifdef USE_VIRTUALKEYSOURCE
		OT_REGISTER_MODULE(VirtualKeyModule, NULL);
#endif	

#ifdef USE_THREETOTWODIMFILTER
		OT_REGISTER_MODULE(ThreeToTwoDimFilterModule, NULL);
#endif	

#ifndef OT_NO_NETWORK_SUPPORT
        OT_REGISTER_MODULE(MulticastInputModule, NULL);
#endif

#ifndef _WIN32_WCE
#ifndef DARWIN
#ifndef OT_NO_INTERSENSE_SUPPORT
        OT_REGISTER_MODULE(InterSenseModule, NULL);
#endif
#endif //DARWIN
#endif //_WIN32_WCE

#ifndef OT_NO_TIMEMODULE_SUPPORT
        OT_REGISTER_MODULE(TimeModule, NULL);
#endif

#ifndef OT_NO_FILEMODULE_SUPPORT
        OT_REGISTER_MODULE(FileModule, NULL);
#endif

#ifndef _WIN32_WCE
#ifndef __APPLE__
#ifndef OT_NO_PARBUTTON_SUPPORT
        OT_REGISTER_MODULE(ParButtonModule, NULL);
#endif
#endif //DARWIN
#endif //_WIN32_WCE

#ifndef OT_NO_NETWORK_SUPPORT
        OT_REGISTER_MODULE(TCPModule, NULL);
#endif

#ifndef _WIN32_WCE
#ifndef OT_NO_FOB_SUPPORT
        OT_REGISTER_MODULE(FOBModule, NULL);
#endif
#endif //_WIN32_WCE

#ifndef _WIN32_WCE
#ifndef OT_NO_FASTTRACK_SUPPORT
        OT_REGISTER_MODULE(FastTrakModule, NULL);
			
#endif
#endif //_WIN32_WCE

#ifndef _WIN32_WCE
#ifndef OT_NO_ARTDATATRACKER_SUPPORT
        OT_REGISTER_MODULE(ARTDataTrackerModule, NULL);
#endif
#endif //_WIN32_WCE

#ifndef OT_NO_ULTRATRACK_SUPPORT
        OT_REGISTER_MODULE(UltraTrakModule, NULL);
#endif

#ifndef OT_NO_GROUPGATE_SUPPORT
        OT_REGISTER_MODULE(GroupGateModule, NULL);
#endif
    
//#ifndef OT_NO_SPEECH_SUPPORT                                  // DEPRECATED Mendez 20070606
//        OT_REGISTER_MODULE(SpeechModule, NULL);
//#endif

#ifndef OT_NO_INTERPORLATORMODULE_SUPPORT
        OT_REGISTER_MODULE(InterpolatorModule, NULL);
#endif

#ifndef OT_NO_BUTTON_SUPPORT
        OT_REGISTER_MODULE(ButtonHoldFilterModule, NULL);
#endif

#ifdef USE_P5GLOVE
        OT_REGISTER_MODULE(P5GloveModule, NULL);
#endif

#ifndef OT_NO_GPS_SUPPORT
        OT_REGISTER_MODULE(GPSModule, NULL);
#endif

#ifndef EXCLUDE_MOBILAB_SUPPORT
        OT_REGISTER_MODULE(MobilabModule, NULL);
#endif

#ifndef _WIN32_WCE
#ifndef OT_NO_DYNASIGHT_SUPPORT
        OT_REGISTER_MODULE(DynaSightModule, NULL);
#endif
#endif //_WIN32_WCE


#ifndef OT_NO_MAGICY_SUPPORT
        OT_REGISTER_MODULE(MagicYModule, NULL);
#endif
    
#ifndef OT_NO_CALLBACKMODULE_SUPPORT
        OT_REGISTER_MODULE(CallbackModule, NULL);
#endif

#ifndef OT_NO_CALLFORWARDMODULE_SUPPORT
        OT_REGISTER_MODULE(CallforwardModule, NULL);
#endif

#ifdef USE_DWARF
        OT_REGISTER_MODULE(DwarfModule, NULL);
#endif

#ifdef USE_CORBA
        OT_REGISTER_MODULE(CORBAModule, NULL);
#endif

#ifdef USE_PHANTOMMIDDLEWARE
        OT_REGISTER_MODULE(PhantomMiddlewareModule, NULL);
#endif

#ifdef USE_VRPN
        OT_REGISTER_MODULE(VRPNModule, NULL);
#endif

#ifndef _WIN32_WCE
#ifndef OT_NO_XSENS_SUPPORT
        OT_REGISTER_MODULE(XSensModule, NULL);
#endif
#endif //_WIN32_WCE

#ifdef USE_UBISENSE
        OT_REGISTER_MODULE(UbisenseModule, NULL);
#endif

#ifdef USE_RAWINPUT
#ifdef WIN32
        OT_REGISTER_MODULE(RawInputModule, NULL);
#endif
#endif

#ifndef OT_NO_TIMESTAMPGENERATOR_SUPPORT
        OT_REGISTER_MODULE(TimestampGeneratorModule, NULL);
#endif

#ifdef USE_PHANTOM
        OT_REGISTER_MODULE(PhantomModule, NULL);
        OT_REGISTER_MODULE(PhantomSourceModule, NULL);
        OT_REGISTER_MODULE(PhantomSinkModule, NULL);
#endif


#ifdef USE_PYTHON
        OT_REGISTER_MODULE(PythonModule, NULL);
#endif
        logPrintI("Initializing Done\n");
	}

    void OPENTRACKER_API initializeContext( Context * context , void *)
    {
        //one time initialization stuff goes here.
        // register the creator functions and generic names of all known types
        Event::registerAllKnownTypes();
        // register all creation functions within the Configurator registry
        initializeOpenTracker();
	
        // use the registered functions to initialize the context. 
        Configurator::doInitialization(*context);

    }

} // namespace ot

/* 
 * ------------------------------------------------------------
 *   End of OpenTracker.cxx
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
