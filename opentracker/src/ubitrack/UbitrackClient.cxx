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
/** Ubitrack Client class
 *
 * @author Alexander Bornik
 *
 * $Id$
 * @file                                                                   */
/* ======================================================================= */

#include <OpenTracker/OpenTracker.h>
#include <OpenTracker/ubitrack/UbitrackClient.h>
#include <OpenTracker/core/Context.h>
#include <OpenTracker/core/ThreadContext.h>

#include <TinyXML/tinyxml.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <set>

using namespace std;
namespace ot{

    UbitrackClient * UbitrackClient::self = NULL;

    UbitrackClient::UbitrackClient(ACE_INET_Addr nServerAddr, int ctx_type) 
        :
        ctx(NULL),
        server_addr(nServerAddr),
        requestHandler(this)

    {
        switch (ctx_type)
        {
            case NORMAL:
                ctx = new Context(1);
                break;
            case THREAD:
                ctx = new ThreadContext(1);
                break;
            default:
                ctx = new Context(1);
                break;
        }

        // start context
        ctx->start();

        // start internal communication thread
        start();
    }


    UbitrackClient * UbitrackClient::instance(ACE_INET_Addr nServerAddr, int ctx_type)
    {
	if (self == NULL)
        {
            initializeOpenTracker();            
            self = new UbitrackClient(nServerAddr, ctx_type);
	}
        else if (self->server_addr !=nServerAddr)
        {
            delete self;
            self = new UbitrackClient(nServerAddr, ctx_type); 
        }

	return self;
    }

    UbitrackClient * UbitrackClient::instance(int ctx_type)
    {
	if (self == NULL)
        {
            initializeOpenTracker();  
            ACE_INET_Addr localhost(3000, "localhost");
            self = new UbitrackClient(localhost, ctx_type);
	}

	return self;
    }

    int
    UbitrackClient::start()
    {
        
        int ret = activate();
        
        if(ret==-1)
        {
            logPrintI("UbitrackClient: failed to activate server daemon\n");
        }
        else
        {
            logPrintI("UbitrackClient: daemon activated\n");
        }
        
        return ret;
    }

    int
    UbitrackClient::stop()
    {     
        logPrintI("UbitrackClient: stop called\n");
        
        ACE_Reactor::end_event_loop () ;
        
        wait();
        return 0;
    }

    int UbitrackClient::svc (void)
    {
        UbitrackClientRequestHandler *request_handler = &requestHandler;
    
        if (requestConnector.connect (request_handler, server_addr) == -1)
        {  
            logPrintI("UbitrackClient: could not connect to server\n");
        }

        logPrintI("UbitrackClient: starting service\n");

        ACE_Reactor::instance()->owner (ACE_OS::thr_self ());

        ACE_Reactor::run_event_loop();

        logPrintI("UbitrackClient: service finished\n");

        return 0;
    }

    int UbitrackClient::sendMessage(UbitrackMessage & inMsg)
    {
        // send message
        requestHandler.sendMessage(inMsg);
        return 0;
    }

    UbitrackClient::~UbitrackClient()
    {

        stop();

        if (ctx) 
        {
            delete ctx;
        }
    }
    /*    
    void UbitrackClient::doInitialization(Context & newctx){
        for (Registry::iterator i = initFunctions.begin(); 
             i != initFunctions.end(); i++)
        {
            (* (i->second).function) (&newctx, (i->second).data);
        }
    }

    void UbitrackClient::loadModule(Context & newctx, const char * module)
    {
        logPrintS("UbitrackClient loading module %s\n", module);
        Registry::iterator i = initFunctions.find(module);
        if (i != initFunctions.end()){
            (* (i->second).function) (&newctx, (i->second).data);
        } else
            logPrintE("module %s not found\n", module);
    };


    void UbitrackClient::addModuleInit(const char * name, ModuleInitFunc function, 
                                     void * data)
    {
        MIFunctor functor;
        functor.function = function;
        functor.data = data;
        //logPrintS("UbitrackClient adding %s = %p \n", name, function);
    
        initFunctions[name]=(functor);
    }
    */
    
    
    void UbitrackClient::sendUTQL(std::string utql)
    {
        using namespace std;
        logPrintI("UbitrackClient::sendUTQL\n");
        
        UbitrackMessage um;
        um.setXMLString(utql);
        sendMessage(um);
        //cout << "UTQL string: \"" 
        //     << utql << "\"" << endl;            
    }

