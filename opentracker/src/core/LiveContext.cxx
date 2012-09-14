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
/** source file for LiveContext class
 *
 * @author Joseph Newman
 *
 * $Id: LiveContext.cxx 1790 2007-03-26 09:55:14Z jfn $
 * @file                                                                   */     
/* ======================================================================= */

#include <OpenTracker/OpenTracker.h>
#ifdef USE_LIVE
#include <OpenTracker/network/CORBAModule.h>
#include <OpenTracker/core/LiveContext.h>
#include <OpenTracker/core/Context.h>

#ifndef SWIG
#include <omniORB4/poa.h>
#include <OpenTracker/skeletons/OTGraph.hh>
#include <algorithm>
#endif
#endif //USE_LIVE

using namespace std;

namespace ot {

#ifdef USE_LIVE
   LiveContext::LiveContext(bool init) : Context(1) {
        cerr << "LiveContext:: Constructor" << endl;
        if (init) {
            corba_module = (CORBAModule*) modules["CORBAConfig"].item();
            StringTable st;
            //st.put("endPoint", "giop:tcp:localhost:9999");
            st.put("persistent", "true");
            
            corba_module->init(st, NULL);
        }
    }

  LiveContext::LiveContext() : Context(1) {
      cerr << "LiveContext:: Constructor" << endl;
      corba_module = (CORBAModule*) modules["CORBAConfig"].item();
      StringTable st;
      //st.put("endPoint", "giop:tcp:localhost:9999");
      st.put("persistent", "true");
      
      corba_module->init(st, NULL);
    }

    LiveContext::LiveContext(const std::string& endPoint) : Context(1) {
      cerr << "LiveContext:: Constructor" << endl;
      corba_module = (CORBAModule*) modules["CORBAConfig"].item();
      StringTable st;
      st.put("endPoint", endPoint);
      st.put("persistent", "true");
      
      corba_module->init(st, NULL);
    }
   

  OTGraph::Node_ptr LiveContext::get_node(const char* id) {
      PortableServer::POA_var poa = corba_module->getPOA();
      PortableServer::ObjectId_var node_id = PortableServer::string_to_ObjectId(CORBA::string_dup(id));
      return OTGraph::Node::_duplicate(OTGraph::Node::_narrow(poa->id_to_reference(node_id)));
  }


  OTGraph::Node_var LiveContext::create_node(const char* _name, const OTGraph::StringTable& _attributes) {
      string name(_name);
      StringTable attributes(_attributes);
      lock();
      Node* value = createNode(name, attributes);
      // Add node to the name-Node mapping
      logPrintI("just about to getNode(value)\n");

      OTGraph::Node_var node_ref = getNode(value);
      graph->addNode(value); // also add it to the CORBA independent graph representation;

      logPrintI("just did getNode(value)\n");
      unlock();
      logPrintI("unlocked create_node\n");
      return node_ref;
    }


    OTGraph::Edge* LiveContext::connect_nodes(const OTGraph::Node_var& upstreamNode, const OTGraph::Node_var& downstreamNode) {
      logPrintI("connecting nodes\n");
      ACE_Guard<ACE_Thread_Mutex> mutexlock(*_mutex);
      logPrintI("getting upstreamNode\n");
      Node* upstream_node = getNode(upstreamNode);
      logPrintI("getting downstreamNode\n");
      Node* downstream_node = getNode(downstreamNode);

      // ********* SORT OUT THE THREAD SAFETY OF THIS!!!! ******
      if ((upstream_node == NULL) || (downstream_node == NULL)) {
          throw OTGraph::NodeNonExistent();
      }
      upstream_node->lock();
      downstream_node->lock();
          
      downstream_node->addChild(*upstream_node);
      upstream_node->addParent(downstream_node);

      OTGraph::Edge* _edge = new OTGraph::Edge;
      _edge->sender   = upstreamNode;
      _edge->receiver = downstreamNode;

      graph->touch(); // mark graph as changed;

      // ********* SORT OUT THE THREAD SAFETY OF THIS!!!! ******
      downstream_node->unlock();
      upstream_node->unlock();

      // add new edge to list of edges
      edges.push_back(Edge(((Node*) upstream_node), downstream_node));
      logPrintI("finished connecting nodes\n");
      return _edge;
    }

    void LiveContext::remove_edge(const OTGraph::Edge& edge) {
        OTGraph::Node_var upstreamNode = edge.sender;
        OTGraph::Node_var downstreamNode = edge.receiver;
        disconnect_nodes(upstreamNode, downstreamNode);
    }

  void LiveContext::disconnect_nodes(const OTGraph::Node_var& upstreamNode, const OTGraph::Node_var& downstreamNode) {
      logPrintI("disconnecting nodes\n");
      ACE_Guard<ACE_Thread_Mutex> mutexlock(*_mutex);
      Node* upstream_node   = getNode(upstreamNode);
      Node* downstream_node = getNode(downstreamNode);
      if ((upstream_node == NULL) || (downstream_node == NULL)) {
          throw OTGraph::NodeNonExistent();
      }
      ACE_Guard<ACE_Thread_Mutex> upstreamlock(*(upstream_node->getMutex()));
      ACE_Guard<ACE_Thread_Mutex> downstreamlock(*(downstream_node->getMutex()));
      NodeVector::iterator parent =
	std::find( upstream_node->parents.begin(),  upstream_node->parents.end(), downstream_node);
      NodeVector::iterator child =
	std::find( downstream_node->children.begin(),  downstream_node->children.end(), upstream_node);
      if ((parent == upstream_node->parents.end()) || (child == downstream_node->children.end())) {
          throw OTGraph::NodesNotConnected();
      }
      upstream_node->parents.erase(parent);
      // The sender node is the child of the receiver node
      downstream_node->children.erase(child);
      Edge edge(upstream_node, downstream_node);
      EdgeVector::iterator result = std::find( edges.begin(), edges.end(), edge );
      if( result != edges.end())
	{
	  edges.erase( result );
	}
      graph->touch();
    }

