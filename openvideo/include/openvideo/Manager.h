/* ========================================================================
 * Copyright (C) 2005  Graz University of Technology
 *
 * This framework is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This framework is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this framework; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * For further information please contact Denis Kalkofen under
 * <kalkofen@icg.tu-graz.ac.at> or write to Denis Kalkofen,
 * Graz University of Technology, Inffeldgasse 16a, A8010 Graz,
 * Austria.
 * ========================================================================
 * PROJECT: OpenVideo
 * ======================================================================== */
/** The header file for the Manager class.
  *
  * @author Denis Kalkofen
  *
  * $Id: Manager.h 226 2007-02-14 07:24:03Z breiting $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef MANAGER_H
#define MANAGER_H

#include "openVideo.h"

#include <vector>

#ifdef WIN32
	#define _WIN32_WINNT  0x0400
	#define WIN32_LEAN_AND_MEAN
	
	#include "windows.h"
#endif

#ifdef LINUX
#include <GL/glx.h>
#endif


#include <openvideo/Logger.h>
#include <openvideo/OvLogger.h>
#include <openvideo/Scheduler.h>



class TiXmlElement;
class  ACE_Thread_Mutex;
class  ACE_Condition_Thread_Mutex;

namespace openvideo {
class Node;
class NodeFactory;


/**@ingroup core
*	The ‘Manager’ class represents OpenVideo's main object which constructs, holds and updates OpenVideo’s runtime data structure. OpenVideo defines its data structure as a directed acyclic graph which is defined in an xml based configuration file. To construct such an OpenVideo graph the ‘Manager’ provides the necessary functionality to parse a given OpenVideo configuration as well as to construct the graph out of the information gained in the parsing step. To construct a single node the ‘Manager’ uses the related factory this specific node implements. Therefore, a list of factories of all known OpenVideo nodes is stored in an instance of a ‘Manager’. \n 
*	The basic lifecycle of a Manager object looks like this:
*@verbatim
//1)
//1a) initialize the list of node factories
//1b) set the callback function to update the graph
//1c) set the function which initializes the graph 
//
Manager manager;

//2)
//2a) parse the give configuration 
//2b) construct on OpenVideo graph
manager.parseConfiguration(“aConfiguration”);

//3)
//3a)	initialize the graph (by calling the function defined in (1b) –> this 
//	perhaps rearranges the node list the manager holds) 
//	the list 
//3b)	initialize a timer to permanently update the graph data 
//3c)	start the timer which constantly calls the traversal function which was
//   	defined in 1b)
manager.run()
@endverbatim
*
*	The following drawing shows an example of an OpenVideo graph.\n
*
*	\image html ovgraph.gif
*
*	Internal dependencies ->	* Timer -> to create the timer to trigger the traversal \n
*								* NodeFactory + Various Node Factories -> to create the nodes \n 
*								* Node -> basic elements in the graph \n 
*
*	External dependencies ->	* TinyXML – to parse OpenVideo’s xml based configuration 
*
*
* 
*/
class OPENVIDEO_API Manager 
{
 public:
	static Manager* getInstance();
    /** 
	*	destructor 
	*/
    ~Manager();

    /**
    *                                                                     
    */
    static void update(void*);


	/// Traverses the OpenVideo graph one single time
	/**
	 *  This method should only be called to let OpenVideo run in the
	 *  same thread as the application does. In such a scenario it is the
	 *  application author's duty to call this method periodically.
	 */
	void updateSingleThreaded();

    /** 
	*	adds a NodeFactory to the list of known factories'
	*/
	void addNodeFactory(openvideo::NodeFactory *aFactory);
    
    void parseConfiguration(TiXmlElement* element);

    /** 
	*	This method parses an xml configuartion of OpenVideo. 
	*	While parsing the file the OpenVideo graph will be created.
    */
    bool parseConfiguration(const std::string& filename);
    
	/**
	*	Constructs the sub graph for a given (tinyxml)Element. 
	*	It adds all child nodes to 'parentNode'
	*/
    void buildSubGraph(TiXmlElement * parentElement, openvideo::Node* parentNode);
    
    void initTraversal();
	/**
    *	Starts the manger's mainLoop. 
	*	Before it actually activates the timer -who is resposible for calling the update function- the traversal data 
	*	is initialized and the graph is 'started' by calling 'initTraversalFunc(...)'.
    */
    void run();
    
