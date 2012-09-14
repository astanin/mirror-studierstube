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
/** source file for Context class
 *
 * @author Gerhard Reitmayr
 *
 * $Id: Context.cxx 2133 2008-03-11 17:06:47Z mendez $
 * @file                                                                   */     
/* ======================================================================= */

#ifdef WIN32
#pragma warning(disable:4244)
#endif

#include <cstdlib>
#include <OpenTracker/tool/FixWinCE.h>
#include <ace/OS.h>
#include <ace/FILE.h>
#include <ace/Thread_Mutex.h>
#include <ace/Condition_Thread_Mutex.h>
#include <ace/Thread_Mutex.h>
#include <algorithm>

#include <OpenTracker/OpenTracker.h>

#include <OpenTracker/core/ConfigurationParser.h>
#include <OpenTracker/common/CommonNodeFactory.h>
#include <OpenTracker/common/CallbackModule.h>

#include <OpenTracker/core/Configurator.h>
#ifdef USE_LIVE
#include <omniORB4/CORBA.h>
#include <OpenTracker/network/CORBAModule.h>
#include <OpenTracker/common/StaticTransformation.h>
#endif


namespace ot {

    Context::Context( int init ) :
        //        rootNode( NULL ),
        cleanUp( false ),
        stoploopflag(false),
        dosync(false),
        rootNamespace( "" ),
        lastlooptime(0.0),
        loopcount(0),
        isStarted(false)
    {

#ifdef USE_LIVE
        activatelive = true;
#endif
        // initialize graph
	graph = new Graph();


        if( init != 0 )
        {
            logPrintS("Context::Constructor initializing self with %p\n", this);
            initializeContext( this , NULL);
            cleanUp = true;
        }
        else {
            cleanUp = false;
        }
        directories.push_back(".");
        //#ifdef WIN32
        _havedatamutex = new ACE_Thread_Mutex("context_havedatamutex");
        _consumeddatamutex = new ACE_Thread_Mutex("context_consumeddatamutex");
        _havedatacondition = new ACE_Condition_Thread_Mutex((*_havedatamutex));//, "context_havedatacondition");
        _waitingpendingcondition = new ACE_Condition_Thread_Mutex((*_havedatamutex));//, "context_havedatacondition");
        _consumeddatacondition = new ACE_Condition_Thread_Mutex((*_consumeddatamutex));//, "context_havedatacondition");
        _waitingconsumedcondition = new ACE_Condition_Thread_Mutex((*_consumeddatamutex));//, "context_havedatacondition");
        pendingdata = false;
        waitingpending = false;
        dataconsumed = false;
        waitingconsumed = false;
        //#else
        //        _havedatamutex = new pthread_mutex_t;
        //        _havedatacondition = new pthread_cond_t;
        //#endif
        _mutex = new ACE_Thread_Mutex("context_mutex");
    }

    // Destructor method.
    Context::~Context()
    {
        using namespace std;
        logPrintI("Context::~Context()\n");

        // stop the loop, if it is running
        stopLoop();

        // delete all modules
	/*        if( cleanUp )
        {
            for( ModuleMap::iterator it = modules.begin(); it != modules.end(); it++ )
            {
                delete (*it).second;
            }
	    }*/
        modules.clear();
        //        if (rootNode != NULL) {
        //        }

        //#ifdef WIN32
        delete _havedatacondition;
        delete _waitingpendingcondition;
        delete _consumeddatacondition;
        delete _waitingconsumedcondition;
        delete _havedatamutex;
        delete _consumeddatamutex;

        //#else
        //        pthread_cond_destroy(_havedatacondition);
        //        pthread_mutex_destroy(_havedatamutex);
        //#endif
        delete _mutex;
    }

     void Context::lock() { _mutex->acquire(); };
     void Context::unlock() { _mutex->release(); };

    // adds a new factory to the NodeFactoryContainer

    void Context::addFactory(NodeFactory& newfactory)
    {
        factory.addFactory( newfactory );
    }

    // removes a factory from the NodeFactoryContainer

    void Context::removeFactory(NodeFactory & oldfactory)
    {
        factory.removeFactory( oldfactory );
    }

    // adds a module to the contexts collection

    void Context::addModule(const std::string & name, Module & module)
    {
        modules[name] = &module;
        module.context = this;
    }

    void Context::updateModuleContexts()
    {
        ModuleMap::iterator it;
        for (it=modules.begin(); it != modules.end(); it++)
        {
            it->second->setContext(this);
        }
    }
    // returns a module indexed by its configuration elements name

    Module * Context::getModule(const std::string & name)
    {	
        //logPrintI("getModule(%s)\n", name.c_str());
        Module * result = NULL;
        ModuleMap::iterator it = modules.find( name );
        if( it != modules.end()){
        //    logPrintI("found module\n");
            result = (*it).second;
        } else {
            //            logPrintI("Couldn't find module\n");
            std::string modname;
            std::string::size_type loc = name.find("Config");
            modname = name.substr(0, loc) + "Module";
            logPrintW("Context could not find module %s, requesting Configurator to load %s\n", name.c_str(), modname.c_str());            

            Configurator::loadModule(*this, modname.c_str());
            it = modules.find(name);
            if (it != modules.end())
                result = (*it).second;
			else logPrintD("Couldn't find module 2nd time\n");
        }

        return result;
    }

