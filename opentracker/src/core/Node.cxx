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
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF  SUCH DAMAGE.
 * ========================================================================
 * PROJECT: OpenTracker
 * ======================================================================== */
/** The source file for the basic Node class.
 *
 * @author Gerhard Reitmayr
 * @author Eduardo Veas
 *
 * $Id: Node.cxx 2133 2008-03-11 17:06:47Z mendez $
 *
 * @file                                                                   */
/* ======================================================================= */

#ifdef WIN32 
#pragma warning(disable:4244)
#endif

#include <cstdio>
#include <iostream>

#include <memory>
#include <algorithm>

#include <ace/Thread.h>
#include <ace/Synch.h>

#include <OpenTracker/OpenTracker.h>

#include <OpenTracker/core/Node.h>
#include <OpenTracker/core/StringTable.h>
#include <OpenTracker/core/Configurator.h>
#include <OpenTracker/core/Context.h>

#ifdef USE_LIVE
#include <OpenTracker/skeletons/OTGraph.hh>
#endif

//using namespace std;


namespace ot {


    // emtpy string to be returned, if key is not in the map
    // the object itself is part of StringTable.cxx
    extern const std::string empty("");



    // constructor
    Node::Node() : name("")
    {
        logPrintD("creating Node\n");
#ifndef USE_LIVE
        OT_INITIALIZE_IREFCOUNTED;
#endif
        graph=0;
        traversalorder = 0;
	mutex = new ACE_Thread_Mutex;
        logPrintD("created\n");
    }

    // destructor

    Node::~Node()
    {
        logPrintW("Destructor of node %s\n",getType().c_str());
        std::cerr << "Destructor of node" << std::endl;
        // this one will call disconnect indirectly!
#ifndef USE_LIVE
        disconnect();
        std::cerr <<"remnode"<<std::endl;
        if (graph) graph->remNode(this);
        //        setGraph(0);
#endif
        std::cerr <<"delete mutex"<<std::endl;
        delete mutex;
        std::cerr <<"finishede"<<std::endl;

    }

    void Node::disconnect()
    {
        for (NodeVector::iterator i = children.begin(); i != children.end(); i++){
            (*i) ->removeParent(this);
        }

        children.clear();
       
        //        logPrintW("children cleared\n");
        for (NodeVector::iterator i = parents.begin(); i != parents.end(); i++){
            (*i) ->removeChild(*this);
        }
        
        parents.clear();
        //        logPrintW("parents cleared\n");

    }

    // returns a pointer to the parent node

    Node * Node::getParent()
    {
        return parents[0];
    }

    // returns the Context this node lives in
    Context * Node::getContext() const
    {
        // fix for the moment
        // must be changed for a stronger statement
        return Configurator::instance()->getContext();
    }

    // returns the number of children
    unsigned int Node::countChildren()
    {

        unsigned int count = 0;
        for (NodeVector::iterator it = children.begin(); it != children.end(); it++)
            if ((*it)->isNodePort() == 0)
                count ++;

        return count;

    }


    void Node::setGraph(Graph * g){
        if (graph){
            Graph * tmp = graph;
            graph = 0;
            tmp->remNode(this);
        }
        graph = g;
           
    };
    Graph * Node::getGraph(){
        return graph;
    };
    // adds a new child

    Node::error Node::addChild(Node & child)
    {
        logPrintD("setting the childs %s parent to %p\n", (child.getType()).c_str(),this);
        // add this to the parent list of the child
        //        child.addParent(this);
        // add the child to the children list of this node
        children.push_back(&child);
        updateTraversalOrder();

        return OK;
    }

    // removes a child

    Node::error Node::removeChild(Node & child)
    {

        bool found = false;

        NodeVector::iterator it = children.begin();
        while (it != children.end()){
            if (((Node*)*it) == (&child)){
                found = true;
                //                logPrintE("Erasing parent %p = %p\n", parent, (*it));                
                it = children.erase(it);
                
                
            }else
                it ++;
            
        }

        if (found)
        {
            updateTraversalOrder();
            return OK;
        }
        else
            return NOT_FOUND;
    }

