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
/** header file for SpeechVoiceModule.
 *
 * @author Reinhard Steiner
 *
 * $Id: SpeechVoiceModule.h 1547 2006-10-25 10:33:10Z veas $
 * @file                                                                   */
/* ======================================================================= */


#if !defined(__SPEECHVOICEMODULE_H)
#define __SPEECHVOICEMODULE_H

#include "../OpenTracker.h"

#include "SpeechDef.h"

/**
 * This class represents the core component of the TTS.
 * Multiple instances of this class could speak in parallel (not at Win9x).
 * Multiple voices (as installed) are available, and could be changed at runtime.
 * The default voice is the windows standard voice (as in the control panel defined).
 * Text could be spoken synchron or asynchron.
 */


#ifndef OT_NO_SPEECH_SUPPORT


namespace ot {

    class SpeechVoiceModule
    {
        // protected data members
    protected:
        SpeechVoiceBase *m_Voice;                              // Voice


        // protected init & destroy methods
    protected:
        void Initialize();    /// initializes all class members
        void Destroy();       /// destroys (cleanup) all class members


        // constructor & destructor
    public:
        SpeechVoiceModule()
        {
            Initialize();
        }

        virtual ~SpeechVoiceModule()
        {
            Destroy();
        }


        // public methods
    public:
        /// get the total number of voices
        DWORD GetNumVoices();

        /// get the voice name with this id
        const char* GetVoiceName(DWORD p_Id, std::string &p_Name);


        /// set the current voice by id
        void SetVoice(DWORD p_Id);
        /// set the current voice by name
        void SetVoice(const char *p_Name);


        /// Speak something
        void Speak(const char *p_Sentence, bool p_Async = true);
    };

} // namespace ot


#endif // OT_NO_SPEECH_SUPPORT


#endif //#if !defined(__SPEECHVOICEMODULE_H)

/* 
 * ------------------------------------------------------------
 *   End of SpeechVoiceModule.h
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
