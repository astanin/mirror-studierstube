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
/** source file for SpeechCore.
 *
 * @author Reinhard Steiner
 *
 * $Id: SpeechCore.cxx 2051 2007-09-24 10:15:53Z bornik $
 * @file                                                                   */
/* ======================================================================= */

// this will remove the warning 4786
#include <OpenTracker/tool/disable4786.h>

#include <OpenTracker/input/SpeechCore.h>
#include <OpenTracker/input/SpeechSet.h>


#ifndef OT_NO_SPEECH_SUPPORT


namespace ot {

    DWORD SpeechCoreBase::s_GrammarId = 1;

    SpeechSetBase * SpeechCoreBase::GetSpeechSet(const char *p_Name, bool p_Create)
    {
        if(!m_Initialized)
            return(NULL);

        for(unsigned int i = 0; i < m_SpeechSets.size(); ++i)
        {
            if(!strcmp(m_SpeechSets[i]->GetName(), p_Name))
                return(m_SpeechSets[i]);
        }

        if(!p_Create)
            return(NULL);

        SpeechSetBase *set = new SpeechSetBase(p_Name, m_NextRuleId++, this);
        if(!set) throw CSpeechException("no memory");
        m_SpeechSets.push_back(set);
        return(set);
    }


    SpeechSetBase* SpeechCoreBase::GetSpeechSet(DWORD p_Id)
    {
        if(!m_Initialized)
            return(NULL);

        for(unsigned int i = 0; i < m_SpeechSets.size(); ++i)
        {
            if(m_SpeechSets[i]->GetId() == p_Id)
                return(m_SpeechSets[i]);
        }
        return(NULL);
    }


    void SpeechCoreBase::RemoveSpeechSet(const char *p_Name)
    {
        if(!m_Initialized)
            return;

        for(std::vector<SpeechSetBase*>::iterator i = m_SpeechSets.begin(); i < m_SpeechSets.end(); ++i)
        {
            if(!strcmp((*i)->GetName(), p_Name))
            {
                delete(*i);
                m_SpeechSets.erase(i);
            }
        }
    }


    void SpeechCoreBase::RemoveSpeechSet(DWORD p_Id)
    {
        if(!m_Initialized)
            return;

        for(std::vector<SpeechSetBase*>::iterator i = m_SpeechSets.begin(); i < m_SpeechSets.end(); ++i)
        {
            if((*i)->GetId() == p_Id)
            {
                delete(*i);
                m_SpeechSets.erase(i);
            }
        }
    }


    SpeechSetBase* SpeechCoreBase::FindSpeechSet(const char *p_Command, bool p_Active)
    {
        if(!m_Initialized)
            return(NULL);

        for(unsigned int i = 0; i < m_SpeechSets.size(); ++i)
        {
            if(!p_Active || m_SpeechSets[i]->IsActive())
                if(m_SpeechSets[i]->IsCommandRegistered(p_Command))
                    return(m_SpeechSets[i]);
        }
        return(NULL);
    }


    SpeechSetBase* SpeechCoreBase::FindSpeechSet(DWORD p_CommandId, bool p_Active)
    {
        if(!m_Initialized)
            return(NULL);

        for(unsigned int i = 0; i < m_SpeechSets.size(); ++i)
        {
            if(!p_Active || m_SpeechSets[i]->IsActive())
                if(m_SpeechSets[i]->IsCommandIdRegistered(p_CommandId))
                    return(m_SpeechSets[i]);
        }
        return(NULL);
    }

#ifdef USE_SAPISPEECH

    DWORD CSpeechCore::s_GrammarId = 1;

    void CSpeechCore::Initialize()
    {
        m_Initialized = false;
        m_NextRuleId = 1;
    }


