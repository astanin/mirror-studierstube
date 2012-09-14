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
/** The header file for the OpenCVSrcFactory class.
 *
 * @author Jorn Skaarud Karlsen
 *
 * $Id: OpenCVSrcFactory.h 252 2007-06-13 14:34:11Z mendez $
 * @file                                                                   */
/* ======================================================================= */
#ifndef _OPENCVSRCFACTORY_H
#define _OPENCVSRCFACTORY_H

#include <openvideo/openVideo.h>

// If OpenCV is available
#ifdef ENABLE_OPENCV

#include <openvideo/nodes/OpenCVSrc.h>
#include <openvideo/NodeFactory.h>



namespace openvideo {
    /**
    *@ingroup nodes
    *	A factory to create OpenCVSrc nodes.
    */
  class OPENVIDEO_API  OpenCVSrcFactory :
  public openvideo::NodeFactory
  {
  public:
    /**
     *	constructor
     */
    OpenCVSrcFactory(){};

    /**
     *	creates OpenCVSrc nodes
     */
    virtual const char* getNodeTypeId();
	
    /**
     *	returns OpenCVSrc as the type of known objects
     */
    virtual openvideo::OpenCVSrc* createNode();
  };
} //namespace openvideo {

#endif // ENABLE_OPENCV
#endif // _OPENCVSRCFACTORY_H
