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
/** The header file for the V4L2SrcFactory class.
 *
 * @author Bernhard Reitinger
 *
 * $Id:
 * @file                                                                   */
/* ======================================================================= */

#ifndef _V4L2SRCFACTORY_H
#define _V4L2SRCFACTORY_H

#include <openvideo/openVideo.h>

#ifdef ENABLE_V4L2SRC

#include <openvideo/NodeFactory.h>
#include <openvideo/nodes/V4L2Src.h>

namespace openvideo {

    /**
     *@ingroup nodes
     *	A factory to create V4L2Src nodes.
     */
    class OPENVIDEO_API  V4L2SrcFactory
	: public NodeFactory
    {
    public:
	/**
         *	constructor
         */
        V4L2SrcFactory();

	/**
         *	destructor
         */
	~V4L2SrcFactory();

	/**
         *	creates videowrappersrc nodes
         */
	virtual V4L2Src* createNode();

	/**
         *	returns V4L2Src as the type of known objects
         */
	virtual const char* getNodeTypeId();
    };

} // namespace

#endif // ENABLE_V4L2SRC
#endif // __V4L2SRCFACTORY_H

//========================================================================
// End of $FILENAME$
//========================================================================
// Local Variables:
// mode: c++
// c-basic-offset: 4
// eval: (c-set-offset 'substatement-open 0)
// eval: (c-set-offset 'case-label '+)
// eval: (c-set-offset 'statement 'c-lineup-runin-statements)
// eval: (setq indent-tabs-mode nil)
// End:
//========================================================================
