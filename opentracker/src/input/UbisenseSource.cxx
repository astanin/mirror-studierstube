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
/** source file for Ubisense source node.
 *
 * @author Mathis Csisinko
 *
 * $Id: UbisenseSource.cxx 1547 2006-10-25 10:33:10Z veas $
 * @file                                                                   */
/* ======================================================================= */

// this will remove the warning 4786
#include <OpenTracker/tool/disable4786.h>

#include <OpenTracker/input/UbisenseSource.h>
#ifdef USE_UBISENSE
#ifdef WIN32

#include <ace/Log_Msg.h>

#pragma comment(lib,"UClientAPI.lib")

namespace ot {

    UbisenseSource::UbisenseSource(const Object &object,const LocationClient &locationClient,DataClient &dataClient): Node(),button(0x0000),lastTime(0.),buttonTime(0.),event(),object(object),locationClient(locationClient),dataClient(dataClient)
    {
#ifdef _DEBUG
        dataClient.notify(object,NotifyBeep);
        dataClient.notify(object,NotifyFlashGreen);
#endif
    }

    UbisenseSource::~UbisenseSource()
    {
#ifdef _DEBUG
        dataClient.notify(object,NotifyBeep);
        dataClient.notify(object,NotifyFlashRed);
#endif
    }

    bool UbisenseSource::calcEvent()
    {
        Location location;
        /*My*/NameClient client;

		//ACE_DEBUG((LM_DEBUG,ACE_TEXT("ot:Getting all named people:\n")));
        UClientAPI::Map<Object,String> all_people = client.get_all_named(Type("Person"));
        /*for (UClientAPI::Map<Object,String>::const_iterator i = all_people.begin();i != all_people.end();++ i)
			ACE_DEBUG((LM_DEBUG,ACE_TEXT("ot: %s has name %s\n"),(*i).first.to_string().c_str(),(*i).second->c_str()));*/

        String found_name;
        if (! client.get_object_name(object,found_name))
        {
			ACE_DEBUG((LM_DEBUG,ACE_TEXT("ot:Cannot get name of %s\n"),object.to_string().c_str()));
            return false;
        }
		ACE_DEBUG((LM_DEBUG,ACE_TEXT("ot:Ubitag name: %s\n"),found_name.c_str()));

        if (locationClient.get_object_location(object,location) && location.time_ > lastTime)
        {
            event.clearAttributes();
            event.getPosition()[0] = static_cast<float>(location.pos_.x_);
            event.getPosition()[1] = static_cast<float>(location.pos_.y_);
            event.getPosition()[2] = static_cast<float>(location.pos_.z_);

			ACE_DEBUG((LM_DEBUG,ACE_TEXT("ot: location x: %f location y: %f location z: %f\n"),event.getPosition()[0],event.getPosition()[1],event.getPosition()[2]));

            if (buttonTime > lastTime)
            {
                event.getButton() = button;
                button = 0x0000;
            }
            event.time = (location.time_ > buttonTime ? location.time_: buttonTime) * 1000.;
            event.getConfidence() = 1.f;
            lastTime = location.time_ > buttonTime ? location.time_: buttonTime;
            return true;
        }
        else
            if (buttonTime > lastTime)
            {
                event.getButton() = button;
                event.time = buttonTime * 1000.;
                event.getConfidence() = 1.f;
                button = 0x0000;
                lastTime = buttonTime;
                return true;
            }
        return false;
    }

    void UbisenseSource::setButtonEvent(unsigned short button,double time)
    {
        UbisenseSource::button |= button;
        buttonTime = time;
    }

} // namespace ot


#endif
#else
#ifdef WIN32
#pragma message(">>> no Ubisense support")
#endif
#endif  // USE_UBISENSE

/* 
 * ------------------------------------------------------------
 *   End of UbisenseSource.cxx
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