    // returns a list of module names

    int Context::getModuleNames(std::vector<std::string> &names) const
    {	
        int retval = 0;
        names.clear();

        ModuleMap::const_iterator it;

        retval = modules.size();
        names.reserve(retval);

        for (it=modules.begin(); it != modules.end(); it++)
        {
            names.push_back(it->first);
        }

        return retval;
    }

    // removes a module

    void Context::removeModule(Module & module)
    {
        ModuleMap::iterator it = modules.begin();
        while( it != modules.end())
        {
            if((*it).second == &module )
                modules.erase( it-- );
            it++;
        }
    }

    // calls start on all modules to do some initialization.

    void Context::start()
    {
        if(isStarted) logPrintW("Conmtext is already started\n");
        for( ModuleMap::iterator it = modules.begin(); it != modules.end(); it++ )
        {
            (*it).second->start();
        }
		isStarted = true;
    }

    // calls close on all modules to close any resources.

    void Context::close()
    {
        int i = 0;
        logPrintI("Closing Opentracker Context ... \n");
        for( ModuleMap::iterator it = modules.begin(); it != modules.end(); it++ )
        {
            (*it).second->close();
            i++;
        }
        logPrintI("Closing Opentracker Context done.\n");
        // HACK: give some threads time to close down
        OSUtils::sleep(1000);
        isStarted = false;
    }

    // parses the file and builds the tree.

    void Context::parseConfiguration(const std::string& filename)
    {

        file = filename;
        std::string::size_type limit = file.find_last_of( "/\\" );
        if( limit != std::string::npos )
            addDirectoryFirst( file.substr(0, limit));
		
        ConfigurationParser *  parser = getConfigurationParser(*this);

        graph = parser->parseConfigurationFile( filename );
        graph-> writeGraph("graph.txt");

        delete parser;
#ifdef USE_LIVE
        liveContextConsistencyCheck();
#endif        
    }

#ifdef USE_LIVE
    void Context::liveContextConsistencyCheck() {
        edges.clear();
        for (NodeIDMapIterator it=node_id_map.begin(); it != node_id_map.end(); ++it) 
        {
            Node* node = it->first;
            for (NodeVector::const_iterator n_it=node->parents.begin(); n_it != node->parents.end(); ++n_it) {
                edges.push_back(Edge( it->first, (*n_it)));
            }
        }
    }
#endif

    void Context::parseConfigurationString(const char* xmlstring)
    {

        ConfigurationParser * parser = getConfigurationParser( *this );
        graph = parser->parseConfigurationString( xmlstring );

        rootNamespace = "";
        delete parser;
#ifdef USE_LIVE
        liveContextConsistencyCheck();
#endif        

    }


    // calls pullEvent on all modules to get data out again.

    void Context::pullEvents()
    {
        for( ModuleMap::iterator it = modules.begin(); it != modules.end(); it++ )
        {
            (*it).second->pullEvent();
        }

        graph->pullEvents();
    }

    // This method calls pushEvent on all modules to get new data into the shared data tree.

    void Context::pushEvents()
    {
        //logPrintI("modulessize: %d", modules.size());
        for( ModuleMap::iterator it = modules.begin(); 
             it != modules.end(); it++ )
        {
        
            (*it).second->pushEvent();
        }
        graph->pushEvents();
    }


    int Context::loopOnce()
    {
        using namespace std;
        //logPrintI("Context::loopOnce() ...\n");

        double looptime = OSUtils::currentTime();

       if (loopcount == 0)
	 {
	    lastlooptime = looptime;
	    loopcount = 1;
	 }
       else if (looptime - lastlooptime > 10000.0)
	 {
	    loopcount++;
	    logPrintD("OpenTracker loop rate: %f Hz\n", loopcount/10.0);
	    loopcount=0;
	 }
       else
	 {
	    loopcount++;
	 }

       int stopflag=1;
       // lock the Graph first
       lock();
       
       // push and pull parts of the main loop
       pushEvents();
       pullEvents();

        // check for stop flag
        stopflag = stop(); 

        // unlock the graph
        unlock();
        //cerr << "done."<< endl;

        //double loopetime = OSUtils::currentTime();
        //logPrintI("looptime : %lf\n", loopetime - looptime);
        return stopflag;
    }
   
