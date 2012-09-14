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
/** header file for Mobilab Module
 *
 * @author Alexander Bornik
 * 
 * $Id $
 *
 * @file                                                                   */
/* ======================================================================= */
/**
 * @page module_ref Module Reference
 * @section mobilabmodule MobilabModule 
 *
 * This module provides and drives the @ref mobilabsource which
 * generate events for data retrieved from biomedical sensors events. 
 * The configuration element is called @c MobilabConfig.
 * It reads data from a g.Mobilab device connected on the serial port 
 * and transmits corresponding events.
 *
 * It has the following attributes :
 * @li @c dev serial port where the g.Mobilab device is attached.
 * @li @c logfile path and name of a file to log all received strings 
 * into (optional)
 *
 * An example configuration element looks like this :
 * @verbatim <MobilabConfig dev="COM1" /> @endverbatim
 *
 */

#ifndef _MOBILABMODULE_H
#define _MOBILABMODULE_H

#include "../OpenTracker.h"
#include "MobilabDriver.h"
//#include "MobilabSource.h"

#ifndef EXCLUDE_MOBILAB_SUPPORT

#include <string>
#include <ace/INET_Addr.h>

class ACE_FILE_IO;
class ACE_SOCK_Dgram;

namespace ot {

    class MobilabSource;

    typedef std::vector<MobilabSource*> SourceVector;

    /**
     * This module supports a g.Mobilab device. It drives a number of Mobilab 
     * source nodes associated with it by a channel number Currently it only 
     * supports one device.
     *
     * It uses the MobilabDriver class to operate the g.Mobilab device. 
     * @see MobilabSource for more information on the source node.
     *
     * @ingroup input
     * @author Alexander Bornik
     */
    class OPENTRACKER_API MobilabModule : public ThreadModule ,
                                          public NodeFactory, 
                                          public MobilabListener
    {
    public:
        MobilabModule();
        virtual ~MobilabModule();
    
        virtual void init(StringTable& attributes,  ConfigNode * localTree);
    
        virtual Node * createNode( const std::string & name, 
                                   const StringTable & attributes );
    
        virtual void pushEvent();
    
        virtual void start();
    
        virtual void close();
    
        virtual void newData( short sampleValue, double timev);
        virtual void newData( const short * samples, int ssize);
    
    protected:
        unsigned char sendbuffer[16];
        virtual void run();
        
        bool debug;
        std::string devicename;
	std::string devicenamet;
    
        SourceVector sources;
        MobilabDriver * driver;
    
        ACE_FILE_IO * logFile;
        ACE_SOCK_Dgram * logHost;
        ACE_INET_Addr server_addr;
        int logsamplenr;
    
        void * mobilab_reactor;

        friend class MobilabSource;
    };

	OT_MODULE(MobilabModule);

}  // namespace ot

#endif //EXCLUDE_MOBILAB_SUPPORT
#endif // !defined(_MOBILABMODULE_H)

/* 
 * ------------------------------------------------------------
 *   End of MobilabModule.h
 * ------------------------------------------------------------
 *   Automatic Emacs configuration follows.
 *   Local Variables:
 *   mode:c++
 *   c-basic-offset: 4    try {
      LiveContext* context_impl = new LiveContext();
      cerr << "got LiveContext instance" << endl;
      ModuleMap modules = context_impl->getModules();
      ModuleMap::iterator module_iterator  = modules.find("CORBAConfig");
      if ( module_iterator == modules.end() ) {
	exit(-1);
      } 
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

      PortableServer::POA_var poa = corba_module->getPOA();
      if (CORBA::is_nil(poa)) {
	cerr << "got nil reference to POA. Exiting...." << endl;
	exit(-1);
      }

      POA_OTGraph::DataFlowGraph_tie<LiveContext>* context = new POA_OTGraph::DataFlowGraph_tie<LiveContext>(context_impl);

      PortableServer::ObjectId_var configurator_id = PortableServer::string_to_ObjectId("livecontext");
      poa->activate_object_with_id(configurator_id, context);
      cerr << "activated configurator" << endl;

      // Obtain a reference to the object, and register it in
      // the naming service.
      CORBA::Object_var obj = context->_this();
      //CosNaming::NamingContextExt::StringName_var string_name = CORBA::string_dup(argv[1]);
      CosNaming::NamingContextExt::StringName_var string_name = argv[1];
      CORBAUtils::bindObjectReferenceToName(orb, obj, string_name);
      //      orb->run(); 
      context_impl->runAtRate(30);
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

        }

 *   eval: (c-set-offset 'substatement-open 0)
 *   eval: (c-set-offset 'case-label '+)
 *   eval: (c-set-offset 'statement 'c-lineup-runin-statements)
 *   eval: (setq indent-tabs-mode nil)
 *   End:
 * ------------------------------------------------------------ 
 */