	/**
	*	Creates and configures a node by a given (tinyxml)Element. Before the created node is returned, the node will be added 
	*	to the managers list of nodes. 
	*	Notice: The connections between all nodes are set in 'buildSubGraph(void * parentElement, Node* parentNode)'.
	*/
    openvideo::Node* addNode(TiXmlElement *element);
    
	/**
	*	Returns the node with the given 'nodeName'. NULL is returned if non of those currently exist. 
	*/
    openvideo::Node* getNode(std::string nodeName);

#ifdef OV_IS_WINDOWS
    void setGLContext(HGLRC _glContext,HDC _dc);
#endif

#ifdef OV_IS_LINUX
     void setGLContext(GLXDrawable _drawable, GLXContext _glContext, Display* _dsp);
#endif

    void deleteGLContext();

    static bool hasGLContext;
	/**
    *   Stops the manger's mainLoop. 
    */
    void stop();
    
    /**
    *   Sets the manager's traversal function and data.
	*  "setTravFunction(..)" is called during construction time with 
    *   Manager::topologicalSortedTraversal as argument to set the default traversal strategy. \n
	*   Use this function to change OpenVideo's traversal strategy.
    */
    void setTravFunction(void (*travFunction)(void*),void* data);
	 
	/**
	*	Sets the function and data to be called before the actual traversal begins.
	*/
    void setInitTravFunction(void (*initTravFunction)(void*),void* data);

    bool isStarted();

    Logger* getLogger(){return logger;}

	bool glContextChanged;

 protected:
	 /** 
	 *	constructor 
	 */
	 Manager();

	static Manager* instance;

    openvideo::Scheduler* scheduler;
	/** 
	*	Initializes all factories. 
	*	Adds all known (hard coded) factories to the manager's list of node factories' 
	*/
    void initNodeFactories();

    bool     isOVStarted;
    
    /**
	*	THE GRAPH. 
	*	This vector holds all nodes in the current OpenVideo graph in a list. 
    */
	
	std::vector<openvideo::Node*> nodes;

    /**
    *   A vector to hold all 'DEF' marked nodes in the xml file. 
	*	This list holds references to nodes which are previously marked for later referencing.
    */
	std::vector<openvideo::Node *> defNodes;
    
    /**
    *   A vector which holds objects of all known node factories. 
    *   This vector will be searched when a new node is going to be created.
    */
    std::vector<openvideo::NodeFactory*> factories;

    /**
    *	A pointer to the function which initializes the traversal data. 
    */
    static void (*initTraversalFunc)(void* data);

    /**
    *   A pointer to the traversal function which is called by the run function. 
    */
    static void (*traversalFunc)    (void* data);


    /**
    *	The traversal data. This void pointer is passed to the traversal function every time it is going to be invoked.
    *	Per default, this points to the managers list of nodes. It's value is overriden by 	setTravFunction(...);
    */
    static void* traversalData;


	/**
	*	A pointer to the traversal data 
	*	Per default, this points to the managers list of nodes. It's value is overriden by 	setInitTravFunction(...);
	*/
    static void* initTraversalData;
    
    /**
    *   Implementation of a topological sorted graph traversal. -- this is the default traversal of OpenVideo.
    */
	static void topologicalSortedTraversal(void* nodeList);

	/**
	*	An implementation of a topological sorted traversal strategy. 
	*	This is used as the default traversal strategy OpenVideo implements.
	*/
	static void initTopologicalSortedTraversal(void* nodeList);

	/**
	*	A Mutex to block the invocation of a new traversal before the current traversal finishes 
	*/
	static bool travBlock;

    Logger* logger;

	bool isRunning;

	bool hasParsed;

    bool updating;

    void* controlThreadHandle;
    /**
    *	Mutex used in updateLockCond .
    */
    ACE_Thread_Mutex* updateLock;

    /** 
    *       A condition variable which waits for an update to updateLock.
    */
    ACE_Condition_Thread_Mutex* updateLockCondition;

    /**
    *   Pauses the manger's mainLoop. 
    */
    void pause();
    void doIdleTasks();
    void resume();

    bool idleSetGLContext;
    bool idleDeleteGLContext;
    
    static void* startUserInterface(void *);
    static bool isUserInterfaceRunning;
#ifdef OV_IS_LINUX
    GLXDrawable dc;
    Display* dsp;
    GLXContext glContext;
#endif

#ifdef OV_IS_WINXP
    HGLRC glContext;
    HDC dc;
#endif


};
}//namespace openvideo 

#endif
