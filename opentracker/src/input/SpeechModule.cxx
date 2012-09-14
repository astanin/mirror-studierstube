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
/** source file for SpeechModule module.
 *
 * @author Reinhard Steiner
 *
 * $Id: SpeechModule.cxx 2106 2008-01-02 19:42:32Z samset $
 * @file                                                                   */
/* ======================================================================= */


// this will remove the warning 4786
#include <OpenTracker/tool/disable4786.h>

#include <cstdio>
#include <iostream>

#include <OpenTracker/input/SpeechModule.h>
#include <OpenTracker/input/SpeechSource.h>
#include <OpenTracker/input/SpeechSet.h>
#include <OpenTracker/input/SpeechCore.h>


#ifndef OT_NO_SPEECH_SUPPORT


namespace ot {

	

	OT_MODULE_REGISTER_FUNC(SpeechModule){		
		OT_MODULE_REGISTRATION_DEFAULT(SpeechModule , "SpeechRecoConfig" );
	}

    // Destructor methode
    SpeechModule::~SpeechModule()
    {
        m_Nodes.clear();

        if(m_Voice)
        {
            delete(m_Voice);
            m_Voice = 0;
        }
        if(m_SpeechCore)
        {
            delete(m_SpeechCore);
            m_SpeechCore = 0;
        }
    }



    // This methode is called to construct a new Node.
    Node* SpeechModule::createNode(const std::string& name, const StringTable& attributes)
    {
        if(name.compare("SpeechRecoSource") == 0)
        {
            assert(initialized == 1);   // must be initialized

            std::string SpeechSetName, SpeechSetId;
            SpeechSetId = attributes.get("set");
            SpeechSetName = attributes.get("name");

            printf("SR: Create SpeechSource, SpeechSetName = '%s', SpeechSetId = '%s'\n", SpeechSetName.c_str(), SpeechSetId.c_str());

            SpeechSetBase *speechset = m_SpeechCore->GetSpeechSet(SpeechSetName.c_str());
            assert(speechset);

            SpeechSource *source = new SpeechSource(this, speechset);
            assert(source);


            // Search for the speechset in the config tree, and register the appropriate commands
            for(unsigned int i = 0; i < m_ConfigTree->countChildren(); ++i)
            {
                ConfigNode *SpeechSetNode = (ConfigNode*)m_ConfigTree->getChild(i);
                std::string SpeechSetId2;
                SpeechSetId2 = SpeechSetNode->getAttributes().get("id");

                if(!SpeechSetId.compare(SpeechSetId2))
                {
                    for(unsigned int j = 0; j < SpeechSetNode->countChildren(); ++j)
                    {
                        ConfigNode *CommandNode = (ConfigNode*)SpeechSetNode->getChild(j);
                        int CommandId;
                        std::string CommandName;
                        CommandNode->getAttributes().get("id", &CommandId);
                        CommandName = CommandNode->getAttributes().get("name");
                        float CommandWeight = 1.0f;
                        if(CommandNode->getAttributes().get("weight").size() > 0)
                            CommandWeight = (float)atof(CommandNode->getAttributes().get("weight").c_str());
          
                        printf("SR:  Register CommandId = %i, Command = '%s', Weight = %.2f\n", CommandId, CommandName.c_str(), CommandWeight);
                        speechset->AddCommand(CommandName.c_str(), CommandId, CommandWeight);
                    }
                }
            }

            m_Nodes.push_back(source);
            return(source);
        }
        return(NULL);
    }


    void SpeechModule::init(StringTable& attributes, ConfigNode *localTree)
    {
        Module::init( attributes, localTree );
    
        // store the config node tree for later use
        m_ConfigTree = localTree;

        std::string Language;
        Language = localTree->getAttributes().get("language");

        printf("SR: Language = '%s'\n", Language.c_str());


        // create the sr core component
#ifdef USE_SAPISPEECH
        m_SpeechCore = new CSpeechCore;
#else
        m_SpeechCore = new SpeechCoreBase;
#endif
        assert(m_SpeechCore);

        if(!Language.compare("english"))
        {
            m_SpeechCore->Init();
        }
        else
        {
            printf("SR: ERROR: Invalid Language: '%s'\n", Language.c_str());
            m_SpeechCore->Init();
        }

        // get the standard system voice
        m_Voice = new SpeechVoiceModule;
        assert(m_Voice);


        //m_Voice->Speak("The SR system is initialized!");
    }


    void SpeechModule::close()
    {
        if(m_Voice)
        {
            delete(m_Voice);
            m_Voice = 0;
        }
        if(m_SpeechCore)
        {
            delete(m_SpeechCore);
            m_SpeechCore = 0;
        }
    }


    // pushes events into the tracker tree.
    void SpeechModule::pushEvent()
    {
        if(m_SpeechCore)
        {
            assert(m_SpeechCore);
            assert(m_Voice);

            if(m_Push2Nodes.size() > 0)
            {
                for(unsigned int i = 0; i < m_Push2Nodes.size(); ++i)
                {
                    SpeechSource *source = (SpeechSource*) ((Node*)m_Push2Nodes[i]);
                    source->push2();
                }
                m_Push2Nodes.clear();
            }
            else if(m_SpeechCore->ProcessRecognitionPoll())
            {
                //printf("SR: SpeechModule::pushEvent\n");

                for(NodeVector::iterator it = m_Nodes.begin(); it != m_Nodes.end(); it++)
                {
                    SpeechSource *source = (SpeechSource*)((Node*)*it);
                    if(source->push())
                    {
                        m_Push2Nodes.push_back(source);
                    }
                }
            }
        }
    }


    bool SpeechModule::GetCommand(DWORD p_CommandId, DWORD p_SpeechSetId, std::string &p_Command)
    {
        // try to get the speech set via the SpeechSetId
        SpeechSetBase *set = m_SpeechCore->GetSpeechSet(p_SpeechSetId);
        if(!set)
        {
            printf("SR: ERROR: Invalid SpeechSet Id\n");
            return(false);
        }

        // retrieve the command via the CommandId from the speechset
        return(set->GetCommand(p_CommandId, p_Command));
    }


    void SpeechModule::Speak(const char *p_Sentence, bool p_Async)
    {
        assert(m_Voice);
        m_Voice->Speak(p_Sentence, p_Async);
    }

} // namespace ot


#else
#ifdef WIN32
#pragma message(">>> OT_SPEECH_SUPPORT")
#endif
#endif // OT_SPEECH_SUPPORT

/* 
 * ------------------------------------------------------------
 *   End of SpeechModule.cxx
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
