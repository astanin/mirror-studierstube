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
/** source file for DwarfModule module.
 *
 * @author Gerhard Reitmayr
 *
 * $Id: DwarfModule.cxx 1547 2006-10-25 10:33:10Z veas $
 * @file                                                                   */
/* ======================================================================= */

// this will remove the warning 4786
#include <OpenTracker/tool/disable4786.h>

#include <OpenTracker/network/DwarfModule.h>
#include <OpenTracker/network/DwarfSource.h>
#include <OpenTracker/network/DwarfSink.h>

#ifdef USE_DWARF

#include <cstdio>
#include <cstdlib>

#include <DWARF/Service.h>
#include <DWARF/PoseData.h>
#include <common/cpp/PoseSender.h>
#include <middleware/helpers/corbainit_cpp/corbainit.h>

//using namespace std;
using namespace DWARF;

namespace ot {

	

	OT_MODULE_REGISTER_FUNC(DwarfModule){
        OT_MODULE_REGISTRATION_DEFAULT(DwarfModule , "DwarfConfig" );
	}

    // Destructor method

    DwarfModule::~DwarfModule()
    {
    }

    void DwarfModule::init(StringTable & attributes, ConfigNode * localTree)
    {
        string param = "";
        // passes the service manager address on to CORBA, if given
        if( attributes.containsKey("servicemanager") )
        {
            param = "-DserviceManager=";
            param.append( attributes.get("servicemanager"));
        }
        char * argc = (char *) param.c_str();
        myOrbConnection = CorbaInit::initializeOrb( 1, & argc); 
    
        string name = attributes.get("name");
        // construct and send service description, if required !
        if( attributes.get("senddescription").compare("true") == 0)
        {        
            ServiceManager_var smgr = myOrbConnection->getServiceManager();
        
            ServiceDescription_var sd;
            NeedDescription_var nd;
            AbilityDescription_var ad;
            ConnectorDescription_var pd;                
        
            sd=smgr->newServiceDescription(name.c_str());
            sd->setStartOnDemand(false);
            sd->setStopOnNoUse(false);
        
            unsigned int i;
            for( i = 0;i < localTree->countChildren(); i++)
            {
                ConfigNode * child = (ConfigNode*)localTree->getChild(i);
                StringTable & strTab = child->getAttributes();
                if( child->getType().compare("ability") == 0)
                {
                    ad = sd->newAbility(strTab.get("name").c_str());
                    ad->setType(strTab.get("type").c_str());
                
                    thingIdMap[strTab.get("name")] = strTab.get("thingid");
                    thingTypeMap[strTab.get("name")] = strTab.get("thingtype");
                
                    unsigned int j;
                    for( j = 0; j < child->countChildren(); j++)
                    {
                        ConfigNode * subChild = (ConfigNode *) child->getChild(j);
                        StringTable & strTab = subChild->getAttributes();
                        if( subChild->getType().compare("attribute") == 0)
                        {
                            ad->setAttribute(strTab.get("name").c_str(), strTab.get("value").c_str());
                        }
                        else if( subChild->getType().compare("connector") == 0)
                        {
                            pd=ad->newConnector(strTab.get("protocol").c_str());                    
                        }
                    }
                }		
                else if( child->getType().compare("need") == 0)
                {
                    nd = sd->newNeed(strTab.get("name").c_str());
                    nd->setType(strTab.get("type").c_str());
                    nd->setPredicate(strTab.get("predicate").c_str());
                    nd->setDelegated(strTab.get("delegated").c_str());
                
                    unsigned int j;
                    for( j = 0; j < child->countChildren(); j++)
                    {
                        ConfigNode * subChild = (ConfigNode *)child->getChild(j);
                        StringTable & strTab = subChild->getAttributes();
                        if( subChild->getType().compare("attribute") == 0)
                        {
                            nd->setAttribute(strTab.get("name").c_str(), strTab.get("value").c_str());
                        }
                        else if( subChild->getType().compare("connector") == 0)
                        {
                            pd=nd->newConnector(strTab.get("protocol").c_str());                    
                        }
                    }
                }
                else
                    ACE_LOG_INFO("ot:DwarfModule got something strange here!\n");
            }        
            smgr->activateServiceDescription(name.c_str());
        }

        service = new PoseSenderService(name.c_str());
        myOrbConnection->registerService(name.c_str(), service->_this());
        service->blockUntilRunning();

        Module::init( attributes, localTree );
    }

    // This method is called to construct a new Node.

    Node * DwarfModule::createNode( const string& name, StringTable& attributes)
    {
        if( name.compare("DwarfSource") == 0 )
        {
            DwarfSource * source = NULL;
            return source;
        }
        else if( name.compare("DwarfSink") == 0 )
        {
            DwarfSink * sink = NULL;

            PoseSender * sender = service->getPoseSender(attributes.get("name"));
            if( sender != NULL )
            {
                sink = new DwarfSink( attributes.get("name"));
                sinks.push_back( sink );
            
                sender->setPosAccuracy(0);
                sender->setOriAccuracy(0);
            
                sender->setThingID(& thingIdMap[attributes.get("name")] );
                sender->setThingType(& thingTypeMap[attributes.get("name")] );
            }
            return sink;
        }
        return NULL;
    }

    // pushes events into the tracker tree.

    void DwarfModule::pushEvent()
    {
        // here we would push new events from any needs/DwarfSources into the graph
    }

    void DwarfModule::pullEvent()
    {
        vector<DwarfSink *>::iterator it;
        for( it = sinks.begin(); it != sinks.end(); it ++ )
        {
            DwarfSink * sink = (*it);
            if( sink->changed )
            {
                PoseSender * sender = service->getPoseSender( sink->name );
                if( sender != NULL )
                {
                    // FIXME: better to use the array copy, after switch to doubles !!
                    sender->setPos( 0, sink->event.getPosition()[0]);
                    sender->setPos( 1, sink->event.getPosition()[1]);
                    sender->setPos( 2, sink->event.getPosition()[2]);
                    sender->setOri( 0, sink->event.getOrientation()[0]);
                    sender->setOri( 1, sink->event.getOrientation()[1]);
                    sender->setOri( 2, sink->event.getOrientation()[2]);
                    sender->setOri( 3, sink->event.getOrientation()[3]);

                    // FIXME: use opentracker time with setTimestamp
                    sender->updateTimestamp();

                    sender->sendPoseData();
                }
                //  create event here and send it !
                sink->changed = false;
            }
        }
    }

} // namespace ot


#else
#ifdef WIN32
#pragma message(">>> no DWARF support")
#endif
#endif // USE_DWARF

/* 
 * ------------------------------------------------------------
 *   End of DwarfModule.cxx
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
