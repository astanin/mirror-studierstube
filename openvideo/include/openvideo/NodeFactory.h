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
/** The header file for the NodeFactory class.
  *
  * @author Denis Kalkofen
  *
  * $Id: NodeFactory.h 148 2006-05-26 14:53:05Z daniel $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _ABSTRACTNODEFACTORY_H
#define _ABSTRACTNODEFACTORY_H

#include <openvideo/openVideo.h>

namespace openvideo {
class Node;

/**
*@ingroup core
*
*	The NodeFactory class implements the 'AbstractFactory' part from the 'AbstractFactory' pattern [POSA96].
*	
*/


class OPENVIDEO_API  NodeFactory
{
public:
	/**
	*	Constructor
	*/
	NodeFactory(){};

    virtual ~NodeFactory(){};
	/**
	*	The function to construct a Node.
	*/
	virtual openvideo::Node* createNode()=0;
	
	/**
	*	Returns the node name. 
	*	With this function a factory can be 'asked' which type of nodes it can 'produce'.
	*/
	virtual const char* getNodeTypeId()=0;
};
}//namespace openvideo {

#endif