    Node::error Node::removeParent(Node * parent)
    {

        bool found = false;
        NodeVector::iterator it = parents.begin();
        while (it != parents.end()){
            if (((Node*)*it) == (parent)){
                found = true;
                //                logPrintE("Erasing parent %p = %p\n", parent, (*it));
                it = parents.erase(it);
                
            }else
                it ++;
            
        }
        if (found)         
            return OK;            
        else
            return NOT_FOUND;
    }

    void Node::updateTraversalOrder()
    {
        int maxtraversalorder = -1;
        NodeVector::iterator it = children.begin();
        int acttravorder = 0;

        while (it != children.end()) 
        {
            if ((acttravorder = (*it)->getTraversalOrder()) > maxtraversalorder)
            {
                maxtraversalorder = acttravorder;
            }
            ++it;
        }
        if (traversalorder != maxtraversalorder + 1)
        {
            traversalorder = maxtraversalorder + 1;

            // propagate to all parents
            if (countParents() > 0)
            {
                NodeVector::iterator pit;
                for (pit=parents.begin(); pit != parents.end(); pit++)
                {
                    (*pit)->updateTraversalOrder();
                }
            }
        }
    }

    // iterates through the children by returning the child by index

    Node * Node::getChild( unsigned int index )
    {

        Node * result=NULL;
        for (int i = 0; i < (int) children.size(); i++){
            Node * tmp = children[i];
            if (tmp->isNodePort() == 0){
                if (index == 0){
                    result = tmp;
                    break;
                }else index --;
            }
        }
        return result;

    }

    int Node::getTraversalOrder() const
    {
        return traversalorder;
    }

    // returns number of NodePorts present on this Node

    unsigned int Node::countPorts()
    {
        unsigned int result = 0;
        for (NodeVector::iterator it = children.begin(); it != children.end(); it ++)
            if ((*it)->isNodePort() == 1)
                result++;

        return result;
    }
            

    // returns a NodePort child object indexed by Name and index

    NodePort * Node::getPort( const std::string & name, unsigned int index )
    {
        for (NodeVector::iterator it = children.begin(); it != children.end(); it++){
            if ((*it)->isNodePort() == 1){
                if (name.compare((*it)->getType()) == 0){
                    if (index == 0)
                        return (NodePort *) ((Node*) *it);
                    else index --;
                }
            }
        }
        return NULL;
    }

    // returns a NodePort child object by index

    NodePort * Node::getPort( unsigned int index )
    {
        NodePort * result = NULL;
        for (int i = 0; i < (int)children.size(); i ++){
            Node * tmp = children[i];
            if (tmp->isNodePort() == 1)
                if (index == 0){
                    result = (NodePort*) tmp;
                    break;
                } else index--;
        }
        return result;
    }


    // creates and adds a new child NodePort object of the given name.
    Node::error Node::addPort( const std::string & name )
    {

        // To create the node port, we can use any instance of the context
        StringTable table;
        Context * ctx = getContext();
        Node * node = ctx->createNode( name, table );
        if (node == NULL) 
            return GRAPH_CONSTRAINT;
        else
            return addChild(*node);

    }
     
    // removes a child NodePort object indexed by name
    Node::error Node::removePort( const std::string & name )
    {

        NodePort * port = getPort( name );
        if( port == NULL )
            return NOT_FOUND;
        return removePort( *port );

    }

    // removes a child NodePort object by index
    Node::error Node::removePort( unsigned int index )
    {

        NodePort * port = getPort( index );
        if( port == NULL )
            return NOT_FOUND;
        return removePort( *port );


    }

    Node::error Node::removePort( NodePort & port)
    {
        return removeChild((Node &) port );
    }


