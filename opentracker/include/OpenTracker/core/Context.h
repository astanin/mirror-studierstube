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
/** header file for Context class
 *
 * @author Gerhard Reitmayr
 *
 * $Id: Context.h 2116 2008-02-05 11:29:45Z sareika $
 * @file                                                                   */
/* ======================================================================= */

#ifndef _CONTEXT_H
#define _CONTEXT_H

#include "../dllinclude.h"

#include "Node.h"
#include "Graph.h"
#include "Module.h"
#include "NodeFactoryContainer.h"
#include "VideoUser.h"

#ifndef SWIG
#include <sys/types.h>
#include <sys/timeb.h>
#ifdef WIN32
#include <sys/utime.h>
#endif
#endif

class ACE_Thread_Mutex;
class ACE_Condition_Thread_Mutex;




namespace ot {

    /// a string map to store modules in
    typedef std::map<std::string, Module::Ptr> ModuleMap;
#ifdef USE_LIVE
    typedef std::map<Node*, std::string> NodeIDMap;
    typedef NodeIDMap::iterator NodeIDMapIterator;
    typedef std::map<std::string, Node*> IDNodeMap;
    typedef IDNodeMap::iterator IDNodeMapIterator;
    typedef std::pair<Node *, Node *> Edge;
    typedef std::vector<Edge> EdgeVector;

    //typedef std::map<std::string, PortableServer::ServantBase*> IDServantMap;
    //typedef IDServantMap::iterator IDServantMapIterator;
#endif

    /**
     * This class represents one context. It keeps its own modules and tracker tree. Using
     * a ConfigurationParser it builds the tree from a given configuration file. Any
     * factories and modules need to be added before the parsing happens, otherwise
     * the nodes can not be parsed and the modules will not be initialized with
     * their respective configuration elements.
     *
     * Finally it implements the main loop driving the tracker. This consists of
     * the following basic algorithm described in pseudo code here :
     *@verbatim
     start();
     while( !stop())
     {
     pushEvents();
     pullEvents();
     }
     close(); @endverbatim
     * The whole loop is implemented in the member function run, which can be used
     * instead of implementing it yourself.
     *
     * The Context also keeps track of various information. The following items
     * are stored and used by modules :
     *
     * The Context stores a stack of directories. Any module that needs to load
     * a file can use the stack to find files relative to a number of directories.
     * By default the stack contains the path component of the configuration file,
     * followed by the current directory. See the method findFile for details.
     *
     * @author Gerhard Reitmayr
     * @ingroup core
     */

    class OPENTRACKER_API Context
    {
	private:
		bool isStarted;

    protected:
        /// A map of the modules this context works with
        ModuleMap modules;
#ifdef USE_LIVE
        NodeIDMap  node_id_map;
        IDNodeMap  id_node_map;
        EdgeVector edges;
        void liveContextConsistencyCheck();
        //IDServantMap id_servant_map;
#endif
        /// Pointer to the root node of the local tracker tree
        //        Node::Ptr rootNode;
#ifndef USE_LIVE
        Graph::Ptr graph;
#else
        Graph* graph;
        bool activatelive;
#endif
        /// A NodeFactoryContainer used by the ConfigurationParser to create new nodes.
        NodeFactoryContainer factory;
        /// flag to remember whether the Context is responsible for cleaning up the modules.
        bool cleanUp;
        /// flag for stopping the main loop
        bool stoploopflag;
	/// flag whether synchronization between data producers and consumers
	//  should be done
	bool dosync;
        /// stores the filename of the current configuration.
        std::string file;
        /// stores a stack of directory names to search for files in
        std::vector<std::string> directories;
        /// stores the namespace string of the root element
        std::string rootNamespace;

	VideoUserVector videoUsers;

        /// used for synchronization between the main loop and the reconfiguration scheme
        ACE_Thread_Mutex* _mutex;
        /// used for synchronization in data-driven traversal
        /// in this case the graph is only traversed if some
        /// event-driven enabled event generator node produces new data
        //#ifdef WIN32
        ACE_Thread_Mutex* _havedatamutex;
        ACE_Thread_Mutex* _consumeddatamutex;
        ACE_Condition_Thread_Mutex* _havedatacondition;
        ACE_Condition_Thread_Mutex* _waitingpendingcondition;
        ACE_Condition_Thread_Mutex* _consumeddatacondition;
        ACE_Condition_Thread_Mutex* _waitingconsumedcondition;
        bool pendingdata;
        bool waitingpending;
        bool dataconsumed;
        bool waitingconsumed;

