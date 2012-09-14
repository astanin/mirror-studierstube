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
/** The header file for the SwitchFactory class.
  *
  * @author Kopeinigg Daniel
  *
  * $Id: 
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _SWITCHFACTORY_H
#define _SWITCHFACTORY_H

#include <openvideo/openVideo.h>

#ifdef ENABLE_SWITCH

#include <openvideo/NodeFactory.h>
#include <openvideo/nodes/Switch.h>

/**
*@ingroup nodes
*	A factory to create Switch nodes.
*/
class OPENVIDEO_API  SwitchFactory	
	: public openvideo::NodeFactory
{
public:
	/**
	*	constructor
	*/
    SwitchFactory();

	/**
	*	destructor
	*/
	~SwitchFactory();

	/**
	*	creates Switch nodes
	*/
	virtual openvideo::Switch* createNode();

	/**
	*	returns Switch as the type of known objects
	*/
	virtual const char* getNodeTypeId();
};

#endif // ENABLE_SWITCH
#endif // __SwitchFactory_H