    void CSpeechCore::Destroy()
    {
        try
        {
            for(int i = m_SpeechSets.size() - 1; i >= 0; --i)
                delete(m_SpeechSets[i]);
            m_SpeechSets.clear();
        }
        catch(...) {}

        try
        {
            // Release grammar, if created
            if(m_CmdGrammar)
            {
                m_CmdGrammar.Release();
            }
            // Release recognition context, if created
            if(m_RecoCtxt)
            {
                m_RecoCtxt->SetNotifySink(NULL);
                m_RecoCtxt.Release();
            }
            // Release recognition engine instance, if created
            if(m_RecoEngine)
            {
                m_RecoEngine.Release();
            }
        }
        catch(...) {}

        m_Initialized = false;
    }


    void CSpeechCore::Init(LANGID p_LanguageId)
    {
        Destroy();
        Initialize();


        HRESULT hr = S_OK;
  
        // create a recognition engine
        hr = m_RecoEngine.CoCreateInstance(CLSID_SpSharedRecognizer);
        if(FAILED(hr))
        {
            Destroy();
            throw CSpeechException("Unable to create Recognizer");
        }
  
  
        // create the command recognition context
        hr = m_RecoEngine->CreateRecoContext(&m_RecoCtxt);
        if(FAILED(hr))
        {
            Destroy();
            throw CSpeechException("Unable to create Recognition Context");
        }
  
  
        // Tell SR what types of events interest us.  Here we only care about command recognition.
        hr = m_RecoCtxt->SetInterest(SPFEI(SPEI_RECOGNITION), SPFEI(SPEI_RECOGNITION));
        if(FAILED(hr))
        {
            Destroy();
            throw CSpeechException("Unable to set event Interest");
        }
  
  
        // Create our grammar
        hr = m_RecoCtxt->CreateGrammar(CSpeechCore::s_GrammarId++, &m_CmdGrammar);
        if(FAILED(hr))
        {
            Destroy();
            throw CSpeechException("Unable to create Grammar");
        }
  
  
        // Set language
        if(p_LanguageId != MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT))
        {
            hr = m_CmdGrammar->ResetGrammar(p_LanguageId);
            if(FAILED(hr))
            {
                Destroy();
                throw CSpeechException("Unable to set Language");
            }
        }