    void UbitrackClient::sendUTQLFile(std::string utqlfilename)
    {
        logPrintI("UbitrackClient::sendUTQLFile(%s)\n", utqlfilename.c_str());
        using namespace std;
        ifstream utqlfile;
        ostringstream utqls;
        char c;
        try
        {
            utqlfile.open(utqlfilename.c_str(),ifstream::in);
            while (!utqlfile.eof())
            {  
                utqlfile.get(c);
                if (!utqlfile.eof()) utqls << c;
            }
            utqlfile.close();
        }
        catch (exception &e)
        {
            cerr << "UbitrackClient::sendUTQLFile: Exception: "
                 << e.what() << endl;
        }
        sendUTQL(utqls.str());
    }

    Context * UbitrackClient::getContext(){
	return ctx;
    }

    bool UbitrackClient::handleMessage(UbitrackMessage &retMsg)
    {
        using namespace std;
        logPrintI("UbitrackClient::handleMessage()\n");

        TiXmlDocument document;

        if  (!document.Parse(retMsg.XMLString().c_str()))
        {
            logPrintE("UbitrackClient::handleMessage:\n Parsing Error:\n Message: %s\n", document.ErrorDesc());
            return false;
        }
        else
        {
            TiXmlElement* root = document.RootElement();  
            const string patternString("Pattern");
            const string inputString("Input");
            const string outputString("Output");
            const string dfgString("DataflowConfiguration");
            //const string otNodeString("UbitrackLib");
            const string otNodeString("OpenTracker");
            const string edgeString("Edge");
            const string attrString("Attribute");
            
            ConnectionDescription connections;
            NodeDescription nodedescs;

            string actname;
            
            TiXmlElement* pattern = 
                root->FirstChildElement(patternString.c_str());
            while (pattern)
            {
                actname = "";

                // read pattern's attributes

                auto_ptr<StringTable> lookupattributes(new StringTable());
                auto_ptr<StringTable> attributes(new StringTable());
                TiXmlAttribute* attribute = pattern->FirstAttribute();
                while (attribute)
                {
                    if (string(attribute->Name()) == "id")
                    {
                        cerr << attribute->Value() << endl;
                        //attributes->put(attribute->Name(), attribute->Value());
                        attributes->put("UTID", attribute->Value());
                        actname = attribute->Value();
                    }
                    attribute = attribute->Next();
                }

                // obtain input child
                TiXmlElement *cinput = 
                    pattern->FirstChildElement(inputString.c_str());
                if (cinput)
                {
                    cerr << " input:" << cinput->Value() << endl;
                    // search for references
                    TiXmlElement *inode = cinput->FirstChildElement();
                    while (inode)
                    {
                        TiXmlAttribute *inattr = inode->FirstAttribute();
                        while (inattr)
                        {
                            if (string(inattr->Name()) == "pattern-ref")
                            {
                                cerr << "    " 
                                     << inattr->Name() 
                                     << " -> " << inattr->Value() << endl;
                                connections.insert(make_pair(actname,inattr->Value()));
                            }
                            inattr = inattr->Next();
                        }

                        inode = inode->NextSiblingElement();
                    }
                }
                
                // obtain output child
                TiXmlElement *coutput = 
                    pattern->FirstChildElement(outputString.c_str());
                if (coutput)
                {
                    cerr << " output:" << coutput->Value() << endl;
                    // search for the edge 
                    TiXmlElement *oedge = 
                        coutput->FirstChildElement(edgeString.c_str());
                    if (!oedge)
                    {
                         // No edge specification -> strange dataflow
                         logPrintW("No edge in output - strange dataflow!\n");
                    }
                    else
                    {
                        // find OT relevant attributes
                        TiXmlElement * outattrel = oedge->FirstChildElement(attrString.c_str());
                        while (outattrel)
                        {
                            const char *oattrname = outattrel->Attribute("name");
                            const char *oattrval = outattrel->Attribute("value");
                            if (oattrname && oattrval)
                            {
                                lookupattributes->put(oattrname, oattrval);
                            }
                            else
                            {
                                logPrintW("Attribute element is missing name and  value attributes!\n");
                            }

                            outattrel = outattrel->NextSiblingElement(attrString.c_str());
                        }
                        oedge = oedge->NextSiblingElement(edgeString.c_str());
                        if (oedge)
                        {
                            logPrintW("found multiple edge output for one pattern!\n");
                        }
                    }
                }

                // obtain dataflow configuration child
                TiXmlElement *cdfg = 
                    pattern->FirstChildElement(dfgString.c_str());
                if (cdfg)
                {
                    cerr << " dfg:" << cdfg->Value() << endl;

                    // search for OpenTracker configuration stuff

                    TiXmlElement *dfgnode = 
                        cdfg->FirstChildElement(otNodeString.c_str());
                     if (!dfgnode)
                     {
                         // No OT configuration -> DFG will be incomplete!
                         logPrintW("No OpenTracker configuration for pattern available!\n");
                     }
                     else
                     {
                         
                         TiXmlAttribute *dfgattr = dfgnode->FirstAttribute();
                         string otnodename("");
                         vector<string> ports;
                         // find out node name, static attributes and
                         // dynamic attributes -> these are read from
                         // the output section
                         while (dfgattr)
                         {
                             string attname(dfgattr->Name());
                             string attval(dfgattr->Value());

                             if (attname == "class")
                             {
                                 otnodename = attval;
                             }
                             else
                             {
                                 if (attname.find("classport") == 0)
                                 {
                                     logPrintI("port found: %s\n", attval.c_str());
                                     // replace n-th direct connection by
                                     // connections via port
                                     string portname(getPortNodeName(actname, attname));
                                     int i=0;
                                     int n=getPortNumber(attname);
                                     set<pair<string, string> >::iterator sit;
                                     for (sit = connections.begin();
                                          sit != connections.end(); sit++)
                                     {
                                         if (sit->first == actname)
                                         {
                                             if (i<n) { i++; continue; }
                                             connections.insert(make_pair(sit->first, portname));
                                             connections.insert(make_pair(portname, sit->second));
                                             connections.erase(sit);
                                             break;
                                         }
                                     }
                                 }

                                 if (attval.find("$") != 0)
                                 {
                                     attributes->put(attname,attval);
                                 }
                                 else
                                 {
                                     string lookattr = attval.erase(0,1);
                                     string lookval = lookupattributes
                                         ->get(lookattr);
                                     if (lookval != "")
                                     {
                                         attributes->put(attname, lookval);
                                     }
                                     else
                                     {
                                         logPrintW("Lookup attribute %s for attribute not found.\n", lookattr.c_str(), attname.c_str());
                                     }
                                 }                                     
                             }
                             dfgattr = dfgattr->Next();
                         }

                         if (otnodename != "")
                         {
                             attributes->put("OtNodeType",  otnodename);
                             nodedescs.push_back(*attributes);
                         }
                         
                         dfgnode = dfgnode->NextSiblingElement(otNodeString.c_str());
                         // Each Ubitrack pattern should have a unique node 
                         // representation in OpenTracker!
                         if (dfgnode)
                         {
                             logPrintW("Multiple OpenTracker representations for pattern found! Ignoring all but the first.\n");
                         }
                     }
                }

                cerr << endl;
                pattern = pattern->NextSiblingElement(patternString.c_str());
            }

            // update graph
            
            updateDFG(nodedescs, connections);
            
            // finally write out graph
            ctx->getRootNode()->writeGraph("tmpgraph.txt");

        }

        return true;
    }
    
