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
/** source file for the Ubisense interface module.
 *
 * @author Mathis Csisinko
 *
 * $Id: UbisenseModule.cxx 2131 2008-02-29 16:28:36Z mathis $
 * @file                                                                   */
/* ======================================================================= */

// this will remove the warning 4786
#include <OpenTracker/tool/disable4786.h>

#include <OpenTracker/input/UbisenseModule.h>
#include <OpenTracker/input/UbisenseSource.h>
#include <iostream>

#ifdef USE_UBISENSE
#ifdef WIN32

#include <ace/Log_Msg.h>

#include "UClientAPI/name_client.h"
#include "UClientAPI/location_client.h"

#pragma comment(lib,"UClientAPI.lib")

namespace ot {


	OT_MODULE_REGISTER_FUNC(UbisenseModule){
		OT_MODULE_REGISTRATION_DEFAULT(UbisenseModule , "UbisenseConfig" );
	}


    UbisenseModule::UbisenseModule(): Module(),NodeFactory(),locationClient(),dataClient(pSources),cells(locationClient.get_all_cells())
        //UbisenseModule::UbisenseModule(): Module(),NodeFactory(),locationClient(),dataClient(pSources)
    {
    }

    UbisenseModule::~UbisenseModule()
    {
#ifdef USE_LIVE
        for (NodeVector::iterator it = pSources.begin();it != pSources.end();it ++)
            delete *it;
#endif
        pSources.clear();
    }

    Node* UbisenseModule::createNode(const std::string &name,StringTable &attributes)
    {
        if (name.compare("UbisenseSource") == 0)
        {
            NameClient nameClient;
            Object object;
            const char* objectString = attributes.get("object").c_str();
            UClientAPI::String str(objectString);
            if (nameClient.get_named_object(objectString,object))
            {
                UbisenseSource* pSource = new UbisenseSource(object,locationClient,dataClient);
                pSources.push_back(pSource);

				logPrintI("Build UbisenseSource node\n");
				initialized = 1;
				return pSource;
            }
        }
        return NULL;
    }

    void UbisenseModule::start()
    {
		for (UClientAPI::Set<String>::const_iterator i = cells.begin();i != cells.end();++ i)
            locationClient.load_cell(*i);
    }

    void UbisenseModule::close()
    {
		locationClient.unload_all_cells();
    }

    void UbisenseModule::pushEvent()
    {
        for (NodeVector::iterator it = pSources.begin();it != pSources.end();it ++)
        {
            UbisenseSource* pSource = static_cast<UbisenseSource*>((Node*)*it);
            if (pSource->calcEvent())
                pSource->updateObservers(pSource->getEvent());
        }
    }

    void UbisenseModule::init(StringTable &attributes,ConfigNode* pLocalTree)
    {
		NameClient nameClient;
		Object object;

		cells.clear();
		for (unsigned int i = 0;i < pLocalTree->countChildren();i ++)
		{
            ConfigNode* pNode = static_cast<ConfigNode*>(pLocalTree->getChild(i));
            if (! pNode->getType().compare("UbisenseCell"))
            {
                ot::StringTable& attributes = pNode->getAttributes();
                std::string str = attributes.get("name");
                cells.insert(str.c_str());

				ACE_DEBUG((LM_DEBUG,ACE_TEXT("ot:Ubisense location cell:\n")));
                for (UClientAPI::Set<String>::const_iterator i = cells.begin();i != cells.end();++ i) 
					ACE_DEBUG((LM_DEBUG,ACE_TEXT("ot: %s\n"),i->c_str()));
                if (cells.empty()) 
					ACE_DEBUG((LM_DEBUG,ACE_TEXT(" No available cells\n")));

                //cell_name = *(cells.begin()); 
				//ACE_DEBUG((LM_DEBUG,ACE_TEXT("ot:Loading cell %s\n"),cell_name.c_str()));
            }
		}
    }


    UbisenseModule::WrappedDataClient::WrappedDataClient(NodeVector &pSources): DataClient(),pSources(pSources)
    {
    }

    void UbisenseModule::WrappedDataClient::on_button(const Object &tag,UbitagButton button,double time)
    {
		Object owner;
		get_tag_owner(tag,owner);
        for (NodeVector::iterator it = pSources.begin();it != pSources.end();it ++)
        {
            UbisenseSource* pSource = static_cast<UbisenseSource*>((Node*)*it);
            const Object &object = pSource->getObject();
            if (tag == object || owner == object)
                pSource->setButtonEvent(button == Orange ? 0x0001: button == Blue ? 0x0002: 0x0000,time);
		}
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
 *   End of UbisenseModule.cxx
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