    void LiveContext::collectEdges(EdgeVector& _edges) {
        for (NodeIDMapIterator it=node_id_map.begin(); it != node_id_map.end(); ++it) 
        {
            Node* node = it->first;
            // get all the parent (i.e. downstream nodes)
            for (NodeVector::iterator downstream_it=node->parents.begin(); downstream_it != node->parents.end(); ++downstream_it) {
                //Node* downstream_node = &(*downstream_it);
                Node* downstream_node = (Node*) *downstream_it;
                _edges.push_back(Edge(node, downstream_node));
            }
        } 
    }


    char* LiveContext::getDot() {
        std::string dot = "digraph DataFlowGraph {\n";
        ACE_Guard<ACE_Thread_Mutex> mutexlock(*_mutex);
        EdgeVector _edges;
        collectEdges(_edges);
        for (EdgeVector::iterator it=_edges.begin(); it != _edges.end(); ++it) {
            Node* upstreamNode =   it->first;
            Node* downstreamNode = it->second;
            std::string upstream_id = getIDFromNode(upstreamNode);
                std::string downstream_id = getIDFromNode(downstreamNode);
                dot += std::string("\t") + upstream_id + " -> " + downstream_id + std::string(";\n");
        }
        for (NodeIDMapIterator it=node_id_map.begin(); it != node_id_map.end(); ++it) 
        {
            Node* node = it->first;
            std::string id = it->second;
            //std::string type = node->getType();
            //dot += std::string("\t") + id + std::string(" [label=\"") + 
            //    node->getType() + std::string("\\n") + id + std::string("\"];\n");
            dot += std::string("\t") + id + std::string(" [label=\"") + 
                node->getType() + std::string("\\n") + id + std::string("\"];\n");
            //node_refs[i] = getNodeRefFromID(it->second);	
            //i++;
        } 

        dot += "}";
        return CORBA::string_dup(dot.c_str());
    }

  OTGraph::NodeVector* LiveContext::get_nodes() {
      OTGraph::NodeVector_var node_refs = new OTGraph::NodeVector;
      int len = node_id_map.size();
      node_refs->length(len);
      int i = 0;
      for (NodeIDMapIterator it=node_id_map.begin(); it != node_id_map.end(); ++it) 
      {
          node_refs[i] = getNodeRefFromID(it->second);	
          i++;
     } 
      return node_refs._retn();
    };

  OTGraph::EdgeVector* LiveContext::get_edges() {
      ACE_Guard<ACE_Thread_Mutex> mutexlock(*_mutex);
      EdgeVector edge_collection;
      collectEdges(edge_collection);
      OTGraph::EdgeVector_var _edges = new OTGraph::EdgeVector;
      int len = edge_collection.size();
      _edges->length(len);
      for (int i=0; i < len; i++) {
	OTGraph::Edge _edge;
	_edge.sender   = getNode(edge_collection[i].first);
	_edge.receiver = getNode(edge_collection[i].second);
	_edges[i] = _edge;
      }
      return _edges._retn();
    };

  void LiveContext::remove_node(const OTGraph::Node_var& target_ref) {
      logPrintI("remove_node\n");
      ACE_Guard<ACE_Thread_Mutex> mutexlock(*_mutex);
      Node* target = getNode(target_ref);
      if (target == NULL) {
          throw OTGraph::NodeNonExistent();
      }
      // Handle child nodes (upstream nodes)
      NodeVector::iterator child_it = target->children.begin();
      while ( child_it != target->children.end())
        {
	  Node* child = (Node *) *child_it;
	  target->removeChild(*child);
	  child_it++;
        }
      // Handle parents
      NodeVector::iterator parent_it = target->parents.begin();
      while ( parent_it != target->parents.end())
        {
	  Node * parent = (Node *) (*parent_it);
	  parent->removeChild(*target);
	  parent_it++;
        }

      // identify module, and instruct module to remove references to Node
      Module * mod = getModuleFromNode(target);
      if (mod != NULL) {
	logPrintI("calling mod->removeNode\n");
	mod->removeNode(target);
	logPrintI("called mod->removeNode\n");
      } else {
          logPrintE("unable to find module associated with node %s\n", getIDFromNode(target).c_str());
      }
      // Having removed all references to Node it can now be deactivated
      logPrintI("about to deactivateNode\n");
      deactivateNode( target );

      graph->touch(); // mark graph as changed;
    }

  void LiveContext::run()
    {
      //start();
      int stopflag = stop();
      while ( stopflag == 0 )
        {
	  stopflag=loopOnce();
        }
      //close(); 
    };

  char* LiveContext::getXMLString() {
    return CORBA::string_dup("<foo/>");
  }
  

#endif //USE_LIVE

} // namespace ot

/* 
 * ------------------------------------------------------------
 *   End of LiveContext.cxx
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