    //bool UbitrackClient::updateDFG(const std::string &nodetype, 
    //                               const std::auto_ptr<StringTable> &attributes)

    bool UbitrackClient::updateDFG(const NodeDescription &ndescs, 
                                   const ConnectionDescription &cdescs)    
    {
        using namespace std;
        logPrintI("UbitrackClient::updateDFG\n");

        ctx->lock();

        Graph *graph = ctx->getRootNode();

        NodeDescription::const_iterator nit;
        ConnectionDescription::const_iterator cit;
        vector<pair<string,Module*> > newmodules;

        for (nit = ndescs.begin(); nit != ndescs.end(); nit++)
        {
            cerr << "node type: " << nit->get("OtNodeType") << endl;
            string utid(nit->get("UTID"));
            KeyIterator it(*nit);
            StringTable rnode;
            map<int, StringTable> pnodes;
            map<int, StringTable>::const_iterator pit;

            while (it.hasMoreKeys())
            {
                string akey(it.nextElement());
                string aval(nit->get(akey));

                if (akey.find("classport") == 0) // port
                {                    
                    int pnum = getPortNumber(akey);                    
                    pnodes[pnum] = StringTable();
                    ostringstream oss;
                    oss << utid << pnum;
                    pnodes[pnum].put("OtNodeType",aval);
                    pnodes[pnum].put("UTID", oss.str());                    
                }
                else if (akey.find("port") == 0) // port parameter
                {
                    pair<int, string> attdecomp = 
                        getPortAttributeNameAndNumber(akey);
                    pnodes[attdecomp.first].put(attdecomp.second, aval);
                }
                else // normal attributes
                {
                    rnode.put(akey,aval);
                }

                cerr << "  "
                     << akey << " -> " 
                     << nit->get(akey) << endl;
            }
            
            // create node
            createNode(rnode, newmodules);

            // create node ports, if any
            for (pit = pnodes.begin(); pit !=pnodes.end(); pit++)
            {
                createNode(pit->second, newmodules);
            }
                                                          
        } // for nit
        
        for (cit=cdescs.begin(); cit != cdescs.end(); cit++)
        {
            Node* src = graph->findNode("UTID", cit->first);
            Node* dst = graph->findNode("UTID", cit->second);
            if (!src)
            {
                logPrintW("Source node %s not found\n", 
                          cit->first.c_str());
            }
            else if (!dst)
            {
                logPrintW("Destination node %s not found\n", 
                          cit->second.c_str());
            }
            else
            {
                logPrintI("connecting %s -> %s \n", 
                          cit->first.c_str(),
                          cit->second.c_str());
                graph->connectNodes(src, dst);
            }
        }
        
        // start all new modules
        logPrintI("# of loaded modules: %d \n", ctx->getModuleCount());
        if (newmodules.size() > 0) 
        {
            logPrintI("Starting new modules:\n");
            vector<pair<string,Module*> >::const_iterator mit;
            for (mit = newmodules.begin(); mit != newmodules.end(); mit++)
            {
                cerr << "  " << mit->first << endl;
                mit->second->start();
            }
        }

        ctx->unlock();

        // start all modules
        // FIXXXME: this is really ugly, there should be a method, which
        // updates all modules newly created, or where the configuration 
        // has changed ...

        ctx->start();

        return true;
    }