        //#else
        //        pthread_mutex_t* _havedatamutex;
        //        pthread_cond_t* _havedatacondition;
        //#endif
	double lastlooptime;
	int loopcount;

    public:
        ///shortcut to stop the main loop by acquiring the mutex
        virtual void lock();
        virtual void unlock();
        /// methods for data driven traversal
        void waitDataSignal();
        void dataSignal();
        void dataBroadcast();
        void dataLock();
        void dataUnlock();

#ifdef WIN32
        void consumedWait(int usecs = 0); // wait forever
#else
        void consumedWait(suseconds_t usecs = 0); // wait forever
#endif

        void consumedSignal(); 
        void consumedBroadcast(); 
        // Methods
    public:

        /** a constructor method.
         * @param init If init is not equal to 0, it instantiates all known modules and
         *        factories, adds them to its local containers and also takes care of
         *        removing them again in the destructor.*/
        Context( int init = 0 );
        /** destructor method clears containers and removes any modules instantiated in
         * the default setup, if cleanUp is st. */
        virtual ~Context();

        /** adds a new factory to the NodeFactoryContainer.
         * @param newfactory reference to the new factory */
        void addFactory(NodeFactory& newfactory);
        /** removes a factory from the NodeFactoryContainer.
         * @param factory reference to the factory to remove */
        void removeFactory(NodeFactory & factory);

        /** adds a module to the contexts container.
         * @param name the element name of the modules configuration element
         * @param module reference to the module */
        void addModule(const std::string & name, Module & module);

        /** updates the context member of all modules.
         * This can be necessary if modules are copied from another context.
         */
        void updateModuleContexts(); 

        /** removes a module from the map
         * @param reference to the module */
        void removeModule(Module & module);

        /** This method creates a new node based on a given element name and
         * a table of attributes. It will delegate the construction to the
         * stored NodeFactory list and return a new Node, if one of the
         * factories can create the desired type. Otherwise it returns NULL.
         * It makes sure that the node is associated with this context, therefore
         * this is the only way to create new nodes for the user.
         * @param name the name of the element
         * @param attributes a StringTable containing the attributes of the element
         * @return pointer to the new node or NULL
         */
        Node * createNode(const std::string & name, const StringTable & attributes);

        /** returns the root node, that contains the tracker tree. This node
         * is an instance of the generic node class and only deal with the
         * children API. It does also contain the configuration element in the first node.
         * @returns pointer to the root node */
        Graph * getRootNode();

        /** returns the node with the given unique ID tag. If none is found
         * NULL is returned.
         * @param id string containing the value of the ID tag
         * @returns pointer to the found node, or NULL.
         */
        Node * findNode(const std::string & id);
#ifdef USE_LIVE
        std::string getIDFromNodeRef(const OTGraph::Node_var& node_ref);
        std::string getIDFromNode(Node* node);
        OTGraph::Node_var getNodeRefFromID(const std::string& id);
        OTGraph::Node_var getNode(Node* node);
        //        PortableServer::ServantBase* getServant(const std::string& id);
        Node* getNode(const OTGraph::Node_var& node_ref);
        Node* getNode(const std::string& id);
        void eraseNode(Node* node);
        void appendNode(Node* node, const std::string& id);
        void activateNode(Node* node, const char* id);
        void activateNode(Node* node);
        void deactivateNode(Node* node);
        bool getActivateLive() const { return activatelive; };
        void setActivateLive(const bool val) { activatelive = val; };
#endif
        /** This method initialies the context with a tracker tree described by
         * a configuration file. It parses the file and builds the tree.
         * @param filename the path and name of the configuration file */
        void parseConfiguration(const std::string& filename);
        void parseConfigurationString(const char* xmlstring);

        /** calls start on all modules to do some initialization. */
        void start();

