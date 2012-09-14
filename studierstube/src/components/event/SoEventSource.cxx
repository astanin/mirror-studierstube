/* ======================================================================== 
 * Copyright (C) 2006  Graz University of Technology
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
 * For further information please contact 
 * Dieter Schmalstieg
 * <schmalstieg@icg.tu-graz.ac.at>
 * Graz University of Technology, 
 * Institute for Computer Graphics and Vision,
 * Inffeldgasse 16a, 8010 Graz, Austria.
 * ========================================================================
 * PROJECT: Studierstube
 * ======================================================================== */
/** header file for SoEventAction node
  *
  * @author   Gerhard Reitmayr
  *
  * $Id: SoEventSource.cpp 44 2005-07-18 15:03:44Z bara $
  * @file                                                                   */
 /* ======================================================================= */

#include <stb/components/event/SoEventSource.h>
#include <stb/components/event/SoInputEvent.h>

SO_NODE_ABSTRACT_SOURCE(SoEventSource);

void SoEventSource::initClass(void)
{
    SO_NODE_INIT_ABSTRACT_CLASS(SoEventSource, SoNode, "Node");
}

SoEventSource::SoEventSource(void)
{
    SO_NODE_CONSTRUCTOR(SoEventSource);
}

void SoEventSource::publish( SoInputEvent * event )
{
    EventBus::getSingleton().publish( event );
}