    // updates any observers ( the parent and the references )
    void Node::updateObservers( Event &data )
    {
        //logPrintI("updateObservers\n");
        //#ifdef USE_LIVE
        //        lock();
        //#endif
        if (isEventGenerator() == 1 || isNodePort() == 1){
            for (NodeVector::iterator it = parents.begin() ; it != parents.end() ; it++){
                //logPrintI("about to invoke onEventGenerated\n");
                (*it)->onEventGenerated(data, *this);
                //logPrintI("invoked onEventGenerated\n");
            }

        }
        //#ifdef USE_LIVE
        //unlock();
        //#endif
    }

    Event& Node::getEvent(unsigned int index) {
        return Event::null;
    }
    
    Event& Node::getEventNearTime(double time) {
        return Event::null;
    }

    Event& Node::getEventAtTime(double time) {
        return Event::null;
    }

    // returns a value to a given key
    const std::string Node::get( const std::string & key ) const
    {

        Node * self = const_cast<Node*>(this);
        const std::string & result =  self->attributes.get(key) ;

        return result;

    }

    // stores a key, value pair

    void Node::put( const std::string & key, const std::string & value )
    {

        if (key.compare("OtNodeType") == 0)
        {
            this->type = value;
        } 
        else
        {
            attributes.put(key, value);
        }

    }

    // removes a key, value pair

    void Node::remove( const std::string & key )
    {

        attributes.remove(key);

    }

    // some put and get methods

    void Node::put(const std::string & key, const int value)
    {
        char buffer[20];

        sprintf( buffer, "%i", value );
        put( key, buffer );
    }

    void Node::put(const std::string & key, const float value)
    {
        char buffer[20];

        sprintf( buffer, "%f", value );
        put( key, buffer );
    }

    void Node::put(const std::string & key, const double value)
    {
        char buffer[30];

        sprintf( buffer, "%lf", value );
        put( key, buffer );
    }

    void Node::put(const std::string & key, const int * value, int len)
    {
        char buffer[20];
        std::string strvalue;

        sprintf(buffer, "%i", value[0] );
        strvalue.append(buffer);
        for( int i = 1; i < len; i++ )
        {
            sprintf(buffer, " %i", value[i] );
            strvalue.append(buffer);
        }
        put( key, strvalue );
    }

    void Node::put(const std::string & key, const float * value, int len)
    {
        char buffer[20];
        std::string strvalue;

        sprintf(buffer, "%f", value[0] );
        strvalue.append(buffer);
        for( int i = 1; i < len; i++ )
        {
            sprintf(buffer, " %f", value[i] );
            strvalue.append(buffer);
        }
        put( key, strvalue );
    }

    void Node::put(const std::string & key, const double * value, int len)
    {
        char buffer[20];
        std::string strvalue;

        sprintf(buffer, "%lf", value[0] );
        strvalue.append(buffer);
        for( int i = 1; i < len; i++ )
        {
            sprintf(buffer, " %lf", value[i] );
            strvalue.append(buffer);
        }
        put( key, strvalue );
    }

    int Node::get(const std::string & key, int * value, int len ) 
    {
        std::string data = get( key );
        const char * start = data.c_str();
        char * end = (char *)start;
        int count = 0;
        value[count++] = strtol( start, &end, 0 );
        while( end != start && count < len){
            start = end;
            value[count++] = strtol( start, &end, 0 );
        }
        return count;
    }

    int Node::get(const std::string & key, float * value, int len ) 
    {
        std::string data = get( key );
        const char * start = data.c_str();
        char * end = (char *)start;
        int count = 0;
        value[count++] = (float)strtod( start, &end );
        while( end != start && count < len){
            start = end;
            value[count++] = (float)strtod( start, &end );
        }
        return count;
    }

    int Node::get(const std::string & key, double * value, int len ) 
    {
        std::string data = get( key );
        const char * start = data.c_str();
        char * end = (char *)start;
        int count = 0;
        value[count++] = strtod( start, &end );
        while( end != start && count < len){
            start = end;
            value[count++] = strtod( start, &end );
        }
        return count;
    }

