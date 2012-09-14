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
/** source file for SpeechSet.
 *
 * @author Reinhard Steiner
 *
 * $Id: SpeechSet.cxx 2051 2007-09-24 10:15:53Z bornik $
 * @file                                                                   */
/* ======================================================================= */


// this will remove the warning 4786
#include <OpenTracker/tool/disable4786.h>

#include <OpenTracker/input/SpeechSet.h>
#include <OpenTracker/input/SpeechCore.h>


#ifndef OT_NO_SPEECH_SUPPORT


namespace ot {

    DWORD SpeechSetBase::GetId()
    {
        assert(m_SpeechCore);

        return(m_Id);
    }


    const char* SpeechSetBase::GetName()
    {
        assert(m_SpeechCore);

        return(m_Name.c_str());
    }


    bool SpeechSetBase::IsCommandRegistered(const char *p_Command)
    {
        for(unsigned int i = 0; i < m_RegisteredCommands.size(); ++i)
        {
            if(!strcmp(m_RegisteredCommands[i].m_Command.c_str(), p_Command))
                return(true);
        }
        return(false);
    }


    bool SpeechSetBase::IsCommandIdRegistered(DWORD p_CommandId)
    {
        for(unsigned int i = 0; i < m_RegisteredCommands.size(); ++i)
        {
            if(m_RegisteredCommands[i].m_CommandId == p_CommandId)
                return(true);
        }
        return(false);
    }


    long SpeechSetBase::GetCommandId(const char *p_Command)
    {
        for(unsigned int i = 0; i < m_RegisteredCommands.size(); ++i)
        {
            if(!strcmp(m_RegisteredCommands[i].m_Command.c_str(), p_Command))
                return(m_RegisteredCommands[i].m_CommandId);
        }
        return(-1);     // no command found
    }


    bool SpeechSetBase::GetCommand(DWORD p_CommandId, std::string &p_Command)
    {
        for(unsigned int i = 0; i < m_RegisteredCommands.size(); ++i)
        {
            if(m_RegisteredCommands[i].m_CommandId == p_CommandId)
            {
                p_Command = m_RegisteredCommands[i].m_Command;
                return(true);
            }
        }
        p_Command = ""; // no command found
        return(false);
    }


    void SpeechSetBase::AddCommand(const char *p_Command, DWORD p_CommandId, float p_Weight)
    {
        assert(m_SpeechCore);

        // no need for 2 times the same command
        if(IsCommandRegistered(p_Command))
            return;

        if((int)p_CommandId == -1)
            p_CommandId = m_RegisteredCommands.size() + 1;

        SSpeechCommand command;
        command.m_CommandId = p_CommandId;
        command.m_Command = p_Command;
        command.m_Seperator = " ";
        command.m_Weight = p_Weight;
        m_RegisteredCommands.push_back(command);
    }


    void SpeechSetBase::RemoveCommand(const char *p_Command)
    {
        for(std::vector<SSpeechCommand>::iterator i = m_RegisteredCommands.begin(); i < m_RegisteredCommands.end(); ++i)
        {
            if(!strcmp(i->m_Command.c_str(), p_Command))
            {
                m_RegisteredCommands.erase(i);
                return;
            }
        }
    }


    void SpeechSetBase::RemoveCommand(DWORD p_CommandId)
    {
        for(std::vector<SSpeechCommand>::iterator i = m_RegisteredCommands.begin(); i < m_RegisteredCommands.end(); ++i)
        {
            if(i->m_CommandId == p_CommandId)
            {
                m_RegisteredCommands.erase(i);
                return;
            }
        }
    }

    void SpeechSetBase::Activate()
    {
        if(IsActive())
            return;
        m_Active = true;
    }

    void SpeechSetBase::Deactivate()
    {
        if(!IsActive())
            return;
        m_Active = false;
    }

    bool SpeechSetBase::IsActive()
    {
        return(m_Active);
    }

#ifdef USE_SAPISPEECH


    CSpeechSet::CSpeechSet(const char *p_Name, DWORD p_Id, CSpeechCore *p_SpeechCore)
        : SpeechSetBase( p_Name, p_Id, p_SpeechCore)
    {
        Initialize();
        m_Name = p_Name;
        m_Id = p_Id;
        m_SpeechCore = p_SpeechCore;
    }

    void CSpeechSet::Initialize()
    {
        m_SpeechCore = 0;
        m_Active = true;
    }