    int Context::syncLoopOnce()
    {
        // store synchronization settings
        bool syncsetting = dosync;
        dosync = true;
       
        //double looptime = OSUtils::currentTime();
       
        _havedatamutex->acquire();	
       
        //logPrintI("Context: Waiting for new data\n");
        waitingpending = true;
        _waitingpendingcondition->broadcast();
            
        while (!pendingdata)
	{
	    _havedatacondition->wait(); 
	}
        pendingdata = false;

        //logPrintI("Context: got data -> processing\n");
            
        //double loopetime = OSUtils::currentTime();
        //logPrintI("lock acquisition time : %lf\n", loopetime - looptime);

        bool stopflag = loopOnce();

        _havedatamutex->release();

        _consumeddatamutex->acquire();
        if (stoploopflag == 0)
	 {
	    while (!waitingconsumed)
	      {
		 _waitingconsumedcondition->wait();
	      }
	 }
        waitingconsumed = false;

        //logPrintI("Context: data processing finished -> telling drivers\n");
        dataconsumed = true;
        _consumeddatacondition->broadcast();
        //logPrintI("Context:  telling drivers done\n");
        _consumeddatamutex->release();
       
        // restore synchronization settings
        dosync = syncsetting;
       
       logPrintI("Context::syncLoopOnce returning ...\n");
        return stopflag;
    }
   
    // This method implements the main loop and runs until it is stopped somehow.

    void Context::run()
    {
        using namespace std;

        logPrintI( "Context::run()\n" );

        setSynchronization(false);

        start();
        int stopflag = stop();
        while ( stoploopflag == 0 && stopflag == 0 )
        {
            //logPrintI("running\n");
            stopflag=loopOnce();	    
        }

        stoploopflag = 0;

        logPrintI("closing loop \n");
        close();
    }

    // This method implements the main loop and runs until it is stopped somehow.
    // This differs from the run() method in that a (crude) attempt is made 
    // to run at a particular rate. Affected by OS-specific implementation 
    // of wakeup and preemption events; however, can act as a throttle.

    void Context::runAtRate(double rate)
    {
        using namespace std;
		logPrintI( "Context::runAtRate: %f Hz\n", rate );

        setSynchronization(false);
   
        double t1 = OSUtils::currentTime(); // in milliseconds
        start();
        int stopflag = stop();
        while ( stoploopflag == 0 && stopflag == 0 )
        {
            // push and pull parts of the main loop
  
            stopflag = loopOnce();
  
            double t2 = OSUtils::currentTime(); // in milliseconds
            double sleep_time = 1/rate*1000.0 - (t2 - t1);
            if (sleep_time > 0.f) {
                OSUtils::sleep(sleep_time);
            }
            t1 = t2;
        }

        stoploopflag = 0;

        logPrintI( "closing loop\n");
        close();
    }

    void Context::runOnDemand()
    {
        using namespace std;

	setSynchronization(true);

        logPrintI("Context::runOnDemand()\n");

        start();

        _consumeddatamutex->acquire();
        dataconsumed = true;
        _consumeddatacondition->broadcast();        
        _consumeddatamutex->release();

        _havedatamutex->acquire();	
        pendingdata = false;
        _havedatacondition->broadcast();
        _havedatamutex->release();	

        int stopflag = stop();
        while ( stoploopflag ==0 && stopflag == 0 )
        {
            //double looptime = OSUtils::currentTime();

            _havedatamutex->acquire();	

            //logPrintI("Context: Waiting for new data\n");
            waitingpending = true;
            _waitingpendingcondition->broadcast();
            
            while (!pendingdata)
            {
                _havedatacondition->wait(); 
            }
            pendingdata = false;

            //logPrintI("Context: got data -> processing\n");
            
            //double loopetime = OSUtils::currentTime();
            //logPrintI("lock acquisition time : %lf\n", loopetime - looptime);

            stopflag = loopOnce();

            _havedatamutex->release();

            _consumeddatamutex->acquire();
            if (stoploopflag == 0)
            {
                while (!waitingconsumed)
                {
                    _waitingconsumedcondition->wait();
                }
            }
            waitingconsumed = false;

            //logPrintI("Context: data processing finished -> telling drivers\n");
            dataconsumed = true;
            _consumeddatacondition->broadcast();
            //logPrintI("Context:  telling drivers done\n");
            _consumeddatamutex->release();
        } // loop

        _havedatamutex->acquire();	
        waitingpending = true;
        _waitingpendingcondition->broadcast();
        
        if (stoploopflag == 0)
        {
            while (!pendingdata)
            {
                _havedatacondition->wait(); 
            }
        }
        pendingdata = false;
        _havedatamutex->release();
        
        logPrintI("closing loop\n");
        close();

        stoploopflag = 0;

	setSynchronization(true);
    }

    void Context::waitDataSignal() 
    {
        //logPrintI(" Context::waitDataSignal()\n");
        _havedatamutex->acquire();	

        while (!pendingdata)
        {
            _havedatacondition->wait(); 
        }
        pendingdata = false;

        _havedatamutex->release();
    };
    void Context::dataSignal() 
     { 
         //logPrintI(" Context::dataSignal()\n");
         
        _havedatamutex->acquire();

        if (stoploopflag == 0)
        {
            while (!waitingpending)
            {
                _waitingpendingcondition->wait();
            }
            waitingpending = false;
        }

         pendingdata = true;
         _havedatacondition->signal();
 
         _havedatamutex->release();        
     };
   
    void Context::dataBroadcast() 
     { 
         _havedatamutex->acquire();

        if (stoploopflag == 0)
        {
            while (!waitingpending)
            {
                _waitingpendingcondition->wait();
            }
            waitingpending = false;
        }

        pendingdata = true;
        _havedatacondition->broadcast(); 
        
        _havedatamutex->release();
     };