        /** This method calls pushEvent on all moadules to get new data into the
         * shared data tree. */
        void pushEvents();

        /** calls pullSEvent on all modules to get data out again.*/
        void pullEvents();

        /** do one atomic cycle of the OpenTracker loop, returns the value of
	 * of stop(), to indicate whether the main loop needs to be stopped.
	 * Introduced for reconfiguration purposes, it is not safe to call
	 * pushEvent(), pullEvent(), and stop() separatelly, since the configuration
	 * may change between calls. For safety use loopOnce() and test the return.*/
        int loopOnce();

        /* blocking loop traversal -> needs at least one event from some source to finish */
        int syncLoopOnce();
       
        /** This method implements the main loop and runs until it is stopped
         * somehow. Then it calls close() on all modules. */
        void run();

        /** This method implements the main loop in a way that it is
         * run at a specificed rate until it is stopped
         * somehow. Then it calls close() on all modules. */
        void runAtRate(double rate);

        /** This is a data-driven implementation of the main loop */
        void runOnDemand();

        /** tests whether the mainloop should stop, by asking all modules whether
         * they need to stop.*/
        int stop();

        /** sets a termination flag for the main loop*/
        void stopLoop();

        inline bool doSynchronization() const { return dosync; };
        void setSynchronization( bool val) { dosync = val; };

        /** calls close on all modules to close any resources.*/
        void close();

        /** add a directory to the front of the directory stack
         * @param dir directory to add
         */
        void addDirectoryFirst( const std::string & dir );

        /** add a directory to the end of the directory stack
         * @param dir directory to add
         */
        void addDirectoryLast( const std::string & dir );

        /** remove a directory from the directory stack
         * @param dir directory to add
         */
        void removeDirectory( const std::string & dir );

        /** tries to locate a file by prepending the directory names in the
         * stack. If a file at that location is found, the full name is
         * returned in fullname and true will be returned. Otherwise fullname
         * is not changed and false is returned.
         * @param filename the partial filename of the file to look for
         * @param fullname will contain the full filename upon successful return
         * @return returns true, if a file could be found, false otherwise.
         */
        bool findFile( const std::string & filename, std::string & fullname );

        /**
         * returns the XML namespace of the root element
         * @return string reference containing the XML namespace
         */
        const std::string & getRootNamespace();

		const std::string& getConfigFile();
	/// Called by a video providing instance to pass a new video frame into OpenTracker
	void newVideoFrame(const unsigned char* image, int width, int height, PIXEL_FORMAT format, bool forceTraversal=false);


	/// Called by a module to register itself as a video user
	void registerVideoUser(VideoUser* videoUser);


	/// Called by a module to unregister itself as a video user
	void unregisterVideoUser(VideoUser* videoUser);

	/// Called by the Configurator upon reconfiguration. The other context contains the new 
	/// configuration that must be copied by the running one.
	void copyFrom( Context & other);
	void addNode(std::string parentid, Node * newnode);
	void removeSubtree(std::string nodeid);
	void removeNode(std::string nodeid);
	void replaceNode(std::string nodeid, Node * newnode);

	/// Obtain the Module responsible for a certain node, Notice that this may return NULL
	/// for nodes like EventUtilityNode, which are part of opentracker
	Module * getModuleFromNode(const Node * node);
	/// Obtain the Module responsible for a node of type nodename. It might return NULL
	/// for nodes like EventUtilityNode which are part of opentracker (but have no Module)
	Module * getModuleFromNodeType(std::string nodename);

        /** returns the module associated with a certain configuration element
         * @param name the element name
         * @returns pointer to the module or NULL, if name does not exist */
        Module * getModule(const std::string & name);
        /** returns the number of modules available in the context
         * @returns module number */
        int getModuleCount() const { return modules.size(); };

        /** returns a list of module names present in the context
         * @returns vector of strings containing the names */
        int getModuleNames( std::vector<std::string> &names) const;
	
	bool isConfigured();

    friend class ConfigurationParser;
    };

    // returns the namespace of the root node
    inline const std::string & Context::getRootNamespace()
    {
        return rootNamespace;
    }

} // namespace ot

#endif

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
