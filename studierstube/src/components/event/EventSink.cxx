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
/** Source file for EventSink Node.
  *
  * @author Gerhard Reitmayr
  *
  * $Id: EventSink.cpp 52 2006-02-02 20:14:26Z bara $
  * @file                                                                   */
 /* ======================================================================= */

// #include "../tool/disable4786.h"
#include <stb/components/event/EventSink.h>
#include <stb/components/event/EventModule.h>
#include <stb/base/OS.h>

#ifdef STB_IS_WINDOWS
#  pragma warning(disable:4786)
#endif


EventSink::EventSink( EventModule * parent_ ) :
    parent( parent_ )
{
  type="EventSink";
}


int EventSink::isEventGenerator()
{
    return 1;
}


#ifdef USE_OT_1_1
    void EventSink::onEventGenerated( ot::State& event, ot::Node& )
#else
    void EventSink::onEventGenerated( ot::Event& event, ot::Node& )
#endif
{
  parent->processEvent( &event, this );
  updateObservers( event );
}
