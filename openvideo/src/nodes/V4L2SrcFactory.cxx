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
/** The source file for the V4L2SrcFactory class.
 *
 * @author Bernhard Reitinger
 *
 * $Id:
 * @file
 * ======================================================================= */

#include <openvideo/nodes/V4L2SrcFactory.h>
#include <openvideo/openVideo.h>

#ifdef ENABLE_V4L2SRC

namespace openvideo {

    V4L2SrcFactory::V4L2SrcFactory()
    {
    }


    V4L2SrcFactory::~V4L2SrcFactory()
    {

    }

    V4L2Src*
    V4L2SrcFactory::createNode()
    {
        return new V4L2Src;

    }

    const char*
    V4L2SrcFactory::getNodeTypeId()
    {
        return "V4L2Src";
    }

} // namespace

#endif // ENABLE_V4L2SRC

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