    void CSpeechSet::Destroy()
    {
        if(m_SpeechCore)
        {
            HRESULT hr = S_OK;
            SPSTATEHANDLE hEventTravel;
            std::wstring wName;

            CSpeechCore::StrToWide(GetName(), wName);

            // remove rule
            hr = m_SpeechCore->m_CmdGrammar->GetRule(&wName[0], m_Id, SPRAF_TopLevel | SPRAF_Active, TRUE, &hEventTravel);
            if(SUCCEEDED(hr))
                m_SpeechCore->m_CmdGrammar->ClearRule(hEventTravel);
        }

        m_SpeechCore = 0;
        m_Active = false;
    }


    DWORD CSpeechSet::GetId()
    {
        assert(m_SpeechCore);

        return(m_Id);
    }


    const char* CSpeechSet::GetName()
    {
        assert(m_SpeechCore);

        return(m_Name.c_str());
    }


    bool CSpeechSet::IsCommandRegistered(const char *p_Command)
    {
        for(unsigned int i = 0; i < m_RegisteredCommands.size(); ++i)
        {
            if(!strcmp(m_RegisteredCommands[i].m_Command.c_str(), p_Command))
                return(true);
        }
        return(false);
    }


    bool CSpeechSet::IsCommandIdRegistered(DWORD p_CommandId)
    {
        for(unsigned int i = 0; i < m_RegisteredCommands.size(); ++i)
        {
            if(m_RegisteredCommands[i].m_CommandId == p_CommandId)
                return(true);
        }
        return(false);
    }


    long CSpeechSet::GetCommandId(const char *p_Command)
    {
        for(unsigned int i = 0; i < m_RegisteredCommands.size(); ++i)
        {
            if(!strcmp(m_RegisteredCommands[i].m_Command.c_str(), p_Command))
                return(m_RegisteredCommands[i].m_CommandId);
        }
        return(-1);     // no command found
    }


    bool CSpeechSet::GetCommand(DWORD p_CommandId, std::string &p_Command)
    {
        for(unsigned int i = 0; i < m_RegisteredCommands.size(); ++i)
        {
            if(m_RegisteredCommands[i].m_CommandId == p_CommandId)
            {
                p_Command = m_RegisteredCommands[i].m_Command;
                return(true);
            }
        }
        p_Command = ""; // no command found
        return(false);
    }


    void CSpeechSet::AddCommand(const char *p_Command, DWORD p_CommandId, float p_Weight)
    {
        assert(m_SpeechCore);

        HRESULT hr = S_OK;
        SPSTATEHANDLE hEventTravel;
        std::wstring wName, wCommand, wSeperator;

        // no need for 2 times the same command
        if(IsCommandRegistered(p_Command))
            return;

        CSpeechCore::StrToWide(GetName(), wName);
        CSpeechCore::StrToWide(p_Command, wCommand);
        CSpeechCore::StrToWide(" ", wSeperator);

        // create (if rule does not already exist) top-level Rule, defaulting to Active
        hr = m_SpeechCore->m_CmdGrammar->GetRule(&wName[0], m_Id, SPRAF_TopLevel | SPRAF_Active, TRUE, &hEventTravel);
        if(FAILED(hr))
        {
            Destroy();
            throw CSpeechException("Unable to get Rule");
        }

        // add a command to the rule
        hr = m_SpeechCore->m_CmdGrammar->AddWordTransition(hEventTravel, NULL, &wCommand[0], &wSeperator[0], SPWT_LEXICAL, p_Weight, NULL);
        if(FAILED(hr))
        {
            Destroy();
            throw CSpeechException("Unable to add Word");
        }

        // Must Commit the grammar changes before using the grammar.
        hr = m_SpeechCore->m_CmdGrammar->Commit(0);
        if (FAILED(hr))
        {
            Destroy();
            throw CSpeechException("Unable to Commit");
        }

        // Activate the Rule
        hr = m_SpeechCore->m_CmdGrammar->SetRuleState(&wName[0], NULL, SPRS_ACTIVE);
        if (FAILED(hr))
        {
            Destroy();
            throw CSpeechException("Unable to activate rule");
        }

        if(p_CommandId == -1)
            p_CommandId = m_RegisteredCommands.size() + 1;

        SSpeechCommand command;
        command.m_CommandId = p_CommandId;
        command.m_Command = p_Command;
        command.m_Seperator = " ";
        command.m_Weight = p_Weight;
        m_RegisteredCommands.push_back(command);
    }