        m_Initialized = true;
    }



    SpeechSetBase* CSpeechCore::GetSpeechSet(const char *p_Name, bool p_Create)
    {
        if(!m_Initialized)
            return(NULL);

        for(unsigned int i = 0; i < m_SpeechSets.size(); ++i)
        {
            if(!strcmp(m_SpeechSets[i]->GetName(), p_Name))
                return(m_SpeechSets[i]);
        }

        if(!p_Create)
            return(NULL);

        CSpeechSet *set = new CSpeechSet(p_Name, m_NextRuleId++, this);
        if(!set) throw CSpeechException("no memory");
        m_SpeechSets.push_back(set);
        return(set);
    }


    SpeechSetBase* CSpeechCore::GetSpeechSet(DWORD p_Id)
    {
        if(!m_Initialized)
            return(NULL);

        for(unsigned int i = 0; i < m_SpeechSets.size(); ++i)
        {
            if(m_SpeechSets[i]->GetId() == p_Id)
                return(m_SpeechSets[i]);
        }
        return(NULL);
    }


    void CSpeechCore::RemoveSpeechSet(const char *p_Name)
    {
        if(!m_Initialized)
            return;

        for(std::vector<CSpeechSet*>::iterator i = m_SpeechSets.begin(); i < m_SpeechSets.end(); ++i)
        {
            if(!strcmp((*i)->GetName(), p_Name))
            {
                delete(*i);
                m_SpeechSets.erase(i);
            }
        }
    }


    void CSpeechCore::RemoveSpeechSet(DWORD p_Id)
    {
        if(!m_Initialized)
            return;

        for(std::vector<CSpeechSet*>::iterator i = m_SpeechSets.begin(); i < m_SpeechSets.end(); ++i)
        {
            if((*i)->GetId() == p_Id)
            {
                delete(*i);
                m_SpeechSets.erase(i);
            }
        }
    }


    SpeechSetBase* CSpeechCore::FindSpeechSet(const char *p_Command, bool p_Active)
    {
        if(!m_Initialized)
            return(NULL);

        for(unsigned int i = 0; i < m_SpeechSets.size(); ++i)
        {
            if(!p_Active || m_SpeechSets[i]->IsActive())
                if(m_SpeechSets[i]->IsCommandRegistered(p_Command))
                    return(m_SpeechSets[i]);
        }
        return(NULL);
    }


    SpeechSetBase* CSpeechCore::FindSpeechSet(DWORD p_CommandId, bool p_Active)
    {
        if(!m_Initialized)
            return(NULL);

        for(unsigned int i = 0; i < m_SpeechSets.size(); ++i)
        {
            if(!p_Active || m_SpeechSets[i]->IsActive())
                if(m_SpeechSets[i]->IsCommandIdRegistered(p_CommandId))
                    return(m_SpeechSets[i]);
        }
        return(NULL);
    }


    void CSpeechCore::NotifyWindowMessage(HWND p_hWnd, UINT p_Msg, WPARAM p_wParam, LPARAM p_lParam)
    {
        // Let SR know that window we want it to send event information to, and using what message
        HRESULT hr = m_RecoCtxt->SetNotifyWindowMessage(p_hWnd, p_Msg, p_wParam, p_lParam);
        if(FAILED(hr))
            throw CSpeechException("Unable to set notify window message");
    }


    bool CSpeechCore::ProcessRecognitionPoll()
    {
        if(!m_Initialized)
            return(false);

        bool Recognized = false;
        CSpEvent event;  // Event helper class
  
        // Loop processing events while there are any in the queue
        while(event.GetFrom(m_RecoCtxt) == S_OK)
        {
            // Look at recognition event only
            switch(event.eEventId)
            {
                case SPEI_RECOGNITION:
                    {
                        HRESULT hr;
                        ISpRecoResult *RecoResult = event.RecoResult();
                        WCHAR   *ppszCoMemText;

                        hr = RecoResult->GetText(SP_GETWHOLEPHRASE, SP_GETWHOLEPHRASE, TRUE, &ppszCoMemText, NULL);

                        std::string RecoText;  // convert to ANSI string
                        CSpeechCore::WideToStr(ppszCoMemText, RecoText);
                        ::CoTaskMemFree(ppszCoMemText);

                        CSpeechSet* set = reinterpret_cast<CSpeechSet *>(FindSpeechSet(RecoText.c_str()));
                        if(set)
                        {
                            set->Recognize(RecoText.c_str());
                        }

                        Recognized = true;
                    }      
            }
        }
        return(Recognized);
    }


    bool CSpeechCore::GetReco(std::string &p_Result, std::string &p_SpeechSet)
    {
        if(!m_Initialized)
            return(false);

        for(unsigned int i = 0; i < m_SpeechSets.size(); ++i)
        {
            if(m_SpeechSets[i]->GetReco(p_Result))
            {
                p_SpeechSet = m_SpeechSets[i]->GetName();
                return(true);
            }
        }
        return(false);
    }


    void CSpeechCore::StrToWide(const char *p_String, std::wstring &p_WideString)
    {
        int len = strlen(p_String) + 1;
        p_WideString.resize(len);
  
        MultiByteToWideChar(CP_ACP, 0, p_String, -1, &p_WideString[0], len-1);
        p_WideString[len-1] = 0;
    }


    void CSpeechCore::WideToStr(const WCHAR *p_WideString, std::string &p_String)
    {
        int len = wcslen(p_WideString)+1;
        p_String.resize(len);
  
        WideCharToMultiByte(CP_ACP, 0, p_WideString, -1, &p_String[0], len-1, NULL, NULL);
        p_String[len-1] = 0;
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
 *   End of SpeechCore.cxx
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