    void Context::dataLock()
    {
        _havedatamutex->acquire();
    }
    void Context::dataUnlock()
    {
        _havedatamutex->release();
    }

#ifdef WIN32
    void Context::consumedWait(int usecs)
#else
    void Context::consumedWait(suseconds_t usecs)
#endif
    {
        _consumeddatamutex->acquire();
        waitingconsumed = true;
        _waitingconsumedcondition->broadcast();
        
        if (stoploopflag == 0)
        {
            while (!dataconsumed)
            { 
                if (usecs != 0)
                {
                    ACE_Time_Value tv(0, usecs);                
                    if (_consumeddatacondition->wait(&tv) == -1)
                    {
                        //logPrintW("main loop processing events too slow!\n");
                        break;
                    }
                }
                else
                {
                    _consumeddatacondition->wait();
                }
            }
            
        }
        dataconsumed = false;
        _consumeddatamutex->release();
    }

    void Context::consumedSignal()
    {
        //dataconsumed = true;
        //_consumeddatacondition->broadcast();


        _consumeddatamutex->acquire();
        //logPrintI("Context: data processing finished -> telling drivers\n");
        dataconsumed = true;
        _consumeddatacondition->broadcast();
        //logPrintI("Context:  telling drivers done\n");
        _consumeddatamutex->release();

    }

    void Context::consumedBroadcast()
    {
        _consumeddatamutex->acquire();
        //logPrintI("Context: data processing finished -> telling drivers\n");
        
	dataconsumed = true;
        _consumeddatacondition->broadcast();
        
	//logPrintI("Context:  telling drivers done\n");
        
	_consumeddatamutex->release();

    }
   
    // tests all modules for stopping

    int Context::stop()
    {
        int value = 0;
        for( ModuleMap::iterator it = modules.begin(); it != modules.end(); it++ )
        {
            int retval = (*it).second->stop();
            /*
            if (retval != 0)
            {
                cout << it->first << " not ready for stop" << endl;
            }
            else
            {
                cout << it->first << " ready for stop" << endl;
                }
            */
            value |= retval;
        }
        return value;
    }

    void Context::stopLoop()
    {
        lock();
        stoploopflag = 1;
        dataSignal();
        dataSignal();
        unlock();
    }

#ifdef USE_LIVE
    std::string Context::getIDFromNodeRef(const OTGraph::Node_var& node_ref) {
        CORBAModule* corba_module = (CORBAModule*) modules["CORBAConfig"].item();
        PortableServer::POA_var poa = corba_module->getPOA();
        PortableServer::ObjectId_var node_id = poa->reference_to_id(OTGraph::Node::_duplicate(node_ref));
        std::string id(PortableServer::ObjectId_to_string(node_id));
        return id;
    }    

    std::string Context::getIDFromNode(Node* node) {
        return node_id_map[node];
    }

    OTGraph::Node_var Context::getNodeRefFromID(const std::string& id) {
        logPrintI("getNodeRefFromID(%s)\n",id.c_str());
        CORBAModule* corba_module = (CORBAModule*) modules["CORBAConfig"].item();
        PortableServer::POA_var poa = corba_module->getPOA();
        PortableServer::ObjectId_var oid = PortableServer::string_to_ObjectId(CORBA::string_dup(id.c_str()));
        OTGraph::Node_var node_ref = OTGraph::Node::_narrow(poa->id_to_reference(oid));
        return node_ref;
    }
    
    OTGraph::Node_var Context::getNode(Node* node) {
        NodeIDMapIterator result = node_id_map.find(node);
        if (result == node_id_map.end()) {
            return OTGraph::Node::_nil();
        }
        return getNodeRefFromID(result->second);
    }
    
    Node* Context::getNode(const OTGraph::Node_var& node_ref) {
        std::string id = getIDFromNodeRef(node_ref);
        std::cerr << "recovered id: " << id << std::endl;
        return getNode(id);
    }
    
    Node* Context::getNode(const std::string& id) {
        IDNodeMapIterator result = id_node_map.find(id);
        if( result != id_node_map.end())
        {
            return &(*(result->second));
        } else {
            return NULL;
        }
    }


    void Context::eraseNode(Node* node) {
        std::string id = getIDFromNode(node);
        IDNodeMapIterator result1 = id_node_map.find(id);
        if (result1 != id_node_map.end()) {
            id_node_map.erase(result1);
        }
        NodeIDMapIterator result2 = node_id_map.find(node);
        if (result2 != node_id_map.end()) {
           node_id_map.erase(result2);
        }
    }

    void Context::appendNode(Node* node, const std::string& id) {
        logPrintI("appended Node\n");
        logPrintI("size of node_id_map is %d\n", node_id_map.size());
        node_id_map[node] = id;
        id_node_map[id] = node;
    }

    void Context::deactivateNode(Node* node) {
        logPrintI("deactivating node\n");
        NodeIDMapIterator result = node_id_map.find(node);
        if( result != node_id_map.end())
            {
                PortableServer::ObjectId_var id = PortableServer::string_to_ObjectId((result->second).c_str());
                CORBAModule* corba_module = (CORBAModule*) modules["CORBAConfig"].item();
                PortableServer::POA_var poa = corba_module->getPOA();
                poa->deactivate_object(id);
                eraseNode(node);
            }
    }

