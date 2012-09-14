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
/** Header file for EventMouseSource Node.
  *
  * @author Gerhard Reitmayr
  *
  * $Id: EventMouseSource.h 44 2005-07-18 15:03:44Z bara $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef __EVENTMOUSESOURCE_H__
#define __EVENTMOUSESOURCE_H__

#ifndef USE_OT_2_0
#include <OpenTracker.h>
#else
#include <OpenTracker/OpenTracker.h>
#endif
#include <stb/components/event/event.h>


class SoEvent;
class SbViewportRegion;

/**
 * This class implements a simple node that generates tracking events
 * based on mouse input. It is controlled by the StbKeyboardModule class
 * that uses events from the windowing system to drive tracking stations.
 * It outputs the x, y coordinates of the mosue curser in the x, y components
 * of the position field. Mouse button events are coded into the first three bits
 * of the button value (bit 0 - button 1, bit 1 - button 2, bit 3 - button 3).
 * Everything else is set to default values. The origin is
 * in the upper left corner of the window, x points to the right and y down.
 * The corresponding OpenTracker configuration element has the following attributes :
 * @li @c mode (absolute|relative) relative defines whether the position is absolute            
 *             (in pixels) or relative (0..1)
 * @li @c windows the number of the window to listen to (not implemented yet)
 *
 * An example element looks like this :
 * @verbatim
<EventMouseSource mode="relative" window="0"/>@endverbatim
 *
 * @author Gerhard Reitmayr
 * @ingroup tracking
 */
class EVENT_API EventMouseSource : public ot::Node
{
// Members
public:
    /// defining the mode the node is working in
    enum Mode { StbRelative , StbAbsolute } mode;   
    /// window number
    int window;
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
     * @param mode_ relative or absolute mode to use
     * @param window_ which Studierstube window to use
     */
    EventMouseSource( Mode mode_, int window_ = -1 );
    
    /**
     * processes the actual input event from OpenInventor
     * @param event pointer to the event
     * @param viewer pointer to the SoStudierstubeViewer generating the event.
     */
    void processEvent( const SoEvent & event, const SbViewportRegion & region );

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