    void CSpeechSet::RemoveCommand(const char *p_Command)
    {
        for(std::vector<SSpeechCommand>::iterator i = m_RegisteredCommands.begin(); i < m_RegisteredCommands.end(); ++i)
        {
            if(!strcmp(i->m_Command.c_str(), p_Command))
            {
                m_RegisteredCommands.erase(i);
                RebuildRule();
                return;
            }
        }
    }


    void CSpeechSet::RemoveCommand(DWORD p_CommandId)
    {
        for(std::vector<SSpeechCommand>::iterator i = m_RegisteredCommands.begin(); i < m_RegisteredCommands.end(); ++i)
        {
            if(i->m_CommandId == p_CommandId)
            {
                m_RegisteredCommands.erase(i);
                RebuildRule();
                return;
            }
        }
    }


    void CSpeechSet::RebuildRule()
    {
        HRESULT hr = S_OK;
        SPSTATEHANDLE hEventTravel;
        std::wstring wName, wCommand, wSeperator;

        CSpeechCore::StrToWide(GetName(), wName);

        // create (if rule does not already exist) top-level Rule, defaulting to Active
        hr = m_SpeechCore->m_CmdGrammar->GetRule(&wName[0], m_Id, SPRAF_TopLevel | SPRAF_Active, TRUE, &hEventTravel);
        if(FAILED(hr))
        {
            Destroy();
            throw CSpeechException("Unable to get Rule");
        }

        // remove rule
        hr = m_SpeechCore->m_CmdGrammar->ClearRule(hEventTravel);
        if(FAILED(hr))
        {
            Destroy();
            throw CSpeechException("Unable to clear Rule");
        }

        for(unsigned int i = 0; i < m_RegisteredCommands.size(); ++i)
        {
            CSpeechCore::StrToWide(m_RegisteredCommands[i].m_Command.c_str(), wCommand);
            CSpeechCore::StrToWide(m_RegisteredCommands[i].m_Seperator.c_str(), wSeperator);

            // add a command to the rule
            hr = m_SpeechCore->m_CmdGrammar->AddWordTransition(hEventTravel, NULL, &wCommand[0], &wSeperator[0], SPWT_LEXICAL, m_RegisteredCommands[i].m_Weight, NULL);
            if(FAILED(hr))
            {
                Destroy();
                throw CSpeechException("Unable to add Word");
            }
        }

        // Must Commit the grammar changes before using the grammar.
        hr = m_SpeechCore->m_CmdGrammar->Commit(0);
        if (FAILED(hr))
        {
            Destroy();
            throw CSpeechException("Unable to Commit");
        }

        if(IsActive())
            Activate();
        else
            Deactivate();
    }


    void CSpeechSet::Recognize(const char *p_String)
    {
        m_RecogizedCommands.push(p_String);
    }


    bool CSpeechSet::IsReco()
    {
        assert(m_SpeechCore);
        return(m_RecogizedCommands.size() > 0);
    }


    bool CSpeechSet::GetReco(std::string &p_Result)
    {
        assert(m_SpeechCore);

        if(m_RecogizedCommands.size() > 0)
        {
            p_Result = m_RecogizedCommands.front();
            m_RecogizedCommands.pop();
            return(true);
        }

        return(false);
    }


    void CSpeechSet::Activate()
    {
        if(IsActive())
            return;

        HRESULT hr = S_OK;
        std::wstring wName;
        CSpeechCore::StrToWide(GetName(), wName);

        // Activate the Rule
        hr = m_SpeechCore->m_CmdGrammar->SetRuleState(&wName[0], NULL, SPRS_ACTIVE);
        if (FAILED(hr))
        {
            Destroy();
            throw CSpeechException("Unable to activate rule");
        }

        m_Active = true;
    }


    void CSpeechSet::Deactivate()
    {
        if(!IsActive())
            return;

        HRESULT hr = S_OK;
        std::wstring wName;
        CSpeechCore::StrToWide(GetName(), wName);

        // Activate the Rule
        hr = m_SpeechCore->m_CmdGrammar->SetRuleState(&wName[0], NULL, SPRS_INACTIVE);
        if (FAILED(hr))
        {
            Destroy();
            throw CSpeechException("Unable to deactivate rule");
        }

        m_Active = false;
    }


    bool CSpeechSet::IsActive()
    {
        return(m_Active);
    }


#endif //ifdef USE_SAPISPEECH


} // namespace ot


#else
#ifdef WIN32
#pragma message(">>> OT_NO_SPEECH_SUPPORT")
#endif
#endif // OT_NO_SPEECH_SUPPORT

/* 
 * ------------------------------------------------------------
 *   End of SpeechSet.cxx
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
