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
/** The header file for the ImageSrcFactory class.
  *
  * @author Daniel Wagner
  *
  * $Id: ImageSrcFactory.h 148 2006-05-26 14:53:05Z daniel $
  * @file                                                                   */
 /* ======================================================================= */


#ifndef _IMAGESRCFACTORY_H
#define _IMAGESRCFACTORY_H

#include <openvideo/openVideo.h>

#ifdef ENABLE_IMAGESRC

#include <openvideo/NodeFactory.h>
#include <openvideo/nodes/ImageSrc.h>

namespace openvideo {
    /**
    *@ingroup nodes
    *	A factory to create DSVLSrc nodes.
    */
class OPENVIDEO_API  ImageSrcFactory : public openvideo::NodeFactory
{
public:
	/**
	*	constructor
	*/
    ImageSrcFactory();

	/**
	*	destructor
	*/
	~ImageSrcFactory();

	/**
	*	creates DSVL nodes
	*/
	virtual openvideo::ImageSrc* createNode();

	/**
	*	returns VideoWrapperSrc as the type of known objects
	*/
	virtual const char* getNodeTypeId();
};

}//namespace openvideo {

#endif // ENABLE_IMAGESRC

#endif // _IMAGESRCFACTORY_H