    void Context::activateNode(Node* node) {
        CORBAModule* corba_module = (CORBAModule*) modules["CORBAConfig"].item();
        PortableServer::POA_var poa = corba_module->getPOA();
        PortableServer::ObjectId_var corba_id;
        std::string id;
        
	CORBASource* corba_source = dynamic_cast<CORBASource*>(node);
	StaticTransformation* static_transformation = dynamic_cast<StaticTransformation*>(node);
#ifdef USE_OMNIEVENTS
        PushCons* push_cons = dynamic_cast<PushCons*>(node);
#endif // USE_OMNIEVENTS
        CORBASink* corba_sink = dynamic_cast<CORBASink*>(node);
	if (corba_source != NULL) {
            POA_OT_CORBA::OTSource_tie<CORBASource>* tie_node = 
                new POA_OT_CORBA::OTSource_tie<CORBASource>(corba_source, (CORBA::Boolean) 1);
            corba_id = poa->activate_object(tie_node);
            tie_node->_remove_ref();
	}
        else if (static_transformation != NULL) {
            POA_OTGraph::StaticTransformation_tie<StaticTransformation>* tie_node = 
                new POA_OTGraph::StaticTransformation_tie<StaticTransformation>(static_transformation, (CORBA::Boolean) 1);
            corba_id = poa->activate_object(tie_node);
            tie_node->_remove_ref();
        } 
#ifdef USE_OMNIEVENTS
        else if (push_cons != NULL) {
            POA_OT_EventChannel::PushConsNode_tie<PushCons>* tie_node = 
                new POA_OT_EventChannel::PushConsNode_tie<PushCons> (push_cons);
            corba_id = poa->activate_object(tie_node);
            tie_node->_remove_ref();
        }
#endif //USE_OMNIEVENTS
        else if (corba_sink != NULL) {
            POA_OT_CORBA::OTSink_tie<CORBASink>* tie_node = 
                new POA_OT_CORBA::OTSink_tie<CORBASink> (corba_sink);
            corba_id = poa->activate_object(tie_node);
            tie_node->_remove_ref();
        } else if (node != NULL) {
            POA_OTGraph::Node_tie<Node>* tie_node = new POA_OTGraph::Node_tie<Node>(node, (CORBA::Boolean) 1);
            corba_id = poa->activate_object(tie_node);
            tie_node->_remove_ref();
        }
        else
        {
            logPrintE("Context::activateNode(): Node is null !\n");
        }
        id = PortableServer::ObjectId_to_string(corba_id);
        appendNode(node, id);
    }

    void Context::activateNode(Node* node, const char* id) {
        std::string stringid(id);
        CORBAModule* corba_module = (CORBAModule*) modules["CORBAConfig"].item();
        PortableServer::POA_var poa = corba_module->getPOA();
        
	CORBASource* corba_source = dynamic_cast<CORBASource*>(node);
	StaticTransformation* static_transformation = 
            dynamic_cast<StaticTransformation*>(node);
#ifdef USE_OMNIEVENTS
        PushCons* push_cons = dynamic_cast<PushCons*>(node);
#endif //USE_OMNIEVENTS
        CORBASink* corba_sink = dynamic_cast<CORBASink*>(node);
	if (corba_source != NULL) {
            POA_OT_CORBA::OTSource_tie<CORBASource>* tie_node = 
                new POA_OT_CORBA::OTSource_tie<CORBASource>(corba_source);//, (CORBA::Boolean) 1);
            PortableServer::ObjectId_var corba_id = PortableServer::string_to_ObjectId(stringid.c_str());
            poa->activate_object_with_id(corba_id, tie_node);
            tie_node->_remove_ref();
	} else if (static_transformation != NULL) {
            POA_OTGraph::StaticTransformation_tie<StaticTransformation>* tie_node = 
                new POA_OTGraph::StaticTransformation_tie<StaticTransformation>(static_transformation);//, (CORBA::Boolean) 1);
            PortableServer::ObjectId_var corba_id = PortableServer::string_to_ObjectId(stringid.c_str());
            logPrintI("About to activate StaticTransformation object with id\n");
            poa->activate_object_with_id(corba_id, tie_node);
            tie_node->_remove_ref();
        } 
#ifdef USE_OMNIEVENTS
        else if (push_cons != NULL) {
            POA_OT_EventChannel::PushConsNode_tie<PushCons>* tie_node = new POA_OT_EventChannel::PushConsNode_tie<PushCons> (push_cons);
            PortableServer::ObjectId_var corba_id = PortableServer::string_to_ObjectId(stringid.c_str());
            poa->activate_object_with_id(corba_id, tie_node);
            tie_node->_remove_ref();
        } 
#endif //USE_OMNIEVENTS
        else if (corba_sink != NULL) {
            POA_OT_CORBA::OTSink_tie<CORBASink>* tie_node = 
                new POA_OT_CORBA::OTSink_tie<CORBASink> (corba_sink);
            PortableServer::ObjectId_var corba_id = PortableServer::string_to_ObjectId(stringid.c_str());
            poa->activate_object_with_id(corba_id, tie_node);
            tie_node->_remove_ref();
        } else {
            POA_OTGraph::Node_tie<Node>* tie_node = new POA_OTGraph::Node_tie<Node>(node);//, (CORBA::Boolean) 1);
            PortableServer::ObjectId_var corba_id = PortableServer::string_to_ObjectId(stringid.c_str());
            poa->activate_object_with_id(corba_id, tie_node);
            tie_node->_remove_ref();
        }
        logPrintI("About to append Node\n");
        appendNode(node, stringid);
        
    }
#endif
    // creates a new node from a given element name and an attribute table
    
