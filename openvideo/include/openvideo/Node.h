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
/** The header file for the Nodelass.
  *
  * @author Denis Kalkofen
  *
  * $Id: Node.h 245 2007-05-08 13:31:53Z bornik $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _NODEOV_H
#define _NODEOV_H

#include <openvideo/openVideo.h>

#include <vector>
#include <string>

#include <openvideo/PixelFormat.h>

namespace openvideo {
class State;
/**
*@ingroup core
*	The Node class implements the 'AbstractProduct' part from the 'AbstractFactory' pattern [POSA96].
*	None of the functions is pure virtual but a node implementation should 
*	at least override the traversal function (process) to actually implement some action for a specific node.
*/
class OPENVIDEO_API Node
{
public:
	/**
	*	The Constructor
	*/
	Node();

	/**
	*	The Destructor
	*/
	virtual ~Node();

	/**
	* return value = the number of supported input formats. the formats are stored in 'pixelFormats'.
	*/
	virtual void initPixelFormats()=0;

	/**
	*
	*/
	virtual bool validateCurrentPixelFormat();

	/**
	*
	*/
	virtual const char*  getName() const;

	/**
	 *  Retrungs the active pixel format
	 */
	PIXEL_FORMAT getCurrentPixelFormat() const;

	/**
	*	Is called once before the (process)traversal starts.
	*	Init should be used to implement any initializations a specific node needs.
	*/
	virtual void init();


	/**
	*	Is called once before the (process)traversal starts.
	*	Start should be used to implement any start routines a specific node needs.
	*/
	virtual void start();

	/**
	*	Is called after the (process)traversal finishes. 
	*	Stop should be called to clean up any node specific data.
	*/
	virtual void stop();

	/**
	*	This is the actual traversal fucntion. Process is called whenever a new traversal is invoked by the managers timer. 
	*	A specific node implementation should do all its computations which need to be updated here.
	*/
	virtual void process();

	/**
	*	This function is called after OpenVideo's graph is entirely traversed and before a new traversal is invoked.
	*	Here, a specific node implementation can reset some data before a new traversal will take place.
	*/
	virtual void postProcess();

    /**
    *	This function is called after OpenVideo's graph is entirely traversed and before a new traversal is invoked.
    *	Here, a specific node implementation can reset some data before a new traversal will take place.
    */
    virtual void preProcess();

	/**
	*	setParameter is called by OpenVideo's parser. The function gets the xml (key)string and the corresponding value right out of the xml text file.
	*	A specific node implementation should override 'setParameter' to set all its parameters comming from the xml configuration. \n
	*	Notice: Node::setParameter implements these for the keyword DEF and name. If you want your node to be able to get referenced via USE or to just 'have' a name xo should call 
	*	Node::setParameter(key,value) in your node implementation.
	*/
	virtual bool setParameter(std::string key, std::string value);

	/**
	*	Returns the node's state;
	*/
	openvideo::State* getState();


	/**
	*	Adds a new children(output) to the node.
	*	This function gets called during the contruction of the graph and usually doesn't need to be overriden.
	*/
	void addOutput( openvideo::Node * output);

	/**
	*	Adds a new input node.
	*	This function gets called during the contruction of the graph and usually doesn't need to be overriden.
	*
	*/
	void addInput( openvideo::Node * output);

	/**
	*	Returns a vector with all the output(children) nodes.
	*/
	std::vector<openvideo::Node*>* getOutputs();

	/**
	*	Returns a vector with all the input(parent) nodes.
	*/
	std::vector<openvideo::Node*>* getInputs();

	/**
	*	Returns the total number of parents(inputs).
	*/
	int getInDegree();

	/**
	*	Returns the current number of parents(inputs).
	*	Next to the total number of inputs, a node is able to keeps track of the current number of 'meet' inputs.
	*	This for example gets useful during the construction of a traversal order.
	*	
	*/
	int getCurInDegree();

	/**
	*	Decreases the current numer of imputs.
	*	Next to the total number of inputs, a node is able to keeps track of the current number of 'meet' inputs.
	*	This for example gets useful during the construction of a traversal order.
	*
	*/
	void decCurInDegree();
	
	/**
	*	Returns the total number of children(outputs).*
	*/
	int getOutDegree();

	/**
	*	Returns the current number of children(outputs).
	*	Next to the total number of outputs, a node is able to keeps track of the current number of 'updated' outputs.
	*	This for example gets useful during the construction of a traversal order.
	*/
	int getCurOutDegree();

	/**
	*	Decreases the current numer of outputs.
	*	Next to the total number of outputs, a node is able to keeps track of the current number of 'updated' outputs.
	*	This for example gets useful during the construction of a traversal order.
	*/
	void decCurOutDegree();

	/**
	*	Resets the current numer of outputs and inputs to the total numer of in- and outputs.
	*/
	void resetCurInOutDegree();

	/**
	*	Return the nodes 'DEF' name.
	*/
	const char* getDefName();


	virtual const std::string& getTypeName() const  {  return typeName;  }


protected:
	/**
	*	A vector to hold the children (outputs)
	*/
	std::vector<openvideo::Node*> outputs;

	/**
	*	A vector to hold the children (inputs)
	*/
	std::vector<openvideo::Node*> inputs;

	/**
	*	The State. Default=NULL.
	*/
	openvideo::State *state;

	/**
	*	Current input degree.
	*	Next to the total number of inputs, a node is able to keeps track of the current number of 'meet' inputs.
	*	This for example gets useful during the construction of a traversal order.
	*/
	int curInDegree;

	/**
	*	current output degree
	*	Next to the total number of outputs, a node is able to keeps track of the current number of 'updated' outputs.
	*	This for example gets useful during the construction of a traversal order.
	*/
	int curOutDegree;

	/**
	*	The node's name.
	*/
	std::string name;

	/// The node's type name
	std::string typeName;


	/**
	*	The node's 'DEF' name
	*/
	std::string defName;

	/**
	*	The node's 'DEF' name
	*/
	std::vector<PIXEL_FORMAT> pixelFormats;

	PIXEL_FORMAT curPixelFormat;
};
} //namespace openvideo {

#endif
