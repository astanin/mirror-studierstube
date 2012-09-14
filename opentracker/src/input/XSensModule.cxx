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
/** source file for XSensModule
 *
 * @author Gerhard Reitmayr
 * 
 * $Id: XSensModule.cxx 717 2004-07-27 11:54:49Z reitmayr $
 *
 * @file                                                                   */
/* ======================================================================= */

#include <ace/Log_Msg.h>

#ifdef WIN32
#include <objbase.h>
#include <xsens/IMTObj.h>
#include <xsens/IMTObj_i.c>
#endif

#include <OpenTracker/input/XSensModule.h>
#include <OpenTracker/input/XSensSource.h>


#ifndef OT_NO_XSENS_SUPPORT


namespace ot {

	OT_MODULE_REGISTER_FUNC(XSensModule){
		OT_MODULE_REGISTRATION_DEFAULT(XSensModule , "XSensConfig" );
	}


    XSensModule::XSensModule() :
        source( NULL )
    {
#ifdef WIN32
	HRESULT hr;

	// first try multi-threaded. if that fails try single threaded...
	//
	hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if ( hr != S_OK && hr != S_FALSE )
	{
            hr = CoInitialize(NULL); 
            if ( hr != S_OK && hr != S_FALSE )
            {
                ACE_DEBUG((LM_ERROR,"XSensModule: failed to initialize COM library!\n" ));
                exit(1);
            }
	}
#endif
    }

    XSensModule::~XSensModule()
    {
        if( source != NULL )
            delete source;

#ifdef WIN32
        CoUninitialize();
#endif
    }

    Node * XSensModule::createNode( const std::string & name, const StringTable & attributes )
    {
	if( name.compare("XSensSource") == 0 )
	{
            if( source != NULL )
            {
                ACE_DEBUG((LM_ERROR, "XSensModule: Only one GPSSource can be build !\n" ));
                return NULL;
            }
            int comport = 1;
            attributes.get("comport", &comport);        
            source = new XSensSource( comport );

            ACE_DEBUG((LM_INFO, "XSensModule: Built XSensSource for comport %i\n", comport ));

            initialized = true;
            return source;
	}    
	return NULL;	
    }

    void XSensModule::pushEvent()
    {
        // nothing to do
    }

    void XSensModule::start()
    {
        if( source != NULL )
        {
#ifdef WIN32
            source->pMT->MT_StartProcess();
#endif
        }
    }

    void XSensModule::close()
    {
        if( source != NULL )
        {
#ifdef WIN32
            source->pMT->MT_StopProcess();
#endif
        }
    }


}  // namespace ot


#endif // OT_NO_XSENS_SUPPORT

/* 
 * ------------------------------------------------------------
 *   End of XSensModule.cxx
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
