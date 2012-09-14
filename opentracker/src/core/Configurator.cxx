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
/** Configurator class
 *
 * @author Eduardo Veas
 *
 * $Id$
 * @file                                                                   */
/* ======================================================================= */

#include <OpenTracker/OpenTracker.h>
#include <OpenTracker/core/Configurator.h>
#include <OpenTracker/core/Context.h>
#ifdef USE_LIVE
#include <OpenTracker/core/LiveContext.h>
#endif
#include <OpenTracker/core/ThreadContext.h>
#include <OpenTracker/misc/FileConfigurationThread.h>
#include <iostream>
#include <fstream>
using namespace std;
namespace ot{

    Configurator * Configurator::self = NULL;
    Configurator::Registry Configurator::initFunctions;

    Configurator::Configurator(int ctx_type):ctx(NULL), thread(NULL)
    {
        switch (ctx_type)
        {
            case NORMAL:
                ctx = new Context(0);
                break;
            case THREAD:
                ctx = new ThreadContext(0);
                break;
#ifdef USE_LIVE
            case LIVE:
                ctx = createLiveContext();
                break;
#endif
            default:
                ctx = new Context(0);
                break;
        }
        //this->loadModule(*ctx, "OpenTracker");
        this->doInitialization(*ctx);
    }


    Configurator * Configurator::instance(int ctx_type)
    {
	if (self == NULL){
            initializeOpenTracker();
            //addModuleInit("OpenTracker", initializeContext, NULL);
            self = new Configurator(ctx_type);
	}
	return self;
    }

#ifdef USE_LIVE
    LiveContext* Configurator::createLiveContext() 
    {
        LiveContext* context_impl = NULL;
        try
        {
            context_impl = new LiveContext("giop:tcp::12090");
            cerr << "got LiveContext instance" << endl;
            ModuleMap modules = context_impl->getModules();
            ModuleMap::iterator module_iterator  = modules.find("CORBAConfig");
            if ( module_iterator == modules.end() ) {
                exit(-1);
            } 
            cerr << "found CORBA module" << endl;
            CORBAModule *corba_module = (CORBAModule*) module_iterator->second.item();
            if (corba_module == NULL) {
                cerr << "cast from iterator failed. Exiting...";
                exit(-1);
            }
            CORBA::ORB_var orb = corba_module->getORB();
            if (CORBA::is_nil(orb)) {
                cerr << "Unable to obtain orb reference. Exiting...";
                exit(-1);
            }
            cerr << "got reference to ORB" << endl;
            //PortableServer::POA_var poa = corba_module->getPOA();
            PortableServer::POA_var poa = corba_module->getINSPOA();
            PortableServer::POAManager_var poa_manager = corba_module->getINSPOAManager();
            poa_manager->activate();

            if (CORBA::is_nil(poa)) {
                cerr << "got nil reference to POA. Exiting...." << endl;
                exit(-1);
            }
            POA_OTGraph::DataFlowGraph_tie<LiveContext>* context = new POA_OTGraph::DataFlowGraph_tie<LiveContext>(context_impl);
             
            //PortableServer::ObjectId_var id = corba_module->getRootPOA()->activate_object(context);
            PortableServer::ObjectId_var corba_id = PortableServer::string_to_ObjectId("DFG");
            poa->activate_object_with_id(corba_id, context);
            CORBA::Object_var obj = poa->id_to_reference(corba_id);
             
            // Obtain a reference to the object, and register it in
            // the naming service.
            //CORBA::Object_var obj = corba_module->getRootPOA()-> id_to_reference(corba_id);
             
            CosNaming::NamingContextExt::StringName_var string_name = "Stb.Context";
            CORBAUtils::bindObjectReferenceToName(orb, obj, string_name);  
        }
        catch(CORBA::SystemException&) {
            logPrintE("Caught CORBA::SystemException.\n");
        }
        catch(CORBA::Exception&) {
            logPrintE("Caught CORBA::Exception.\n");
        }
        catch(omniORB::fatalException& fe) {
            logPrintE("Caught omniORB::fatalException:\n");
            logPrintE("  file: %s\n", fe.file());
            logPrintE("  line: %s \n", fe.line());
            logPrintE("  mesg: %s \n", fe.errmsg());
        }
        catch(...) {
            logPrintE("Caught unknown exception.\n");
        }
        
        // return the context obtained
        return context_impl;
    }
#endif

