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
/** source file for Time module.
 *
 * @author Gerhard Reitmayr
 *
 * $Id: TimeModule.cxx 1547 2006-10-25 10:33:10Z veas $
 * @file                                                                   */
/* ======================================================================= */

// this will remove the warning 4786
#include <OpenTracker/tool/disable4786.h>

#include <cstdlib>
#include <OpenTracker/common/TimeModule.h>
#include <cstdio>

//using namespace std;

#include <ace/Log_Msg.h>


#ifndef OT_NO_TIMEMODULE_SUPPORT


namespace ot {

	OT_MODULE_REGISTER_FUNC(TimeModule){
		TimeModule * time = new TimeModule();
		context->addModule( "TimeConfig", * time );
	}

    void TimeModule::init( StringTable & attributes,  ConfigNode * localTree)
    {
        Module::init( attributes, localTree );
    
        int num;   
        if( attributes.get("sleep").compare("") != 0 )
        {
            num = sscanf(attributes.get("sleep").c_str(), " %i", &sleep );
            if( num == 0 )
            {
                sleep = 1;
            }
        } else if( attributes.get("rate").compare("") != 0 )
	{
            num = sscanf( attributes.get("rate").c_str(), " %lf", &rate );
            if( num == 0 )
	    {
                rate = 0.1;
	    } else 
            {
		rate = rate / 1000;
            }
	}
        if( attributes.get("display").compare("true") == 0 )
        {
            display = 1;
        }
    }
  
    void TimeModule::start()
    {
        if( isInitialized() == 1 )
        {
            count = 0;
            startTime = OSUtils::currentTime();
        }    
    } 
    
    int TimeModule::stop()
    {    
        count++;
        if( rate != 0 )
        {        
            double s = count/rate - ( OSUtils::currentTime() - startTime );
            if( s >= 10 )
            {
                OSUtils::sleep( s );
            }
        }
        else if( sleep != 0 )
        {
            OSUtils::sleep( sleep );
        }
        return 0;
    }

    void TimeModule::close()
    {
        if( display == 1 )
        {        
            ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Framerate %f\n"), count * 1000 / ( OSUtils::currentTime() - startTime )));
        }
    }

} // namespace ot


#else
#pragma message(">>> OT_NO_TIMEMODULE_SUPPORT")
#endif // OT_NO_TIMEMODULE_SUPPORT

/* 
 * ------------------------------------------------------------
 *   End of TimeModule.cxx
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