    std::string UbitrackClient::getPortNodeName(const std::string &nodename, 
                                string portatt) const
    {
        portatt.erase(0, 9);
        istringstream iss(portatt);
        int n;
        try
        {
            iss >> n;
        }
        catch (exception &e)
        {
            n = 0;
        }
        ostringstream oss;
        oss << nodename << n;

        return oss.str();
    }

    int UbitrackClient::getPortNumber(std::string portatt) const
    {
        portatt.erase(0, 9);
        istringstream iss(portatt);
        int n;
        try
        {
            iss >> n;
        }
        catch (exception &e)
        {
            n = 0;
        }
        return n;
    }
    std::pair<int, std::string> UbitrackClient::getPortAttributeNameAndNumber(std::string portattr) const
    {
        portattr.erase(0,4);
        istringstream iss(portattr);
        int n;
        string attname;
        try
        {
            iss >> n;
        }
        catch (exception &e)
        {
            n = 0;
        } 
        try
        {
            iss >> attname;
        }
        catch (exception &e)
        {
            attname = "dummy";
        }
        return make_pair(n,attname);
    }

    bool UbitrackClient::createNode(const StringTable &rnode,
                                    std::vector<std::pair<std::string, Module*> > &newmodules)
    {
        Graph *graph = ctx->getRootNode();
        // search if node is already there
        Node* node = graph->findNode("UTID", rnode.get("UTID"));
        if (node)
        {
            logPrintI("Node %s already found -> removing first\n", 
                      rnode.get("UTID").c_str());
            // found -> remove node
            graph->remNode(node);
        }

        logPrintI("Creating node %s ... \n", 
                  rnode.get("UTID").c_str());           
        
        node = ctx->createNode(rnode.get("OtNodeType").c_str(), rnode);
        if (node == NULL)
        {
            logPrintI("Trying to load module first\n");

            Module* newmodule = 
                ctx->getModuleFromNodeType(rnode.get("OtNodeType"));

            if (newmodule != NULL)
            {
                node = ctx->createNode(rnode.get("OtNodeType").c_str() ,
                                       rnode);
                newmodules.push_back(make_pair(rnode.get("OtNodeType"),newmodule));
            }
        }
        if (node)
        {
            logPrintI("Adding node to graph.\n");
            graph->addNode(node);
        }
        else
        {
            logPrintE("Node creation failed!\n");
            return false;
        }
        return true;
    }

} //namespace ot

    /* 
     * ------------------------------------------------------------
     *   End of UbitrackClient.cxx
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
