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
/** header file for SpeechSource Node.
 *
 * @author Reinhard Steiner
 *
 * $Id: SpeechSource.h 1913 2007-06-25 14:24:04Z mathis $
 * @file                                                                   */
/* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section speechrecosource SpeechRecoSource
 *
 * The SpeechRecoSource node pushes speech events. It is instantiated
 * and managed by the @ref speechmodule. A speech event
 * is coded into an attribute called 'commandId' as configured in the
 * opentracker XML config file. The speech set ID is hold by an attribute
 * called 'speechSetId'. The attributes 'commandName' and 'speechSetName'
 * also encode the according command string and speech set description,
 * respectively.
 *
 * This node always pushes 2 events, one speech "start" event,
 * and one speech "end" event. This is used for button press/release
 * actions. The start is coded as button 0 pressed, end as button 0
 * released. The source only fires events when a command is successfully
 * recognised. It has the following attributes :
 * @li @c name is the name of the source node.
 * @li @c set specifies the name of the command set to
 *            use in this node.
 *
 * An example element looks like this:
 * @verbatim
 <SpeechRecoSource name="numberset" set="CmdSet1"/>@endverbatim
 *
 */

#ifndef _SPEECHSOURCE_H
#define _SPEECHSOURCE_H

#include "../OpenTracker.h"

#include "SpeechDef.h"
#include "SpeechCore.h"
#include "SpeechSet.h"
#include "SpeechVoice.h"
#include "SpeechModule.h"

/**
 * This class implements a speech source that is fired by its module
 * when any speech recognition occured.
 * @ingroup input
 * @author Reinhard Steiner
 */


#ifndef OT_NO_SPEECH_SUPPORT


namespace ot {

    class OPENTRACKER_API SpeechSource : public Node
    {
        // Members
    public:
        /// speech module parent object
        SpeechModule *m_SpeechModule;

        /// speech set of this source node
        SpeechSetBase *m_SpeechSet;
        /// the event that is posted to the EventObservers
        Event event;
        /// the last command string
        std::string command;

        // Methods
    protected:
        /** simple constructor, sets members to initial values
         * @param p_SpeechModule the parent module (node factory)
         * @param p_SpeechSet the speech set of this node
         */
        SpeechSource(SpeechModule *p_SpeechModule, SpeechSetBase *p_SpeechSet) :
            Node(),
            m_SpeechModule(p_SpeechModule),
            m_SpeechSet(p_SpeechSet)
        {
            assert(m_SpeechModule); // must be a valid pointer
            assert(m_SpeechSet);    // must be a valid pointer
        }
    public:
        /** tests for EventGenerator interface being present. Is overriden to
         * return 1 always.
         * @return always 1
         */
        int isEventGenerator()
        {
            return 1;
        }

        /** pushes event down the line. Needed to access protected
         * updateObservers method in EventGenerator
         */
        bool push()
        {
            if(m_SpeechSet->GetReco(command))
            {
                //m_SpeechModule->Speak(command.c_str(), true); // TEST only

                long commandId = m_SpeechSet->GetCommandId(command.c_str());
                unsigned long speechSetId = m_SpeechSet->GetId();
                std::string commandName;
                bool success = m_SpeechModule->GetCommand(commandId, speechSetId, commandName);
                std::string speechSetName(m_SpeechSet->GetName());

                event.setButton(1);
                event.setAttribute("commandId", commandId);
                event.setAttribute("speechSetId", speechSetId);
                event.setAttribute("speechSetName", speechSetName);
                if (success)
                    event.setAttribute("commandName", commandName);

                printf("SR: SpeechSource::push, SpeechSetId = '%lu', SpeechSetName = '%s', CommandId = %li, Command = '%s'\n",
                       speechSetId, speechSetName.c_str(), commandId, commandName.c_str());
                event.timeStamp();
                updateObservers(event);

                return(true);
            }
            return(false);
        }

        void push2()
        {
            // Fire a second time with no button pressed
            //printf("SpeechSource::push2\n");

            long commandId = m_SpeechSet->GetCommandId(command.c_str());
            unsigned long speechSetId = m_SpeechSet->GetId();
            std::string commandName;
            bool success = m_SpeechModule->GetCommand(commandId, speechSetId, commandName);
            std::string speechSetName(m_SpeechSet->GetName());

            event.setButton(0);
            event.setAttribute("commandId", commandId);
            event.setAttribute("speechSetId", speechSetId);
            event.setAttribute("speechSetName", speechSetName);
            if (success)
                event.setAttribute("commandName", commandName);

            //printf("SR: SpeechSource::push, SpeechSetId = '%i', SpeechSetName = '%s', CommandId = %i, Command = '%s'\n",
            //	     speechSetId, speechSetName.c_str(), commandId, commandName.c_str());
            event.timeStamp();
            updateObservers(event);

        }
        void pushEvent();
        void pullEvent();

        friend class SpeechModule;
    };

} // namespace ot


#endif //OT_SPEECH_SUPPORT


#endif

/* 
 * ------------------------------------------------------------
 *   End of SpeechSource.h
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