    void Node::lock()
    {
	mutex->acquire();
    };

    // leaves a critical section. 

    void Node::unlock()
    {
	mutex->release();
    };

#ifndef USE_LIVE
    OT_IMPLEMENT_IREFCOUNTED(Node);
//         int Node ::_ref(){
//         logPrintW("node %p rcount %d\n", this, _rcount);
//         return ++ _rcount;
//     };
  
//   int Node ::_deref(){
//      if (--_rcount == 0){
//          logPrintW("Node deleting %p, rcount %d\n", this, _rcount);
//         delete this;
//         return 0;
//      } 
//      return _rcount;
//    };
  
//   int Node ::getRefCount(){
//      return _rcount;
//   }
#endif
    // add one parent node
    void Node::addParent( Node * parent ){
        parents.push_back( parent );
        parent->updateTraversalOrder();
    }

    /**
     * returns a parent indicated by the index.
     * @param index unsigned number =>0 and < countParents()
     * @return pointer to the parent node or NULL if index is out of range.
     */
    Node * Node::getParent( unsigned int index){
        if (index > parents.size())
            return NULL;
        return parents[(int)index];
    };
    
    /**
     * returns the number of parents of this node
     * @return unsigned number of parents
     */
    unsigned int Node::countParents(){
        return (unsigned int)parents.size();
    };

    Node * Node::findNode(const std::string & key, const std::string & val){
        Node * result = NULL;

        if (attributes.containsKey(key)){
            const std::string & value = attributes.get(key);
            if (value.compare(val) == 0)
                result = this;
        }
        // search the children
        if (result == NULL){
            for (int i = 0; i < (int)children.size() ; i++){
                result = children[i] ->findNode(key, val);
            }
        }

        return result;
    };

    unsigned int Node::countAllChildren(){
        return (unsigned int) children.size();
    };

    /*
     * returns a child at index, no matter whether it is a wrapper or not
     * @return unsigned number of children */
    Node * Node::getAnyChild( unsigned int index){
        if (index >= children.size())
            return NULL;
        return children[(int)index];
    };


    StringTable & Node::getAttributes(){
        return attributes;
    };

    bool Node::operator<(const Node &nd) const
    {
        return traversalorder < nd.traversalorder;
    }

#ifdef USE_LIVE
    char* Node::get_type() {
        return CORBA::string_dup(getType().c_str());
    }

    char* Node::get_name() {
        return CORBA::string_dup(getName().c_str());
    }

    char* Node::get_id() {
        return CORBA::string_dup(get("DEF").c_str());
    }

    char* Node::get_attribute(const char* _key) {
        std::string key(CORBA::string_dup(_key));
        return CORBA::string_dup(get(key).c_str());
    }

    OTGraph::StringTable* Node::get_attributes() {
#ifdef NO_OT_LOCAL_GRAPH
        StringTable* value = new StringTable;
        DOMElement * element =  (DOMElement* ) parent;
        DOMNamedNodeMap * map = element->getAttributes();
        for( unsigned int i = 0; i < map->getLength(); i++ )
        {
            DOMAttr * attribute = (DOMAttr *)map->item( i );
            char * nameTemp = XMLString::transcode( attribute->getName());
            char * valueTemp = XMLString::transcode( attribute->getValue());
            value->put(nameTemp, valueTemp );
            XMLString::release( &valueTemp );
            XMLString::release( &nameTemp );
        }
        OTGraph::StringTable_var attributes = value->getStringTable();
        delete value;
        return attributes._retn();
#endif //NO_OT_LOCAL_GRAPH
#ifndef NO_OT_LOCAL_GRAPH
        Node * self = const_cast<Node*>(this);
        StringTable value(self->attributes);
        OTGraph::StringTable_var attributes = value.getStringTable();
        return attributes._retn();
#endif //NO_OT_LOCAL_GRAPH
   };


#endif

} // namespace ot

/* 
 * ------------------------------------------------------------
 *   End of Node.cxx
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