    Configurator::~Configurator()
    {
        if (thread != NULL)
        {
            FileConfigurationThread * ct = dynamic_cast<FileConfigurationThread *>(thread);
            ct->finish();
            OSUtils::sleep(1000);
            delete ct;
	}

        if (ctx) delete ctx;
    }
    
    void Configurator::doInitialization(Context & newctx){
        for (Registry::iterator i = initFunctions.begin(); i != initFunctions.end(); i++)
        {
			//logPrintS("initialization running function of module %s\n", (i->first.c_str()));
            (* (i->second).function) (&newctx, (i->second).data);
        }
    }

    void Configurator::loadModule(Context & newctx, const char * module)
    {
        logPrintS("Configurator loading module %s\n", module);
        Registry::iterator i = initFunctions.find(module);
        if (i != initFunctions.end()){
            (* (i->second).function) (&newctx, (i->second).data);
        } else
            logPrintE("module %s not found\n", module);
    };


    void Configurator::addModuleInit(const char * name, ModuleInitFunc function, 
                                     void * data)
    {
        MIFunctor functor;
        functor.function = function;
        functor.data = data;
        //logPrintS("Configurator adding %s = %p \n", name, function);
    
        initFunctions[name]=(functor);
    }
    
    
    
    void Configurator::parseConfigurationString(std::string configString)
    {
  
    }

    Context * Configurator::getContext(){
	return ctx;
    }

    void Configurator::changeConfiguration(std::string configString)
    {
        // check whether we have a file or string
        if ( (configString.substr(configString.length() - 4)) == ".xml"){
            // its a file
            changeConfigurationFile(configString.c_str());
        } else {
            // its a string
            changeConfigurationString(configString.c_str());
        }   
    }

    void Configurator::changeConfigurationFile(const char * file)
    {
        // check whether the runtime context is already configured
        if (ctx->isConfigured())
        {
            logPrintI("Configurator::changeConfigurationFile using updating a initialized context\n");
            //if it is, we want to change its configuration
        
            //create a new context object
            Context newContext(0);
            //this->loadModule(newContext, "OpenTracker");
            this->doInitialization(newContext);
        
            newContext.parseConfiguration(file);
        
            this->ctx->copyFrom(newContext);
        } 
        else 
        {
            logPrintI("Configurator::changeConfigurationString using a virgin context\n");
            // if it has not been configured for the first time, do it now
            ctx->parseConfiguration(file);
        }
    
    }


    // not up to date

    void Configurator::changeConfigurationString(const char* xmlstring) 
    {
        if(ctx->isConfigured()){
            logPrintI("Configurator::changeConfigurationString using updating a initialized context\n");
            //create a new context object
            Context newContext(0);
            
            this->doInitialization(newContext);            
            /*			std::ofstream of("tmp.xml");
                                of << xmlstring << flush;
                                printf("CONFIGURATOR::CHANGING CONFIGURATION %s\n", xmlstring);
                                of.close();
            */
            newContext.parseConfigurationString(xmlstring);
            
            this->ctx->copyFrom(newContext);
        } else {
            logPrintI("Configurator::changeConfigurationString using a virgin context\n");
            this->ctx->parseConfigurationString(xmlstring);
        }
    }

    void Configurator::runConfigurationThread(const char * filename)
    {
        if (thread == NULL){
            // Create reconfiguration thread
            FileConfigurationThread * fct= new FileConfigurationThread( filename );
            // start it up
            fct->start();
            
            thread = fct;
        }
    }
    
} //namespace ot
/* 
 * ------------------------------------------------------------
 *   End of Configurator.cxx
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
