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
/** source file containing the main function for standalone use.
 *
 * @author Joseph Newman
 *
 * $Id: corba_configurable.cxx 1164 2006-06-14 05:27:23Z jfn $
 * @file                                                                   */
/* ======================================================================= */

#include <ace/Thread_Mutex.h>

#include <OpenTracker/OpenTracker.h>
#include <OpenTracker/network/CORBAUtils.h>
#include <OpenTracker/network/CORBAModule.h>

//#define USE_stub_in_nt_dll
#include <OpenTracker/skeletons/OTGraph.hh>
//#undef USE_stub_in_nt_dll
#include <omniORB4/poa.h>

#include <OpenTracker/core/Node.h>
#include <OpenTracker/core/Module.h>
#include <OpenTracker/core/Configurator.h>
#include <OpenTracker/core/NodeFactoryContainer.h>
#include <OpenTracker/core/LiveContext.h>

#include <iostream>

using namespace std;
using namespace ot;

/**
 * The main function for the standalone program.
 */
int main(int argc, char **argv)
{
    if( argc < 2 || argc > 3)
    {
        cout << "Usage : " << argv[0] << " NamingContextName <rate>" << endl;
        return 1;
    }
    try {
      LiveContext* context_impl = new LiveContext();
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
      PortableServer::POA_var poa = corba_module->getPOA();
      if (CORBA::is_nil(poa)) {
	cerr << "got nil reference to POA. Exiting...." << endl;
	exit(-1);
      }
      POA_OTGraph::DataFlowGraph_tie<LiveContext>* context = new POA_OTGraph::DataFlowGraph_tie<LiveContext>(context_impl);

      PortableServer::ObjectId_var id = corba_module->getRootPOA()->activate_object(context);

      // Obtain a reference to the object, and register it in
      // the naming service.
      CORBA::Object_var obj = corba_module->getRootPOA()-> id_to_reference(id);

      CosNaming::NamingContextExt::StringName_var string_name = argv[1];
      CORBAUtils::bindObjectReferenceToName(orb, obj, string_name);
      //context_impl->runAtRate(30);
      context_impl->runOnDemand();

    }
    catch(CORBA::SystemException&) {
      cerr << "Caught CORBA::SystemException." << endl;
    }
    catch(CORBA::Exception&) {
      cerr << "Caught CORBA::Exception." << endl;
    }
    catch(omniORB::fatalException& fe) {
      cerr << "Caught omniORB::fatalException:" << endl;
      cerr << "  file: " << fe.file() << endl;
      cerr << "  line: " << fe.line() << endl;
      cerr << "  mesg: " << fe.errmsg() << endl;
    }
    catch(...) {
      cerr << "Caught unknown exception." << endl;
    }
    return 1;
}

/* 
 * ------------------------------------------------------------
 *   End of configurable.cxx
 * ------------------------------------------------------------
 *   Automatic Emacs configuration follows.
 *   oLcal Variables:
 *   mode:c++
 *   c-basic-offset: 4
 *   eval: (c-set-offset 'substatement-open 0)
 *   eval: (c-set-offset 'case-label '+)
 *   eval: (c-set-offset 'statement 'c-lineup-runin-statements)
 *   eval: (setq indent-tabs-mode nil)
 *   End:
 * ------------------------------------------------------------ 
 */