    Node * Context::createNode( const std::string & name, const StringTable & attributes)
    {
#ifdef USE_LIVE
	StringTable xattributes(attributes);
        Node * value = factory.createNode( name , xattributes );
        if (activatelive)
        {
            CORBAModule* corba_module = (CORBAModule*) modules["CORBAConfig"].item();
            if (CORBAModule::persistent) {
                if (!xattributes.containsKey("DEF")) {
                    xattributes.put("DEF", CORBAUtils::generateUniqueId());
                }
                if (name.compare("PushCons") != 0) { // PushCons node is already activated!
                    logPrintI("About to activate node#1\n");
                    activateNode(value, xattributes.get("DEF").c_str());
                }
            } else {
                logPrintI("About to activate node#2\n");
                if (name.compare("PushCons") != 0) { // PushCons node is already activated!
                    activateNode(value);
                }
            }
        }
#else	
        Node * value = factory.createNode( name , attributes );
#endif
        // add all the attributes to the node
        if( value != NULL )
        {
            KeyIterator keys(attributes);
            while( keys.hasMoreKeys())
            {

                const std::string & key = keys.nextElement();

                value->put( key, attributes.get( key ));

            }
#ifdef USE_LIVE
            if (activatelive)
            {
                // If there is a nameBinding attribute, then bind the node to the NamingService
                if (value->get("nameBinding").compare("") != 0) {
                    std::cerr << "nameBinding: " << value->get("nameBinding") << std::endl;
                    // bind the node to the Naming Service
                    CosNaming::NamingContextExt::StringName_var string_name = CORBA::string_dup((const char*) value->get("nameBinding").c_str());
                    CORBA::Object_var obj = CORBA::Object::_narrow(getNode(value));
                    CORBA::ORB_var orb = CORBAModule::getORB();
                    CORBAUtils::bindObjectReferenceToName(orb, obj, string_name);
                }
            }
#endif

        }
        return value;
    }


    Graph * Context::getRootNode()
    {
        return graph;
    }

    Node * Context::findNode(const std::string & id)
    {

        return graph->findNode("ID", id);
    }

    // add a directory to the front of the directory stack

    void Context::addDirectoryFirst( const std::string & dir )
    {
        if( std::find( directories.begin(), directories.end(), dir) == directories.end())
        {
            directories.insert(directories.begin(), dir);
        }
    }

    // add a directory to the end of the directory stack

    void Context::addDirectoryLast( const std::string & dir )
    {
        if( std::find( directories.begin(), directories.end(), dir) == directories.end())
        {
            directories.push_back(dir);
        }
    }

    // remove a directory from the directory stack

    void Context::removeDirectory( const std::string & dir )
    {
        std::vector<std::string>::iterator it = std::find( directories.begin(), directories.end(), dir);
        if( it != directories.end())
        {
            directories.erase(it);
        }
    }

    // searches for a file by prepending a stack of directory names

    bool Context::findFile( const std::string & filename, std::string & fullname )
    {
        ACE_stat stat;
        if( filename.at(0) == '/' || filename.at(0) == '\\' ) // don't deal with paths from root !
        {
            if( ACE_OS::stat( ACE_TEXT_CHAR_TO_TCHAR(filename.c_str()), &stat ) != 0 )
            {
                return false;
            }
            fullname = filename;
            return true;
        }
        std::vector<std::string>::iterator it;
        for( it = directories.begin(); it != directories.end(); it++ )
        {
            std::string name = (*it) + "/" + filename;
            if( ACE_OS::stat( ACE_TEXT_CHAR_TO_TCHAR(name.c_str()), &stat ) == 0 )
            {
                fullname = name;
                return true;
            }
        }
        return false;
    }


    void Context::newVideoFrame(const unsigned char* image, int width, int height, PIXEL_FORMAT format, bool forceTraversal) 
    {
        for(VideoUserVector::iterator it=videoUsers.begin(); it!=videoUsers.end(); it++)
            (*it)->newVideoFrame(image, width, height, format);

        // after each camera frame we to an opentracker graph traversal
        if (forceTraversal)
            dataSignal();
    }


    void Context::registerVideoUser(VideoUser* videoUser)
    {
        for(VideoUserVector::iterator it=videoUsers.begin(); it!=videoUsers.end(); it++)
            if(*it == videoUser)
                return;

        videoUsers.push_back(videoUser);
    }


