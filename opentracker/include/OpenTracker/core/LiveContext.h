#ifndef _LIVECONTEXT_H
#define _LIVECONTEXT_H

#ifdef USE_LIVE

#include "../dllinclude.h"

#ifndef SWIG
#include <omniORB4/poa.h>
#endif

#include <OpenTracker/common/CommonNodeFactory.h>
#include <OpenTracker/OpenTracker.h>
#include <OpenTracker/core/Context.h>
#include <OpenTracker/core/Module.h>
#include <OpenTracker/core/NodeFactoryContainer.h>
#include <OpenTracker/network/CORBAModule.h>

#ifndef SWIG
#include <OpenTracker/skeletons/OTGraph.hh>
#endif

#include <OpenTracker/core/OtLogger.h>

#ifndef SWIG
#include <xercesc/dom/DOM.hpp>
#include <xercesc/dom/DOMWriter.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#endif

#ifndef SWIG
#if defined(XERCES_NEW_IOSTREAMS)
#include <iostream>
#else
#include <iostream.h>
#endif
#endif

#ifndef SWIG
XERCES_CPP_NAMESPACE_USE
#endif

using namespace std;

namespace ot {
  
 class OPENTRACKER_API LiveContext : public Context, 
    public PortableServer::RefCountServantBase {
  private:
    CORBAModule* corba_module;
  public:
    LiveContext();
    LiveContext(bool init);
    LiveContext(const std::string& endPoint);
    virtual ~LiveContext() {
      cerr << "LiveContext:: Destructor" << endl;
    };
    
    ModuleMap getModules() {
      return modules;
    };

    OTGraph::Node_ptr get_node(const char* id);

    OTGraph::Node_var create_node(const char* _name, const OTGraph::StringTable& _attributes);

    OTGraph::Edge* connect_nodes(const OTGraph::Node_var& upstreamNode, const OTGraph::Node_var& downstreamNode);
    
    void disconnect_nodes(const OTGraph::Node_var& upstreamNode, const OTGraph::Node_var& downstreamNode); 

    OTGraph::NodeVector* get_nodes();

    OTGraph::EdgeVector* get_edges();

    void collectEdges(EdgeVector& _edges);

    void remove_node(const OTGraph::Node_var& target_ref);
    void remove_edge(const OTGraph::Edge& edge);
    
    void run();

    char* getXMLString();
    char* getDot();

  protected:
    ConfigNode* configNode;
  };
}

#endif //USE_LIVE
#endif //_LIVECONTEXT_H
