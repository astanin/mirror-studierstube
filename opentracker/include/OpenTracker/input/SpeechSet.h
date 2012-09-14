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
/** header file for SpeechSet.
 *
 * @author Reinhard Steiner
 *
 * $Id: SpeechSet.h 1547 2006-10-25 10:33:10Z veas $
 * @file                                                                   */
/* ======================================================================= */


#if !defined(__SPEECHSET_H)
#define __SPEECHSET_H

#include "../OpenTracker.h"

#include "SpeechDef.h"

/**
 * This class represents the abstract interface a command set/group and a simple
 * non functional implementation. Any real implementation should be derived from 
 * this one. Currently there is a big overlap between the SAPI implementation
 * and this base class, because the refactoring was done rather quickly. It
 * might also be interesting to move more of the SAPI interface to this 
 * super class.
 *
 * @author Reinhard Steiner
 * @ingroup input 
 */


#ifndef OT_NO_SPEECH_SUPPORT


namespace ot {

    class SpeechSetBase
    {
        // protected data members
    protected:
        bool m_Active;                    /// is this speech set active

        std::string m_Name;               /// Name of this Speech Set
        DWORD m_Id;                       /// Id of this Speech Set
        SpeechCoreBase *m_SpeechCore;        /// Parent Speech Core object (must be always valid)

        std::vector<SSpeechCommand> m_RegisteredCommands; /// All registered Commands in this SpeechSet
  
        StringQueue m_RecogizedCommands;  /// Recognized Commands queue


        // protected init & destroy methods
    protected:
	/// initializes all class members
	virtual	void Initialize()
	{
            m_SpeechCore = 0;
            m_Active = true;
	};

	/// destroys (cleanup) all class members
	virtual void Destroy()
	{	
            m_SpeechCore = 0;
            m_Active = false;
	};      


        // constructor & destructor
    protected:
        SpeechSetBase(const char *p_Name, DWORD p_Id, SpeechCoreBase *p_SpeechCore)
        {
            Initialize();
            m_Name = p_Name;
            m_Id = p_Id;
            m_SpeechCore = p_SpeechCore;
        }

        virtual ~SpeechSetBase()
        {
            Destroy();
        }

  
        // public methods
    public:
        /// get the id of this SpeechSet
        DWORD GetId();

        /// get the name of this SpeechSet
        virtual const char* GetName();


        /// wheter this command is register or not
        virtual bool IsCommandRegistered(const char *p_Command);

        /// wheter this command is register or not
        virtual bool IsCommandIdRegistered(DWORD p_CommandId);


        /// get id of the command
        virtual long GetCommandId(const char *p_Command);

        /// get the command from a command id
        virtual bool GetCommand(DWORD p_CommandId, std::string &p_Command);


        /// Add a new command to this set
        virtual void AddCommand(const char *p_Command, DWORD p_CommandId = -1, float p_Weight = 1.0f);

        /// Remove a command from this set
        virtual void RemoveCommand(const char *p_Command);

        /// Remove a command from this set
        virtual void RemoveCommand(DWORD p_CommandId);

        /// get the next recognized command, returns true when there is a result
        virtual bool GetReco(std::string &p_Result)
        {
            return false;
        };


        /// Activate this SpeechSet
        void Activate();
        /// Deactivate this SpeechSet
        void Deactivate();
        /// Wether this SpeechSet is active or not
        bool IsActive();
  
        friend class SpeechCoreBase;
    };

} // namespace ot


#ifdef USE_SAPISPEECH


// #include "SpeechInc.h"

/**
 * This class represents a command set/group. The set could be enhanced, activated
 * or deactivated on the fly. Each speech set depents on a speech core object.
 *
 * @author Reinhard Steiner
 * @ingroup input 
 */

namespace ot {

    class CSpeechSet : public SpeechSetBase
    {
        // protected data members
    protected:
        bool m_Active;                    /// is this speech set active

        std::string m_Name;               /// Name of this Speech Set
        DWORD m_Id;                       /// Id of this Speech Set
        CSpeechCore *m_SpeechCore;        /// Parent Speech Core object (must be always valid)

        std::vector<SSpeechCommand> m_RegisteredCommands; /// All registered Commands in this SpeechSet
  
        StringQueue m_RecogizedCommands;  /// Recognized Commands queue


        // protected init & destroy methods
    protected:
        void Initialize();    /// initializes all class members
        void Destroy();       /// destroys (cleanup) all class members


        // constructor & destructor
    protected:
        CSpeechSet(const char *p_Name, DWORD p_Id, CSpeechCore *p_SpeechCore);


        virtual ~CSpeechSet()
        {
            Destroy();
        }


        // protected methods
    protected:
        /// when a command of this set is recognized by SpeechCore, this method will be called
        void Recognize(const char *p_String);

        /// clear and rebuild the rule
        void RebuildRule();

  
        // public methods
    public:
        /// get the id of this SpeechSet
        DWORD GetId();

        /// get the name of this SpeechSet
        const char* GetName();


        /// wheter this command is register or not
        bool IsCommandRegistered(const char *p_Command);

        /// wheter this command is register or not
        bool IsCommandIdRegistered(DWORD p_CommandId);


        /// get id of the command
        long GetCommandId(const char *p_Command);

        /// get the command from a command id
        bool GetCommand(DWORD p_CommandId, std::string &p_Command);


        /// Add a new command to this set
        void AddCommand(const char *p_Command, DWORD p_CommandId = -1, float p_Weight = 1.0f);

        /// Remove a command from this set
        void RemoveCommand(const char *p_Command);

        /// Remove a command from this set
        void RemoveCommand(DWORD p_CommandId);


        /// wheter there is a reco command in the queue or not
        bool IsReco();

        /// get the next recognized command, returns true when there is a result
        bool GetReco(std::string &p_Result);


        /// Activate this SpeechSet
        void Activate();
        /// Deactivate this SpeechSet
        void Deactivate();
        /// Wether this SpeechSet is active or not
        bool IsActive();

  
        friend class CSpeechCore;
    };

} // namespace ot

#endif //ifdef USE_SAPISPEECH


#endif // OT_NO_SPEECH_SUPPORT


#endif //#if !defined(__SPEECHSET_H)

/* 
 * ------------------------------------------------------------
 *   End of SpeechSet.h
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