    void Context::unregisterVideoUser(VideoUser* videoUser)
    {
        for(VideoUserVector::iterator it=videoUsers.begin(); it!=videoUsers.end(); it++)
            if(*it == videoUser)
            {
                videoUsers.erase(it);
                return;
            }
    }


	const std::string& Context::getConfigFile(){
		return file;
	}

    void Context::copyFrom(Context & other){
        // lock the loop, so that we can destroy all datastructures
        lock();
        
        // safely stop all the modules
        bool stoppedHere = false;
        if(isStarted) { stoppedHere = true; close(); }
        
        
#ifndef OT_NO_CALLBACKMODULE_SUPPORT
        // copy Callbacks from old context
        CallbackModule *cbmodule = dynamic_cast<CallbackModule*>
            (getModule("CallbackConfig"));
        CallbackModule *ocbmodule = dynamic_cast<CallbackModule*>
            (other.getModule("CallbackConfig"));
       
        // this is only possible, if the callback module exists
        // in both contexts
        if (cbmodule && ocbmodule)
        {
            // obtain callback mapping from current module
            fctmap_type fctmap;
            cbmodule->getLocalCallbackMap(fctmap);
           
            // and set it in the newly setup context's module
            fctmap_type::iterator it;
            for (it=fctmap.begin(); it != fctmap.end(); it++)
            {            
                ocbmodule->setCallback(it->first, it->second.first, 
                                       it->second.second);
            }

            // the same for the global callback function
            ocbmodule->setGlobalCallback(cbmodule->getGlobalCallbackFunction(),
                                         cbmodule->getGlobalCallbackData());
        }
#endif //OT_NO_CALLBACKMODULE_SUPPORT
        // destroy all the modules
        logPrintW("CONTEXT: deleting all modules\n");
        //if (cleanUp)
	/*        for (ModuleMap::iterator it = modules.begin(); 
             it != modules.end(); it++)
        {
            delete (*it).second;
        }*/
        modules.clear();
        logPrintW("CONTEXT: deleted all modules\n");

   
        // remove all the factories, as they point to the already removed modules
        factory.removeAll();
        videoUsers.clear();

        // copy the modules from other
        modules = other.modules;
        // update their context information
        updateModuleContexts();

        /*        // copy the rootNode from other
	Node::Ptr tmp = rootNode;
        rootNode = other.rootNode;

        // let the other context clean up the old rootNode
        other.rootNode = tmp; */
        //        Graph * tmp = graph;
        graph = other.graph;
        //        other.graph = tmp;

        // set this context in the new rootNode
        
        // copy the factories from other
        factory.copyFrom(other.factory);

	// copy the videouser vector
	videoUsers = other.videoUsers;

        // copy synchronization settings
        //dosync = other.dosync;
        logPrintI("sync setting: %d\n",dosync );

        // change the cleanUp flag so the other won't destroy the modules when its gone
        this->cleanUp = other.cleanUp;
        other.cleanUp = false;

#ifdef USE_LIVE
        node_id_map = other.node_id_map;
        id_node_map = other.id_node_map;
        edges            = other.edges;
#endif
        if(stoppedHere) this->start();
        unlock();
    }

	void Context::addNode(std::string parentid, Node * newnode){
		Node * parent = findNode(parentid);
		Module * mod = getModuleFromNode(newnode);
		if (parent != NULL){
			lock();
			parent->addChild(*newnode);
			mod->addNode(newnode);
			unlock();
		}
	};

	

	Module * Context::getModuleFromNode(const Node * node){
		
		std::string nodename = node->getType();
		
		return getModuleFromNodeType(nodename);
	}

	Module * Context::getModuleFromNodeType(std::string nodename){
            logPrintI("getModuleFromNodeType(%s)\n", nodename.c_str());
		// gcc does not allow doing this
		// char * nodeTypes[3] = {{"Source"},{"Node"},{"Sink"}};
		Module * result= NULL;
		const char * source = "Source";
		const char * node = "Node";
		const char * sink = "Sink";
		const char * sinksource = "SinkSource";
		const char * nodeTypes[4];
                nodeTypes[1] = source;
                nodeTypes[2] = node;
                nodeTypes[3] = sink;
	        nodeTypes[0] = sinksource;
		std::string modname;
        logPrintI("Context GetModuleFromNode node->getType() %s \n", nodename.c_str());
		if ((nodename.compare("NetworkSource")==0) || (nodename.compare("NetworkSink")==0)){
			modname = nodename ;
			result = getModule(modname);
		} else if ((nodename.compare("EventMouseSource") == 0)){
			modname = "EventConfig";
                        result = getModule(modname);
		} else if ((nodename.compare("TestSource") == 0)){
                    logPrintI("TestSource maps to TestConfig\n");
			modname = "TestConfig";
                        result = getModule(modname);
		} else if ((nodename.compare("CORBASink") == 0)
                           || (nodename.compare("CORBASource") == 0)
                           || (nodename.compare("PushCons") == 0)
                           || (nodename.compare("PushSupp") == 0)
                           || (nodename.compare("SharedEngineNode") == 0)) {
                    logPrintI("CORBA nodes map to CORBAConfig\n");
			modname = "CORBAConfig";
                        result = getModule(modname);
		} else if ((nodename.compare("PhantomZoneSource") == 0) 
                           || (nodename.compare("PhantomLocationSource") == 0)
                           || (nodename.compare("PhantomLocationSink") == 0)){
			modname = "PhantomMiddlewareConfig";
                        result = getModule(modname);
		} else {
			std::string::size_type loc= std::string::npos;
			
			for (int i = 0; i < 4; i++){
				loc = nodename.find(nodeTypes[i]);
				if (loc != std::string::npos)
					break;
			}	

			if (loc != std::string::npos){
				modname = nodename.substr(0, loc);
				modname += "Config";
	
				result = getModule(modname);
  				logPrintI("Context GetModuleFromNode %s  result %p\n", modname.c_str(), result);
			}
		}
		return result;


	}

