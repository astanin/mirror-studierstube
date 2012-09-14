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
/** Header file for EventKeyboardSource Node.
  *
  * @author Gerhard Reitmayr
  *
  * $Id: EventKeyboardSource.h 44 2005-07-18 15:03:44Z bara $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef __EVENTKEYBOARDSOURCE_H__
#define __EVENTKEYBOARDSOURCE_H__

#ifndef USE_OT_2_0
#include <OpenTracker.h>
#else
#include <OpenTracker/OpenTracker.h>
#endif
#include <stb/components/event/event.h>


/**
 * This class implements a simple node that generates tracking events
 * based on keyboard input. It is controlled by the StbKeyboardModule class
 * that uses key events from the windowing system to drive tracking stations.
 * The corresponding OpenTracker configuration element has the following attributes :
 * @li @c number a number ( 0 - 9 ) giving the station number.
 *
 * An example element looks like this :
 * @verbatim
<EventKeyboardSource number="2"/>@endverbatim
 *
 * @author Gerhard Reitmayr
 * @ingroup tracking
 */
class EVENT_API EventKeyboardSource : public ot::Node
{
// Members
public:
    /// station number
    int number;
    /// the state that is stored
#ifdef USE_OT_1_1
    ot::State state;
#else
    ot::Event state;
#endif
    /// a flag to indicate whether it was changed during processing
    bool changed;

// Methods
protected:
    /** constructor method.
     * @param number_ number of the station to use */
    EventKeyboardSource( int number_ ) :
         ot::Node(),
        number( number_ ),
        changed( false )
    {}

public:    
    /** tests for EventGenerator interface being present. Is overriden to
     * return 1 always.
     * @return always 1 */
    virtual int isEventGenerator()
    {
        return 1;
    }  
    
    friend class EventModule;
};

#endif
