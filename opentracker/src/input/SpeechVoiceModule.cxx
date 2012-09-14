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
/** source file for SpeechVoiceModule module.
 *
 * @author Reinhard Steiner
 *
 * $Id: SpeechVoiceModule.cxx 2051 2007-09-24 10:15:53Z bornik $
 * @file                                                                   */
/* ======================================================================= */


// this will remove the warning 4786
#include <OpenTracker/tool/disable4786.h>

#include <OpenTracker/input/SpeechVoiceModule.h>
#include <OpenTracker/input/SpeechVoice.h>


#ifndef OT_NO_SPEECH_SUPPORT


namespace ot {

    void SpeechVoiceModule::Initialize()
    {
#ifdef USE_SAPISPEECH
        m_Voice = new CSpeechVoice;
#else
        m_Voice = new SpeechVoiceBase;
#endif
        assert(m_Voice);
    }


    void SpeechVoiceModule::Destroy()
    {
        // Release voice, if created
        if(m_Voice)
        {
            delete(m_Voice);
            m_Voice = 0;
        }
    }



    DWORD SpeechVoiceModule::GetNumVoices()
    {
        assert(m_Voice);
        return(m_Voice->GetNumVoices());
    }


    const char* SpeechVoiceModule::GetVoiceName(DWORD p_Id, std::string &p_Name)
    {
        assert(m_Voice);
        return(m_Voice->GetVoiceName(p_Id, p_Name));
    }


    void SpeechVoiceModule::SetVoice(DWORD p_Id)
    {
        assert(m_Voice);
        m_Voice->SetVoice(p_Id);
    }


    void SpeechVoiceModule::SetVoice(const char *p_Name)
    {
        assert(m_Voice);
        m_Voice->SetVoice(p_Name);
    }


    void SpeechVoiceModule::Speak(const char *p_Sentence, bool p_Async)
    {
        assert(m_Voice);
        m_Voice->Speak(p_Sentence, p_Async);
    }

} // namespace ot


// #endif //ifdef USE_SAPISPEECH


#else
#ifdef WIN32
#pragma message(">>> OT_NO_SPEECH_SUPPORT")
#endif
#endif // OT_NO_SPEECH_SUPPORT

/* 
 * ------------------------------------------------------------
 *   End of SpeechVoiceModule.cxx
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