	/* 
	in order to properly remove a node, it is necessary to remove it from the module that created it.
	Most modules keep some datastructures for the nodes they create. These are actualized, for example 
	in the create node method. It seems that either a node is a common node, thus created by the 
	CommonNodeFactory, or it is some kind of specialized node, created by a module. We need to cope with
	this two situations.
	*/
	void Context::removeSubtree(std::string nodeid){

		Node * target = findNode(nodeid);
		
		
		if (target != NULL){
			
			if (CommonNodeFactory::isKnownNode(target->getType())){
				// The node is a common node, and does not belong to a module
				lock();
				// remove the node from the graph
				Node * parent = target->getParent();
				parent->removeChild(*target);
				unlock();
			}else{
				//the node belongs to a module, which must be notified of the node's removal
				// from the graph.
				Module * mod = getModuleFromNode(target);
				if (mod != NULL){
					lock();
					Node * parent = target->getParent();
					parent->removeChild(*target);
					mod->removeNode(target);
					unlock();
				} else{
					// the node cannot be removed, because its module is unknown, we must throw
					logPrintE("Context Remove_Subtree could not remove node of type %s, because its module was not found\n", (target->getType().c_str()));

				}
			}
				
		}
		
	};

	void Context::removeNode(std::string nodeid){

		Node * target = findNode(nodeid);
		logPrintI("Context Remove_Node target is %p\n", target);
		if (target != NULL){
			unsigned int count = target->countChildren();
            logPrintW("Context Remove_Node %u children before removing\n", count);


			logPrintI("Context Remove_Node removing node %p, of type %s\n", target, (target->getType().c_str()));
			if (CommonNodeFactory::isKnownNode(target->getType())){
				// The node is a common node, and does not belong to a module
				Node * parent = target->getParent();
				lock();
				unsigned int childrencount = target->countChildren();
				logPrintI("Context Remove_Node must add %u node's children to its parent's\n", childrencount);
				for(unsigned int i = 0; i < childrencount; i++){
					Node * child = target->getChild(i);
					parent->addChild(*child);
					
					logPrintI("Context Remove_Node adding child of type %s, to parent of type%s\n", ((child->getType()).c_str()), ((parent->getType()).c_str()));
					target->removeChild(*child);					
				}

				// remove the node from the graph

				parent->removeChild(*target);


				// delete target;
				unlock();
			}else{
				//the node belongs to a module, which must be notified of the node's removal
				// from the graph.
				Module * mod = getModuleFromNode(target);
				if (mod != NULL){
					lock();
					Node * parent = target->getParent();
					parent->removeChild(*target);
					mod->removeNode(target);
					// the node has been removed, now all its children should be passed on to
					// its father.
					unsigned int childrencount = target->countChildren();
					for(unsigned int i = 0; i < childrencount; i++){
						Node * child = target->getChild(i);
						target->removeChild(*child);
						parent->addChild(*child);
					}
					unlock();
				} else{
					// the node cannot be removed, because its module is unknown, we must throw
					logPrintE("Context Remove_Subtree could not remove node of type %s, because its module was not found\n", (target->getType().c_str()));

				}
			}
				
		}


                //		((TiXmlNode*)rootNode->parent)->GetDocument()->Print()	;
	}

	void Context::replaceNode(std::string nodeid, Node * newnode){
		if (newnode != NULL){
			Module * newmodule = getModuleFromNode(newnode);
			Node * target = findNode(nodeid);
			if (target != NULL){
				Module * targetmodule = getModuleFromNode(target);
				lock();
				Node * parent = target->getParent();
				parent->removeChild(*target);
				targetmodule->removeNode(target);

				parent->addChild(*newnode);
				newmodule->addNode(newnode);
				unlock();
			} else{
				// something bad here, should throw
			}
		} else{
			// there is something wrong here, should at least throw
		}
	}

    bool Context::isConfigured(){
		// WARNING: For some reason this always returned true. 
		// So I set it to always return false and allow me to create my nodes (otherwise they were deleted)
		// Please, those working on reconfigurability fix this. Mendez 20080311.
        //return (graph != NULL);
		return false;
    }
    
} // namespace ot

/* 
 * ------------------------------------------------------------
 *   End of Context.cxx
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
