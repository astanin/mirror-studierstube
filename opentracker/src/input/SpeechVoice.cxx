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
/** source file for SpeechVoice.
 *
 * @author Reinhard Steiner
 *
 * $Id: SpeechVoice.cxx 2051 2007-09-24 10:15:53Z bornik $
 * @file                                                                   */
/* ======================================================================= */


// this will remove the warning 4786
#include <OpenTracker/tool/disable4786.h>

#include <OpenTracker/input/SpeechVoice.h>
#include <OpenTracker/input/SpeechCore.h>


#ifndef OT_NO_SPEECH_SUPPORT


#ifdef USE_SAPISPEECH
//using namespace std;

namespace ot {

    void CSpeechVoice::Initialize()
    {
        HRESULT hr = S_OK;
        hr = m_Voice.CoCreateInstance(CLSID_SpVoice);
        if(FAILED(hr))
            throw CSpeechException("Unable create Voice");

        m_Voice->SetPriority(SPVPRI_OVER);

        GetVoices();
    }


    void CSpeechVoice::Destroy()
    {
        // Release all voice tokens
        for(unsigned int i = 0; i < m_VoiceToken.size(); ++i)
            m_VoiceToken[i].Release();

        // Release voice, if created
        if(m_Voice)
        {
            m_Voice.Release();
        }
    }



    DWORD CSpeechVoice::GetNumVoices()
    {
        return(m_VoiceName.size());
    }


    const char* CSpeechVoice::GetVoiceName(DWORD p_Id, std::string &p_Name)
    {
        if(!m_Voice)
            return(NULL);

        if(p_Id >= m_VoiceName.size())
            throw CSpeechException("Voice id out of bounds");
        p_Name = m_VoiceName[p_Id];
        return(p_Name.c_str());
    }


    void CSpeechVoice::SetVoice(DWORD p_Id)
    {
        if(!m_Voice)
            return;

        if(p_Id >= m_VoiceName.size())
            throw CSpeechException("Voice id out of bounds");
        m_Voice->SetVoice(m_VoiceToken[p_Id]);
    }


    void CSpeechVoice::SetVoice(const char *p_Name)
    {
        if(!m_Voice)
            return;

        for(unsigned int i = 0; i < m_VoiceName.size(); ++i)
            if(!strcmp(m_VoiceName[i].c_str(), p_Name))
            {
                SetVoice(i);
                break;
            }
    }


    void CSpeechVoice::Speak(const char *p_Sentence, bool p_Async)
    {
        if(!m_Voice)
            return;

        std::wstring wSentence;
        CSpeechCore::StrToWide(p_Sentence, wSentence);
  
        if(p_Async)
            m_Voice->Speak(&wSentence[0], SPF_ASYNC, NULL);
        else
            m_Voice->Speak(&wSentence[0], 0, NULL);
    }


    void CSpeechVoice::GetVoices()
    {
        if(!m_Voice)
            return;

        HRESULT                      hr = S_OK;
        CComPtr<ISpObjectToken>      VoiceToken;
        CComPtr<IEnumSpObjectTokens> Enum;
        ULONG                        Count = 0;
    
  
        // Enumerate the available voices 
        hr = SpEnumTokens(SPCAT_VOICES, NULL, NULL, &Enum);
        if(FAILED(hr))
            throw CSpeechException("Unable to enum Speakers");
  
        // Get the number of voices
        hr = Enum->GetCount(&Count);
        if(FAILED(hr))
            throw CSpeechException("Unable to count Speakers");
  
        // Obtain a list of available voice tokens, set the voice to the token, and call Speak
        for(unsigned int i = 0; i < Count; ++i)
        {
            VoiceToken.Release();
            hr = Enum->Item(i, &VoiceToken);
            //hr = Enum->Next(1, &VoiceToken, NULL);

            if(SUCCEEDED(hr))
            {
                WCHAR *ppszCoMemText;      
                hr = VoiceToken->GetStringValue(NULL, &ppszCoMemText);
      
                if(SUCCEEDED(hr))
                {
                    std::string Speaker;  // convert to ANSI string
                    CSpeechCore::WideToStr(ppszCoMemText, Speaker);
                    ::CoTaskMemFree(ppszCoMemText);
                    m_VoiceName.push_back(Speaker);
                    m_VoiceToken.push_back(VoiceToken);
                }
            }
        }
    }

} // namespace ot

#endif //ifdef USE_SAPISPEECH


#else
#ifdef WIN32
#pragma message(">>> OT_NO_SPEECH_SUPPORT")
#endif
#endif // OT_NO_SPEECH_SUPPORT

/* 
 * ------------------------------------------------------------
 *   End of SpeechVoice.cxx
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
